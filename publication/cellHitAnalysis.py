"""
cellshit-analysis.py

Analysis of root files for Cells Hit paper.
"""
from __future__ import division, unicode_literals, print_function

import ROOT
import pdb

import numpy as np
import matplotlib.pyplot as plt

from scipy.stats import ks_2samp
from scipy.spatial import ConvexHull

from matplotlib import rcParams

rcParams['font.family'] = 'serif'
rcParams['font.serif'] = ['Times']
rcParams['text.usetex'] = True
rcParams['axes.labelsize'] = 8
rcParams['xtick.labelsize'] = 7
rcParams['ytick.labelsize'] = 7
rcParams['legend.fontsize'] = 8

base_dir = '/home/nlampe/gits/geant4sims/multiscale/publication/'
files = {'macro': {'gammabg': base_dir + 'gammabg/gammabgmacro.root',
                   'kelectron': base_dir + 'kelectron/kelectronmacro.root',
                   'muon': base_dir + 'muon/muonmacro.root',
                   'neutron': base_dir + 'neutron/neutronmacro.root'},
         'micro': {'gammabg': base_dir + 'gammabg/gammabgmicro.root',
                   'kelectron': base_dir + 'kelectron/kelectronmicro.root',
                   'muon': base_dir + 'muon/muonmicro.root',
                   'neutron': base_dir + 'neutron/neutronmicro.root',
                   'test-pen': base_dir + 'test-pen/gammabgmicro.root',
                   'test-liv': base_dir + 'test-liv/gammabgmicro.root',
                   'test-option4': base_dir + 'test-option4/gammabgmicro.root',
                   'test-dna': base_dir + 'test-dna/gammabgmicro.root',},
         'secs': {'gammabg': base_dir + 'gammabg/secondary_ps.root',
                  'kelectron': base_dir + 'kelectron/secondary_ps.root',
                  'muon': base_dir + 'muon/secondary_ps.root',
                  'neutron': base_dir + 'neutron/secondary_ps.root'}
                  }

normalisation = {'gammabg': 106.95, 'kelectron': 1.106, 'muon': 23.75,
                 'neutron': 11.76}

n_initial = {'gammabg': 1e8, 'kelectron': 1e7, 'muon': 1e8,
             'neutron': 1e8}

typeDict = {"Char_t": 'S16', 'Double_t': 'f8', 'Int_t': 'i8'}


def convertTupleToArray(rfilecode, source, tuplename):
    """
    Return root tuple as numpy array
    rfilecode = macro | micro | secs
    source = gammabg[-dna -pen] | kelectron | muon | neutron
    """
    filename = files[rfilecode][source]
    f = ROOT.TFile(filename)
    if rfilecode == 'micro':
        f.cd('tuples')

    tree = ROOT.gDirectory.Get(tuplename)
    names = [x.GetName() for x in tree.GetListOfBranches()]
    types = [tree.GetLeaf(n).GetTypeName() for n in names]


    dt = np.dtype([(n, typeDict[t]) for (n,t) in zip(names, types)])
    n_entries = x.GetEntries()

    lists = {n: [] for n in names}

    # Populate the lists
    for ii in xrange(0, n_entries):
        if (ii//1000000)*1000000 == ii:
            print("> Read " + str(ii) + " of " + str(n_entries) + " values.")
        tree.GetEntry(ii)
        for (n, l) in lists.items():
            l.append(tree.GetLeaf(n).GetValue())

    # list as array
    arrlist = zip(*[lists[n] for n in names])
    print("> Building array of " + str(n_entries) + " elements")
    return np.array(arrlist, dtype=dt)



def cellsHitPerDay(source, n_initial, ncells=7.5e8):
    """
    Give cells hit per day for a specified source
    """
    macro_evts = convertTupleToArray("macro", source, "SecondarySpectrum")
    micro_cell_depos = convertTupleToArray("micro", source, "CellDepos")
    micro_events = convertTupleToArray("micro", source, "Event")

    n_macro_evts = len(macro_evts)
    macro_secs = n_initial/normalisation[source]
    n_micro_evts = len(micro_events)
    n_micro_cells_hit = len(micro_cell_depos)

    # micro evts/sec
    micro_time = n_micro_evts/n_macro_evts*macro_secs

    evts_per_second = n_micro_cells_hit/micro_time
    evts_per_day = evts_per_second*8.64e4/ncells

    return evts_per_day, n_macro_evts, macro_secs, n_micro_evts, micro_time


def getEnergyDepoStatistics(source):
    """
    stat_data = getAverageEnergyDepo(source)
    Get statistics for a source at the micro level

    Yields: mean, std, histogram, edges, mode
    """
    print("Finding statistics for " + source)
    micro_cell_depos = convertTupleToArray("micro", source, "CellDepos")

    out = dict()
    out['mean'] = np.mean(micro_cell_depos["Edepo_keV"])
    out['std'] = np.std(micro_cell_depos["Edepo_keV"])

    out['h'], out['edges'] = np.histogram(micro_cell_depos["Edepo_keV"], 1024)

    mode_idx = np.argmax(out['h'])
    out['mode'] = 0.5*(out['edges'][mode_idx] + out['edges'][mode_idx+1])
    out['pct25'] = np.percentile(micro_cell_depos["Edepo_keV"], 25)
    out['pct50'] = np.percentile(micro_cell_depos["Edepo_keV"], 50)
    out['pct75'] = np.percentile(micro_cell_depos["Edepo_keV"], 75)
    out['iqr'] = out['pct75'] - out['pct25']
    out['n'] = len(micro_cell_depos)

    return out


def ksCompare(source1, source2):
    """
    KS test on 2 sources
    """
    micro_cell_depos1 = convertTupleToArray("micro", source1, "CellDepos")
    micro_cell_depos2 = convertTupleToArray("micro", source2, "CellDepos")

    cell1 = micro_cell_depos1['Edepo_keV']
    cell2 = micro_cell_depos2['Edepo_keV']

    return ks_2samp(cell1, cell2)


def plotHistogramsOfEdep(sources, labels, bins):
    """
    fig = plotHistogramsOfEdep(sources, labels):
    return a matplotlib.figure instance containing a histogram of edeps

    args:
    -----
    sources: list of sources to plot
    labels: list of labels for figure
    bins: histogram bin edges
    """

    fig = plt.figure(figsize=[2.5, 2.5])
    ax = fig.add_subplot(111)
    ax.set_yscale('log')

    styles = ["r-", "g--", "b:", 'k-.']

    assert len(sources) <= 4, 'Method supports up to four sources'

    axis = 0.5*(bins + np.roll(bins, 1))[1:len(bins)]
    for (source, label, style) in zip(sources, labels, styles):
        cell_depos = convertTupleToArray("micro", source, "CellDepos")
        h, b = np.histogram(cell_depos["Edepo_keV"], bins)
        ax.plot(axis, h, style, label=label)

    ax.set_xlabel("Energy deposited (keV)")
    ax.set_ylabel("Counts")
    ax.legend(loc=0, frameon=False, handlelength=3)
    return fig


def plotCellHitFrequency(sources, labels, bins):
    """
    fig = plotHistogramsOfEdep(sources, labels):
    return a matplotlib.figure instance containing a histogram of edeps

    args:
    -----
    sources: list of sources to plot
    labels: list of labels for figure
    bins: histogram bin edges
    """

    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.set_yscale('log')

    styles = ["ro", "g^", "bv", 'k*']

    assert len(sources) <= 4, 'Method supports up to four sources'

    # axis = 0.5*(bins + np.roll(bins, 1))[1:len(bins)]
    axis = bins[:(len(bins)-1)]
    for (source, label, style) in zip(sources, labels, styles):
        event_res = convertTupleToArray("micro", source, "Event")
        h, b = np.histogram(event_res["NCellsHit"], bins)
        ax.plot(axis, h, style, label=label)

    ax.legend(loc=0)
    ax.set_xlabel('Number of cells hit by one primary')
    ax.set_ylabel('Counts')

    return fig


def evaluateBiologicalDamage(source):
    """
    evaluateBiologicalDamage(source)

    Evaluate the biological damage for a source
    The biological damage is the approximate number of single strand breaks
    induced

    """
    cell_wall = convertTupleToArray("micro", source, "CellWall")
    cell_interior = convertTupleToArray("micro", source, "CellInterior")

    cell_wall_evts = cell_wall["EventID"]
    cell_wall_ids = cell_wall["CellID"]

    cell_int_evts = cell_interior["EventID"]
    cell_int_ids = cell_interior["CellID"]
    cell_int_ens = cell_interior["Energy_keV"]
    cell_int_x = cell_interior["PosX_um"]
    cell_int_y = cell_interior["PosY_um"]
    cell_int_z = cell_interior["PosZ_um"]

    cell_events = []
    recent = [(0, 0)]*100  # keep last 100 events to check when 2 primaries
                           # enter cell
    skip_count = 0
    #pdb.set_trace()
    for (evt, cid) in zip(cell_wall_evts, cell_wall_ids):
        recent.pop()
        cell_events.append((evt, cid))
        if (evt, cid) in recent:
            skip_count += 1
        recent.append((evt, cid))

    res = []
    this_event = -1
    ok = []
    nn = 0
    for (evt, cid) in cell_events:
        if (nn//100000)*nn: print("> nn")
        #config
        if evt != this_event:
            # delete un-needed array data to speed up where
            if len(ok) > 1:
                cell_int_evts = cell_int_evts[ok[-1]:]
                cell_int_ids = cell_int_ids[ok[-1]:]
                cell_int_ens = cell_int_ens[ok[-1]:]
            this_event = evt
            ok = np.where(cell_int_evts == this_event)[0]
            this_event_cells = cell_int_ids[ok]
            this_event_energies = cell_int_ens[ok]
            # this_event_x = cell_int_x[ok]
            # this_event_y = cell_int_y[ok]
            # this_event_z = cell_int_z[ok]

        this_cell = np.where(this_event_cells == cid)[0]
        if len(this_cell) > 0:
            en = sum(this_event_energies[this_cell])
            # No convex hull approach
            vol = len(this_cell)*np.pi*(1e-2)**2*1e-2
            res.append([en, vol, en/vol])

            # Convex Hull approach
            # pos = np.transpose(np.array([this_event_x[this_cell],
            #                              this_event_y[this_cell],
            #                              this_event_z[this_cell]]))
            # if len(pos) >= 4:
            #     try:
            #         vol = ConvexHull(pos).volume
            #         res.append([en, vol, en/vol])
            #     except:
            #         pass
                    # if len(pos) == 1:
                    #     vol = 0.010**3
                    #     print("only one hit")
                    # elif len(pos) == 2:
                    #     vol = 0.010**2*((pos[0,0] - pos[1,0])**2 +
                    #                     (pos[0,1] - pos[1,1])**2 +
                    #                     (pos[0,2] - pos[1,2])**2)**0.5
                    # else:
                    #     x = pos[:,0]
                    #     y = pos[:,1]
                    #     z = pos[:,2]
                    #     vol = (max(x) - min(x))*(max(y) - min(y))*(max(z) - min(z))
    return np.array(res), skip_count


def reportBiologicalDamage():
    """
    """
    sources = ["gammabg", "kelectron"]#, "muon", "neutron"]
    results = {source: {} for source in sources}
    dnafrac = 4.77e-3/1.65  # percentage of cell that is dna
    ebreak = 8.22e-3  # 8.22 eV/break
    k = ebreak/dnafrac


    for source in sources:
        biodamage, nskips = evaluateBiologicalDamage(source)

        results[source]["skipped"] = nskips
        results[source]["meanEnergyDepo"] = np.mean(biodamage[:, 0])
        results[source]["meanEnergyDensity"] = np.mean(biodamage[:, 2])
        results[source]["meanVolume"] = np.mean(biodamage[:, 1])
        results[source]["25th_percentile"] = np.percentile(biodamage[:, 2], 25)
        results[source]["50th_percentile"] = np.percentile(biodamage[:, 2], 50)
        results[source]["75th_percentile"] = np.percentile(biodamage[:, 2], 75)
        results[source]["meanSSBs"] = np.mean(biodamage[:,2])/k

    keys = ["skipped", "meanEnergyDepo", "meanEnergyDensity", "meanVolume",
             "25th_percentile", "50th_percentile", "75th_percentile",
             "meanSSBs"]
    print("\n===============================================================\n")
    print("".join(["{0:<18} ".format("param")] + ["{0:<18} ".format(source)
                                                  for source in sources]))
    print("---------------------------------------------------------------")
    for item in keys:
        print("".join(["{0:<18} ".format(item)] +
                      ["{0:<18} ".format(results[source][item])
                       for source in sources]))
    print("\n===============================================================\n")
    return None


def reportCellStatistics():
    """
    reportCellStatistics()
    print to std out the cells that were hit from each source
    """
    sources = ["gammabg", "kelectron", "neutron", "muon"]
    n_macro = {"gammabg": 1e8, "kelectron": 1e7, "muon": 1e8, "neutron": 1e8,}
    results = {source: {"n_macro": n_macro[source]} for source in sources}


    for source in sources:
        nhit, evts_macro, time_macro, evts_micro, time_micro =\
            cellsHitPerDay(source, n_macro[source], ncells=7.5e8)
        results[source]["cellsHitPerDay"] = nhit
        results[source]["n_macro_evts"] = evts_macro
        results[source]["macro_seconds"] = time_macro
        results[source]["n_micro"] = evts_micro
        results[source]["micro_seconds"] = time_micro
        stats = getEnergyDepoStatistics(source)
        results[source]["meanEnergyDepo"] = stats["mean"]
        results[source]["stdEnergyDepo"] = stats["std"]
        results[source]["modeEnergyDepo"] = stats["mode"]
        results[source]["n_cells_hit"] = stats["n"]
        results[source]["25th_percentile"] = stats["pct25"]
        results[source]["50th_percentile"] = stats["pct50"]
        results[source]["75th_percentile"] = stats["pct75"]

    keys = ["n_macro", "macro_seconds", "n_macro_evts", "n_micro",
            "micro_seconds", "n_cells_hit", "cellsHitPerDay", "meanEnergyDepo",
            "stdEnergyDepo", "modeEnergyDepo", "25th_percentile",
            "50th_percentile", "75th_percentile"]
    print("\n===============================================================\n")
    print("".join(["{0:<18} ".format("param")] + ["{0:<18} ".format(source)
                                                  for source in sources]))
    print("---------------------------------------------------------------")
    for item in keys:
        print("".join(["{0:<18} ".format(item)] +
                      ["{0:<18} ".format(results[source][item])
                       for source in sources]))
    print("\n===============================================================\n")
    return None


def reportTestStatistics():
    """
    reportTestStatistics()
    print to std out the cells that were hit from each source
    """
    sources = ["test-liv", "test-pen", "test-option4"]  # "test-dna",
    results = {source: {} for source in sources}


    for source in sources:
        stats = getEnergyDepoStatistics(source)
        results[source]["meanEnergyDepo"] = stats["mean"]
        results[source]["stdEnergyDepo"] = stats["std"]
        results[source]["modeEnergyDepo"] = stats["mode"]
        results[source]["n_cells_hit"] = stats["n"]
        results[source]["25th_percentile"] = stats["pct25"]
        results[source]["50th_percentile"] = stats["pct50"]
        results[source]["75th_percentile"] = stats["pct75"]

    print("\n===============================================================\n")
    print("".join(["param           "] + ["{0:<15} ".format(source)
                                     for source in sources]))
    print("---------------------------------------------------------------")
    for item in results[sources[0]].keys():
        print("".join(["{0:<15} ".format(item)] +
                      ["{0:<15} ".format(results[source][item])
                       for source in sources]))
    print("\n===============================================================\n")
    return None


def makeplots():
    """
    makeplots()

    Make the plots
    """
    sources = ["gammabg", "kelectron", "neutron", "muon"]
    labels = ["Gamma background", "Beta electrons", "Cosmic Neutrons",
              "Cosmic Muons"]

    f = plotCellHitFrequency(sources, labels, np.linspace(0, 30, 30))
    f.savefig("cellshit.pdf", bbox_inches="tight")

    f = plotHistogramsOfEdep(sources, labels, np.linspace(0, 5, 100))
    f.savefig("edep.pdf", bbox_inches="tight")

    # f = plotHistogramsOfEdep(["test-pen", "test-liv"],
    #                          ["test-liv", "tes"],
    #                          np.linspace(0, 5, 100))
    f.savefig("dna-compare.pdf", bbox_inches="tight")

    return None


def kstest():
    """
    """
    ks = {}
    p = {}
    for l in ["test-option4", "test-liv"]:  #, "test-pen"]:
        distance, pvalue = ksCompare("test-pen", l)
        print(distance, pvalue)
        ks[l] = distance
        p[l] = pvalue
    print("\n===============================================================")
    print("Comparing to test-dna:\n")
    print("List           KS-Statistic       p-value (two-tailed)")
    print("----------     ------------       -----------------------")
    for l in ["test-option4", "test-liv"]:#, "test-pen"]:
        print("{0:<14} {1:<14} {2:<14}".format(l, ks[l], p[l]))
    print("\n===============================================================\n")

    return None



if __name__ == "__main__":
    #reportCellStatistics()
    #reportBiologicalDamage()
    #reportTestStatistics()
    #kstest()
    makeplots()
