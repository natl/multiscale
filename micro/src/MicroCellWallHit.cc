/******************************************************************************
 *
 * \file MicroCellWallHit.cc
 * \brief
 *
 *****************************************************************************/
#include "MicroCellWallHit.hh"


#include "G4UnitsTable.hh"

G4ThreadLocal G4Allocator<MicroCellWallHit>* MicroCellWallHitAllocator=0;

/**
 * \brief Hit object for this simulation
 * A hit contains a reference to the cellID, the interaction process, the
 * particle depositing the energy, a list of secondaries produced and their
 * energies
 */
MicroCellWallHit::MicroCellWallHit():
    G4VHit(),
     fEventID(-1),
     fCellID(-1),
     fEnergy(0),
     fPosition(G4ThreeVector()),
     fMomentumDirection(G4ThreeVector()),
     fParticleName(G4String())

{
    
}

MicroCellWallHit::MicroCellWallHit(G4int eventID, G4int cellID, G4double energy, G4ThreeVector pos, G4ThreeVector dir, G4String particleName)
:G4VHit(),
fEventID(eventID),
fCellID(cellID),
fEnergy(energy),
fPosition(pos),
fMomentumDirection(dir),
fParticleName(particleName)
{

}

MicroCellWallHit::~MicroCellWallHit()
{

}

/**
 * \brief optional constructor building from another event
 */
MicroCellWallHit::MicroCellWallHit(const MicroCellWallHit& right):
    G4VHit()
{
    this->SetEventID      (right.GetEventID());
    this->SetCellID       (right.GetCellID());
    this->SetEnergy       (right.GetEnergy());
    this->SetPosition     (right.GetPosition());
    this->SetMomDirection (right.GetMomDirection());
    this->SetParticleName (right.GetParticleName());
}


const MicroCellWallHit& MicroCellWallHit::operator=(const MicroCellWallHit& right){
    this->SetEventID      (right.GetEventID());
    this->SetCellID       (right.GetCellID());
    this->SetEnergy       (right.GetEnergy());
    this->SetPosition     (right.GetPosition());
    this->SetMomDirection (right.GetMomDirection());
    this->SetParticleName (right.GetParticleName());

    return *this;
}

G4int MicroCellWallHit::operator==(const MicroCellWallHit& right) const{
    return (this == &right) ? 1 : 0;
}
