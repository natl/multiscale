#include "MacroEnergyDepoHit.hh"

G4ThreadLocal G4Allocator<MacroEnergyDepoHit>* MacroEnergyDepoHitAllocator=0;

MacroEnergyDepoHit::MacroEnergyDepoHit():
G4VHit(),
    fEventID(-1),
    fTrackID(-1),
    fParticleType(G4String()),
    fEdep(0),
    fNsecs(-1),
    fProcessName(G4String()),
    fPosition(G4ThreeVector())
{
    //ctor
}

MacroEnergyDepoHit::MacroEnergyDepoHit(G4int eventID, G4int trackID, G4String ptype, G4double edep, G4int nsecs, G4String processName, G4ThreeVector pos):
    G4VHit(),
    fEventID(eventID),
    fTrackID(trackID),
    fParticleType(ptype),
    fEdep(edep),
    fNsecs(nsecs),
    fProcessName(processName),
    fPosition(pos)
    {

    }



MacroEnergyDepoHit::~MacroEnergyDepoHit()
{
    //dtor
}

/**
 * \brief optional constructor building from another event
 */
MacroEnergyDepoHit::MacroEnergyDepoHit(const MacroEnergyDepoHit& right){
    this->SetEventID     (right.GetEventID());
    this->SetTrackID     (right.GetTrackID());
    this->SetParticleType(right.GetParticleType());
    this->SetEdep        (right.GetEdep());
    this->SetNsecs       (right.GetNsecs());
    this->SetProcessName (right.GetProcessName());
    this->SetPosition    (right.GetPosition());
}


const MacroEnergyDepoHit& MacroEnergyDepoHit::operator=(const MacroEnergyDepoHit& right){
    this->SetEventID     (right.GetEventID());
    this->SetTrackID     (right.GetTrackID());
    this->SetParticleType(right.GetParticleType());
    this->SetEdep        (right.GetEdep());
    this->SetNsecs       (right.GetNsecs());
    this->SetProcessName (right.GetProcessName());
    this->SetPosition    (right.GetPosition());
    return *this;
}

G4int MacroEnergyDepoHit::operator==(const MacroEnergyDepoHit& right) const{
    return (this == &right) ? 1 : 0;
}
