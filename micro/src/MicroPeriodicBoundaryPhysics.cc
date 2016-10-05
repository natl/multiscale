#include "MicroPeriodicBoundaryPhysics.hh"

#include "G4SystemOfUnits.hh"

//Processes to include
#include "MicroPeriodicBoundaryProcess.hh"
#include "G4ParallelWorldProcess.hh"

//Particles to include
#include "G4BaryonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4IonConstructor.hh"


#include "G4BuilderType.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"

//factory
#include "G4PhysicsConstructorFactory.hh"
G4_DECLARE_PHYSCONSTR_FACTORY(MicroPeriodicBoundaryPhysics);

MicroPeriodicBoundaryPhysics::MicroPeriodicBoundaryPhysics(G4int ver)
: G4VPhysicsConstructor("MicroPeriodicBoundaryPhysics"),
verbose(ver)
{
    SetPhysicsType(bUnknown);
}

MicroPeriodicBoundaryPhysics::MicroPeriodicBoundaryPhysics(G4int ver, const G4String&)
: G4VPhysicsConstructor("MicroPeriodicBoundaryPhysics"),
verbose(ver)
{
    SetPhysicsType(bUnknown);
}

MicroPeriodicBoundaryPhysics::~MicroPeriodicBoundaryPhysics()
{
    //dtor
}

void MicroPeriodicBoundaryPhysics::ConstructParticle()
{
    G4BaryonConstructor baryonConstructor;
    baryonConstructor.ConstructParticle();

    G4BosonConstructor bosonConstructor;
    bosonConstructor.ConstructParticle();

    G4LeptonConstructor leptonConstructor;
    leptonConstructor.ConstructParticle();

    G4IonConstructor ionConstructor;
    ionConstructor.ConstructParticle();

}

void MicroPeriodicBoundaryPhysics::ConstructProcess()
{
    if (verbose > -1)
    {
        G4cout << "### MicroPeriodicBoundaryPhysics -> Constructing Processes"
            << G4endl;
    }

    G4ParallelWorldProcess* theParallelWorldProcess = new G4ParallelWorldProcess("ParaWorld");
    theParallelWorldProcess->SetParallelWorld("PeriodicDomain");

    MicroPeriodicBoundaryProcess* theMicroPeriodicBoundaryProcess = new MicroPeriodicBoundaryProcess();

    G4ParticleDefinition* particle = 0;
    G4ProcessManager* pmanager = 0;

    aParticleIterator->reset();

    while ( (*aParticleIterator)() )
    {
        particle = aParticleIterator->value();
        pmanager = particle->GetProcessManager();

        pmanager->AddProcess(theParallelWorldProcess);
        //The parallel world is added following the method in G4ParallelWorld
        if(theParallelWorldProcess->IsAtRestRequired(particle))
        {
            pmanager->SetProcessOrdering(theParallelWorldProcess, idxAtRest, 9999);
        }
        pmanager->SetProcessOrderingToSecond(theParallelWorldProcess, idxAlongStep);
        pmanager->SetProcessOrdering(theParallelWorldProcess, idxPostStep, 9999);

        pmanager->AddProcess(theMicroPeriodicBoundaryProcess);
        pmanager->SetProcessOrderingToLast(theMicroPeriodicBoundaryProcess, idxPostStep);

    }

}







