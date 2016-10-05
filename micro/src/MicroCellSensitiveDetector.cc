//
//
//
/******************************************************************************
 *
 * \file MicroCellSensitiveDetector.cc
 * \brief Implementation of MicroCellSensitiveDetector.cc
 *
 *****************************************************************************/
#include "MicroCellSensitiveDetector.hh"

#include "MicroTrackInformation.hh"
#include "MicroDetectorConstruction.hh"

#include "G4RotationMatrix.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroCellSensitiveDetector::MicroCellSensitiveDetector(const G4String& name):
G4VSensitiveDetector(name)
{
    collectionName.push_back("wall");
    collectionName.push_back("interior");
    fNBoxes = (static_cast<const MicroDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction()))
        ->GetNBoxes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroCellSensitiveDetector::~MicroCellSensitiveDetector()
{
    // dtor
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroCellSensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
    // Create Hits Collections
    fMicroCellWallHitsCollection = new MicroCellWallHitsCollection(
        SensitiveDetectorName, "wall");
    fMicroCellInteriorHitsCollection =
        new MicroCellInteriorHitsCollection(
        SensitiveDetectorName, "interior");

    //Add them to the hce
    G4int hcid = G4SDManager::GetSDMpointer()->GetCollectionID("wall");
    hce->AddHitsCollection(hcid, fMicroCellWallHitsCollection);

    hcid = G4SDManager::GetSDMpointer()->GetCollectionID("interior");
    hce->AddHitsCollection(hcid, fMicroCellInteriorHitsCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MicroCellSensitiveDetector::ProcessHits(G4Step* aStep,
    G4TouchableHistory*)
{
    // Inside a cell, record edep if it occurs, record spectrum if we are on
    // a boundary
    G4double edep = aStep->GetTotalEnergyDeposit();

    G4bool isOnExternalBoundary =
        ((MicroTrackInformation*) aStep->GetTrack()->GetUserInformation())
            ->IsEnteringCell();

    if (isOnExternalBoundary)
    {
        ((MicroTrackInformation*) aStep->GetTrack()->GetUserInformation())
            ->UnsetEnteringCell();
    }

    // Only the first step has a parent of 0 and a status of fNotDefined
    G4bool isFirstStep =
        ( (aStep->GetPreStepPoint()->GetStepStatus() == fUndefined) &&
           (aStep->GetTrack()->GetParentID() == 0) );

    MicroTrackInformation* trackInfo =
        (MicroTrackInformation*) aStep->GetTrack()->GetUserInformation();

    G4int boxid = (fNBoxes.getZ()*fNBoxes.getY()*trackInfo->getX())
        + (fNBoxes.getZ()*trackInfo->getY()) + trackInfo->getZ();

    if (isOnExternalBoundary || edep || isFirstStep)
    {
        //Get Transformations
        const G4RotationMatrix* rot =
            aStep->GetPreStepPoint()->GetPhysicalVolume()->GetFrameRotation();

        G4ThreeVector trans =
            aStep->GetPreStepPoint()->GetPhysicalVolume()->GetFrameTranslation();

        // Point of entry
        G4ThreeVector entryPos = aStep->GetPreStepPoint()->GetPosition();
        entryPos = entryPos + trans;
        entryPos.transform(*rot);

        // Energy depo takes place along step
        G4ThreeVector depoPos = aStep->GetPreStepPoint()->GetPosition() +
                G4UniformRand()*(aStep->GetPostStepPoint()->GetPosition() -
                                 aStep->GetPreStepPoint()->GetPosition());

        depoPos = depoPos + trans;
        depoPos.transform(*rot);

        G4ThreeVector direction =
            aStep->GetPreStepPoint()->GetMomentumDirection();
        direction.transform(*rot);

        //Data for saving
        G4int eventID = G4EventManager::GetEventManager()
            ->GetConstCurrentEvent()->GetEventID();
        G4int cellID = 10000*boxid +
            aStep->GetPreStepPoint()->GetTouchable()->GetCopyNumber();
        G4String particleName =
            aStep->GetTrack()->GetParticleDefinition()->GetParticleName();
        G4double kinEnergy = aStep->GetPreStepPoint()->GetKineticEnergy();

        if (edep)
        {
            //Save the deposit
            MicroCellInteriorHit* cihit =
                new MicroCellInteriorHit(eventID, cellID, depoPos,
                edep, aStep->GetStepLength());
            fMicroCellInteriorHitsCollection->insert(cihit);
        }

        if (isOnExternalBoundary || isFirstStep)
        {   // Entering Cell
            MicroCellWallHit* cwhit = new MicroCellWallHit(eventID, cellID,
                kinEnergy, entryPos, direction, particleName);
            fMicroCellWallHitsCollection->insert(cwhit);
        }
        return true;
    }
    return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroCellSensitiveDetector::EndOfEvent(G4HCofThisEvent*){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
