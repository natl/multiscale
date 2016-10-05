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
/// \file MicroActionInitialization.cc
/// \brief Implementation of the MicroActionInitialization class

#include "MicroActionInitialization.hh"

#include "MicroPrimaryGeneratorAction.hh"
#include "MicroRunAction.hh"
#include "MicroEventAction.hh"
#include "MicroCellHistoManager.hh"
#include "MicroTesterHistoManager.hh"
#include "MicroTrackingAction.hh"
#include "MicroSteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroActionInitialization::MicroActionInitialization() :
    G4VUserActionInitialization()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroActionInitialization::~MicroActionInitialization()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroActionInitialization::BuildForMaster() const
{
    MicroCellHistoManager* cellHM = new MicroCellHistoManager();
    MicroTesterHistoManager* testerHM = new MicroTesterHistoManager();

    SetUserAction(new MicroRunAction(cellHM, testerHM));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroActionInitialization::Build() const
{
    SetUserAction(new MicroPrimaryGeneratorAction);

    MicroCellHistoManager* cellHM = new MicroCellHistoManager();
    MicroTesterHistoManager* testerHM = new MicroTesterHistoManager();

    MicroRunAction* runAction = new MicroRunAction(cellHM, testerHM);
    SetUserAction(runAction);

    MicroEventAction* eventAction = new MicroEventAction(runAction, cellHM, testerHM);
    SetUserAction(eventAction);

    MicroTrackingAction* trackingAction = new MicroTrackingAction();
    SetUserAction(trackingAction);

    MicroSteppingAction* steppingAction = new MicroSteppingAction();
    SetUserAction(steppingAction);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
