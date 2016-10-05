//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file medical/dna/svalue/src/MicroPhysicsList.cc
/// \brief Implementation of the MicroPhysicsList class
//
// $Id: MicroPhysicsList.cc 85260 2014-10-27 08:53:35Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "MicroPhysicsList.hh"
#include "MicroPhysicsListMessenger.hh"
#include "MicroPeriodicBoundaryPhysics.hh"


// Physics Lists available
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmDNAPhysics.hh"
#include "G4EmDNAPhysics_option1.hh"
#include "G4EmDNAPhysics_option2.hh"

// Required for Hadron Physics
#include "G4EmExtraPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"

// Geant 4 Components
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

// particles
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4DNAGenericIonsManager.hh"

// Managers
#include "G4ProcessManager.hh"
#include "G4EmProcessOptions.hh"
#include "G4PhysicsConstructorRegistry.hh"
#include "G4RunManager.hh"

// User Limits
#include "G4StepLimiterPhysics.hh"
// #include "G4PhysicsListHelper.hh"
// #include "G4UserSpecialCuts.hh"
// #include "G4Electron.hh"
// #include "G4Proton.hh"
// #include "G4MuonPlus.hh"
// #include "G4MuonMinus.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroPhysicsList::MicroPhysicsList(G4bool withHadronic) :
    G4VModularPhysicsList(),
    fMessenger(0)
{
    fMessenger = new MicroPhysicsListMessenger(this);
    SetVerboseLevel(1);

    if (withHadronic)
    {
        // Synchrotron Radiation and GN Physics
        RegisterPhysics(new G4EmExtraPhysics(verboseLevel) );

        // Register the Hadronic Physics
        RegisterPhysics(new G4DecayPhysics(verboseLevel));
        RegisterPhysics(new G4HadronElasticPhysicsHP(verboseLevel));
        RegisterPhysics(new G4HadronPhysicsQGSP_BIC_HP(verboseLevel));
        RegisterPhysics(new G4StoppingPhysics(verboseLevel));
        RegisterPhysics(new G4IonPhysics(verboseLevel));
    }
    // Register the periodic boundary physics
    RegisterPhysics(new G4StepLimiterPhysics());
    RegisterPhysics(new MicroPeriodicBoundaryPhysics(verboseLevel));


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroPhysicsList::~MicroPhysicsList()
{
    delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroPhysicsList::ConstructProcess()
{
    // physics constructors
    //
    if(G4VModularPhysicsList::GetPhysics(0) == 0) return;

    G4VModularPhysicsList::ConstructProcess();

    // User Limits
    // G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
    // ph->RegisterProcess(new G4UserSpecialCuts(),
    //     G4Electron::ElectronDefinition());
    // ph->RegisterProcess(new G4UserSpecialCuts(),
    //     G4Proton::ProtonDefinition());
    // ph->RegisterProcess(new G4UserSpecialCuts(),
    //     G4MuonMinus::MuonMinusDefinition());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroPhysicsList::RegisterConstructor(const G4String& name)
{
    if (verboseLevel > -1)
    {
        G4cout << "MicroPhysicsList::RegisterConstructor: <" << name << ">"
               << G4endl;
    }

    if (name == fEmName) return;

    fEmName = name;
    G4VPhysicsConstructor* physics = G4PhysicsConstructorRegistry::Instance()->
        GetPhysicsConstructor(name);

    RegisterPhysics(physics);

    if(physics)
    {
        physics->ConstructParticle();
    }

    G4RunManager::GetRunManager()->PhysicsHasBeenModified();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroPhysicsList::SetMinEnergyRange(const G4double minEnergy)
{
    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(minEnergy,
        1*GeV);
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
}
