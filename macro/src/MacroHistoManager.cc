#include "MacroHistoManager.hh"


MacroHistoManager::MacroHistoManager()
{
    fAnalysisManager = G4AnalysisManager::Instance();
}

MacroHistoManager::~MacroHistoManager()
{
    delete fAnalysisManager;
}


void MacroHistoManager::Initialise(){

    G4cout << "Using " << fAnalysisManager->GetType() << G4endl;

    fAnalysisManager->SetVerboseLevel(1);
    fAnalysisManager->SetFirstHistoId(1);

    //Create Histograms
    fAnalysisManager->CreateH1("TotalDepos", "Edep in well", 3000, 0, 3.0*MeV);
    fAnalysisManager->CreateH1("IndividualDepos", "Individual Energy Deposits",
        3000, 0., 3.0*MeV);
    fAnalysisManager->CreateH1("SavedElectrons", "Saved Electron Energies",
        3000, 0., 3*MeV);
    fAnalysisManager->CreateH1("InitialEn", "Initial Energies",
        3000, 0., 3.0*MeV);
    fAnalysisManager->CreateH3("SpatialEn", "Spatial Energy Distribution",
                               40,  -5,   5,
                               40,  -5,   5,
                               60,  -7.5, 7.5,
                               "mm", "mm", "mm");

    //Create ntuples
    fAccum         = fAnalysisManager->CreateNtuple("AccumulatedEnergy",
        "AccumulatedEnergy");
    fAccumEventID  = fAnalysisManager->CreateNtupleIColumn(fAccum, "EventID");
    fAccumEinit    = fAnalysisManager->CreateNtupleDColumn(fAccum, "Einit");
    fAccumEdep     = fAnalysisManager->CreateNtupleDColumn(fAccum, "Edep");
    fAnalysisManager->FinishNtuple(fAccum);

    fDepos           = fAnalysisManager->CreateNtuple("EnergyDepositions",
        "EnergyDepositions");
    fDeposEventID    = fAnalysisManager->CreateNtupleIColumn(fDepos, "EventID");
    fDeposTrackID    = fAnalysisManager->CreateNtupleIColumn(fDepos, "TrackID");
    fDeposEdep       = fAnalysisManager->CreateNtupleDColumn(fDepos, "Edep");
    fDeposPType    = fAnalysisManager->CreateNtupleSColumn(fDepos,
        "ParticleType");
    fDeposNSavedSecs = fAnalysisManager->CreateNtupleIColumn(fDepos,
        "SavedSecondaries");
    fDeposProcess    = fAnalysisManager->CreateNtupleSColumn(fDepos,
        "ProcessName");
    fDeposPosX       = fAnalysisManager->CreateNtupleDColumn(fDepos, "PosX");
    fDeposPosY       = fAnalysisManager->CreateNtupleDColumn(fDepos, "PosY");
    fDeposPosZ       = fAnalysisManager->CreateNtupleDColumn(fDepos, "PosZ");
    fAnalysisManager->FinishNtuple(fDepos);

    fSecondaries      = fAnalysisManager->CreateNtuple("SecondarySpectrum",
        "SecondarySpectrum");
    fSecondaryEventID = fAnalysisManager->CreateNtupleIColumn(fSecondaries,
        "EventID");
    fSecondaryTrackID = fAnalysisManager->CreateNtupleIColumn(fSecondaries,
        "ParentTrackID");
    fSecondaryType    = fAnalysisManager->CreateNtupleSColumn(fSecondaries,
        "ParticleType");
    fSecondaryCharge  = fAnalysisManager->CreateNtupleDColumn(fSecondaries,
        "Charge");
    fSecondaryEdep    = fAnalysisManager->CreateNtupleDColumn(fSecondaries,
        "EnergyDepo");
    fSecondaryKEnergy = fAnalysisManager->CreateNtupleDColumn(fSecondaries,
        "KineticEnergy");
    fSecondaryPosX    = fAnalysisManager->CreateNtupleDColumn(fSecondaries,
        "PosX");
    fSecondaryPosY    = fAnalysisManager->CreateNtupleDColumn(fSecondaries,
        "PosY");
    fSecondaryPosZ    = fAnalysisManager->CreateNtupleDColumn(fSecondaries,
        "PosZ");
    fSecondaryDirX    = fAnalysisManager->CreateNtupleDColumn(fSecondaries,
        "DirX");
    fSecondaryDirY    = fAnalysisManager->CreateNtupleDColumn(fSecondaries,
        "DirY");
    fSecondaryDirZ    = fAnalysisManager->CreateNtupleDColumn(fSecondaries,
        "DirZ");
    fAnalysisManager->FinishNtuple(fSecondaries);

    //always create the file LAST
    fAnalysisManager->OpenFile("macro.root");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroHistoManager::Close(){
    fAnalysisManager->Write();
    fAnalysisManager->CloseFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroHistoManager::RecordAccumulation(G4int EventID, G4double einit,
    G4double edep)
{
    fAnalysisManager->FillNtupleIColumn(fAccum, fAccumEventID, EventID);
    fAnalysisManager->FillNtupleDColumn(fAccum, fAccumEinit,   einit);
    fAnalysisManager->FillNtupleDColumn(fAccum, fAccumEdep,    edep);
    fAnalysisManager->AddNtupleRow(fAccum);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroHistoManager::RecordEnergyDeposit(G4int eventID, G4int trackID,
    G4String ptype, G4double edep, G4int nsecs, G4String processName,
    G4ThreeVector pos)
{
    fAnalysisManager->FillNtupleIColumn(fDepos, fDeposEventID,    eventID);
    fAnalysisManager->FillNtupleIColumn(fDepos, fDeposTrackID,    trackID);
    fAnalysisManager->FillNtupleSColumn(fDepos, fDeposPType,      ptype);
    fAnalysisManager->FillNtupleDColumn(fDepos, fDeposEdep,       edep);
    fAnalysisManager->FillNtupleIColumn(fDepos, fDeposNSavedSecs, nsecs);
    fAnalysisManager->FillNtupleSColumn(fDepos, fDeposProcess,    processName);
    fAnalysisManager->FillNtupleDColumn(fDepos, fDeposPosX,       pos.getX());
    fAnalysisManager->FillNtupleDColumn(fDepos, fDeposPosY,       pos.getY());
    fAnalysisManager->FillNtupleDColumn(fDepos, fDeposPosZ,       pos.getZ());
    fAnalysisManager->AddNtupleRow(fDepos);
    fAnalysisManager->FillH3(1, pos.getX()/mm, pos.getY()/mm, pos.getZ()/mm,
        edep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroHistoManager::RecordSecondary(G4int eventID, G4int trackID,
    G4String particleName, G4double charge, G4double edep, G4double ekin, G4ThreeVector pos,
    G4ThreeVector dir)
{
    fAnalysisManager->FillNtupleIColumn(fSecondaries, fSecondaryEventID,
        eventID);
    fAnalysisManager->FillNtupleIColumn(fSecondaries, fSecondaryTrackID,
        trackID);
    fAnalysisManager->FillNtupleSColumn(fSecondaries, fSecondaryType,
        particleName);
    fAnalysisManager->FillNtupleDColumn(fSecondaries, fSecondaryCharge,
        charge);
    fAnalysisManager->FillNtupleDColumn(fSecondaries, fSecondaryEdep, edep);
    fAnalysisManager->FillNtupleDColumn(fSecondaries, fSecondaryKEnergy, ekin);
    fAnalysisManager->FillNtupleDColumn(fSecondaries, fSecondaryPosX,
        pos.getX());
    fAnalysisManager->FillNtupleDColumn(fSecondaries, fSecondaryPosY,
        pos.getY());
    fAnalysisManager->FillNtupleDColumn(fSecondaries, fSecondaryPosZ,
        pos.getZ());
    fAnalysisManager->FillNtupleDColumn(fSecondaries, fSecondaryDirX,
        dir.getX());
    fAnalysisManager->FillNtupleDColumn(fSecondaries, fSecondaryDirY,
        dir.getY());
    fAnalysisManager->FillNtupleDColumn(fSecondaries, fSecondaryDirZ,
        dir.getZ());
    fAnalysisManager->AddNtupleRow(fSecondaries);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroHistoManager::AddToAccumulatedEdepHisto(G4double edep){
    fAnalysisManager->FillH1(1, edep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroHistoManager::AddToIndividualEdepHisto(G4double edep){
    fAnalysisManager->FillH1(2, edep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroHistoManager::AddToSecondaryHisto(G4double energy){
    fAnalysisManager->FillH1(3, energy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroHistoManager::AddToInitHisto(G4double einit){
    fAnalysisManager->FillH1(4, einit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
