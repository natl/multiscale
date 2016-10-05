//
// macroanalysis.C
//
// *********************************************************************
// To execute this macro under ROOT after your simulation ended,
//   1 - launch ROOT (usually type 'root' at your machine's prompt)
//   2 - type '.X plot.C' at the ROOT session prompt
// *********************************************************************
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


void h12ascii (TH1* hlow, TH1* hmid, TH1* hhigh, TH1* hvhigh, char* filename)
{
    cout << "Opening File\n";
    FILE* outfile = fopen(filename, "w");
    cout << "File Opened\n";

    int nlow = hlow->GetNbinsX();
    int nmid = hmid->GetNbinsX();
    int nhigh = hhigh->GetNbinsX();
    int nvhigh = hvhigh->GetNbinsX();
    std::printf("Saving %i bins\n", nlow+nmid+nhigh+nvhigh);

    sleep(2);
    for (int ii = 1; ii <= nlow; ii++)
    {
        std::fprintf(outfile, "%g %g\n",
            hlow->GetBinLowEdge(ii)+hlow->GetBinWidth(ii)/2,
            hlow->GetBinContent(ii));
    }

    for (int ii = 1; ii <= nmid; ii++)
    {
        std::fprintf(outfile, "%g %g\n",
            hmid->GetBinLowEdge(ii)+hmid->GetBinWidth(ii)/2,
            hmid->GetBinContent(ii));
    }

    for (int ii = 1; ii <= nhigh; ii++)
    {
        std::fprintf(outfile, "%g %g\n",
            hhigh->GetBinLowEdge(ii)+hhigh->GetBinWidth(ii)/2,
            hhigh->GetBinContent(ii));
    }

        for (int ii = 1; ii <= nvhigh; ii++)
    {
        std::fprintf(outfile, "%g %g\n",
            hvhigh->GetBinLowEdge(ii)+hvhigh->GetBinWidth(ii)/2,
            hvhigh->GetBinContent(ii));
    }

    fclose(outfile);
}


int macroanalysis(char* filename)
{
    // char* filename = "combined.root";
    // Read in the file
    TFile* f = new TFile(filename);
    // TDirectory* hists  = f->GetDirectory("hists;1");
    // TDirectory* tuples = f->GetDirectory("tuples;1");

    TTree* accum = (TTree*) f->GetObjectUnchecked("AccumulatedEnergy;1");
    TLeaf* einit = (TLeaf*) accum->GetLeaf("Einit");
    TLeaf* edepo = (TLeaf*) accum->GetLeaf("Edep");

    TTree* edeps = (TTree*) f->GetObjectUnchecked("EnergyDepositions;1");
    TLeaf* individualdepos = (TLeaf*) edeps->GetLeaf("Edep");

    TTree* secs  = (TTree*) f->GetObjectUnchecked("SecondarySpectrum;1");
    TLeaf* secsenergy = (TLeaf*) secs->GetLeaf("KineticEnergy");

    double ein = 0;
    double eout = 0;



    int nevents = accum->GetEntries();
    for (int ii = 0; ii<nevents; ii++)
    {
        accum->GetEntry(ii);
        ein+=einit->GetValue();
        eout+=edepo->GetValue();
    }


    int ndepos = edeps->GetEntries();
    double deposum = 0;
    for (int ii = 0; ii<ndepos; ii++)
    {
        edeps->GetEntry(ii);
        deposum+=individualdepos->GetValue();
    }


    int nsecs = secs->GetEntries();
    double secsum = 0;
    for (int ii = 0; ii<nsecs; ii++)
    {
        secs->GetEntry(ii);
        secsum+=secsenergy->GetValue();
    }

    cout << "Making Summary\n";
    FILE* summary = fopen("macro_summ.txt", "a+");

    time_t t = time(NULL);
    char* c_time_string = std::ctime(&t);
    char mytime[20];
    std::strncpy(mytime, c_time_string, 19);
    mytime[ std::strlen(mytime) - 1 ] = '\0';

    std::fprintf(summary, "%s,%s,%i,%e,%e,%i,%e,%i,%e\n", mytime, filename,
            nevents, ein, eout, ndepos, deposum, nsecs, secsum);
    fclose(summary);

    cout << "Making Histogram\n";

    nevents = secs->GetEntries();
    TH1F* secondarieslow = new TH1F("secondarieslow", "Secondary Electrons <1 keV", 900, 0.000100, 0.001000);
    TH1F* secondariesmid = new TH1F("secondariesmid", "Secondary Electrons 1<E<10 keV", 900, 0.001, 0.010);
    TH1F* secondarieshigh = new TH1F("secondarieshigh", "Secondary Electrons 10<E<100 keV", 900, 0.010, 0.100);
    TH1F* secondariesveryhigh = new TH1F("secondariesveryhigh", "Secondary Electrons 100keV<E<3 MeV", 2902, 0.100, 3.002);

    for (int ii = 0; ii<nevents; ii++)
    {
        secs->GetEntry(ii);
        double en = secsenergy->GetValue();
        if (en < 0.001) secondarieslow->Fill(en, 1);
        else if (en < 0.01) secondariesmid->Fill(en, 0.1);
        else if (en < 0.1) secondarieshigh->Fill(en, 0.01);
        else secondariesveryhigh->Fill(en, 0.001);

    }

    char suffix[] = ".secondaryhisto";

    cout << "Making Filenames\n";


    char* outfilename = (char*) malloc(std::strlen(filename) + std::strlen(suffix) + 1);
    std::strcpy(outfilename, filename);
    std::strcat(outfilename, suffix);

    std::printf("Saving Histogram: %s \n", outfilename);
    h12ascii(secondarieslow, secondariesmid, secondarieshigh, secondariesveryhigh, outfilename);

    return 0;

}

