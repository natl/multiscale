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
/// \file multiscale/micro/include/MicroPrimaryGeneratorMessenger.hh
/// \brief Definition of the MicroPrimaryGeneratorMessenger class
//
// $Id: MicroPrimaryGeneratorMessenger.hh 66241 2012-12-13 18:34:42Z gunter $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef MICRO_PRIMARY_GENERATOR_MESSENGER_HH
#define MICRO_PRIMARY_GENERATOR_MESSENGER_HH

#include "G4UImessenger.hh"
#include "globals.hh"

class MicroPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MicroPrimaryGeneratorMessenger: public G4UImessenger
{
    public:
        MicroPrimaryGeneratorMessenger(MicroPrimaryGeneratorAction*);
       ~MicroPrimaryGeneratorMessenger();

        virtual void SetNewValue(G4UIcommand*, G4String);

    protected:
        // Function to split strings when processing histogram data
        std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
            std::stringstream ss(s);
            std::string item;
            while (std::getline(ss, item, delim)) {
                if (!item.empty()) elems.push_back(item);
            }
            return elems;
        }
        std::vector<std::string> split(const std::string &s, char delim) {
            std::vector<std::string> elems;
            split(s, delim, elems);
            return elems;
        }

        // Function to load a histogram from a file
        void LoadHistoFile(G4String);

    private:
        MicroPrimaryGeneratorAction* fpGenerator;

        G4UIdirectory*              fpGunDir;
        G4UIcmdWithoutParameter*    fpRandPosCmd;
        G4UIcmdWithoutParameter*    fpRandDirCmd;
        G4UIcmdWith3VectorAndUnit*  fpSetPosCmd;
        G4UIcmdWith3Vector*         fpSetDirCmd;

        // Histogram messengers
        G4UIcmdWithAString*         fpAddToHistoCmd;
        G4UIcmdWithAString*         fpHistoFileCmd;

        // Root File
        G4UIcmdWithAString*         fpRootFileCmd;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

