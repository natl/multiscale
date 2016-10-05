#include "MicroCellHistoManager.hh"

#include "MicroAnalysis.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


MicroCellHistoManager::MicroCellHistoManager()
{
    fAnalysisManager = G4AnalysisManager::Instance();
}

MicroCellHistoManager::~MicroCellHistoManager()
{

}


void MicroCellHistoManager::Initialise(){

    // add new units for dose
    const G4double milligray = 1.e-3*gray;
    const G4double microgray = 1.e-6*gray;
    const G4double nanogray  = 1.e-9*gray;
    const G4double picogray  = 1.e-12*gray;

    new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
    new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
    new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
    new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray);

    //Create Histograms
    G4cout << "Building Histograms" << G4endl;
    fHistoCWEl = fAnalysisManager->CreateH1("CellWallSpectElect", "CellWallSpectElect",    1000, 0., 100.*keV, "keV");
    fHistoCWGa = fAnalysisManager->CreateH1("CellWallSpectGamma", "CellWallSpectGamma",    1000, 0., 100.*keV, "keV");
    fHistoCellInt  = fAnalysisManager->CreateH1("CellInt", "EnergyDepositSpect", 1000, 0., 10.00*keV, "keV");
    fHistoEinits   = fAnalysisManager->CreateH1("Einitial", "InitialEnergies",   1000, 0., 100.*keV, "keV");

    //Create ntuples
    G4cout << "Building Cell Wall Tuples" << G4endl;
    fCellWall   = fAnalysisManager->CreateNtuple("CellWall", "CellWall");
    fCWEventID  = fAnalysisManager->CreateNtupleIColumn(fCellWall, "EventID");
    fCWCellID   = fAnalysisManager->CreateNtupleIColumn(fCellWall, "CellID");
    fCWEnergy   = fAnalysisManager->CreateNtupleDColumn(fCellWall, "Energy_keV");
    fCWPosX     = fAnalysisManager->CreateNtupleDColumn(fCellWall, "PosX_um");
    fCWPosY     = fAnalysisManager->CreateNtupleDColumn(fCellWall, "PosY_um");
    fCWPosZ     = fAnalysisManager->CreateNtupleDColumn(fCellWall, "PosZ_um");
    fCWDirX     = fAnalysisManager->CreateNtupleDColumn(fCellWall, "DirX");
    fCWDirY     = fAnalysisManager->CreateNtupleDColumn(fCellWall, "DirY");
    fCWDirZ     = fAnalysisManager->CreateNtupleDColumn(fCellWall, "DirZ");
    fCWParticle = fAnalysisManager->CreateNtupleSColumn(fCellWall,
        "ParticleType");
    fAnalysisManager->FinishNtuple(fCellWall);

    G4cout << "Building CellInterior Tuples" << G4endl;
    fCellInt   = fAnalysisManager->CreateNtuple("CellInterior", "CellInterior");
    fCIEventID = fAnalysisManager->CreateNtupleIColumn(fCellInt, "EventID");
    fCICellID  = fAnalysisManager->CreateNtupleIColumn(fCellInt, "CellID");
    fCIPosX    = fAnalysisManager->CreateNtupleDColumn(fCellInt, "PosX_um");
    fCIPosY    = fAnalysisManager->CreateNtupleDColumn(fCellInt, "PosY_um");
    fCIPosZ    = fAnalysisManager->CreateNtupleDColumn(fCellInt, "PosZ_um");
    fCIEnergy  = fAnalysisManager->CreateNtupleDColumn(fCellInt, "Energy_keV");
    fCITLength  = fAnalysisManager->CreateNtupleDColumn(fCellInt, "TLength_um");
    fAnalysisManager->FinishNtuple(fCellInt);

    G4cout << "Building Event Summary Tuples" << G4endl;
    fEvt       = fAnalysisManager->CreateNtuple("Event", "Event");
    fEvtID     = fAnalysisManager->CreateNtupleIColumn(fEvt, "EventID");
    fEvtEinit  = fAnalysisManager->CreateNtupleDColumn(fEvt, "Einit_keV");
    fEvtPType  = fAnalysisManager->CreateNtupleSColumn(fEvt, "ParticleType");
    fEvtPosX   = fAnalysisManager->CreateNtupleDColumn(fEvt, "PosX_um");
    fEvtPosY   = fAnalysisManager->CreateNtupleDColumn(fEvt, "PosY_um");
    fEvtPosZ   = fAnalysisManager->CreateNtupleDColumn(fEvt, "PosZ_um");
    fEvtDirX   = fAnalysisManager->CreateNtupleDColumn(fEvt, "DirX_um");
    fEvtDirY   = fAnalysisManager->CreateNtupleDColumn(fEvt, "DirY_um");
    fEvtDirZ   = fAnalysisManager->CreateNtupleDColumn(fEvt, "DirZ_um");
    fEvtEdepo  = fAnalysisManager->CreateNtupleDColumn(fEvt, "Edepo_keV");
    fEvtNCells = fAnalysisManager->CreateNtupleIColumn(fEvt, "NCellsHit");
    fAnalysisManager->FinishNtuple(fEvt);

    G4cout << "Building Cell Depo Tuples" << G4endl;
    fCDep       = fAnalysisManager->CreateNtuple("CellDepos", "CellDepos");
    fCDepEvtID  = fAnalysisManager->CreateNtupleIColumn(fCDep, "EventID");
    fCDepCellID = fAnalysisManager->CreateNtupleIColumn(fCDep, "CellID");
    fCDepEinit  = fAnalysisManager->CreateNtupleDColumn(fCDep, "Einit_keV");
    fCDepEdep   = fAnalysisManager->CreateNtupleDColumn(fCDep, "Edepo_keV");
    fAnalysisManager->FinishNtuple(fCDep);

}

void MicroCellHistoManager::Close(){
    // Any final management methods
}


void MicroCellHistoManager::RecordCellWallHit(G4int eventID,
    G4int cellID, G4double energy, G4ThreeVector pos, G4ThreeVector dir,
    G4String particleType)
{
    fAnalysisManager->FillNtupleIColumn(fCellWall, fCWEventID,  eventID);
    fAnalysisManager->FillNtupleIColumn(fCellWall, fCWCellID,   cellID);
    fAnalysisManager->FillNtupleDColumn(fCellWall, fCWEnergy,   energy/keV);
    fAnalysisManager->FillNtupleDColumn(fCellWall, fCWPosX,     pos.getX()/um);
    fAnalysisManager->FillNtupleDColumn(fCellWall, fCWPosY,     pos.getY()/um);
    fAnalysisManager->FillNtupleDColumn(fCellWall, fCWPosZ,     pos.getZ()/um);
    fAnalysisManager->FillNtupleDColumn(fCellWall, fCWDirX,     dir.getX());
    fAnalysisManager->FillNtupleDColumn(fCellWall, fCWDirY,     dir.getY());
    fAnalysisManager->FillNtupleDColumn(fCellWall, fCWDirZ,     dir.getZ());
    fAnalysisManager->FillNtupleSColumn(fCellWall, fCWParticle, particleType);

    fAnalysisManager->AddNtupleRow(fCellWall);

    // Record the spectrum
    if (particleType.compare("e-") == 0) fAnalysisManager->FillH1(fHistoCWEl, energy);
    else if (particleType.compare("gamma") == 0) fAnalysisManager->FillH1(fHistoCWGa, energy);
}

void MicroCellHistoManager::RecordCellInteriorHit(
    G4int eventID, G4int cellID, G4ThreeVector pos, G4double edep, G4double tl)
{
    fAnalysisManager->FillNtupleIColumn(fCellInt, fCIEventID, eventID);
    fAnalysisManager->FillNtupleIColumn(fCellInt, fCICellID,  cellID);
    fAnalysisManager->FillNtupleDColumn(fCellInt, fCIPosX,    pos.getX()/um);
    fAnalysisManager->FillNtupleDColumn(fCellInt, fCIPosY,    pos.getY()/um);
    fAnalysisManager->FillNtupleDColumn(fCellInt, fCIPosZ,    pos.getZ()/um);
    fAnalysisManager->FillNtupleDColumn(fCellInt, fCIEnergy,  edep/keV);
    fAnalysisManager->FillNtupleDColumn(fCellInt, fCITLength, tl/um);

    fAnalysisManager->AddNtupleRow(fCellInt);

    //record the spectrum
    fAnalysisManager->FillH1(fHistoCellInt, edep);
}

void MicroCellHistoManager::RecordEventSummary(G4int eventID, G4double einit, G4String ptype, G4ThreeVector pos, G4ThreeVector dir, G4double edepo, G4int ncells){
    fAnalysisManager->FillNtupleIColumn(fEvt, fEvtID,     eventID);
    fAnalysisManager->FillNtupleDColumn(fEvt, fEvtEinit,  einit/keV);
    fAnalysisManager->FillNtupleSColumn(fEvt, fEvtPType, ptype);
    fAnalysisManager->FillNtupleDColumn(fEvt, fEvtPosX, pos.getX()/um);
    fAnalysisManager->FillNtupleDColumn(fEvt, fEvtPosY, pos.getY()/um);
    fAnalysisManager->FillNtupleDColumn(fEvt, fEvtPosZ, pos.getZ()/um);
    fAnalysisManager->FillNtupleDColumn(fEvt, fEvtDirX, dir.getX());
    fAnalysisManager->FillNtupleDColumn(fEvt, fEvtDirY, dir.getY());
    fAnalysisManager->FillNtupleDColumn(fEvt, fEvtDirZ, dir.getZ());
    fAnalysisManager->FillNtupleDColumn(fEvt, fEvtEdepo,  edepo/keV);
    fAnalysisManager->FillNtupleIColumn(fEvt, fEvtNCells, ncells);
    fAnalysisManager->AddNtupleRow(fEvt);

    fAnalysisManager->FillH1(fHistoEinits, einit);

}

void MicroCellHistoManager::RecordCellDepo(G4int eventID, G4int cellID, G4double einit, G4double edepo)
{
    fAnalysisManager->FillNtupleIColumn(fCDep, fCDepEvtID,  eventID);
    fAnalysisManager->FillNtupleIColumn(fCDep, fCDepCellID, cellID);
    fAnalysisManager->FillNtupleDColumn(fCDep, fCDepEinit,  einit/keV);
    fAnalysisManager->FillNtupleDColumn(fCDep, fCDepEdep,   edepo/keV);

    fAnalysisManager->AddNtupleRow(fCDep);
}
