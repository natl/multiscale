#include "MicroCellInteriorHit.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

G4ThreadLocal G4Allocator<MicroCellInteriorHit>*
    MicroCellInteriorHitAllocator=0;

MicroCellInteriorHit::MicroCellInteriorHit():
G4VHit(),
    fEventID(-1),
    fCellID(-1),
    fPosition(G4ThreeVector()),
    fEdep(0),
    fTLength(0)
{
    //ctor
}

MicroCellInteriorHit::MicroCellInteriorHit(G4int eventID,
    G4int cellID, G4ThreeVector pos, G4double edep, G4double tl):
    G4VHit(),
    fEventID(eventID),
    fCellID(cellID),
    fPosition(pos),
    fEdep(edep),
    fTLength(tl)
{

}



MicroCellInteriorHit::~MicroCellInteriorHit()
{
    //dtor
}

/**
 * \brief optional constructor building from another event
 */
MicroCellInteriorHit::MicroCellInteriorHit(
    const MicroCellInteriorHit& right)
{
    this->SetEventID    (right.GetEventID());
    this->SetCellID     (right.GetCellID());
    this->SetPosition   (right.GetPosition());
    this->SetEdep       (right.GetEdep());
    this->SetTLength    (right.GetTLength());

}


const MicroCellInteriorHit& MicroCellInteriorHit::operator=(
    const MicroCellInteriorHit& right)
{
    this->SetEventID    (right.GetEventID());
    this->SetCellID     (right.GetCellID());
    this->SetPosition   (right.GetPosition());
    this->SetEdep       (right.GetEdep());
    this->SetTLength    (right.GetTLength());

    return *this;
}

G4int MicroCellInteriorHit::operator==(
    const MicroCellInteriorHit& right) const
{
    return (this == &right) ? 1 : 0;
}
