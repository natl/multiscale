#include "MicroEventInformation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroEventInformation::MicroEventInformation(G4int x, G4int y, G4int z)
:G4VUserEventInformation(),
    xrpt(x),
    yrpt(y),
    zrpt(z)
{
    //ctor
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroEventInformation::~MicroEventInformation()
{
    //dtor
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroEventInformation::Print() const
{
    G4cout << "x: " << xrpt << " y: " << yrpt << " z: " << zrpt << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
