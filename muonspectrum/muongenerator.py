"""
muongenerator.py

Generate Muon departure energies and angles
"""
from __future__ import division

import ROOT
import pdb

import numpy as np
import matplotlib.pyplot as plt

from cFluxModel import differential_flux, MODIFIED_CHIRKIN
from rootpy.plotting import Hist2D

Mmuon = 207*0.511 # MeV
phi      = 0.
altitude = 0.
latitude = None

thetamin = 0
thetamax = 70
thetabins = 150

thetaEdges = np.linspace(thetamin, thetamax, thetabins+1)
thetawidth = thetaEdges[1] - thetaEdges[0]
thetas = np.zeros(thetabins)
for ii in range(len(thetas)):
    thetas[ii] = 0.5*(thetaEdges[ii] + thetaEdges[ii+1])

pmin = 0.1# in GeV/c
pmax = 50
pbins = 100

pedges = np.linspace(pmin, pmax, pbins+1)
ps = np.zeros(pbins)
pwidth = pedges[1]-pedges[0]
for ii in range(len(ps)):
    ps[ii] = 0.5*(pedges[ii]+pedges[ii+1])

def diffFlux(momentum, theta, phi=phi, altitude=altitude):
    momentum = np.asarray(momentum)
    fluxes = np.zeros([len(momentum)])
    for ii in range(0, len(momentum)):
        fluxes[ii] = differential_flux(model=MODIFIED_CHIRKIN,
                                       momentum=momentum[ii],
                                       theta=theta,
                                       phi=phi,
                                       altitude=altitude,
                                       latitude=latitude)
    return fluxes


def makeEventsFile(n):
    """
    Generate a text file with n events
    """
    h = Hist2D(pbins, pmin, pmax, thetabins, thetamin, thetamax, type='F')

    cumFlux = 0

    for theta in thetas:
        for p in ps:
            dflux = differential_flux(model=MODIFIED_CHIRKIN,
                                      momentum=p,
                                      theta=theta,
                                      phi=phi,
                                      altitude=altitude,
                                      latitude=latitude)
            flux = dflux*p*np.cos(theta*np.pi/180.)*thetawidth*np.pi/180.
            cumFlux += flux
            h.Fill(p, theta, flux)

    outfile = file("events.dat", "w")
    theta = ROOT.Double(0)
    p = ROOT.Double(0)
    outfile.write("# Events for a momentum range of " + str(pmin) + " to " + str(pmax) + "GeV/c\n")
    outfile.write("# The cumulative flux in this range is " + str(cumFlux) + " muons/cm^2/s.\n")
    outfile.write("# Energy_MeV theta_degree\n")
    for ii in range(n):
        h.GetRandom2(p, theta)
        en = np.sqrt((p*1000)**2 + Mmuon**2) - Mmuon
        outfile.write(" ".join([str(en), str(theta)]))
        outfile.write("\n")

        if (ii//10000)*10000 == ii: print "event " + str(ii)

    outfile.close()


def testEventsFile():
    thetas = [0, 10, 20, 30, 40, 50, 60, 70, 80, 90]
    mom = ps
    en, t = np.loadtxt("events.dat", unpack=True)

    p = np.sqrt(en*(en+2*Mmuon))/1000. # in GeV

    fig = plt.figure()
    ax = fig.add_subplot(111)
    #ax.set_xlim([0,10])
    #ax.set_ylim([0,90])
    ax.set_yscale('log')
    ax.set_xscale('log')
    ax.set_xlabel("Momentum (GeV)")
    ax.set_ylabel("Normalised count")

    Flux = 0
    for theta in np.linspace(0,90,91):
        Flux+=np.sum(diffFlux(ps, theta))
    Counts = np.sum(p)

    norm = Flux/Counts
    pbmax = 0
    dfmax = 0

    for theta in thetas:
        df = diffFlux(ps, theta)
        t_ok = np.where(np.logical_and(t>(theta-5), t<(theta+5)))[0]
        p_ok = p[t_ok]
        p_binned = np.histogram(p_ok, pedges)[0]
        # p_binned *= norm

        # Can write this kludge because curves are well behaved
        pbmax = max(max(p_binned), pbmax)
        dfmax = max(max(df), dfmax)

        df *= (pbmax/dfmax)

        ax.plot(ps, df, 'r')
        ax.plot(ps, p_binned, 'b')

    fig.savefig("events-test.pdf")

    return fig


if __name__ == "__main__":
    makeEventsFile(1000000)
    testEventsFile()
