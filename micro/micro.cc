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
/// \file micro.cc
/// \brief Implementation of the micro example

#ifdef G4MULTITHREADED
  #include "G4MTRunManager.hh"
#else
  #include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#ifdef G4VIS_USE
  #include "G4VisExecutive.hh"
#endif

#include "MicroActionInitialization.hh"
#include "MicroDetectorConstruction.hh"
#include "MicroPhysicsList.hh"
#include "MicroParallelWorldConstruction.hh"

#include "QGSP_BIC_HP.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace {
    void PrintUsage() {
        G4cerr << " Usage: " << G4endl;
        G4cerr << " exampleB4c [-m macro ] [-t nThreads] --raytracer "
               << "--with-hadronic"  << G4endl;
        G4cerr << "   note: -t option requires multi-threaded mode."
               << G4endl;
    }
}

int main(int argc,char** argv)
{

    // Evaluate arguments
    //
    if ( argc >= 8 ) {
    PrintUsage();
    return 1;
    }
    G4bool raytracer = false;
    G4bool withHadronic = false;
    G4String macro = "micro.in";
#ifdef G4MULTITHREADED
    G4int nThreads = 1;
#endif
    G4int ii = 1;
    while (ii < argc)
    {
        if (G4String(argv[ii]) == "-m")
        {
            macro = argv[ii+1];
            ii = ii + 2;
        }
        else if (G4String(argv[ii]) == "--raytracer")
        {
            raytracer = true;
            ii = ii + 1;
        }
        else if (G4String(argv[ii]) == "--with-hadronic")
        {
            withHadronic = true;
            ii = ii + 1;
        }
        #ifdef G4MULTITHREADED
        else if ( G4String(argv[ii]) == "-t" )
        {
            nThreads = G4UIcommand::ConvertToInt(argv[ii+1]);
            ii = ii + 2;
        }
        #endif
        else
        {
        PrintUsage();
        return 1;
        }
    }

      // Choose the Random engine
      //
      G4Random::setTheEngine(new CLHEP::MTwistEngine);
      time_t timeStart;
      time(&timeStart);
      G4Random::setTheSeed( (long) timeStart);

    // Construct the default run manager
    #ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    runManager->SetNumberOfThreads(nThreads);
    #else
    G4RunManager* runManager = new G4RunManager;
    #endif

    // Set mandatory user initialization classes
    // Detector
    MicroDetectorConstruction* theMicroDetectorConstruction =
        new MicroDetectorConstruction;
    theMicroDetectorConstruction->RegisterParallelWorld(
        new MicroParallelWorldConstruction("PeriodicDomain"));
    runManager->SetUserInitialization(theMicroDetectorConstruction);

  // Physics
    if (raytracer)
    {
        runManager->SetUserInitialization(new QGSP_BIC_HP);
    }
    else
    {
        runManager->SetUserInitialization(new MicroPhysicsList(withHadronic));
    }

    // Action
    runManager->SetUserInitialization(new MicroActionInitialization());

    #ifdef G4VIS_USE
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    #endif

    // Get the pointer to the User Interface manager
    G4UImanager* UI = G4UImanager::GetUIpointer();

    if (argc==1)   // Define UI session for interactive mode.
    {
        #ifdef _WIN32
        G4UIsession * session = new G4UIterminal();
        #else
        G4UIsession * session = new G4UIterminal(new G4UItcsh);
        #endif
        session->SessionStart();
        delete session;
    }
    else
    {
        G4String command = "/control/execute ";
        UI->ApplyCommand(command+macro);
    }

        #ifdef G4VIS_USE
        delete visManager;
        #endif

    delete runManager;

    time_t timeEnd;
    time(&timeEnd);
    long timeDiff = (long) timeEnd - (long) timeStart;
    std::cout << "Geant4 was running for " << timeDiff << " seconds" << std::endl;

    return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
