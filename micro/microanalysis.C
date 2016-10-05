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


void h12ascii (TH1* h, char* filename)
{
    std::cout << "Opening File\n";
    FILE* outfile = std::fopen(filename, "w");
    std::cout << "File Opened\n";

    int n = h->GetNbinsX();
    std::printf("Saving %i bins\n", n);

    sleep(2);
    for (int ii = 1; ii <= n; ii++)
    {
        std::fprintf(outfile, "%g %g\n",
            h->GetBinLowEdge(ii)+h->GetBinWidth(ii)/2,
            h->GetBinContent(ii));
    }
    std::fclose(outfile);
}


int microanalysis(char* filename)
{
    // char* filename = "combined.root";
    // Read in the file

    std::cout << "Reading file\n";
    TFile* f = new TFile(filename);
    TDirectory* hists  = f->GetDirectory("hists;1");
    TDirectory* tuples = f->GetDirectory("tuples;1");

    TTree* cellWall = (TTree*) tuples->GetObjectUnchecked("CellWall;1");
    TLeaf* cwEnergy = cellWall->GetLeaf("Energy_keV");
    TLeaf* cwCellID = cellWall->GetLeaf("CellID");
    TLeaf* cwEventID = cellWall->GetLeaf("EventID");
    TLeaf* cwParticleType = cellWall->GetLeaf("ParticleType");

    TTree* cellInterior = (TTree*) tuples->GetObjectUnchecked("CellInterior;1");
    TLeaf* ciEnergy = cellInterior->GetLeaf("Energy_keV");
    TLeaf* ciCellID = cellInterior->GetLeaf("CellID");
    TLeaf* ciEventID = cellInterior->GetLeaf("EventID");

    TTree* event = (TTree*) tuples->GetObjectUnchecked("Event;1");
    TLeaf* eventInit = event->GetLeaf("Einit_keV");


    // Hits and energy deposit analysis
    std::cout << "Analysing events for energy deposition\n";
    int nhits = cellInterior->GetEntries();
    double deposum = 0;
    for (int ii = 0; ii<nhits; ii++)
    {
        cellInterior->GetEntry(ii);
        deposum+=ciEnergy->GetValue();
    }

    // Get Initial Energy
    std::cout << "Getting Initial Energy" << std::endl;
    int nEvents = event->GetEntries();
    double initsum = 0;
    for (int ii = 0; ii<nEvents; ii++)
    {
//        event->GetEntry(ii); // This line is causing an error
        initsum+=eventInit->GetValue();
    }

    // Process events at cell wall
    std::cout << "Calculating number of entering electrons" << std::endl;
    int nelectrons = cellWall->GetEntries("ParticleType==\"e-\"");
    

    std::cout << "Making Summary\n";
    FILE* summary = fopen("micro_summ.txt", "a+");

    time_t t = time(NULL);
    char* c_time_string = ctime(&t);
    char mytime[20];
    std::strncpy(mytime, c_time_string, 19);
    mytime[ std::strlen(mytime) - 1 ] = '\0';

    fprintf(summary, "%s,%s,%i,%e,%i,%i,%e\n", mytime, filename,
            nEvents, initsum, nelectrons, nhits, deposum);
    fclose(summary);

    std::cout << "Making Histogram\n";

    TH1F* entering = new TH1F("entering", "Electrons entering cells <100 keV", 102, 0, 102);
    for (int ii = 0; ii< (cellWall->GetEntries()); ii++)
    {
        cellWall->GetEntry(ii);
        char* ptype = (char*) cwParticleType->GetValuePointer();
        if (!std::strcmp(ptype, "e-")) entering->Fill(cwEnergy->GetValue());
    }

    char suffix[] = ".cellelectronhisto";

    std::cout << "Making Filenames\n";


    char* outfilename = (char*) malloc(strlen(filename) + strlen(suffix) + 1);
    std::strcpy(outfilename, filename);
    std::strcat(outfilename, suffix);

    printf("Saving Histogram: %s \n", outfilename);
    h12ascii(entering, outfilename);

    return 0;

}

