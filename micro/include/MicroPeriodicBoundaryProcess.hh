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
// File:        MicroPeriodicBoundaryProcess.hh
// Description: Discrete Process -- move tracks to allow for periodic
//                                  boundary conditions
// Version:     0.1
// Created:     2015-07-15
// Modified:    2015-09-21 Uses PV Store to get boundary parameters, box
//                         must be centered now however.
//
// Author:      Nathanael Lampe
//
// mail:        lampe@clermont.in2p3.fr
//
////////////////////////////////////////////////////////////////////////

#ifndef PERIODICBOUNDARYPROCESS_H
#define PERIODICBOUNDARYPROCESS_H


#include "G4VDiscreteProcess.hh"
#include "globals.hh"



class MicroPeriodicBoundaryProcess : public G4VDiscreteProcess
{
    public:
        MicroPeriodicBoundaryProcess(const G4String& processName = "PeriodicBoundary",
                                G4ProcessType type = fUserDefined);

        virtual ~MicroPeriodicBoundaryProcess();

        G4double GetMeanFreePath(const G4Track&, G4double, G4ForceCondition* condition);
            // The mean free path is irrelevant for this process, we return infinity
            // so as not to limit other processes. The forced condition is set however,
            // but the PostStepDoIt contains code that will only act at a boundary

        G4VParticleChange* PostStepDoIt(const G4Track& aTrack, const G4Step& aStep);
            // This method clones and moves the particle when it leaves the repeating
            // area


    protected:
    private:
        G4double fMinX, fMaxX, fMinY, fMaxY, fMinZ, fMaxZ;
        G4double fXSideLength, fYSideLength, fZSideLength;
        G4int    fXboxs, fYboxs, fZboxs;
};
#endif // PERIODICBOUNDARYPROCESS_H
