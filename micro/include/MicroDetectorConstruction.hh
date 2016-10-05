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
/// \file MicroDetectorConstruction.hh
/// \brief Definition of the MicroDetectorConstruction class

#ifndef MICRO_DETECTOR_CONSTRUCTION_HH
#define MICRO_DETECTOR_CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "MicroParallelWorldConstruction.hh"

#include "G4Material.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class MicroDetectorMessenger;
class MicroTesterMessenger;
class G4LogicalVolume;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class MicroDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    MicroDetectorConstruction();

    virtual ~MicroDetectorConstruction();
    virtual G4VPhysicalVolume* Construct();

    void ConstructSDandField();
    void UpdateGeometry();
    void ConstructForTesting(G4LogicalVolume*);
    void ConstructForSimulation(G4LogicalVolume*);
    G4bool InsertOneCell(G4LogicalVolume*, G4LogicalVolume*, G4int);

    // Setters for fMicroTesterMessenger
    void SetTestMode () { fTestMode = true; };

    // Setters for fMicroDetectorMessenger
    void SetParaWorldSize  (G4ThreeVector p)
    {
        fParaWorldSize  = p;
        RecalculateNBoxes();
    };

    void SetMacroWorldSize (G4ThreeVector p)
    {
        fMacroWorldSize = p;
        RecalculateNBoxes();
    };

    void SetCellNumber     (G4int    p) {fCellNumber  = p;};
    void SetCellShape      (G4String p) {fCellShape   = p;};
    void SetCellSizeX      (G4double p) {fCellSizeX   = p;};
    void SetCellSizeY      (G4double p) {fCellSizeY   = p;};
    void SetCellSizeZ      (G4double p) {fCellSizeZ   = p;};
    void SetCellSizeRad    (G4double p) {fCellSizeRad = p;};

    G4ThreeVector GetParaWorldSize()  const { return fParaWorldSize; };
    G4ThreeVector GetMacroWorldSize() const { return fMacroWorldSize; };
    G4ThreeVector GetNBoxes() const { return fNBoxes; };

    G4LogicalVolume* GetCellLogical() const { return fpCellLogical; };
    G4LogicalVolume* GetWaterLogical() const { return fpWaterLogical; };

protected:
    void RecalculateNBoxes()
    {
        fNBoxes = G4ThreeVector(std::floor(fMacroWorldSize.getX()
                                /(fParaWorldSize.getX())),
                                std::floor(fMacroWorldSize.getY()
                                /(fParaWorldSize.getY())),
                                std::floor(fMacroWorldSize.getZ()
                                /(fParaWorldSize.getZ())));
    }

private:
    G4Material* fpWaterMaterial;
    G4Material* fpCellularMaterial;

    void DefineMaterials();

    G4VPhysicalVolume* ConstructDetector();

    MicroTesterMessenger*   fpMicroTesterMessenger;
    G4bool   fTestMode;

    MicroDetectorMessenger* fpMicroDetectorMessenger;
    G4ThreeVector fParaWorldSize;
    G4ThreeVector fMacroWorldSize;
    G4ThreeVector fNBoxes;
    G4int    fCellNumber;
    G4String fCellShape;
    G4double fCellSizeX;
    G4double fCellSizeY;
    G4double fCellSizeZ;
    G4double fCellSizeRad;
    G4double fCellWallOffset;

    G4LogicalVolume* fpCellLogical;
    G4LogicalVolume* fpWaterLogical;
};
#endif //MICRO_DETECTOR_CONSTRUCTION_HH
