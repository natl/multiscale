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
/// \file multiscale/micro/src/MacroPrimaryGeneratorMessenger.cc
/// \brief Implementation of the MacroPrimaryGeneratorMessenger class
//
// $Id: MacroPrimaryGeneratorMessenger.cc 67268 2013-02-13 11:38:40Z ihrivnac $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "MacroPrimaryGeneratorMessenger.hh"
#include "G4UIdirectory.hh"
#include "MacroPrimaryGeneratorAction.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MacroPrimaryGeneratorMessenger::MacroPrimaryGeneratorMessenger(
    MacroPrimaryGeneratorAction* generator):
    G4UImessenger(),
    fpGenerator(generator),
    fpSourceDir(0),
    fpSourceGeomCmd(0),
    fpSourceRadCmd(0),
    fpSourceHalfXCmd(0),
    fpSourceHalfYCmd(0),
    fpSourceHalfZCmd(0),
    fpRotationDirectionCmd(0),
    fpRotationAngleCmd(0),
    fpCentreCmd(0),
    fpSourceDistCmd(0),
    fpSourceEnergyCmd(0),
    fpSourceHistCmd(0),
    fpSourceParticleTypeCmd(0),
    fpSourceAngDistCmd(0),
    fpSourceEmitDirCmd(0),
    fpSourceEneAngFileCmd(0),
    fpMuonCmd(0),
    fpSourceInitialiseCmd(0)
{
    fpSourceDir = new G4UIdirectory("/macro/source/");
    fpSourceDir->SetGuidance("source control");

    fpSourceGeomCmd = new G4UIcmdWithAString("/macro/source/geometry", this);
    fpSourceGeomCmd->SetGuidance("Source Geometry");
    fpSourceGeomCmd->SetGuidance("Options are: sphere box plane point");
    fpSourceGeomCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSourceRadCmd = new G4UIcmdWithADoubleAndUnit("/macro/source/rad", this);
    fpSourceRadCmd->SetGuidance("Source Radius");
    fpSourceRadCmd->SetGuidance("Double and a unit");
    fpSourceRadCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSourceHalfXCmd = new G4UIcmdWithADoubleAndUnit("/macro/source/halfx", this);
    fpSourceHalfXCmd->SetGuidance("Source half x length");
    fpSourceHalfXCmd->SetGuidance("Double and a unit");
    fpSourceHalfXCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSourceHalfYCmd = new G4UIcmdWithADoubleAndUnit("/macro/source/halfy", this);
    fpSourceHalfYCmd->SetGuidance("Source half y length");
    fpSourceHalfYCmd->SetGuidance("Double and a unit");
    fpSourceHalfYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSourceHalfZCmd = new G4UIcmdWithADoubleAndUnit("/macro/source/halfz", this);
    fpSourceHalfZCmd->SetGuidance("Source half z length");
    fpSourceHalfZCmd->SetGuidance("Double and a unit");
    fpSourceHalfZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpRotationDirectionCmd = new G4UIcmdWith3Vector("/macro/source/rotVector",this);
    fpRotationDirectionCmd->SetGuidance("Vector around which to rotate geometry");
    fpRotationDirectionCmd->SetParameterName("x", "y", "z", false, false);
    fpRotationDirectionCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpRotationAngleCmd = new G4UIcmdWithADouble("/macro/source/rotAngle", this);
    fpRotationAngleCmd->SetGuidance("Angle to rotate source around rotationVector");
    fpRotationAngleCmd->SetGuidance("Specify Angle in degrees");
    fpRotationAngleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpCentreCmd = new G4UIcmdWith3VectorAndUnit("/macro/source/centre", this);
    fpCentreCmd->SetGuidance("Set the source center");
    fpCentreCmd->SetParameterName("xdir", "ydir", "zdir", false, false);
    fpCentreCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSourceDistCmd = new G4UIcmdWithAString("/macro/source/dist", this);
    fpSourceDistCmd->SetGuidance("Source Energy Distribution");
    fpSourceDistCmd->SetGuidance("Options are: mono hist");
    fpSourceDistCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSourceEnergyCmd = new G4UIcmdWithADoubleAndUnit("/macro/source/energy", this);
    fpSourceEnergyCmd->SetGuidance("Source Energy");
    fpSourceEnergyCmd->SetGuidance("Double and a unit");
    fpSourceEnergyCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSourceHistCmd = new G4UIcmdWithAString("/macro/source/histPoint", this);
    fpSourceHistCmd->SetGuidance("Histogram point");
    fpSourceHistCmd->SetGuidance("(bin max (MeV), bin weight)");
    fpSourceHistCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSourceParticleTypeCmd = new G4UIcmdWithAString("/macro/source/particle", this);
    fpSourceParticleTypeCmd->SetGuidance("Particle type (string)");
    fpSourceParticleTypeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSourceAngDistCmd = new G4UIcmdWithAString("/macro/source/angDist", this);
    fpSourceAngDistCmd->SetGuidance("Source Angular Distribution");
    fpSourceAngDistCmd->SetGuidance("Options are: iso cos uniform");
    fpSourceAngDistCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSourceEmitDirCmd = new G4UIcmdWith3Vector("/macro/source/direction",this);
    fpSourceEmitDirCmd->SetGuidance("Set the uniform direction of the particles");
    fpSourceEmitDirCmd->SetParameterName("xdir", "ydir", "zdir", false, false);
    fpSourceEmitDirCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSourceEneAngFileCmd = new G4UIcmdWithAString("/macro/source/energyAngleFile",this);
    fpSourceEneAngFileCmd->SetGuidance("File containing initial energy (MeV) and angle (degrees) for directional sources");
    fpSourceEneAngFileCmd->SetGuidance("Angle is measured from the vector (0,0,-1)");
    fpSourceEneAngFileCmd->SetParameterName("filename", false);
    fpSourceEneAngFileCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpMuonCmd = new G4UIcmdWithABool("/macro/source/isMuon", this);
    fpMuonCmd->SetGuidance("true to generate both mu- and mu+");
    fpMuonCmd->SetParameterName("boolean isMuon", false);
    fpMuonCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSourceInitialiseCmd = new G4UIcmdWithoutParameter("/macro/source/initialize", this);
    fpSourceInitialiseCmd->SetGuidance("Initialise the source");
    fpSourceInitialiseCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MacroPrimaryGeneratorMessenger::~MacroPrimaryGeneratorMessenger()
{
    delete fpSourceGeomCmd;
    delete fpSourceRadCmd;
    delete fpSourceHalfXCmd;
    delete fpSourceHalfYCmd;
    delete fpSourceHalfZCmd;
    delete fpRotationDirectionCmd;
    delete fpRotationAngleCmd;
    delete fpCentreCmd;
    delete fpSourceDistCmd;
    delete fpSourceEnergyCmd;
    delete fpSourceHistCmd;
    delete fpSourceParticleTypeCmd;
    delete fpSourceAngDistCmd;
    delete fpSourceEmitDirCmd;
    delete fpSourceEneAngFileCmd;
    delete fpSourceInitialiseCmd;
    delete fpMuonCmd;
    delete fpSourceDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,
                                               G4String newValue)
{
    if (command == fpSourceGeomCmd)              fpGenerator->SetGeometry(split(newValue, ' ').at(0));
    else if (command == fpSourceRadCmd)          fpGenerator->SetRadius(fpSourceRadCmd->GetNewDoubleValue(newValue));
    else if (command == fpSourceHalfXCmd)        fpGenerator->SetHalfX(fpSourceHalfXCmd->GetNewDoubleValue(newValue));
    else if (command == fpSourceHalfYCmd)        fpGenerator->SetHalfY(fpSourceHalfYCmd->GetNewDoubleValue(newValue));
    else if (command == fpSourceHalfZCmd)        fpGenerator->SetHalfZ(fpSourceHalfZCmd->GetNewDoubleValue(newValue));
    else if (command == fpRotationDirectionCmd)  fpGenerator->SetRotDirection(fpRotationDirectionCmd->GetNew3VectorValue(newValue));
    else if (command == fpRotationAngleCmd)      fpGenerator->SetRotAngle(fpRotationAngleCmd->GetNewDoubleValue(newValue));
    else if (command == fpCentreCmd)             fpGenerator->SetCentre(fpCentreCmd->GetNew3VectorValue(newValue));
    else if (command == fpSourceDistCmd)         fpGenerator->SetEnergyDist(newValue);
    else if (command == fpSourceEnergyCmd)       fpGenerator->SetEnergy(fpSourceEnergyCmd->GetNewDoubleValue(newValue));
    else if (command == fpSourceHistCmd)         fpGenerator->AddEnergyHisto(std::stod(split(newValue, ' ').at(0)), std::stod(split(newValue, ' ').at(1)));
    else if (command == fpSourceParticleTypeCmd) fpGenerator->SetParticleType(newValue);
    else if (command == fpSourceAngDistCmd)      fpGenerator->SetAngleDist(newValue);
    else if (command == fpSourceEmitDirCmd)      fpGenerator->SetEmitDirection(fpSourceEmitDirCmd->GetNew3VectorValue(newValue));
    else if (command ==fpSourceEneAngFileCmd)    fpGenerator->SetSpecificAngleEnergyFile(newValue);
    else if (command == fpSourceInitialiseCmd)   fpGenerator->Initialise();
    else if (command == fpMuonCmd)               fpGenerator->SetMuon(fpMuonCmd->GetNewBoolValue(newValue));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
