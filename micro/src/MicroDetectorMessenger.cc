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
// This example is provided by the Geant4-DNA collaboration
// Any report or published results obtained using the Geant4-DNA software
// shall cite the following Geant4-DNA collaboration publication:
// Med. Phys. 37 (2010) 4692-4708
// The Geant4-DNA web site is available at http://geant4-dna.org
//
// $Id$
//
/// \file MicroDetectorMessenger.cc
/// \brief Implementation of the MicroDetectorMessenger class

#include "MicroDetectorMessenger.hh"

#include "MicroDetectorConstruction.hh"
#include "MicroParallelWorldConstruction.hh"

#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroDetectorMessenger::MicroDetectorMessenger(MicroDetectorConstruction* Det) :
    G4UImessenger(), fpDetector(Det)
{
    // Size of box
    fpSizeCmd = new G4UIcmdWith3VectorAndUnit("/micro/det/setSize", this);
    fpSizeCmd->SetGuidance("Set the size of the simulation domain");
    fpSizeCmd->SetParameterName("sizex", "sizey", "sizez", false);
    fpSizeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpMacroSizeCmd = new G4UIcmdWith3VectorAndUnit(
        "/micro/det/setMacroSize", this);
    fpMacroSizeCmd->SetGuidance("Set the size of the macroscopic domain.");
    fpMacroSizeCmd->SetGuidance("This is the size of the global co-ordinate ");
    fpMacroSizeCmd->SetGuidance("space, equivalent to the experimental region");
    fpMacroSizeCmd->SetParameterName("macroSizex", "macroSizey", "macroSizez",
        false);
    fpMacroSizeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Cell qty and shape
    fpCellNumberCmd = new G4UIcmdWithAnInteger("/micro/cell/setNumber", this);
    fpCellNumberCmd->SetGuidance("Set the number of cells to be simulated");
    fpCellNumberCmd->SetParameterName("quantity", false);
    fpCellNumberCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpCellShapeCmd = new G4UIcmdWithAString("/micro/cell/setShape", this);
    fpCellShapeCmd->SetGuidance("Set the shape of the cells");
    fpCellShapeCmd->SetGuidance("Supports sphere or ellipse");
    fpCellShapeCmd->SetParameterName("shape", false);
    fpCellShapeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpCellSizeXCmd = new G4UIcmdWithADoubleAndUnit("/micro/cell/setSizeHalfX",
        this);
    fpCellSizeXCmd->SetGuidance("Set the x dimension of the chosen geometry");
    fpCellSizeXCmd->SetParameterName("xdim", false);
    fpCellSizeXCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpCellSizeYCmd = new G4UIcmdWithADoubleAndUnit("/micro/cell/setSizeHalfY",
        this);
    fpCellSizeYCmd->SetGuidance("Set the y dimension of the chosen geometry");
    fpCellSizeYCmd->SetParameterName("ydim", false);
    fpCellSizeYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpCellSizeZCmd = new G4UIcmdWithADoubleAndUnit("/micro/cell/setSizeHalfZ",
        this);
    fpCellSizeZCmd->SetGuidance("Set the y dimension of the chosen geometry");
    fpCellSizeZCmd->SetParameterName("zdim", false);
    fpCellSizeZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpCellSizeRadCmd = new G4UIcmdWithADoubleAndUnit(
        "/micro/cell/setSizeRadius", this);
    fpCellSizeRadCmd->SetGuidance("Set the radius of the spherical geometry");
    fpCellSizeRadCmd->SetParameterName("rad", false);
    fpCellSizeRadCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpUpdateCmd = new G4UIcmdWithoutParameter("/micro/det/update", this);
    fpUpdateCmd->SetGuidance("Update geometry.");
    fpUpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
    fpUpdateCmd->SetGuidance("if you changed geometrical value(s).");
    fpUpdateCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroDetectorMessenger::~MicroDetectorMessenger()
{
    delete fpSizeCmd;
    delete fpMacroSizeCmd;
    delete fpCellNumberCmd;
    delete fpCellShapeCmd;
    delete fpCellSizeXCmd;
    delete fpCellSizeYCmd;
    delete fpCellSizeZCmd;
    delete fpCellSizeRadCmd;
    delete fpUpdateCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroDetectorMessenger::SetNewValue(G4UIcommand* command,
    G4String newValue)
{
    if (command == fpSizeCmd)
    {
        fpDetector->SetParaWorldSize(fpSizeCmd->GetNew3VectorValue(newValue));
        (static_cast<MicroParallelWorldConstruction*> (fpDetector
            ->GetParallelWorld(0)))->SetParaWorldSize(
            fpDetector->GetParaWorldSize());
    }
    else if (command == fpMacroSizeCmd)
        fpDetector->SetMacroWorldSize(fpMacroSizeCmd->GetNew3VectorValue(
            newValue));
    else if (command == fpCellNumberCmd)
        fpDetector->SetCellNumber(fpCellNumberCmd->GetNewIntValue(newValue));
    else if (command == fpCellShapeCmd)
        fpDetector->SetCellShape(newValue);
    else if (command == fpCellSizeXCmd)
        fpDetector->SetCellSizeX(fpCellSizeXCmd->GetNewDoubleValue(newValue));
    else if (command == fpCellSizeYCmd)
        fpDetector->SetCellSizeY(fpCellSizeYCmd->GetNewDoubleValue(newValue));
    else if (command == fpCellSizeZCmd)
        fpDetector->SetCellSizeZ(fpCellSizeZCmd->GetNewDoubleValue(newValue));
    else if (command == fpCellSizeRadCmd)
        fpDetector->SetCellSizeRad(fpCellSizeRadCmd->GetNewDoubleValue(
            newValue));
    else if (command == fpUpdateCmd)
        fpDetector->UpdateGeometry();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
