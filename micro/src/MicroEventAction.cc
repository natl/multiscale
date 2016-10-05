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
/// \file MicroEventAction.cc
/// \brief Implementation of the MicroEventAction class

#include "MicroEventAction.hh"

#include "MicroTesterHistoManager.hh"
#include "MicroTesterHit.hh"
#include "MicroCellHistoManager.hh"
#include "MicroCellInteriorHit.hh"
#include "MicroCellWallHit.hh"
#include "MicroEventInformation.hh"
#include "MicroDetectorConstruction.hh"

#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4SolidStore.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroEventAction::MicroEventAction(MicroRunAction* runAction,
    MicroCellHistoManager* cellHisto,
    MicroTesterHistoManager* testerHisto):
    G4UserEventAction(),
    fRunAction(runAction),
    fTesterMain(-1),
    fCellInterior(-1),
    fCellWall(-1),
    fTesterActive(false),
    fCellActive(false),
    fCellHistoManager(cellHisto),
    fTesterHistoManager(testerHisto),
    fHalfLengths(G4ThreeVector())
{
    fHalfLengths = GetParaWorldLengths();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroEventAction::~MicroEventAction()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroEventAction::BeginOfEventAction(const G4Event*)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroEventAction::EndOfEventAction(const G4Event* event)
{
    if ((fTesterMain == -1) && (fCellInterior == -1)){
        fTesterMain = G4SDManager::GetSDMpointer()->
            GetCollectionID("MainAccumulator");

        fCellInterior = G4SDManager::GetSDMpointer()->
            GetCollectionID("interior");
        fCellWall = G4SDManager::GetSDMpointer()->GetCollectionID("wall");

        if (fTesterMain != -1) fTesterActive = true;
        if (fCellInterior != -1) fCellActive = true;
    }

    if (fTesterActive) RecordTester(event);
    if (fCellActive) RecordCell(event);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroEventAction:: RecordTester(const G4Event* event)
{

    G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()
        ->GetEventID();

    MicroTesterHitsCollection* main   = static_cast<MicroTesterHitsCollection*>
        (event->GetHCofThisEvent()->GetHC(fTesterMain));

    MicroTesterHit* mainhit = static_cast<MicroTesterHit*>
        (main->GetHit(0));

    fTesterHistoManager->RecordEvent(eventID,
                                     mainhit->GetEInit(),
                                     mainhit->GetEdep());

    std::vector<G4double> edepvector = mainhit->GetEdepVector();
    for (std::vector<G4double>::const_iterator it=edepvector.begin();
        it != edepvector.end(); it++)
    {
        fTesterHistoManager->AddToEdepHisto(*it);
    }


    std::map<G4double, G4double> edepsAtRadii = mainhit->GetEdepsAtRadii();
    for (std::map<G4double, G4double>::const_iterator it=edepsAtRadii.begin();
        it != edepsAtRadii.end(); it++)
    {
        if (it->second > 0) fTesterHistoManager->AddToRadiiHisto(it->first,
                                                                 it->second);
    }

    fTesterHistoManager->AddToTLHisto(mainhit->GetTrackLength());

    //process positions
    std::vector< std::pair<G4double,G4double> > inx  = mainhit->GetXIn();
    std::vector< std::pair<G4double,G4double> > outx = mainhit->GetXOut();
    std::vector< std::pair<G4double,G4double> > iny  = mainhit->GetYIn();
    std::vector< std::pair<G4double,G4double> > outy = mainhit->GetYOut();
    std::vector< std::pair<G4double,G4double> > inz  = mainhit->GetZIn();
    std::vector< std::pair<G4double,G4double> > outz = mainhit->GetZOut();

    G4int xins = (G4int) inx.size(), xouts = (G4int) outx.size(), xpairs = 0;
    while (!inx.empty())
    {
        std::pair<G4double, G4double> inpair = inx.back();
        for (std::vector< std::pair<G4double,G4double> >::iterator
             it = outx.begin(); it != outx.end(); it++)
        {
            if (inpair == *it)
            {
                xpairs++;
                break;
            }
        }
        inx.pop_back();
    }

    G4int yins = (G4int) iny.size(), youts = (G4int) outy.size(), ypairs = 0;
    while (!iny.empty())
    {
        std::pair<G4double, G4double> inpair = iny.back();
        for (std::vector< std::pair<G4double,G4double> >::iterator
            it = outy.begin(); it != outy.end(); it++)
        {
            if (inpair == *it)
            {
                ypairs++;
                break;
            }
        }
        iny.pop_back();
    }

    G4int zins = (G4int) inz.size(), zouts = (G4int) outz.size(), zpairs = 0;
    while (!inz.empty())
    {
        std::pair<G4double, G4double> inpair = inz.back();
        for (std::vector< std::pair<G4double,G4double> >::iterator
             it = outz.begin(); it != outz.end(); it++)
        {
            if (inpair == *it)
            {
                zpairs++;
                break;
            }
        }
        inz.pop_back();
    }

    fTesterHistoManager->recordInsAndOuts(eventID,
                                          xins,
                                          xouts,
                                          xpairs,
                                          yins,
                                          youts,
                                          ypairs,
                                          zins,
                                          zouts,
                                          zpairs );

    inx  = mainhit->GetXIn();
    outx = mainhit->GetXOut();
    iny  = mainhit->GetYIn();
    outy = mainhit->GetYOut();
    inz  = mainhit->GetZIn();
    outz = mainhit->GetZOut();

    G4String side = "x";
    while ( (!inx.empty()) && (!outx.empty()) )
    {
        std::pair<G4double, G4double> p1 = inx.back(), p2 = outx.back();
        fTesterHistoManager->RecordPositionError(eventID,
                                                 side,
                                                 p2.first - p1.first,
                                                 p2.second - p1.second);
        inx.pop_back();
        outx.pop_back();
    }
    side = "y";
    while ( (!iny.empty()) && (!outy.empty()) )
    {
        std::pair<G4double, G4double> p1 = iny.back(), p2 = outy.back();
        fTesterHistoManager->RecordPositionError(eventID,
                                                 side,
                                                 p2.first - p1.first,
                                                 p2.second - p1.second);
        iny.pop_back();
        outy.pop_back();
    }
    side = "z";
    while ( (!inz.empty()) && (!outz.empty()) )
    {
        std::pair<G4double, G4double> p1 = inz.back(), p2 = outz.back();
        fTesterHistoManager->RecordPositionError(eventID,
                                                 side,
                                                 p2.first - p1.first,
                                                 p2.second - p1.second);
        inz.pop_back();
        outz.pop_back();
    }

    fTesterHistoManager->AddToInitHisto(event->GetPrimaryVertex()
                            ->GetPrimary()->GetKineticEnergy());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroEventAction::RecordCell(const G4Event* event)
{
    MicroCellWallHitsCollection* cwhits = static_cast<MicroCellWallHitsCollection*>
        (event->GetHCofThisEvent()->GetHC(fCellWall));

    MicroCellInteriorHitsCollection* cihits =
        static_cast<MicroCellInteriorHitsCollection*>
        (event->GetHCofThisEvent()->GetHC(fCellInterior));

    std::map<G4int, G4double> cellshit;

    for (size_t it = 0; it != cwhits->GetSize(); it++){
        MicroCellWallHit* cwhit = static_cast<MicroCellWallHit*> (cwhits->GetHit(it));
        fCellHistoManager->RecordCellWallHit(cwhit->GetEventID(),
                                             cwhit->GetCellID(),
                                             cwhit->GetEnergy(),
                                             cwhit->GetPosition(),
                                             cwhit->GetMomDirection(),
                                             cwhit->GetParticleName());
        cellshit[cwhit->GetCellID()] = 0;
    }

    for (size_t it = 0; it != cihits->GetSize(); it++){
        MicroCellInteriorHit* cihit =
            static_cast<MicroCellInteriorHit*> (cihits->GetHit(it));
        fCellHistoManager->RecordCellInteriorHit( cihit->GetEventID(),
                                                  cihit->GetCellID(),
                                                  cihit->GetPosition(),
                                                  cihit->GetEdep(),
                                                  cihit->GetTLength());

        try{
            cellshit.at(cihit->GetCellID()) += cihit->GetEdep();
        }
        catch (const std::out_of_range& oor) {
            G4cerr << "Out of Range error: " << oor.what() << G4endl;
            std::cout << cihit->GetCellID() <<std::endl;
          }

    }

    G4ThreeVector halflengths =
        0.5*(static_cast<const MicroDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction()))
        ->GetParaWorldSize();
    G4ThreeVector macrolengths =
        (static_cast<const MicroDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction()))
        ->GetMacroWorldSize();

    G4ThreeVector pos0 = event->GetPrimaryVertex()->GetPosition();
    G4double einit = event->GetPrimaryVertex()->GetPrimary()
        ->GetKineticEnergy();
    G4String ptype = event->GetPrimaryVertex()->GetPrimary()
        ->GetParticleDefinition()->GetParticleName();
    G4ThreeVector dir0 = event->GetPrimaryVertex()->GetPrimary()
        ->GetMomentumDirection();

    MicroEventInformation* eventInfo =
        (MicroEventInformation*) event->GetUserInformation();

    G4ThreeVector offset =  G4ThreeVector(
        2*halflengths.getX()
        *((G4double) eventInfo->getX()) - 0.5*macrolengths.getX(),
        2*halflengths.getY()
        *((G4double) eventInfo->getY()) - 0.5*macrolengths.getY(),
        2*halflengths.getZ()
        *((G4double) eventInfo->getZ()) - 0.5*macrolengths.getZ());

    pos0 = pos0 + offset;

    G4int eventID = event->GetEventID();
    G4double ncells = 0;
    G4double endepo = 0;
    for (std::map<G4int, G4double>::iterator it = cellshit.begin();
        it != cellshit.end(); it++)
    {
        ncells++;
        endepo += it->second;
        if (it->second > 0)
        {
            fCellHistoManager->RecordCellDepo(eventID, it->first, einit,
                it->second);
        }
    }

    fCellHistoManager->RecordEventSummary(eventID, einit, ptype, pos0, dir0,
        endepo, ncells);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector MicroEventAction::GetParaWorldLengths()
{
    G4Box* repSolid = static_cast<G4Box*> (G4SolidStore::GetInstance()
        ->GetSolid("ParaWorldRepeating"));

    G4ThreeVector halfLengths( repSolid->GetXHalfLength(),
                               repSolid->GetYHalfLength(),
                               repSolid->GetZHalfLength() );
    return halfLengths;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
