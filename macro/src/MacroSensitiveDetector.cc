//
//
//
/******************************************************************************
 *
 * \file
 * \brief
 *
 *****************************************************************************/
#include "MacroSensitiveDetector.hh"

#include "G4AffineTransform.hh"


MacroSensitiveDetector::MacroSensitiveDetector(const G4String& name, G4double en):
G4VSensitiveDetector(name),
fEnThreshold(en)
{
    collectionName.push_back("accumulator");
    collectionName.push_back("secondary");
    collectionName.push_back("energy");
}

MacroSensitiveDetector::~MacroSensitiveDetector()
{

}

void MacroSensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
    //std::cout << std::endl << "********* SD INITIALISING***********" << std::endl;
    // Create Hits Collections
    fAccumulatorHitsCollection = new MacroAccumulatorHitsCollection(SensitiveDetectorName, "accumulator");
    fSecondaryHitsCollection = new MacroSecondaryHitsCollection(SensitiveDetectorName, "secondary");
    fEnergyDepoHitsCollection = new MacroEnergyDepoHitsCollection(SensitiveDetectorName, "energy");

    //Add them to the hce
    G4int hcid = G4SDManager::GetSDMpointer()->GetCollectionID("accumulator");
    hce->AddHitsCollection(hcid, fAccumulatorHitsCollection);

    hcid = G4SDManager::GetSDMpointer()->GetCollectionID("secondary");
    hce->AddHitsCollection(hcid, fSecondaryHitsCollection);

    hcid = G4SDManager::GetSDMpointer()->GetCollectionID("energy");
    hce->AddHitsCollection(hcid, fEnergyDepoHitsCollection);

    //Make the hit object
    MacroAccumulatorHit* newAccHit = new MacroAccumulatorHit();


    //Add them to the collections
    fAccumulatorHitsCollection->insert(newAccHit);

}

G4bool MacroSensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*){
    //energyDeposit
    G4double edep = aStep->GetTotalEnergyDeposit();

    G4Track* aTrack = aStep->GetTrack();

    G4AffineTransform preTransform = aStep->GetPreStepPoint()
        ->GetTouchableHandle()->GetHistory()->GetTopTransform();

    const std::vector<const G4Track*>* secondaries =
        aStep->GetSecondaryInCurrentStep();

    G4bool wasOnABoundary =
        (aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary);

    G4bool wasFirstStep =
        ((aStep->GetPreStepPoint()->GetStepStatus() == fUndefined)
        && (aTrack->GetParentID() == 0) );

    G4ThreeVector start = aStep->GetPreStepPoint()->GetPosition();
    G4ThreeVector stop = aStep->GetPostStepPoint()->GetPosition();

    G4ThreeVector location = start + G4UniformRand()*(stop - start);

    location = preTransform.TransformPoint(location);

    G4bool isGammaOrNeutron =
        (!aTrack->GetParticleDefinition()->GetParticleName().compare("gamma")
      || !aTrack->GetParticleDefinition()->GetParticleName().compare("neutron"));

    if (isGammaOrNeutron)
    {
        for (std::vector<const G4Track*>::const_iterator it =
            secondaries->begin(); it != secondaries->end(); it++)
        {
            G4bool SecIsGammaOrNeutron =
            (!(*it)->GetParticleDefinition()->GetParticleName().compare("gamma")
      || !(*it)->GetParticleDefinition()->GetParticleName().compare("neutron"));

            if (((*it)->GetKineticEnergy() <= fEnThreshold)
                && !(SecIsGammaOrNeutron))
            {
                SaveTrack(aStep, (*it), location);
            }
        }
    }

    if ((!isGammaOrNeutron) && (wasFirstStep || wasOnABoundary))
    {
        SaveTrack(aStep, aTrack, preTransform.TransformPoint(start));
    }

    if (edep > 0)
    {
MacroEnergyDepoHit* energyDepoHit = new MacroEnergyDepoHit(
    G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID(),
    aTrack->GetTrackID(),
    aTrack->GetParticleDefinition()->GetParticleName(),
    edep,
    (G4int) secondaries->size(),
    aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName(),
    location);
fEnergyDepoHitsCollection->insert(energyDepoHit);

        (*fAccumulatorHitsCollection)[0]->AddEdep(edep);
    }

    return true;

}

void MacroSensitiveDetector::EndOfEvent(G4HCofThisEvent*){


}

void MacroSensitiveDetector::SaveTrack(G4Step* aStep, const G4Track* aTrack, G4ThreeVector location)
{
    MacroSecondaryHit* secondaryHit = new MacroSecondaryHit(
        G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID(),
        aTrack->GetTrackID(),
        aTrack->GetParticleDefinition()->GetParticleName(),
        aTrack->GetDynamicParticle()->GetCharge(),
        aStep->GetTotalEnergyDeposit(),
        aTrack->GetKineticEnergy(),
        location,
        aTrack->GetMomentumDirection());
    fSecondaryHitsCollection->insert(secondaryHit);
}
