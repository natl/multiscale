#include "MicroTrackingAction.hh"

#include "MicroTrackInformation.hh"
#include "MicroEventInformation.hh"
#include "MicroDetectorConstruction.hh"
#include "G4EventManager.hh"

#include "G4TrackingManager.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroTrackingAction::MicroTrackingAction():
fpTrackInfoAtStart(0)
{
    G4ThreeVector macrolengths = (static_cast<const MicroDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction()))
        ->GetMacroWorldSize();

    fHalfBoxes = G4ThreeVector(std::floor(macrolengths.getX()
                               /(2*fHalfLengths.getX())),
                               std::floor(macrolengths.getY()
                               /(2*fHalfLengths.getY())),
                               std::floor(macrolengths.getZ()
                               /(2*fHalfLengths.getZ())));

     fHalfLengths = 0.5*(static_cast<const MicroDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction()))
        ->GetParaWorldSize();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroTrackingAction::~MicroTrackingAction()
{
    //dtor
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
    // Set the track information to track the global position if not set
    if ( !aTrack->GetUserInformation() )
    {
        MicroEventInformation* eventInfo = (MicroEventInformation*)
            G4EventManager::GetEventManager()->GetConstCurrentEvent()
            ->GetUserInformation();

        MicroTrackInformation* trackInfo = new MicroTrackInformation(
            eventInfo->getX(),
            eventInfo->getY(),
            eventInfo->getZ());

        fpTrackingManager->SetUserTrackInformation(trackInfo);
    }
    fpTrackInfoAtStart = new MicroTrackInformation(
        (MicroTrackInformation*) aTrack->GetUserInformation());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
    G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
    if (secondaries)
    {
        MicroTrackInformation* info =
            (MicroTrackInformation*)(aTrack->GetUserInformation());

        G4ThreeVector posChange = info->subtract(fpTrackInfoAtStart);

        size_t nSeco = secondaries->size();

        if (nSeco>0)
        {
            for(size_t i=0; i < nSeco; i++)
            {
                // If the positions are different same, we aren't at the
                // boundary and so the macro domain doesn't change
                if (aTrack->GetPosition() != (*secondaries)[i]->GetPosition())
                {
                    (*secondaries)[i]->SetUserInformation(
                        new MicroTrackInformation(fpTrackInfoAtStart));

                }
                else
                {
                    // determine the direction of travel
                    G4ThreeVector secMom = (*secondaries)[i]
                        ->GetMomentumDirection();
                    G4bool leaving = (posChange.dot(secMom) > 0);
                    G4bool onEdge = ((info->getX() <= -1)
                                     || (info->getY() <= -1)
                                     || (info->getZ() <= -1)
                                     || (info->getX() >= fHalfBoxes.getX())
                                     || (info->getY() >= fHalfBoxes.getY())
                                     || (info->getZ() >= fHalfBoxes.getZ()));

                    // Transport the particle if a) leaving and
                    // b) not at global edge
                    if (leaving && !onEdge)
                    {
                        G4ThreeVector change(
                            posChange.getX()*2*fHalfLengths.getX(),
                            posChange.getY()*2*fHalfLengths.getY(),
                            posChange.getZ()*2*fHalfLengths.getZ());
                        G4ThreeVector newPos =
                            (*secondaries)[i]->GetPosition() - change;

                        (*secondaries)[i]->SetPosition(newPos);
                        (*secondaries)[i]->SetUserInformation(
                            new MicroTrackInformation(info));

                    }
                    else if (leaving && onEdge)
                    {
                        (*secondaries)[i]->SetTrackStatus(fStopAndKill);
                    }
                    else
                    {
                        // Not Leving
                        (*secondaries)[i]->SetUserInformation(
                            new MicroTrackInformation(fpTrackInfoAtStart));
                    }
                } // endif

                // Set secondaries to inherit the correct information about
                // whether they are entering a cell
                for (auto it = info->GetEnteringTrackIDs()->begin();
                     it != info->GetEnteringTrackIDs()->end(); it++)
                {
                    if ((*secondaries)[i]->GetTrackID() == *it)
                    {
                        ((MicroTrackInformation*) (*secondaries)[i]->GetUserInformation())
                            ->SetEnteringCell();
                        G4cout << "secondary at wall" << G4endl;
                    }
                }
            }
        }
    }
    delete fpTrackInfoAtStart;
}
