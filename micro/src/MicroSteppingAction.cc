#include "MicroSteppingAction.hh"

#include "MicroDetectorConstruction.hh"
#include "MicroTrackInformation.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RunManager.hh"

MicroSteppingAction::MicroSteppingAction():
    G4UserSteppingAction()
{
    MicroDetectorConstruction* det =
        (MicroDetectorConstruction*) G4RunManager::GetRunManager()
            ->GetUserDetectorConstruction();
    fpWaterVolume = det->GetWaterLogical();
    fpCellVolume = det->GetCellLogical();
}

MicroSteppingAction::~MicroSteppingAction()
{
    //dtor
}

void MicroSteppingAction::UserSteppingAction(const G4Step* aStep)
{
    G4VPhysicalVolume* preStepPhysical = aStep->GetPreStepPoint()
        ->GetTouchableHandle()->GetVolume();
    G4VPhysicalVolume* postStepPhysical = aStep->GetPostStepPoint()
        ->GetTouchableHandle()->GetVolume();

    G4LogicalVolume* preStepLogical =
        (preStepPhysical) ? preStepPhysical->GetLogicalVolume() : NULL;
    G4LogicalVolume* postStepLogical =
        (postStepPhysical) ? postStepPhysical->GetLogicalVolume() : NULL;

    if ((preStepLogical == fpWaterVolume)  && (postStepLogical == fpCellVolume))
    {
        MicroTrackInformation* trackInfo =
            (MicroTrackInformation*) aStep->GetTrack()->GetUserInformation();
        trackInfo->SetEnteringCell();

        const std::vector<const G4Track*>* secs = aStep->GetSecondaryInCurrentStep();
        G4ThreeVector postStepPosition = aStep->GetPostStepPoint()->GetPosition();

        for (auto it = secs->begin(); it != secs->end(); it++)
        {
            if ((*it)->GetPosition() == postStepPosition)
            {
                trackInfo->AddEnteringSecondaryTrackID((*it)->GetTrackID());
            }
        }
    }
}
