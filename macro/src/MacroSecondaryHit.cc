//
//
//
/******************************************************************************
 *
 * \file
 * \brief
 *
 *****************************************************************************/
#include "MacroSecondaryHit.hh"

G4ThreadLocal G4Allocator<MacroSecondaryHit>* MacroSecondaryHitAllocator=0;

/**
 * \brief Hit object for this simulation
 * A hit contains a reference to the trackID, the interaction process, the
 * particle depositing the energy, a list of secondaries produced and their
 * energies
 */
MacroSecondaryHit::MacroSecondaryHit():
    G4VHit(),
     fEventID(-1),
     fTrackID(-1),
     fParticleName(G4String()),
     fCharge(0),
     fEdep(0),
     fKineticEnergy(-1),
     fPosition(G4ThreeVector()),
     fMomentumDirection(G4ThreeVector())
{
}

MacroSecondaryHit::MacroSecondaryHit(G4int eventID, G4int trackID, G4String particleName, G4double charge, G4double edep, G4double ekin, G4ThreeVector pos, G4ThreeVector dir):
    G4VHit(),
     fEventID(eventID),
     fTrackID(trackID),
     fParticleName(particleName),
     fCharge(charge),
     fEdep(edep),
     fKineticEnergy(ekin),
     fPosition(pos),
     fMomentumDirection(dir)
     {

     }

MacroSecondaryHit::~MacroSecondaryHit()
{

}

/**
 * \brief optional constructor building from another event
 */
MacroSecondaryHit::MacroSecondaryHit(const MacroSecondaryHit& right):
    G4VHit()
{
    this->SetEventID      (right.GetEventID());
    this->SetTrackID      (right.GetTrackID());
    this->SetParticleName (right.GetParticleName());
    this->SetCharge       (right.GetCharge());
    this->SetEdep         (right.GetEdep());
    this->SetKineticEnergy(right.GetKineticEnergy());
    this->SetPosition     (right.GetPosition());
    this->SetMomentum     (right.GetMomentum());
}


const MacroSecondaryHit& MacroSecondaryHit::operator=(const MacroSecondaryHit& right){
    this->SetEventID      (right.GetEventID());
    this->SetTrackID      (right.GetTrackID());
    this->SetParticleName (right.GetParticleName());
    this->SetCharge       (right.GetCharge());
    this->SetEdep         (right.GetEdep());
    this->SetKineticEnergy(right.GetKineticEnergy());
    this->SetPosition     (right.GetPosition());
    this->SetMomentum     (right.GetMomentum());

    return *this;
}

G4int MacroSecondaryHit::operator==(const MacroSecondaryHit& right) const{
    return (this == &right) ? 1 : 0;
}
