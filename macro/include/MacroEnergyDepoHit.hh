#ifndef ENERGYDEPOHIT_HH
#define ENERGYDEPOHIT_HH

//Parent includes
#include "G4VHit.hh"

//G4 includes
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4UnitsTable.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

//external includes
#include <iomanip>



class MacroEnergyDepoHit : public G4VHit
{
public:
    MacroEnergyDepoHit();
    MacroEnergyDepoHit(G4int eventID, G4int trackID, G4String ptype, G4double edep, G4int nsecs, G4String processName, G4ThreeVector pos);
    MacroEnergyDepoHit(const MacroEnergyDepoHit&);
    virtual ~MacroEnergyDepoHit();


    //operators
    inline void* operator new(size_t);
    inline void  operator delete(void*);

    const MacroEnergyDepoHit& operator=(const MacroEnergyDepoHit&);
    G4int operator==(const MacroEnergyDepoHit&) const;

    //setters
    void SetEventID         (G4int eventID)         { fEventID     = eventID; };
    void SetTrackID         (G4int trackID)         { fTrackID     = trackID; };
    void SetParticleType    (G4String ptype)        { fParticleType= ptype; };
    void SetEdep            (G4double edep)         { fEdep        = edep; };
    void SetNsecs           (G4int nsecs  )         { fNsecs       = nsecs; };
    void SetProcessName     (G4String name)         { fProcessName = name; };
    void SetPosition        (G4ThreeVector pos)     { fPosition    = pos; };

    //getters
    G4int           GetEventID      ( ) const {return fEventID; };
    G4int           GetTrackID      ( ) const {return fTrackID; };
    G4String        GetParticleType ( ) const {return fParticleType; };
    G4double        GetEdep         ( ) const {return fEdep; };
    G4int           GetNsecs        ( ) const {return fNsecs; };
    G4String        GetProcessName  ( ) const {return fProcessName; };
    G4ThreeVector   GetPosition     ( ) const {return fPosition; };




protected:
private:
    G4int           fEventID;
    G4int           fTrackID;
    G4String        fParticleType;
    G4double        fEdep;
    G4int           fNsecs;
    G4String        fProcessName;
    G4ThreeVector   fPosition;
};

//typedefs
typedef G4THitsCollection<MacroEnergyDepoHit> MacroEnergyDepoHitsCollection;

//memory management
extern G4ThreadLocal G4Allocator<MacroEnergyDepoHit>* MacroEnergyDepoHitAllocator;

inline void* MacroEnergyDepoHit::operator new(size_t){
    if(!MacroEnergyDepoHitAllocator) {MacroEnergyDepoHitAllocator = new G4Allocator<MacroEnergyDepoHit>;}
    return (void *) MacroEnergyDepoHitAllocator->MallocSingle();
}

inline void MacroEnergyDepoHit::operator delete(void *hit){
    MacroEnergyDepoHitAllocator->FreeSingle( (MacroEnergyDepoHit*) hit);
}


#endif // ENERGYDEPOHIT_HH
