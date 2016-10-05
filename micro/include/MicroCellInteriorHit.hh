#ifndef MICROCELLINTERIORHIT_HH
#define MICROCELLINTERIORHIT_HH


#include "G4VHit.hh"

#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"




class MicroCellInteriorHit : public G4VHit
{
public:
    MicroCellInteriorHit();
    MicroCellInteriorHit(G4int, G4int, G4ThreeVector, G4double, G4double);
    MicroCellInteriorHit(const MicroCellInteriorHit&);
    virtual ~MicroCellInteriorHit();


    //operators
    inline void* operator new(size_t);
    inline void  operator delete(void*);

    const MicroCellInteriorHit& operator=(const MicroCellInteriorHit&);
    G4int operator==(const MicroCellInteriorHit&) const;

    //setters
    void SetEventID  (G4int eventID)     { fEventID  = eventID; };
    void SetCellID   (G4int cellID)      { fCellID   = cellID; };
    void SetPosition (G4ThreeVector pos) { fPosition = pos; };
    void SetEdep     (G4double edep)     { fEdep     = edep; };
    void SetTLength  (G4double tlength)  { fTLength  = tlength; };


    //getters
    G4int           GetEventID  ( ) const {return fEventID; };
    G4int           GetCellID   ( ) const {return fCellID; };
    G4ThreeVector   GetPosition ( ) const {return fPosition; };
    G4double        GetEdep     ( ) const {return fEdep; };
    G4double        GetTLength  ( ) const {return fTLength; };





protected:
private:
    G4int           fEventID;
    G4int           fCellID;
    G4ThreeVector   fPosition;
    G4double        fEdep;
    G4double        fTLength;

};

//typedefs
typedef G4THitsCollection<MicroCellInteriorHit>
    MicroCellInteriorHitsCollection;

//memory management
extern G4ThreadLocal G4Allocator<MicroCellInteriorHit>*
    MicroCellInteriorHitAllocator;

inline void* MicroCellInteriorHit::operator new(size_t)
{
    if(!MicroCellInteriorHitAllocator)
    {
        MicroCellInteriorHitAllocator =
            new G4Allocator<MicroCellInteriorHit>;
    }
    return (void *) MicroCellInteriorHitAllocator->MallocSingle();
}

inline void MicroCellInteriorHit::operator delete(void *hit)
{
    MicroCellInteriorHitAllocator->FreeSingle(
        (MicroCellInteriorHit*) hit);
}


#endif // MICROCELLINTERIORHIT_HH
