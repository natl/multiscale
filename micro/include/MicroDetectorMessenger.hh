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
/// \file MicroDetectorMessenger.hh
/// \brief Definition of the MicroDetectorMessenger class

#ifndef MicroDetectorMessenger_h
#define MicroDetectorMessenger_h 1

#include "globals.hh"

#include "G4UImessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class MicroDetectorConstruction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class MicroDetectorMessenger: public G4UImessenger
{
  public:

    MicroDetectorMessenger(MicroDetectorConstruction*);
    virtual ~MicroDetectorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    MicroDetectorConstruction*      fpDetector;
    G4UIcmdWith3VectorAndUnit*      fpSizeCmd;       // The size of the cube
    G4UIcmdWith3VectorAndUnit*      fpMacroSizeCmd;  // Macro World Size
    G4UIcmdWithAnInteger*           fpCellNumberCmd; // Ncells to generate
    G4UIcmdWithAString*             fpCellShapeCmd;  // Shape of cells
    G4UIcmdWithADoubleAndUnit*      fpCellSizeXCmd;  // mag and unit of x dim
    G4UIcmdWithADoubleAndUnit*      fpCellSizeYCmd;  // mag and unit of y dim
    G4UIcmdWithADoubleAndUnit*      fpCellSizeZCmd;  // mage and unit of z dim
    G4UIcmdWithADoubleAndUnit*      fpCellSizeRadCmd;// mag and unit of radius
    G4UIcmdWithoutParameter*        fpUpdateCmd;
};

#endif

