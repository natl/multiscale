#include "MacroAccumulatorHit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal G4Allocator<MacroAccumulatorHit>* MacroAccumulatorHitAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MacroAccumulatorHit::MacroAccumulatorHit():
G4VHit(),
fEdep(0.)
{
    //ctor
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MacroAccumulatorHit::~MacroAccumulatorHit()
{
    //dtor
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * \brief optional constructor building from another event
 */
MacroAccumulatorHit::MacroAccumulatorHit(const MacroAccumulatorHit& right){
    this->SetEdep( right.GetEdep() );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const MacroAccumulatorHit& MacroAccumulatorHit::operator=(const MacroAccumulatorHit& right){
    this->SetEdep( right.GetEdep() );
    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int MacroAccumulatorHit::operator==(const MacroAccumulatorHit& right) const{
    return (this == &right) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
