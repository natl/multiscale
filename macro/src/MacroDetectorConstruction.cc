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
// $Id: B1DetectorConstruction.cc 75117 2013-10-28 09:38:37Z gcosmo $
//
/// \file MacroDetectorConstruction.cc
/// \brief Implementation of the MacroDetectorConstruction class

#include "MacroDetectorConstruction.hh"

#include "MacroSensitiveDetector.hh"
#include "MacroDetectorMessenger.hh"

#include "Randomize.hh"
#include "G4Box.hh"
#include "G4Orb.hh"
#include "G4Ellipsoid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4RotationMatrix.hh"
// #include "G4UserLimits.hh"
#include "G4Material.hh"
#include "G4Element.hh"

// Managers
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MacroDetectorConstruction::MacroDetectorConstruction():
    G4VUserDetectorConstruction(),
    fWorldSize(G4ThreeVector(0)),
    fWorldMat("G4_AIR"),
    fSampleXYZ(G4ThreeVector(0)),
    fSamplePos(G4ThreeVector(0)),
    fSampleRad(0),
    fSampleMat("G4_WATER"),
    fSampleShape(""),
    fSampleRotVec(G4ThreeVector(0)),
    fSampleRotAng(0),
    fUseCustomGeom(false),
    fCustomGeomName(""),
    fEnThreshold(100*keV),
    fpWaterMaterial(0),
    fpAirMaterial(0),
    fpPlasticMaterial(0),
    fpWorldMat(0),
    fpSampleMat(0),
    fpDavisMedium(0)
{
    fpMacroDetectorMessenger = new MacroDetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MacroDetectorConstruction::~MacroDetectorConstruction()
{
    delete fpMacroDetectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* MacroDetectorConstruction::Construct()

{
    DefineMaterials();
    return ConstructDetector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroDetectorConstruction::DefineMaterials()
{
    G4double density     = universe_mean_density;     //from PhysicalConstants.h
    G4double pressure    = 1.e-19*pascal;
    G4double temperature = 0.1*kelvin;
    new G4Material("Galactic", 1., 1.01*g/mole, density,
                       kStateGas,temperature,pressure);

    // Water is defined from NIST material database
    G4NistManager* man = G4NistManager::Instance();

    fpWaterMaterial = man->FindOrBuildMaterial("G4_WATER");
    fpAirMaterial = man->FindOrBuildMaterial("G4_AIR");
    fpPlasticMaterial = man->FindOrBuildMaterial("G4_POLYPROPYLENE");

    fpWorldMat = man->FindOrBuildMaterial(fWorldMat);
    fpSampleMat = man->FindOrBuildMaterial(fSampleMat);

    //Davis Medium
    G4Material* water = man->FindOrBuildMaterial("G4_WATER");
    G4Element* elN  = new G4Element("Nitrogen", "N", 14., 14.007*g/mole);
    G4Element* elH  = new G4Element("Hydrogen", "H", 1., 1.008*g/mole);
    G4Element* elS  = new G4Element("Sulfur", "S", 16., 32.06*g/mole);
    G4Element* elO  = new G4Element("Oxygen", "O", 8., 15.999*g/mole);
    G4Element* elK  = new G4Element("Potassium", "K", 19., 39.098*g/mole);
    G4Element* elP  = new G4Element("Phosphorous", "P", 15., 30.973*g/mole);
    G4Element* elMg  = new G4Element("Magnesium", "Mg", 12., 24.305*g/mole);
    G4Element* elNa  = new G4Element("Sodium", "Na", 11., 22.989*g/mole);
    G4Element* elC  = new G4Element("Carbon", "C", 12., 12.011*g/mole);
    fpDavisMedium = new G4Material("DavisMedium", 1000*kg/m3, 10);
    fpDavisMedium->AddMaterial(water, (100-1.0853)*perCent);
    fpDavisMedium->AddElement (elN, 00.0212*perCent);
    fpDavisMedium->AddElement (elH, 00.0163*perCent);
    fpDavisMedium->AddElement (elS, 00.0269*perCent);
    fpDavisMedium->AddElement (elO, 00.443*perCent);
    fpDavisMedium->AddElement (elK, 00.372*perCent);
    fpDavisMedium->AddElement (elP, 00.170*perCent);
    fpDavisMedium->AddElement (elMg, 00.0020*perCent);
    fpDavisMedium->AddElement (elNa, 00.0117*perCent);
    fpDavisMedium->AddElement (elC, 00.0222*perCent);

    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* MacroDetectorConstruction::ConstructDetector()
{
    G4VPhysicalVolume* geom;
    if (fUseCustomGeom){
        if (!fCustomGeomName.compare("microplate")) geom = ConstructMicroplate();
    }
    else {
        geom = BuildUserGeometry();
    }

    return geom;
}


G4VPhysicalVolume* MacroDetectorConstruction::BuildUserGeometry()
{
    G4bool checkOverlaps = true;

    G4Box* solidWorld =
    new G4Box("World",         //its name
              fWorldSize.getX(),  //its size
              fWorldSize.getY(),
              fWorldSize.getZ());

    G4LogicalVolume* logicWorld =
        new G4LogicalVolume(solidWorld,          //its solid
                            fpWorldMat,          //its material
                            "World");            //its name

    G4VPhysicalVolume* physWorld =
        new G4PVPlacement(0,                     //no rotation
                          G4ThreeVector(),       //at (0,0,0)
                          logicWorld,            //its logical volume
                          "World",               //its name
                          0,                     //its mother  volume
                          false,                 //no boolean operation
                          0,                     //copy number
                          checkOverlaps);        //overlaps checking

    G4RotationMatrix* rot = new G4RotationMatrix(fSampleRotVec, fSampleRotAng);

    G4LogicalVolume* logicSample;

    if (!fSampleShape.compare("sphere"))
    {
        G4Orb* solidSample = new G4Orb("Sample", fSampleRad);

        logicSample = new G4LogicalVolume(solidSample,
                                          fpSampleMat,
                                          "SampleLV");
        new G4PVPlacement(rot,
                          fSamplePos,
                          logicSample,
                          "SamplePV",
                          logicWorld,
                          false,
                          0,
                          checkOverlaps);
    }
    else if (!fSampleShape.compare("ellipsoid"))
    {
        G4Ellipsoid* solidSample = new G4Ellipsoid("Sample",
                                                   fSampleXYZ.getX(),
                                                   fSampleXYZ.getY(),
                                                   fSampleXYZ.getZ());

        logicSample = new G4LogicalVolume(solidSample,
                                          fpSampleMat,
                                          "SampleLV");
        new G4PVPlacement(rot,
                          fSamplePos,
                          logicSample,
                          "SamplePV",
                          logicWorld,
                          false,
                          0,
                          checkOverlaps);
    }
    else if (!fSampleShape.compare("box"))
    {
        G4Box* solidSample = new G4Box("Sample",
                                       fSampleXYZ.getX(),
                                       fSampleXYZ.getY(),
                                       fSampleXYZ.getZ());

        logicSample = new G4LogicalVolume(solidSample,
                                          fpSampleMat,
                                          "SampleLV");
        new G4PVPlacement(rot,
                          fSamplePos,
                          logicSample,
                          "SamplePV",
                          logicWorld,
                          false,
                          0,
                          checkOverlaps);
    }
    else
    {
        G4cout << "Invalid shape entered: " << fSampleShape << G4endl;
        G4cout << "Valid shapes are sphere, ellipse, box" << G4endl;
    }

    //logicSample->SetUserLimits(new G4UserLimits(1*nm));

    return physWorld;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VPhysicalVolume* MacroDetectorConstruction::ConstructMicroplate()
{
    std::cout << "===== Constructing Macroplate =====" << std::endl;

    // Option to switch on/off checking of volumes overlaps
    //
    G4bool checkOverlaps = true;

    //
    // World
    //
    G4double world_sizeXY = 0.5*m;
    G4double world_sizeZ  = 0.5*m;

    G4Box* solidWorld =
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size

    G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid
                        fpAirMaterial,       //its material
                        "World");            //its name

    G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking


    //
    // Plastic Box
    //
    G4double plastic_sizeX = 8*11*mm  + 1*mm;
    G4double plastic_sizeY = 12*11*mm + 1*mm;
    G4double plastic_sizeZ = 21*mm;
    //G4ThreeVector posChamber = G4ThreeVector(0, chamber_offsetY, chamber_offsetZ);
    G4ThreeVector posPlastic = G4ThreeVector(0, 0, 0);


    G4Box* solidPlastic =
    new G4Box("Plastic",
        0.5*plastic_sizeX, 0.5*plastic_sizeY, 0.5*plastic_sizeZ);

    G4LogicalVolume* logicPlastic =
    new G4LogicalVolume(solidPlastic,         //its solid
                        fpPlasticMaterial,          //its material
                        "Plastic");           //its name

    new G4PVPlacement(0,                       //no rotation
                    posPlastic,                    //at position
                    logicPlastic,             //its logical volume
                    "Plastic",                //its name
                    logicWorld,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking





    //
    // Empty Holes in Plastic box
    //
    G4double airhole_sizeXY = 10*mm, airhole_sizeZ = 20*mm;
    G4double airhole_offsetZ = 0.5*plastic_sizeZ - 0.5*airhole_sizeZ;
    //G4ThreeVector posSample = G4ThreeVector(0, 0, sample_offsetZ);

    G4Box* solidAirHole =
    new G4Box("AirHole",                                   //its name
              0.5*airhole_sizeXY, 0.5*airhole_sizeXY, 0.5*airhole_sizeZ); //its size

    G4LogicalVolume* logicAirHole =
    new G4LogicalVolume(solidAirHole,         //its solid
                        fpAirMaterial,          //its material
                        "AirHole");           //its name

    // Staggered Holes
    G4int placementCounter = 0;
    for (G4int ii=0;ii<4;ii++){
        for (G4int jj=0;jj<6;jj++){
            G4double airhole_staggerX = ((jj/2)*2 == jj) ? 11*mm : 0*mm;
            G4double airhole_offsetX = -38.5*mm + 22*ii*mm + airhole_staggerX;
            G4double airhole_offsetY = -60.5*mm + 22*jj*mm;
            G4ThreeVector posAirHole = G4ThreeVector(airhole_offsetX, airhole_offsetY, airhole_offsetZ);
            new G4PVPlacement(0,                       //no rotation
                              posAirHole,               //at position
                              logicAirHole,             //its logical volume
                              "AirHole",                //its name
                              logicPlastic,            //its mother  volume
                              false,                   //no boolean operation
                              placementCounter++,      //copy number
                              checkOverlaps);          //overlaps checking
        }
    }
    // Empty Rows
    for (G4int jj =0; jj<6;jj++){
        for (G4int ii=0; ii<8; ii++){
            G4double airhole_offsetX = -38.5*mm + 11*ii*mm;
            G4double airhole_offsetY = -60.5*mm + 11*mm + 22*jj*mm;

            G4ThreeVector posAirHole = G4ThreeVector(airhole_offsetX, airhole_offsetY, airhole_offsetZ);
            new G4PVPlacement(0,                       //no rotation
                              posAirHole,               //at position
                              logicAirHole,             //its logical volume
                              "AirHole",                //its name
                              logicPlastic,            //its mother  volume
                              false,                   //no boolean operation
                              placementCounter++,      //copy number
                              checkOverlaps);          //overlaps checking
        }
    }

    //
    // water
    //
    G4double sample_sizeXY = 10*mm, sample_sizeZ = 15*mm;
    G4double sample_offsetZ = -0.5*airhole_sizeZ + 0.5*sample_sizeZ;
    G4ThreeVector posSample = G4ThreeVector(0, 0, sample_offsetZ);

    G4LogicalVolume* logicAirWithSample = new G4LogicalVolume(solidAirHole, fpAirMaterial, "AirWithSample");
    G4LogicalVolume* logicAirWithWater = new G4LogicalVolume(solidAirHole, fpAirMaterial, "AirWithWater");

    G4Box* solidSample = new G4Box("Sample", 0.5*sample_sizeXY, 0.5*sample_sizeXY, 0.5*sample_sizeZ);
    G4LogicalVolume* logicSample = new G4LogicalVolume(solidSample, fpDavisMedium, "SampleLV");
    new G4PVPlacement(0, posSample, logicSample, "Sample", logicAirWithSample, false, 0, checkOverlaps);

    G4Box* solidWater = new G4Box("Water", 0.5*sample_sizeXY, 0.5*sample_sizeXY, 0.5*sample_sizeZ);
    G4LogicalVolume* logicWater = new G4LogicalVolume(solidWater, fpDavisMedium, "WaterLV");
    new G4PVPlacement(0, posSample, logicWater, "Water", logicAirWithWater, false, 0, checkOverlaps);


    int wellCounter = 0;
    G4int waterPlacementCounter = 0;
    for (G4int ii=0;ii<4;ii++){
        for (G4int jj=0;jj<6;jj++){
            G4double sample_staggerX = ((jj/2)*2 == jj) ? 0*mm : 11*mm;
            G4double sample_offsetX = -38.5*mm + 22*ii*mm + sample_staggerX;
            G4double sample_offsetY = -60.5*mm + 22*jj*mm;
            G4ThreeVector posAirHoleWithWater = G4ThreeVector(sample_offsetX, sample_offsetY, airhole_offsetZ);
            if (wellCounter == 9){
                new G4PVPlacement(0, posAirHoleWithWater, logicAirWithSample, "AirWithSample", logicPlastic, false, 0, checkOverlaps);
            }
            else{
                new G4PVPlacement(0, posAirHoleWithWater, logicAirWithWater, "AirWithWater", logicPlastic, false, waterPlacementCounter++, checkOverlaps);
            }
            wellCounter++;
        }
    }

  //logicSample->SetUserLimits(new G4UserLimits(1*nm));

  return physWorld;
}

void MacroDetectorConstruction::ConstructSDandField(){

  //Attach SD to sample well
  G4SDManager* SDMan = G4SDManager::GetSDMpointer();
  SDMan->SetVerboseLevel(1);
  G4VSensitiveDetector* pSensitiveDetector = new MacroSensitiveDetector("sample", fEnThreshold);
  SetSensitiveDetector("SampleLV", pSensitiveDetector);
}

void MacroDetectorConstruction::UpdateGeometry()
{
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    G4RunManager::GetRunManager()->DefineWorldVolume(ConstructDetector());

}
