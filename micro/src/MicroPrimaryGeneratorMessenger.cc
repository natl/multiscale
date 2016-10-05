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
/// \file multiscale/micro/src/MicroPrimaryGeneratorMessenger.cc
/// \brief Implementation of the MicroPrimaryGeneratorMessenger class
//
// $Id: MicroPrimaryGeneratorMessenger.cc 67268 2013-02-13 11:38:40Z ihrivnac $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "MicroPrimaryGeneratorMessenger.hh"
#include "G4UIdirectory.hh"
#include "MicroPrimaryGeneratorAction.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroPrimaryGeneratorMessenger::MicroPrimaryGeneratorMessenger(
    MicroPrimaryGeneratorAction* generator):
    G4UImessenger(),
    fpGenerator(generator),
    fpGunDir(0),
    fpRandPosCmd(0),
    fpRandDirCmd(0),
    fpSetPosCmd(0),
    fpSetDirCmd(0),
    fpAddToHistoCmd(0),
    fpHistoFileCmd(0),
    fpRootFileCmd(0)
{
    fpGunDir = new G4UIdirectory("/micro/gun/");
    fpGunDir->SetGuidance("gun control");

    fpRandPosCmd = new G4UIcmdWithoutParameter("/micro/gun/useRandPos", this);
    fpRandPosCmd->SetGuidance("Generate particles at random positions");
    fpRandPosCmd->SetGuidance("within the periodic region");
    fpRandPosCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpRandDirCmd = new G4UIcmdWithoutParameter("/micro/gun/useRandDir", this);
    fpRandDirCmd->SetGuidance("Generate particles with random directions");
    fpRandDirCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSetPosCmd = new G4UIcmdWith3VectorAndUnit("/micro/gun/setPos",this);
    fpSetPosCmd->SetGuidance("Set the position of the gun");
    fpSetPosCmd->SetParameterName("xpos", "ypos", "zpos", false, false);
    fpSetPosCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSetDirCmd = new G4UIcmdWith3Vector("/micro/gun/setDir",this);
    fpSetDirCmd->SetGuidance("Set the direction of the particles");
    fpSetDirCmd->SetParameterName("xdir", "ydir", "zdir", false, false);
    fpSetDirCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpAddToHistoCmd = new G4UIcmdWithAString("/micro/gun/hist", this);
    fpAddToHistoCmd->SetGuidance("Histogram point");
    fpAddToHistoCmd->SetGuidance("(bin max (MeV), bin weight)");
    fpAddToHistoCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpHistoFileCmd = new G4UIcmdWithAString("/micro/gun/histFile", this);
    fpHistoFileCmd->SetGuidance("Histogram File");
    fpHistoFileCmd->SetGuidance("filename");
    fpHistoFileCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpRootFileCmd = new G4UIcmdWithAString("/micro/gun/rootFile", this);
    fpRootFileCmd->SetGuidance("Root File");
    fpRootFileCmd->SetGuidance("filename");
    fpRootFileCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroPrimaryGeneratorMessenger::~MicroPrimaryGeneratorMessenger()
{
    delete fpRootFileCmd;
    delete fpHistoFileCmd;
    delete fpAddToHistoCmd;
    delete fpRandPosCmd;
    delete fpRandDirCmd;
    delete fpSetPosCmd;
    delete fpSetDirCmd;
    delete fpGunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,
                                               G4String newValue)
{
    if (command == fpRandPosCmd)
        {fpGenerator->SetUseRandPosition(true);}
    else if (command == fpRandDirCmd)
        {fpGenerator->SetUseRandDirection(true);}
    else if (command == fpSetPosCmd)
    {
        fpGenerator->SetUserPosition(
            fpSetPosCmd->ConvertToDimensioned3Vector(newValue));
    }
    else if (command == fpSetDirCmd)
    {
        fpGenerator->SetUserPosition(
            fpSetPosCmd->ConvertTo3Vector(newValue));
    }
    else if (command == fpAddToHistoCmd)
    {
        fpGenerator->AddEnergyHisto(std::stod(split(newValue, ' ').at(0)),
                                    std::stod(split(newValue, ' ').at(1)));
    }
    else if (command == fpHistoFileCmd)
    {
        LoadHistoFile(newValue);
    }
    else if (command == fpRootFileCmd)
    {
        fpGenerator->SetRootFile(newValue);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroPrimaryGeneratorMessenger::LoadHistoFile(G4String filename)
{
    std::ifstream infile (filename, std::ifstream::in);
    G4String currentline;

    if (infile.is_open())
    {
        while (getline(infile, currentline))
        {
            if (currentline[0] != '#')
            {
                fpGenerator->AddEnergyHisto(
                    std::stod(split(currentline, ' ').at(0)),
                    std::stod(split(currentline, ' ').at(1)));
            }
        }
        infile.close();
    }
    else
    {
        G4cout << "Histogram file read error" << G4endl;
        G4cout << "Format should be <energy> <counts>" << G4endl;
        G4cout << "Separator is space, lines with '#' as the first "
               << "character are commented" << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
