#ifndef MICRO_CELL_WALL_HIT_HH
#define MICRO_CELL_WALL_HIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"


class MicroCellWallHit : public G4VHit
{
public:
    MicroCellWallHit();
    MicroCellWallHit(G4int, G4int, G4double, G4ThreeVector, G4ThreeVector,
        G4String);
    MicroCellWallHit(const MicroCellWallHit&);
    virtual ~MicroCellWallHit();

    //operators
    inline void* operator new(size_t);
    inline void  operator delete(void*);

    const MicroCellWallHit& operator=(const MicroCellWallHit&);
    G4int operator==(const MicroCellWallHit&) const;

    //setters
    void SetEventID         (G4int eventID)     { fEventID = eventID; };
    void SetCellID          (G4int cellID)      { fCellID = cellID; };
    void SetParticleName    (G4String name)     { fParticleName = name;};
    void SetEnergy          (G4double energy)   { fEnergy    = energy; };
    void SetPosition        (G4ThreeVector pos) { fPosition   = pos; };
    void SetMomDirection    (G4ThreeVector mom) { fMomentumDirection = mom; };

    //getters
    G4int           GetEventID      ( ) const {return fEventID; };
    G4int           GetCellID       ( ) const {return fCellID; };
    G4double        GetEnergy       ( ) const {return fEnergy; };
    G4ThreeVector   GetPosition     ( ) const {return fPosition; };
    G4ThreeVector   GetMomDirection ( ) const {return fMomentumDirection; };
    G4String        GetParticleName ( ) const {return fParticleName;};

private:
    G4int fEventID;
    G4int fCellID;
    G4double fEnergy;
    G4ThreeVector fPosition;
    G4ThreeVector fMomentumDirection;
    G4String fParticleName;


};

//typedefs
typedef G4THitsCollection<MicroCellWallHit> MicroCellWallHitsCollection;

//memory management
extern G4ThreadLocal G4Allocator<MicroCellWallHit>* MicroCellWallHitAllocator;

inline void* MicroCellWallHit::operator new(size_t)
{
    if(!MicroCellWallHitAllocator)
    {
        MicroCellWallHitAllocator = new G4Allocator<MicroCellWallHit>;
    }
    return (void *) MicroCellWallHitAllocator->MallocSingle();
}

inline void MicroCellWallHit::operator delete(void *hit){
    MicroCellWallHitAllocator->FreeSingle( (MicroCellWallHit*) hit);
}

#endif // MICRO_CELL_WALL_HIT_HH
