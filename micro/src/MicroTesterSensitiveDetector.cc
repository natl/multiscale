//
//
//
/******************************************************************************
 *
 * \file
 * \brief
 *
 *****************************************************************************/
#include "MicroTesterSensitiveDetector.hh"

#include "G4SystemOfUnits.hh"
#include "G4StepStatus.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4SolidStore.hh"
#include "Randomize.hh"

MicroTesterSensitiveDetector::MicroTesterSensitiveDetector(
    const G4String& name,
    const G4String& hcname) :
    G4VSensitiveDetector(name)
{
    collectionName.push_back(hcname);

    // call the DC volume made for this detector rather than the
    // parallel world volume as the PW isn't yet built
    G4Box* repSolid = static_cast<G4Box*> (G4SolidStore::GetInstance()
        ->GetSolid("MainParallel"));
    fMaxX = repSolid->GetXHalfLength();
    fMaxY = repSolid->GetYHalfLength();
    fMaxZ = repSolid->GetZHalfLength();

    fMinX = -fMaxX;
    fMinY = -fMaxY;
    fMinZ = -fMaxZ;
}

MicroTesterSensitiveDetector::~MicroTesterSensitiveDetector()
{

}

void MicroTesterSensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
    // Make  the hits collection
    fMicroTesterHitsCollection = new MicroTesterHitsCollection(
        SensitiveDetectorName,
        collectionName[0]);
    //Add them to the hce
    G4int hcid = G4SDManager::GetSDMpointer()->GetCollectionID(
        collectionName[0]);
    hce->AddHitsCollection(hcid, fMicroTesterHitsCollection);
    //Make the hit object
    MicroTesterHit* newAccHit = new MicroTesterHit();
    //Add them to the collections
    fMicroTesterHitsCollection->insert(newAccHit);

}

G4bool MicroTesterSensitiveDetector::ProcessHits(G4Step* aStep,
    G4TouchableHistory*)
{
    //energyDeposit
    G4double edep = aStep->GetTotalEnergyDeposit();

    G4bool hasDepositedEnergy = (edep != 0. );
    G4bool isEnteringRegion =
        (aStep->GetPreStepPoint()->GetStepStatus() == fUndefined );
    G4bool isLeavingRegion =
        (aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary );

    G4bool recordHitIfTrue = (hasDepositedEnergy || isEnteringRegion ||
        isLeavingRegion);

    // Record longest track, this method is a little clunky, but we're testing
    // so optimisation isn't too important
    if ((*fMicroTesterHitsCollection)[0]->GetTrackLength() <
        aStep->GetTrack()->GetTrackLength())
    {
        (*fMicroTesterHitsCollection)[0]->SetTrackLength(
            aStep->GetTrack()->GetTrackLength());
    }

    if (recordHitIfTrue == false) return false;

    if (hasDepositedEnergy)
    {
        (*fMicroTesterHitsCollection)[0]->AddEdep(edep);
        G4ThreeVector position = aStep->GetPreStepPoint()->GetPosition() +
            G4UniformRand()*(aStep->GetPostStepPoint()->GetPosition() -
                             aStep->GetPreStepPoint()->GetPosition());
        G4double radius = position.mag();
        (*fMicroTesterHitsCollection)[0]->AddEdepAtRadius(radius, edep);

    }

    if (isEnteringRegion)
    {
        //G4cout<<"entering" << G4endl;
        G4ThreeVector pos = aStep->GetPreStepPoint()->GetPosition();
        //work out which wall we are on
        if ( (pos.x() == fMinX) || (pos.x() == fMaxX) )
        {
            (*fMicroTesterHitsCollection)[0]->AddXIn(pos.y(), pos.z());
            //G4cout<<"entering" << G4endl;
        }
        if ( (pos.y() == fMinY) || (pos.y() == fMaxY) )
        {
            (*fMicroTesterHitsCollection)[0]->AddYIn(pos.x(), pos.z());
            //G4cout<<"entering" << G4endl;
        }
        if ( (pos.z() == fMinZ) || (pos.z() == fMaxZ) )
        {
            (*fMicroTesterHitsCollection)[0]->AddZIn(pos.x(), pos.y());
            //G4cout<<"entering" << G4endl;
        }
    }
    if (isLeavingRegion)
    {
        //G4cout<<"leaving" << G4endl;
        G4ThreeVector pos = aStep->GetPostStepPoint()->GetPosition();
        //work out which wall we are on
        if ( (pos.x() == fMinX) || (pos.x() == fMaxX) )
        {
            (*fMicroTesterHitsCollection)[0]->AddXOut(pos.y(), pos.z());
        }
        if ( (pos.y() == fMinY) || (pos.y() == fMaxY) )
        {
            (*fMicroTesterHitsCollection)[0]->AddYOut(pos.x(), pos.z());
        }
        if ( (pos.z() == fMinZ) || (pos.z() == fMaxZ) )
        {
            (*fMicroTesterHitsCollection)[0]->AddZOut(pos.x(), pos.y());
        }
    }


    return true;

}

void MicroTesterSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    (*fMicroTesterHitsCollection)[0]->SetEInit(
        G4EventManager::GetEventManager()->GetConstCurrentEvent()
        ->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy());
}
