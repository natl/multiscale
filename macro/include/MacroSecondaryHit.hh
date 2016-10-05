#ifndef SENSITIVE_DETECTOR_HIT_HH
#define SENSITIVE_DETECTOR_HIT_HH

//Parent includes
//Parent includes
#include "G4VHit.hh"

//G4 includes
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4UnitsTable.hh"

//external includes

//project includes



class MacroSecondaryHit : public G4VHit
{
public:
    MacroSecondaryHit();
    MacroSecondaryHit(G4int eventID, G4int trackID, G4String particleName, G4double charge, G4double edep, G4double energy, G4ThreeVector pos, G4ThreeVector dir);
    MacroSecondaryHit(const MacroSecondaryHit&);
    virtual ~MacroSecondaryHit();



    //operators
    inline void* operator new(size_t);
    inline void  operator delete(void*);

    const MacroSecondaryHit& operator=(const MacroSecondaryHit&);
    G4int operator==(const MacroSecondaryHit&) const;



    //setters
    void SetEventID         (G4int eventID)     { fEventID = eventID; };
    void SetTrackID         (G4int trackID)     { fTrackID = trackID; };
    void SetParticleName    (G4String name)     {fParticleName = name;};
    void SetCharge          (G4double charge)   {fCharge = charge;};
    void SetEdep            (G4double en)       { fEdep    = en; };
    void SetKineticEnergy   (G4double en)       { fKineticEnergy    = en; };
    void SetPosition        (G4ThreeVector pos) { fPosition   = pos; };
    void SetMomentum        (G4ThreeVector mom) { fMomentumDirection   = mom; };



    //getters
    G4int           GetEventID      ( ) const {return fEventID; };
    G4int           GetTrackID      ( ) const {return fTrackID; };
    G4String        GetParticleName ( ) const {return fParticleName;};
    G4double        GetCharge       ( ) const {return fCharge;};
    G4double        GetEdep         ( ) const {return fEdep; };
    G4double        GetKineticEnergy( ) const {return fKineticEnergy; };
    G4ThreeVector   GetPosition     ( ) const {return fPosition; };
    G4ThreeVector   GetMomentum     ( ) const {return fMomentumDirection; };

private:
    G4int fEventID;
    G4int fTrackID;
    G4String fParticleName;
    G4double fCharge;
    G4double fEdep;
    G4double fKineticEnergy;
    G4ThreeVector fPosition;
    G4ThreeVector fMomentumDirection;



};

//typedefs
typedef G4THitsCollection<MacroSecondaryHit> MacroSecondaryHitsCollection;

//memory management
extern G4ThreadLocal G4Allocator<MacroSecondaryHit>* MacroSecondaryHitAllocator;

inline void* MacroSecondaryHit::operator new(size_t){
    if(!MacroSecondaryHitAllocator){MacroSecondaryHitAllocator = new G4Allocator<MacroSecondaryHit>;}
    return (void *) MacroSecondaryHitAllocator->MallocSingle();
}

inline void MacroSecondaryHit::operator delete(void *hit){
    MacroSecondaryHitAllocator->FreeSingle( (MacroSecondaryHit*) hit);
}

#endif // SENSITIVE_DETECTOR_HIT_HH
