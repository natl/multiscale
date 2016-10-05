#ifndef MICRO_STEPPING_ACTION_HH
#define MICRO_STEPPING_ACTION_HH

#include "G4UserSteppingAction.hh"

class G4LogicalVolume;

class MicroSteppingAction: public G4UserSteppingAction
{
    public:
        MicroSteppingAction();
        virtual ~MicroSteppingAction();

        void UserSteppingAction(const G4Step*);

    protected:
    private:
        G4LogicalVolume* fpWaterVolume;
        G4LogicalVolume* fpCellVolume;
};

#endif // MICROSTEPPINGACTION_HH
