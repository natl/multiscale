#include "MicroTesterHistoManager.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MicroTesterHistoManager::MicroTesterHistoManager()
{
    fAnalysisManager = G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MicroTesterHistoManager::~MicroTesterHistoManager()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MicroTesterHistoManager::Initialise(){

    //Create Histograms
    G4cout << "Building Histograms" << G4endl;
    fHistoInits = fAnalysisManager->CreateH1("Einit", "InitialEnergies",
                                             1000, 0., 1000.); //in kev
    fHistoDeps  = fAnalysisManager->CreateH1("Edep", "EnergyDeposits",
                                             10000, 0.,  100.); //in kev
    fHistoERad  = fAnalysisManager->CreateH1("EofR", "EnergyWithRadius",
                                             300, 0., 300.); //in um
    fHistoTrack = fAnalysisManager->CreateH1("TLength", "TrackLength",
                                             200, 0., 200.); // in um

    //Create ntuples
    fEvt        = fAnalysisManager->CreateNtuple("MainEvents", "MainEvents");
    fEvtEvtID   = fAnalysisManager->CreateNtupleIColumn(fEvt, "EventID");
    fEvtEInit   = fAnalysisManager->CreateNtupleDColumn(fEvt, "Einit");
    fEvtEDepo   = fAnalysisManager->CreateNtupleDColumn(fEvt, "Edepo");
    fEvtEDiff   = fAnalysisManager->CreateNtupleDColumn(fEvt, "Ediff");
    fAnalysisManager->FinishNtuple(fEvt);

    fPos      = fAnalysisManager->CreateNtuple("Position", "Position");
    fPosEvtID = fAnalysisManager->CreateNtupleIColumn(fPos, "EventID");
    fPosSide  = fAnalysisManager->CreateNtupleSColumn(fPos, "Side");
    fPosErr1  = fAnalysisManager->CreateNtupleDColumn(fPos, "Err1");
    fPosErr2  = fAnalysisManager->CreateNtupleDColumn(fPos, "Err2");
    fAnalysisManager->FinishNtuple(fPos);

    fIAO      = fAnalysisManager->CreateNtuple("InsAndOuts", "InsAndOuts");
    fIAOEvtID = fAnalysisManager->CreateNtupleIColumn(fIAO, "EvtID" );
    fIAOXI    = fAnalysisManager->CreateNtupleIColumn(fIAO, "Xins"  );
    fIAOXO    = fAnalysisManager->CreateNtupleIColumn(fIAO, "Xouts" );
    fIAOXP    = fAnalysisManager->CreateNtupleIColumn(fIAO, "Xpairs");
    fIAOYI    = fAnalysisManager->CreateNtupleIColumn(fIAO, "Yins"  );
    fIAOYO    = fAnalysisManager->CreateNtupleIColumn(fIAO, "Youts" );
    fIAOYP    = fAnalysisManager->CreateNtupleIColumn(fIAO, "Ypairs");
    fIAOZI    = fAnalysisManager->CreateNtupleIColumn(fIAO, "Zins"  );
    fIAOZO    = fAnalysisManager->CreateNtupleIColumn(fIAO, "Zouts" );
    fIAOZP    = fAnalysisManager->CreateNtupleIColumn(fIAO, "Zpairs");
    fAnalysisManager->FinishNtuple(fEvt);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MicroTesterHistoManager::Close()
{
    // Any final management methods
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MicroTesterHistoManager::RecordEvent(G4int eventID, G4double eInit,
    G4double eDepo)
{
    fAnalysisManager->FillNtupleIColumn(fEvt, fEvtEvtID, eventID      );
    fAnalysisManager->FillNtupleDColumn(fEvt, fEvtEInit, eInit        );
    fAnalysisManager->FillNtupleDColumn(fEvt, fEvtEDepo, eDepo        );
    fAnalysisManager->FillNtupleDColumn(fEvt, fEvtEDiff, eInit - eDepo);

    fAnalysisManager->AddNtupleRow(fEvt);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MicroTesterHistoManager::RecordPositionError(G4int eventID, G4String side,
    G4double error1, G4double error2)
{
    fAnalysisManager->FillNtupleIColumn(fPos, fPosEvtID, eventID);
    fAnalysisManager->FillNtupleSColumn(fPos, fPosSide , side   );
    fAnalysisManager->FillNtupleDColumn(fPos, fPosErr1 , error1 );
    fAnalysisManager->FillNtupleDColumn(fPos, fPosErr2 , error2 );

    fAnalysisManager->AddNtupleRow(fPos);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MicroTesterHistoManager::recordInsAndOuts(G4int eventID, G4int xins,
    G4int xouts, G4int xpairs, G4int yins, G4int youts, G4int ypairs,
    G4int zins, G4int zouts, G4int zpairs)
{
    fAnalysisManager->FillNtupleIColumn(fIAO, fIAOEvtID, eventID);
    fAnalysisManager->FillNtupleIColumn(fIAO, fIAOXI   , xins   );
    fAnalysisManager->FillNtupleIColumn(fIAO, fIAOXO   , xouts  );
    fAnalysisManager->FillNtupleIColumn(fIAO, fIAOXP   , xpairs );
    fAnalysisManager->FillNtupleIColumn(fIAO, fIAOYI   , yins   );
    fAnalysisManager->FillNtupleIColumn(fIAO, fIAOYO   , youts  );
    fAnalysisManager->FillNtupleIColumn(fIAO, fIAOYP   , ypairs );
    fAnalysisManager->FillNtupleIColumn(fIAO, fIAOZI   , zins   );
    fAnalysisManager->FillNtupleIColumn(fIAO, fIAOZO   , zouts  );
    fAnalysisManager->FillNtupleIColumn(fIAO, fIAOZP   , zpairs );

    fAnalysisManager->AddNtupleRow(fIAO);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MicroTesterHistoManager::AddToInitHisto(G4double einit){
    fAnalysisManager->FillH1(fHistoInits, einit/keV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MicroTesterHistoManager::AddToEdepHisto(G4double edep){
    fAnalysisManager->FillH1(fHistoDeps, edep/keV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MicroTesterHistoManager::AddToRadiiHisto(G4double radius, G4double edep)
{
    fAnalysisManager->FillH1(fHistoERad, radius/um, edep/keV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MicroTesterHistoManager::AddToTLHisto(G4double tl)
{
    fAnalysisManager->FillH1(fHistoTrack, tl/um);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
