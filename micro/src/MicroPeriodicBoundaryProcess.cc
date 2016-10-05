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
////////////////////////////////////////////////////////////////////////
// Implementation of periodic boundary conditions using a Process
////////////////////////////////////////////////////////////////////////
//
// File:        MicroPeriodicBoundaryProcess.cc
// Description: Discrete Process -- move tracks to allow for periodic
//                                  boundary conditions
// Version:     0.2
// Created:     2015-07-15
// Modified:    2015-09-21 Uses PV Store to get boundary parameters, box
//                         must be centered now however.
//
// Author:      Nathanael Lampe
//
// mail:        lampe@clermont.in2p3.fr
//
////////////////////////////////////////////////////////////////////////

#include "MicroPeriodicBoundaryProcess.hh"

#include "MicroTrackInformation.hh"
#include "MicroEventInformation.hh"
#include "MicroDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParallelWorldProcess.hh"
#include "G4ThreeVector.hh"
#include "G4ProcessType.hh"

#include "G4RunManager.hh"
#include "G4EventManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroPeriodicBoundaryProcess::MicroPeriodicBoundaryProcess(
    const G4String& processName, G4ProcessType type):
    G4VDiscreteProcess(processName, type)
{
    G4ThreeVector halfLengths =
        0.5*(static_cast<const MicroDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction()))
        ->GetParaWorldSize();

    G4ThreeVector nboxes =
        (static_cast<const MicroDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction()))
        ->GetNBoxes();

    fMaxX = halfLengths.getX();
    fMaxY = halfLengths.getY();
    fMaxZ = halfLengths.getZ();

    fMinX = -fMaxX;
    fMinY = -fMaxY;
    fMinZ = -fMaxZ;

    fXSideLength = 2*fMaxX;
    fYSideLength = 2*fMaxY;
    fZSideLength = 2*fMaxZ;

    fXboxs = nboxes.getX();
    fYboxs = nboxes.getY();
    fZboxs = nboxes.getZ();

    if ( verboseLevel > -1) {
        G4cout << GetProcessName() << " Process has been registered."<< G4endl
               << "The periodic boundaries are set to give: " << G4endl
               <<  fMinX/um << " < x(um) < " << fMaxX/um << G4endl
               <<  fMinY/um << " < y(um) < " << fMaxY/um << G4endl
               <<  fMinZ/um << " < z(um) < " << fMaxZ/um << G4endl;

        G4cout << "There are " << fXboxs << " x microdomains, " << G4endl
               << "There are " << fYboxs << " y microdomains, " << G4endl
               << "There are " << fZboxs << " z microdomains, " << G4endl;
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroPeriodicBoundaryProcess::~MicroPeriodicBoundaryProcess()
{
    //dtor
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double MicroPeriodicBoundaryProcess::GetMeanFreePath(
    const G4Track&, G4double, G4ForceCondition* condition)
{
  *condition = Forced;

  return DBL_MAX;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VParticleChange* MicroPeriodicBoundaryProcess::PostStepDoIt(
    const G4Track& aTrack, const G4Step& aStep)
{
    pParticleChange->Initialize(aTrack);

    const G4Step* pStep = &aStep;
    const G4Step* hStep = G4ParallelWorldProcess::GetHyperStep();

    if (hStep) pStep = hStep;

    G4bool isOnBoundary =
        (pStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary);

    G4ThreeVector currentPosition;
    MicroTrackInformation* trackInfo = (MicroTrackInformation*)
        aTrack.GetUserInformation();

    // If the boundary corresponds to the periodic region, kill track and
    // recreate it as a secondary. The position is updated in the
    // PostUserTrackingAction
    if (isOnBoundary) {
        currentPosition = pStep->GetPostStepPoint()->GetPosition();
        if (currentPosition.x() >= fMaxX){
            trackInfo->incX();
        }
        else if (currentPosition.x() <= fMinX){
            trackInfo->decX();
        }
        else if (currentPosition.y() >= fMaxY){
            trackInfo->incY();
        }
        else if (currentPosition.y() <= fMinY){
            trackInfo->decY();
        }
        else if (currentPosition.z() >= fMaxZ){
            trackInfo->incZ();
        }
        else if (currentPosition.z() <= fMinZ){
            trackInfo->decZ();
        }
        else{
            return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
        }
    }
    else {
        return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
    }

    // beyond here the function has returned unless we are on the periodic
    // boundary Move the particle to the other side of the box

    // If this triggers, we have a problem in the treatment of secondaries
    // Mainly for debugging
    if ((trackInfo->getX() <= -2)
        || (trackInfo->getY() <= -2)
        || (trackInfo->getZ() <= -2)
        || (trackInfo->getX() >= fXboxs+1)
        || (trackInfo->getY() >= fYboxs+1)
        || (trackInfo->getZ() >= fZboxs+1))
    {
        G4cout << "Uncaught secondary: " << aTrack.GetParticleDefinition()->GetParticleName() << G4endl;
        MicroEventInformation* eInfo = (MicroEventInformation*) G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetUserInformation();
        G4cout << "Event Info: ";
        eInfo->Print();
        G4cout << "Current Track Info: ";
        trackInfo->Print();
        G4cout << "Initial Particle: " << G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetPrimaryVertex()->GetPrimary()->GetParticleDefinition()->GetParticleName() <<G4endl;;

    }

    // Kill the particle if it has left the entire periodic overarching region
    if ((trackInfo->getX() <= -1)
        || (trackInfo->getY() <= -1)
        || (trackInfo->getZ() <= -1)
        || (trackInfo->getX() >= fXboxs)
        || (trackInfo->getY() >= fYboxs)
        || (trackInfo->getZ() >= fZboxs))
    {
        pParticleChange->ProposeTrackStatus(fStopAndKill);
    }
    else
    {
        G4Track* trackClone = new G4Track(aTrack);
        pParticleChange->ProposeTrackStatus(fStopAndKill);
        pParticleChange->AddSecondary(trackClone);
    }

    return pParticleChange;
}








