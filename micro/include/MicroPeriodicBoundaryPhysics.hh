#ifndef PERIODICBOUNDARYPHYSICS_H
#define PERIODICBOUNDARYPHYSICS_H

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class MicroPeriodicBoundaryPhysics : public G4VPhysicsConstructor
{
    public:
        MicroPeriodicBoundaryPhysics(G4int ver = 1);

        //obsolete
        MicroPeriodicBoundaryPhysics(G4int ver, const G4String& name);

        virtual ~MicroPeriodicBoundaryPhysics();

        virtual void ConstructParticle();
        virtual void ConstructProcess();

    protected:
    private:
        G4int verbose;
};

#endif // PERIODICBOUNDARYPHYSICS_H
