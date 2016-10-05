#ifndef MACRODETECTORMESSENGER_H
#define MACRODETECTORMESSENGER_H

#include "globals.hh"
#include "G4UImessenger.hh"

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class MacroDetectorConstruction;


class MacroDetectorMessenger: public G4UImessenger
{
    public:
        MacroDetectorMessenger(MacroDetectorConstruction*);
        virtual ~MacroDetectorMessenger();

        virtual void SetNewValue(G4UIcommand*, G4String);

    protected:
    private:
        MacroDetectorConstruction* fpDetector;

        G4UIdirectory*             fpDirectory;
        G4UIcmdWithABool*          fpUseCustomGeomCmd;
        G4UIcmdWithAString*        fpCustomGeomNameCmd;
        G4UIcmdWithAString*        fpSampleShapeCmd;
        G4UIcmdWith3VectorAndUnit* fpSampleXYZCmd;
        G4UIcmdWith3VectorAndUnit* fpSamplePosCmd;
        G4UIcmdWithADoubleAndUnit* fpSampleRadCmd;
        G4UIcmdWithADouble*        fpSampleRotAngCmd;
        G4UIcmdWith3Vector*        fpSampleRotVecCmd;
        G4UIcmdWithAString*        fpSampleMaterialCmd;
        G4UIcmdWithAString*        fpWorldMaterialCmd;
        G4UIcmdWith3VectorAndUnit* fpWorldSizeCmd;
        G4UIcmdWithADoubleAndUnit* fpEnThresholdCmd;
        G4UIcmdWithoutParameter*   fpUpdateCmd;
};

#endif // MACRODETECTORMESSENGER_H
