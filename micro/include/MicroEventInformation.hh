#ifndef MICROEVENTINFORMATION_HH
#define MICROEVENTINFORMATION_HH

#include "G4VUserEventInformation.hh"

#include "globals.hh"
#include <iostream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MicroEventInformation : public G4VUserEventInformation
{
    public:
        MicroEventInformation(G4int, G4int, G4int);
        virtual ~MicroEventInformation();
        void Print() const;

        G4int getX() {return xrpt;};
        G4int getY() {return yrpt;};
        G4int getZ() {return zrpt;};

    protected:
    private:
        G4int xrpt, yrpt, zrpt;
};

#endif // MICROPRIMARYVERTEXINFORMATION_HH
