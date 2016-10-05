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
/// \file multiscale/micro/include/MacroPrimaryGeneratorMessenger.hh
/// \brief Definition of the MacroPrimaryGeneratorMessenger class
//
// $Id: MacroPrimaryGeneratorMessenger.hh 66241 2012-12-13 18:34:42Z gunter $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef MICRO_PRIMARY_GENERATOR_MESSENGER_HH
#define MICRO_PRIMARY_GENERATOR_MESSENGER_HH

#include "G4UImessenger.hh"
#include "globals.hh"

#include <string>
#include <sstream>
#include <vector>

class MacroPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MacroPrimaryGeneratorMessenger: public G4UImessenger
{
    public:
        MacroPrimaryGeneratorMessenger(MacroPrimaryGeneratorAction*);
       ~MacroPrimaryGeneratorMessenger();

        virtual void SetNewValue(G4UIcommand*, G4String);

    protected:
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
        MacroPrimaryGeneratorAction* fpGenerator;

        G4UIdirectory*              fpSourceDir;

        // Source Geometry
        G4UIcmdWithAString*         fpSourceGeomCmd;

        G4UIcmdWithADoubleAndUnit*  fpSourceRadCmd;
        G4UIcmdWithADoubleAndUnit*  fpSourceHalfXCmd;
        G4UIcmdWithADoubleAndUnit*  fpSourceHalfYCmd;
        G4UIcmdWithADoubleAndUnit*  fpSourceHalfZCmd;

        G4UIcmdWith3Vector*         fpRotationDirectionCmd;
        G4UIcmdWithADouble*         fpRotationAngleCmd;

        G4UIcmdWith3VectorAndUnit*  fpCentreCmd;

        // Source Particle Type/Energy
        G4UIcmdWithAString*        fpSourceDistCmd;

        G4UIcmdWithADoubleAndUnit*  fpSourceEnergyCmd;
        G4UIcmdWithAString*         fpSourceHistCmd;

        G4UIcmdWithAString*         fpSourceParticleTypeCmd;

        // Angular emission law
        G4UIcmdWithAString*         fpSourceAngDistCmd;
        G4UIcmdWith3Vector*         fpSourceEmitDirCmd;

        // Angle and energy file (designed for muons)
        G4UIcmdWithAString*         fpSourceEneAngFileCmd;

        // Muon +/- distribution
        G4UIcmdWithABool*           fpMuonCmd;

        // Management
        G4UIcmdWithoutParameter*    fpSourceInitialiseCmd;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
