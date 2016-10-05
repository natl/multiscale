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
/// \file MicroEventAction.hh
/// \brief Definition of the MicroEventAction class

#ifndef MicroEventAction_h
#define MicroEventAction_h 1

#include "G4UserEventAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class MicroTesterHistoManager;
class MicroCellHistoManager;
class MicroRunAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class MicroEventAction : public G4UserEventAction
{
  public:
    MicroEventAction(MicroRunAction*, MicroCellHistoManager*,
        MicroTesterHistoManager*);
    virtual ~MicroEventAction();

    //inherited methods
    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    // record methods
    void RecordTester(const G4Event*);
    void RecordCell(const G4Event*);

    // Tools
    G4ThreeVector GetParaWorldLengths();

  private:
    MicroRunAction* fRunAction;
    //HC Id's
    G4int  fTesterMain, fCellInterior, fCellWall;
    // Bools to determine what histo to use
    G4bool fTesterActive, fCellActive;
    // Histo Managers
    MicroCellHistoManager*    fCellHistoManager;
    MicroTesterHistoManager*  fTesterHistoManager;

    G4ThreeVector fHalfLengths;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


