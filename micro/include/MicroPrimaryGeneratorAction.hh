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
/// \file MicroPrimaryGeneratorAction.hh
/// \brief Definition of the MicroPrimaryGeneratorAction class

#ifndef MICRO_PRIMARY_GENERATOR_ACTION_HH
#define MICRO_PRIMARY_GENERATOR_ACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ThreeVector.hh"
#include "globals.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MicroPrimaryGeneratorMessenger;
class G4ParticleGun;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MicroPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:

        MicroPrimaryGeneratorAction();
        virtual ~MicroPrimaryGeneratorAction();

        virtual void GeneratePrimaries(G4Event*);

        G4ThreeVector GeneratePosition();
        G4ThreeVector GenerateDirection();

        // Getters and Setters
        void SetUseRandPosition(G4bool b)  { fUseRandomPosition = b; };
        void SetUseRandDirection(G4bool b) { fUseRandomDirection = b; };

        void SetUserPosition (G4ThreeVector pos) {fUserPosition = pos;};
        void SetUserDirection(G4ThreeVector dir) {fUserDirection = dir;};


        // Histo Getters and Setters
        void AddEnergyHisto(G4double en, G4double cnt)
        {
            fUseHisto = true;
            fHistEns.push_back(en);
            fHistCnts.push_back(cnt);
            DoHisto();
        };

        void SetRootFile(G4String rootfile)
        {
            fUseRootFile = true;
            fRootFilename = rootfile;
            std::cout << "Using " << fRootFilename << " as input" << G4endl;
            SetupRootFile();
        }

    private:
        G4ParticleGun* fpParticleGun;
        MicroPrimaryGeneratorMessenger* fpGeneratorMessenger;

        G4bool fUseRandomPosition, fUseRandomDirection;
        G4ThreeVector fUserPosition, fUserDirection;

        void DoNextEvent(G4Event*);
        void DoNextRootEvent(G4Event*);

        // Histogram Options and functions
        G4bool fUseHisto;
        std::vector<G4double> fHistEns, fHistCnts;
        std::vector<G4double> xx, ff, aa, fc; //x, f(x), slopes, cumulative
        G4double fNPoints;
        void DoHisto();
        G4double GetHistoEnergy();

        // Root file options and function
        G4bool fUseRootFile;
        G4String fRootFilename;
        void SetupRootFile();
        std::vector<G4int>* fpRootEvents;
        std::vector<G4ThreeVector>* fpRootPositions;
        std::vector<G4ThreeVector>* fpRootDirections;
        std::vector<G4double>* fpRootEnergies;
        std::vector<G4String>* fpRootPTypes;
        G4int fCurrentEventIndex, fNRootEvents;
        static G4int fRandDist(G4int i) { return std::floor(G4UniformRand()*i); }
};
#endif // MICRO_PRIMARY_GENERATOR_ACTION_HH
