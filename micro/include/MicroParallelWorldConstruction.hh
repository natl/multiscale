#ifndef PARALLELWORLDCONSTRUCTION_H
#define PARALLELWORLDCONSTRUCTION_H

#include "G4VUserParallelWorld.hh"

#include "G4ThreeVector.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MicroParallelWorldConstruction : public G4VUserParallelWorld
{
    public:
        MicroParallelWorldConstruction(G4String);
        virtual ~MicroParallelWorldConstruction();

        virtual void Construct();

        void SetParaWorldSize(G4ThreeVector p) {fParaWorldSize   = p;};

    protected:

    private:

        G4ThreeVector fParaWorldSize;
};

#endif // PARALLELWORLDCONSTRUCTION_H
