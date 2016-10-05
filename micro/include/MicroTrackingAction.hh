#ifndef MICROTRACKINGACTION_H
#define MICROTRACKINGACTION_H

#include "G4UserTrackingAction.hh"

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MicroTrackInformation;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MicroTrackingAction : public G4UserTrackingAction
{
    public:
        MicroTrackingAction();
        virtual ~MicroTrackingAction();

       virtual void PreUserTrackingAction(const G4Track*);
       virtual void PostUserTrackingAction(const G4Track*);
    protected:
    private:
    MicroTrackInformation* fpTrackInfoAtStart;
    G4ThreeVector fHalfLengths, fHalfBoxes;
};

#endif // MICROTRACKINGACTION_H
