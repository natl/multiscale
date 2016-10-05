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
//
/// \file MacroDetectorConstruction.hh
/// \brief Definition of the MacroDetectorConstruction class

#ifndef MACRO_DETECTOR_CONSTRUCTION_HH
#define MACRO_DETECTOR_CONSTRUCTION_HH

// Parent Class
#include "G4VUserDetectorConstruction.hh"

// Geant4 includes
#include "globals.hh"

#include "G4Material.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4UserLimits;
class MacroDetectorMessenger;

/// Detector construction class to define materials and geometry.

class MacroDetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        MacroDetectorConstruction();
        virtual ~MacroDetectorConstruction();

        virtual G4VPhysicalVolume* Construct();
        virtual void ConstructSDandField();

        void UpdateGeometry();

        // Setters for messenger parameters
        void SetWorldSize(G4ThreeVector worldSize) {fWorldSize   = worldSize;};
        void SetWorldMat (G4String      worldMat ) {fWorldMat    = worldMat ;};
        void SetSampleXYZ(G4ThreeVector sampleXYZ) {fSampleXYZ   = sampleXYZ;};
        void SetSamplePos(G4ThreeVector samplePos) {fSamplePos   = samplePos;};
        void SetSampleRad(G4double      sampleRad) {fSampleRad   = sampleRad;};
        void SetSampleMat(G4String      sampleMat) {fSampleMat   = sampleMat;};
        void SetSampleShape(G4String    sampleShp) {fSampleShape = sampleShp;};
        void SetSampleRotAngle(G4double t)         {fSampleRotAng = t*degree;}; // in degrees
        void SetSampleRotVector(G4ThreeVector dir) {fSampleRotVec = dir;};

        void SetUseCustomGeom(G4bool useGeom)      {fUseCustomGeom = useGeom;};
        void SetCustomGeomName(G4String name)      {fCustomGeomName = name;};

        void SetEnThreshold(G4double en) {fEnThreshold = en;};

        // Report current settings
        G4String Description();
        G4String CustomGeometries() {return G4String("microplate;");};


    private:
        // Possible detector options
        G4VPhysicalVolume* ConstructDetector();
        G4VPhysicalVolume* ConstructMicroplate();
        G4VPhysicalVolume* BuildUserGeometry();
        void DefineMaterials();

        // Parameters for BuildDetector() method.
        G4ThreeVector fWorldSize;
        G4String      fWorldMat;
        G4ThreeVector fSampleXYZ;
        G4ThreeVector fSamplePos;
        G4double      fSampleRad;
        G4String      fSampleMat;
        G4String      fSampleShape;
        G4ThreeVector fSampleRotVec;
        G4double      fSampleRotAng;

        // Detector Messenger
        MacroDetectorMessenger* fpMacroDetectorMessenger;

        // Detctor Messenger instruction variables
        G4bool   fUseCustomGeom;
        G4String fCustomGeomName;

        // SD parameters
        G4double fEnThreshold;

        // Materials
        G4Material* fpWaterMaterial;
        G4Material* fpAirMaterial;
        G4Material* fpPlasticMaterial;
        G4Material* fpWorldMat;
        G4Material* fpSampleMat;
        G4Material* fpDavisMedium;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

