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
/// \file MacroEventAction.hh
/// \brief Definition of the MacroEventAction class

#ifndef MacroEventAction_h
#define MacroEventAction_h 1


#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"

#include "MacroAccumulatorHit.hh"
#include "MacroSecondaryHit.hh"
#include "MacroEnergyDepoHit.hh"
#include "MacroHistoManager.hh"

#include <stdio.h>

/// Event action class
///
class MacroHistoManager;
class MacroRunAction;

class MacroEventAction : public G4UserEventAction
{
  public:
    MacroEventAction(MacroRunAction*, MacroHistoManager*);
    virtual ~MacroEventAction();

    //inherited methods
    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);


  private:
    MacroRunAction* fRunAction;

    G4double  fInitialPhotonEnergy;

    //HC Id's
    G4int     fAccID;
    G4int     fSecID;
    G4int     fEnID;
    MacroHistoManager* fHistoManager;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


