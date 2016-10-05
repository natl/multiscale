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
/// \file medical/dna/svalue/src/MacroPhysicsList.cc
/// \brief Implementation of the MacroPhysicsList class
//
// $Id: MacroPhysicsList.cc 85260 2014-10-27 08:53:35Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "MacroPhysicsList.hh"
#include "MacroPhysicsListMessenger.hh"


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
//#include "G4EmDNAPhysics_option2.hh"

// Necessary Hadron Physics
#include "G4EmExtraPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"

// Geant 4

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
// #include "G4UserSpecialCuts.hh"

// particles

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4DNAGenericIonsManager.hh"
#include "G4Electron.hh"

// Managers

#include "G4ProcessManager.hh"
#include "G4EmProcessOptions.hh"
#include "G4PhysicsConstructorRegistry.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MacroPhysicsList::MacroPhysicsList() :
    G4VModularPhysicsList(),
    fMessenger(0)
{
    fMessenger = new MacroPhysicsListMessenger(this);
    SetVerboseLevel(1);


    // Synchrotron Radiation and GN Physics
    RegisterPhysics(new G4EmExtraPhysics(verboseLevel));
    // Register the Hadronic Physics
    RegisterPhysics(new G4DecayPhysics(verboseLevel));
    RegisterPhysics(new G4HadronElasticPhysicsHP(verboseLevel));
    RegisterPhysics(new G4HadronPhysicsQGSP_BIC_HP(verboseLevel));
    RegisterPhysics(new G4StoppingPhysics(verboseLevel) );
    RegisterPhysics(new G4IonPhysics(verboseLevel));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MacroPhysicsList::~MacroPhysicsList()
{
    delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroPhysicsList::ConstructProcess()
{
    // physics constructors
    //
    if(G4VModularPhysicsList::GetPhysics(0) == 0) return;

    // G4ProcessManager* pman = G4Electron::Electron()->GetProcessManager();
    // pman->AddProcess(new G4UserSpecialCuts(), -1, -1, -1);

    G4VModularPhysicsList::ConstructProcess();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroPhysicsList::RegisterConstructor(const G4String& name)
{
    if (verboseLevel > -1)
    {
        G4cout << "MacroPhysicsList::RegisterConstructor: <" << name << ">"
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

void MacroPhysicsList::SetMinEnergyRange(const G4double minEnergy)
{
    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(minEnergy,
        1*GeV);
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
}
