//
// analysis.C
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


int analysis(char* filename)
{
    // char* filename = "combined.root";
    // Read in the file
    TFile* f = new TFile(filename);
    TDirectory* hists  = f->GetDirectory("hists;1");
    TDirectory* tuples = f->GetDirectory("tuples;1");

    TTree* accum = tuples->GetObjectUnchecked("AccumulatedEnergy;1");
    TLeaf* einit = accum->GetLeaf("Einit");
    TLeaf* edepo = accum->GetLeaf("Edep");

    TTree* edeps = tuples->GetObjectUnchecked("EnergyDepositions;1");

    TTree* secs  = tuples->GetObjectUnchecked("SecondarySpectrum;1");
    TLeaf* secsenergy = secs->GetLeaf("KineticEnergy");

    double ein = 0;
    double eout = 0;



    int nevents = accum->GetEntries();
    cout << "Making Summary\n";
    FILE* summary = fopen("summ.txt", "a+");

    for (int ii = 0; ii<nevents; ii++)
    {
        accum->GetEntry(ii);
        ein+=einit->GetValue();
        eout+=edepo->GetValue();
    }

    FILE* summary = fopen("summ.txt", "a+");

    time_t t = time(NULL);
    char* c_time_string = ctime(&t);
    char* mytime[20];
    strncpy(mytime, c_time_string, 19);
    mytime[ strlen(mytime) - 1 ] = '\0';

    fprintf(summary, "%s,%s,%e,%e\n", mytime, filename, ein, eout);//*c_time_string, *filename, ein, eout);
    //cout << *c_time_string << "," << *filename << "," << ein << "," << eout << "\n";
    fclose(summary);

    // Print out or save these values
    cout << "Making Histogram\n";

    nevents = secs->GetEntries();
    TH1F* secondaries = new TH1F("secondaries", "Secondary Electrons <100 keV", 92, 0.010, 0.102);

    for (int ii = 0; ii<nevents; ii++)
    {
        secs->GetEntry(ii);
        secondaries->Fill(secsenergy->GetValue());
    }

    char* suffix = ".secondaryhisto";

    cout << "Making Filenames\n";


    char* outfilename = malloc(strlen(filename) + strlen(suffix) + 1);
    strcpy(outfilename, filename);
    strcat(outfilename, suffix);

    printf("Saving Histogram: %s \n", outfilename);
    h12ascii(secondaries, outfilename);

    return 0;

}

void h12ascii (TH1* h, char* filename)
{
    cout << "Opening File\n";
    FILE* outfile = fopen(filename, "w");
    cout << "File Opened\n";

    int n = h->GetNbinsX();
    printf("Saving %i bins\n", n);

    sleep(2);
    for (int ii = 1; ii <= n; ii++)
    {
        cout << ii << " ";
        fprintf(outfile, "%g %g\n",
            h->GetBinLowEdge(ii)+h->GetBinWidth(ii)/2,
            h->GetBinContent(ii));
    }
    fclose(outfile);
}
