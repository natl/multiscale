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
// $Id: B1EventAction.cc 75117 2013-10-28 09:38:37Z gcosmo $
//
/// \file MacroEventAction.cc
/// \brief Implementation of the MacroEventAction class

#include "MacroEventAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MacroEventAction::MacroEventAction(MacroRunAction* runAction, MacroHistoManager* histo)
: G4UserEventAction(),
  fRunAction(runAction),
  fAccID(-1),
  fSecID(-1),
  fEnID(-1),
  fHistoManager(histo)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MacroEventAction::~MacroEventAction()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroEventAction::BeginOfEventAction(const G4Event* event)
{
    fInitialPhotonEnergy =
        event->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroEventAction::EndOfEventAction(const G4Event* event)
{
    if (fAccID == -1){
        fAccID = G4SDManager::GetSDMpointer()->GetCollectionID("sample/accumulator");
        fSecID = G4SDManager::GetSDMpointer()->GetCollectionID("sample/secondary");
        fEnID = G4SDManager::GetSDMpointer()->GetCollectionID("sample/energy");
    }

    MacroAccumulatorHitsCollection*  accumulatorHits =
        static_cast<MacroAccumulatorHitsCollection*>
            (event->GetHCofThisEvent()->GetHC(fAccID));
    MacroSecondaryHitsCollection* secondaryHits =
        static_cast<MacroSecondaryHitsCollection*>
            (event->GetHCofThisEvent()->GetHC(fSecID));


    //Deal with accumulated energy
    G4double edep = (static_cast<MacroAccumulatorHit*>
        (accumulatorHits->GetHit(0)))->GetEdep();
    if (edep != 0){
        fHistoManager->AddToAccumulatedEdepHisto( edep );
        fHistoManager->AddToInitHisto(
            event->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy());
        fHistoManager->RecordAccumulation(event->GetEventID(),
            event->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy(), edep );
    }
    for (size_t it = 0; it != secondaryHits->GetSize(); it++){
        MacroSecondaryHit* sHit = static_cast<MacroSecondaryHit*>
            (secondaryHits->GetHit(it));
        fHistoManager->RecordSecondary(sHit->GetEventID(), sHit->GetTrackID(),
            sHit->GetParticleName(), sHit->GetCharge(), sHit->GetEdep(),
            sHit->GetKineticEnergy(), sHit->GetPosition(), sHit->GetMomentum());
        fHistoManager->AddToSecondaryHisto(sHit->GetKineticEnergy());
    }

    // MacroEnergyDepoHitsCollection* energyHits =
    //     static_cast<MacroEnergyDepoHitsCollection*>
    //         (event->GetHCofThisEvent()->GetHC(fEnID));
    // for (size_t it = 0; it != energyHits->GetSize(); it++){
    //     MacroEnergyDepoHit* eHit = static_cast<MacroEnergyDepoHit*>
    //         (energyHits->GetHit(it));
    //     fHistoManager->RecordEnergyDeposit(eHit->GetEventID(),
    //         eHit->GetTrackID(), eHit->GetParticleType(), eHit->GetEdep(),
    //         eHit->GetNsecs(), eHit->GetProcessName(), eHit->GetPosition());
    //     fHistoManager->AddToIndividualEdepHisto(eHit->GetEdep());
    //}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
