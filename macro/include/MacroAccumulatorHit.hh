#ifndef ACCUMULATOR_HIT_HH
#define ACCUMULATOR_HIT_HH

//Parent includes
#include "G4VHit.hh"

//G4 includes
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4UnitsTable.hh"

//external includes
#include <iomanip>



class MacroAccumulatorHit : public G4VHit
{
public:
    MacroAccumulatorHit();
    MacroAccumulatorHit(const MacroAccumulatorHit&);
    virtual ~MacroAccumulatorHit();



    //operators
    inline void* operator new(size_t);
    inline void  operator delete(void*);

    const MacroAccumulatorHit& operator=(const MacroAccumulatorHit&);
    G4int operator==(const MacroAccumulatorHit&) const;

    //methods
    /**
     * \brief add edep to the energy depostion stored in this hit
     * \param const G4double& edep
     */
    void AddEdep (const G4double& edep) { fEdep += edep; };

    //setters
    void SetEdep      (G4double edep)  { fEdep    = edep; };

    //getters
    G4double GetEdep()        const { return fEdep; };


protected:
private:
    G4double        fEdep;
};

//typedefs
typedef G4THitsCollection<MacroAccumulatorHit> MacroAccumulatorHitsCollection;

//memory management
extern G4ThreadLocal G4Allocator<MacroAccumulatorHit>* MacroAccumulatorHitAllocator;

inline void* MacroAccumulatorHit::operator new(size_t){
    if(!MacroAccumulatorHitAllocator) {MacroAccumulatorHitAllocator = new G4Allocator<MacroAccumulatorHit>;}
    return (void *) MacroAccumulatorHitAllocator->MallocSingle();
}

inline void MacroAccumulatorHit::operator delete(void *hit){
    MacroAccumulatorHitAllocator->FreeSingle( (MacroAccumulatorHit*) hit);
}
#endif // ACCUMULATOR_HIT
