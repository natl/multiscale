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
/// \file multiscale/micro/src/MicroPhysicsListMessenger.cc
/// \brief Implementation of the MicroPhysicsListMessenger class
//
// $Id: MicroPhysicsListMessenger.cc 82461 2014-06-23 10:44:06Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "MicroPhysicsListMessenger.hh"

#include "MicroPhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroPhysicsListMessenger::MicroPhysicsListMessenger(MicroPhysicsList* pPhys):
    G4UImessenger(),
    fPhysicsList(pPhys),
    fPhysDir(0),
    fListCmd(0),
    fMinEnergyCmd(0)
{
  fPhysDir = new G4UIdirectory("/micro/phys/");
  fPhysDir->SetGuidance("physics list commands");

  fListCmd = new G4UIcmdWithAString("/micro/phys/addPhysics",this);
  fListCmd->SetGuidance("Add modular physics list.");
  fListCmd->SetParameterName("PList",false);
  fListCmd->AvailableForStates(G4State_PreInit);
  fListCmd->SetToBeBroadcasted(false);

  fMinEnergyCmd = new G4UIcmdWithADoubleAndUnit("/micro/phys/minEnergy",this);
  fMinEnergyCmd->SetGuidance("Set the minimum energy production cut");
  fMinEnergyCmd->SetParameterName("energy",false);
  fMinEnergyCmd->AvailableForStates(G4State_PreInit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroPhysicsListMessenger::~MicroPhysicsListMessenger()
{
  delete fListCmd;
  delete fMinEnergyCmd;
  delete fPhysDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroPhysicsListMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if( command == fListCmd )
        fPhysicsList->RegisterConstructor(newValue);
    else if (command == fMinEnergyCmd)
        fPhysicsList->SetMinEnergyRange(fMinEnergyCmd->GetNewDoubleValue(newValue));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
