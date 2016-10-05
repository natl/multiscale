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
/// \file MicroDetectorConstruction.cc
/// \brief Implementation of the MicroDetectorConstruction class

#include "MicroDetectorConstruction.hh"

#include "MicroCellSensitiveDetector.hh"
#include "MicroTesterSensitiveDetector.hh"
#include "MicroDetectorMessenger.hh"
#include "MicroTesterMessenger.hh"

#include "G4Box.hh"
#include "G4VSolid.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4Sphere.hh"
#include "G4Ellipsoid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

#include "G4RotationMatrix.hh"
#include "G4RandomDirection.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Transform3D.hh"
#include "Randomize.hh"

#include "G4UserLimits.hh"

// Managers
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4NistManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MicroDetectorConstruction::MicroDetectorConstruction():
    G4VUserDetectorConstruction(),
    fpWaterMaterial(0),
    fpCellularMaterial(0),
    fTestMode(false),
    fParaWorldSize(200.*um),
    fCellNumber(0),
    fCellShape(""),
    fCellSizeX(0),
    fCellSizeY(0),
    fCellSizeZ(0),
    fCellSizeRad(0),
    fCellWallOffset(0),
    fpCellLogical(0),
    fpWaterLogical(0)
{
    fpMicroDetectorMessenger = new MicroDetectorMessenger(this);
    fpMicroTesterMessenger = new MicroTesterMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MicroDetectorConstruction::~MicroDetectorConstruction()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VPhysicalVolume* MicroDetectorConstruction::Construct()

{
    DefineMaterials();
    return ConstructDetector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MicroDetectorConstruction::DefineMaterials()
{
    // Water is defined from NIST material database
    G4NistManager* man = G4NistManager::Instance();

    G4Material* H2O = man->FindOrBuildMaterial("G4_WATER");
    fpWaterMaterial = H2O;

    G4Material* cellular = new G4Material("cellular", 1110*kg/m3, 1);
    cellular->AddMaterial(H2O, 100*perCent);
    fpCellularMaterial = cellular;


    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VPhysicalVolume* MicroDetectorConstruction::ConstructDetector()
{
    // WORLD VOLUME

    G4double worldSizeX = 1.2 * fParaWorldSize.getX();
    G4double worldSizeY = 1.2 * fParaWorldSize.getY();
    G4double worldSizeZ = 1.2 * fParaWorldSize.getZ();

    G4Box* solidWorld = new G4Box("World", // name
        worldSizeX / 2,   // sizex
        worldSizeY / 2,   // sizex
        worldSizeZ / 2);  // sizex

    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, // solid
        fpWaterMaterial, // material
        "World");        // name

    G4PVPlacement* physiWorld = new G4PVPlacement(0, // no rotation
        G4ThreeVector(), // at (0,0,0)
        "World",         //its name
        logicWorld,      //its logical volume
        0,               //its mother  volume
        false,           //no boolean operation
        0);              //copy number

    // Now decide whether we are running testing mode or cell mode

    if (fTestMode == true) ConstructForTesting(logicWorld);
    else ConstructForSimulation(logicWorld);

    logicWorld->SetUserLimits(new G4UserLimits(10*nm));

    fpWaterLogical = logicWorld;

    return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MicroDetectorConstruction::ConstructForTesting(G4LogicalVolume* logicWorld)
{
    G4double repeatRegionX = fParaWorldSize.getX();
    G4double repeatRegionY = fParaWorldSize.getY();
    G4double repeatRegionZ = fParaWorldSize.getZ();

    G4Box* repeatingRegionPhysical = new G4Box( "MainParallel", // name
        0.5*repeatRegionX,   // x dimension
        0.5*repeatRegionY,   // y dimension
        0.5*repeatRegionZ ); // z dimension

    G4LogicalVolume* repeatingRegionLogical = new G4LogicalVolume(
        repeatingRegionPhysical, // solid
        fpWaterMaterial,         // material
        "MainParallel");         // name

    new G4PVPlacement(0,        // no rotation
        G4ThreeVector(),        // at (0,0,0)
        repeatingRegionLogical, // its name
        "MainParallel",         // its logical volume
        logicWorld,    // its mother  volume
        0,                      // no boolean operation
        0);                     // copy number
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MicroDetectorConstruction::ConstructForSimulation(
    G4LogicalVolume* logicWorld)
{

    //Adding the bacteria
    G4double bacteriaX = fCellSizeX;
    G4double bacteriaY = fCellSizeY;
    G4double bacteriaZ = fCellSizeZ;
    G4double bacteriaRad = fCellSizeRad;

    fCellWallOffset = bacteriaX;
    if (bacteriaY > fCellWallOffset)   fCellWallOffset = bacteriaY;
    if (bacteriaZ > fCellWallOffset)   fCellWallOffset = bacteriaZ;
    if (bacteriaRad > fCellWallOffset) fCellWallOffset = bacteriaRad;

    // iterate through possible shapes
    G4VSolid* cellPhysical;
    if (!strcmp(fCellShape, "ellipse"))
    {
        cellPhysical = new G4Ellipsoid("cell",
                                       bacteriaX,
                                       bacteriaY,
                                       bacteriaZ);
    }
    else if (!strcmp(fCellShape, "sphere"))
    {
        cellPhysical = new G4Orb("cell",       // name
                                 bacteriaRad); // radius
    }
    else if (!strcmp(fCellShape, "rod"))
    {
        G4Tubs* rod = new G4Tubs("rod", 0, bacteriaRad, bacteriaX, 0*degree, 360*degree);
        G4Sphere* sphereTop = new G4Sphere("sphereTop", 0, bacteriaRad, 0*degree, 180*degree, 0*degree, 180*degree);
        G4Sphere* sphereBottom = new G4Sphere("sphereBottom", 0, bacteriaRad, 0*degree, 180*degree, 0*degree, 180*degree);

        G4RotationMatrix topRotation = G4RotationMatrix(0, -90*degree, 0);
        G4RotationMatrix bottomRotation = G4RotationMatrix(0, +90*degree, 0);
        G4Transform3D transPosZ = G4Transform3D(topRotation, G4ThreeVector(0, 0, bacteriaX));
        G4UnionSolid* toppedRod = new G4UnionSolid("rod+top", rod, sphereTop, transPosZ);
        G4Transform3D transNegZ = G4Transform3D(bottomRotation, G4ThreeVector(0, 0, -1*bacteriaX));
        cellPhysical = new G4UnionSolid("cell", toppedRod, sphereBottom, transNegZ);
        fCellWallOffset = bacteriaRad + bacteriaX;
    }
    else
    {
        G4cerr << "A cell shape was not specified in the macro"
            << G4endl;
    }

    G4LogicalVolume* cellLogical = new G4LogicalVolume(cellPhysical,
        fpCellularMaterial,
        "cell");

    G4int cellsPlaced = 0;
    while (cellsPlaced < fCellNumber)
    {
        InsertOneCell(cellLogical, logicWorld, cellsPlaced);
        cellsPlaced++;
        if ((cellsPlaced/100)*100 == cellsPlaced)
        {
            G4cout << "Placed " << cellsPlaced << " of " << fCellNumber
                    << " cells." << G4endl;
        }
    }
    cellLogical->SetUserLimits(new G4UserLimits(10*nm));

    fpCellLogical = cellLogical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

/**
 *  Inserts a defined volume randomly in a mother volume, both the position
 *  and orientation are random
 *
 *  @note Runs a loop placing the volume repeatedly until it can do this
 *        without overlaps, do not use
 *        this method with dense systems
 *
 *  @param cellLogical Logical volume of the cell to be placed
 *  @param motherLogical logical volume in which to place the cell
 *  @param copyNumber the iteration of this volume
 *  @return boolean true
 */
G4bool MicroDetectorConstruction::InsertOneCell(G4LogicalVolume* cellLogical,
                                                G4LogicalVolume* motherLogical,
                                                G4int copyNumber)
{
    G4PVPlacement* physicalCell = 0;
    G4bool doesItOverlap = true;

    // Place a cell randomly until a valid position is found
    while (doesItOverlap)
    {
        if (physicalCell) delete (physicalCell); // delete overlapped cell

        // Calculate rotation
        G4RotationMatrix* randOrientation = new G4RotationMatrix(
            G4RandomDirection(),
            twopi*G4UniformRand());
        //randOrientation = new G4RotationMatrix();

        // Calculate Position
        G4double randx = (G4UniformRand()-0.5)*
            (fParaWorldSize.getX() - 2*fCellWallOffset);
        G4double randy = (G4UniformRand()-0.5)*
            (fParaWorldSize.getY() - 2*fCellWallOffset);
        G4double randz = (G4UniformRand()-0.5)*
            (fParaWorldSize.getZ() - 2*fCellWallOffset);
        G4ThreeVector randPosition (randx, randy, randz);

        physicalCell = new G4PVPlacement(randOrientation, // no rotation
                                         randPosition,    // at (0,0,0)
                                         cellLogical,     // its name
                                         "cell",          // its logical volume
                                         motherLogical,   // its mother  volume
                                         false,           // no boolean op
                                         copyNumber);     // copy number

        doesItOverlap = physicalCell->CheckOverlaps();    // Check overlaps
    }

    return true;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MicroDetectorConstruction::ConstructSDandField()
{
    G4SDManager* SDMan = G4SDManager::GetSDMpointer();
    SDMan->SetVerboseLevel(1);

    if (fTestMode)
    {
        // Attach the tester SD
        MicroTesterSensitiveDetector* pTesterDetector =
            new MicroTesterSensitiveDetector("MainParallel",
                                             "MainAccumulator");
        SetSensitiveDetector("MainParallel", pTesterDetector);
    }
    else
    {
        // Attach a SD to the cells
        MicroCellSensitiveDetector* pCellDetector =
            new MicroCellSensitiveDetector("cell");
        SetSensitiveDetector("cell", pCellDetector);
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MicroDetectorConstruction::UpdateGeometry()
{
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    G4RunManager::GetRunManager()->DefineWorldVolume(ConstructDetector());

}
