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
/// \file MacroPrimaryGeneratorAction.hh
/// \brief Definition of the MacroPrimaryGeneratorAction class

#ifndef MACRO_PRIMARY_GENERATOR_ACTION_HH
#define MACRO_PRIMARY_GENERATOR_ACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "Randomize.hh"

#include <vector>


class G4ParticleGun;
class G4Event;
class MacroPrimaryGeneratorMessenger;

class MacroPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
        MacroPrimaryGeneratorAction();
        virtual ~MacroPrimaryGeneratorAction();

        // method from the base class
        virtual void GeneratePrimaries(G4Event*);

        void Initialise();

        // setters for geometry
        void SetGeometry(G4String geom) {fGeometry = geom;};

        void SetRadius(G4double radius) {fRad = radius;};
        void SetHalfX(G4double len) {fHalfX = len;};
        void SetHalfY(G4double len) {fHalfY = len;};
        void SetHalfZ(G4double len) {fHalfZ = len;};
        void SetRotAngle(G4double theta) {fRotAngle = theta*degree;}; // in degrees
        void SetRotDirection(G4ThreeVector dir) {fRotDirection = dir;};
        void SetCentre(G4ThreeVector pos) {fCentre = pos;};


        // setters for energy
        void SetEnergyDist(G4String dist) {fDistType = dist;};
        void SetParticleType(G4String type) {fParticleType = type;};
        void SetEnergy(G4double ene) {fEnergy = ene;};
        void AddEnergyHisto(G4double en, G4double cnt)
            {fHistEns.push_back(en); fHistCnts.push_back(cnt);};

        // Setters for emission angle
        void SetEmitDirection(G4ThreeVector dir) {fEmitDirection = dir;};
        void SetAngleDist(G4String ang) {fAngleDist = ang;};

        //Specified AngleEnergy File
        void SetSpecificAngleEnergyFile(G4String filename)
        {
            fAngleEnergyFilename = filename;
            LoadSpecifiedAngleEnergyFile();
            fCurrentAngleEnergyIdx = 0;
            fUseAngleEnergyFile = true;
        }

        void SetMuon(G4bool isMuon) {fIsMuon = isMuon;};

    protected:
        // Function to split strings when processing histogram data
        std::vector<std::string> &split(const std::string &str, char delim, std::vector<std::string> &elems) {
            std::stringstream ss(str);
            std::string item;
            while (std::getline(ss, item, delim)) {
                if (!item.empty()) elems.push_back(item);
            }
            return elems;
        }
        std::vector<std::string> split(const std::string &str, char delim) {
            std::vector<std::string> elems;
            split(str, delim, elems);
            return elems;
        }

    private:
        // Private Functions
        void DoHisto();

        G4double GetEnergy();
        G4double GetHistoEnergy();

        G4ThreeVector GetPosition();
        G4ThreeVector GetPlanePosition();
        G4ThreeVector GetCirclePosition();
        G4ThreeVector GetSpherePosition();
        G4ThreeVector GetBoxPosition();
        G4ThreeVector GetMicroplatePosition();

        G4ThreeVector GetEmissionDirection(G4ThreeVector);
        G4ThreeVector GetIsoDirection(G4ThreeVector);
        G4ThreeVector GetHalfIsoDirection(G4ThreeVector);
        G4ThreeVector GetHalfCosineDirection(G4ThreeVector);

        // Internal flow parameters
        G4bool fUseHisto;       // true: use histograms
        G4bool fUseHalfDir;     // true: iso, from surface
        G4bool fUseIsoDir;      // true: iso, from volume
        G4bool fUseCosDir;      // true: cosine, from surface
        G4bool fFixedDirection; // true: use direction in fEmitDirection
        G4bool fIsSphere, fIsPlane, fIsBox, fIsPoint;  //Geometries
        G4bool fIsCircle, fIsMicroplate;               //Geometries
        G4bool fIsMuon;  // Sets to choose between mu+ and mu-

        // Non-Function Members
        G4ParticleGun*  fParticleGun;
        MacroPrimaryGeneratorMessenger* fMessenger;

        // Internal parameters for geometry
        G4String fGeometry, fAngleDist;
        G4double fRad, fHalfX, fHalfY, fHalfZ, fRotAngle;
        G4ThreeVector fRotDirection, fCentre, fEmitDirection, fSurfaceNormal;

        // Internal Parameters for Specified Event energies and angles
        std::vector<std::pair<G4double, G4double>> fSpecifiedEnsAngs;
        G4bool fUseAngleEnergyFile;
        G4bool fSpecifiedAngleIsUnused, fSpecifiedEnergyIsUnused;
        G4String fAngleEnergyFilename;
        G4int fCurrentAngleEnergyIdx;

        static G4int fRandDist(G4int i) {return std::floor(G4UniformRand()*i);}

        G4double GetAngEnSpecifiedEnergy();
        G4ThreeVector GetAngEnSpecifiedDirection();
        void PrepareNextSpecifiedEnergyAngle();
        void LoadSpecifiedAngleEnergyFile();

        // Internal parameters for particle energy
        G4String fDistType, fParticleType;
        G4double fEnergy;
        std::vector<G4double> fHistEns, fHistCnts;
        std::vector<G4double> xx, ff, aa, fc; //x, f(x), slopes, cumulative
        G4double fNPoints;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
