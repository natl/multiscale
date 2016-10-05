#include "MacroDetectorMessenger.hh"

#include "MacroDetectorConstruction.hh"

#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4SystemOfUnits.hh"

MacroDetectorMessenger::MacroDetectorMessenger(
    MacroDetectorConstruction* pDetector):
    G4UImessenger(),
    fpDetector(pDetector)
{
    fpDirectory = new G4UIdirectory("/macro/det/", this);
    fpDirectory->SetGuidance("Directory containing macro commands to ");
    fpDirectory->SetGuidance("manipulate the detector construction");

    fpUseCustomGeomCmd = new G4UIcmdWithABool("/macro/det/useCustomGeometry", this);
    fpUseCustomGeomCmd->SetGuidance("True to use a custom geometry");
    fpUseCustomGeomCmd->SetParameterName("custGeom", false);
    fpUseCustomGeomCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpCustomGeomNameCmd = new G4UIcmdWithAString("/macro/det/geometryName", this);
    fpCustomGeomNameCmd->SetGuidance("Name of the custom geometry");
    fpCustomGeomNameCmd->SetGuidance("Available geometries are: ");
    fpCustomGeomNameCmd->SetGuidance(fpDetector->CustomGeometries());
    fpCustomGeomNameCmd->SetParameterName("geomName", false);
    fpCustomGeomNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSampleShapeCmd = new G4UIcmdWithAString("/macro/det/geometry", this);
    fpSampleShapeCmd->SetGuidance("Detector shape: ellipsoid, box, sphere");
    fpSampleShapeCmd->SetParameterName("shape", false);
    fpSampleShapeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSampleXYZCmd = new G4UIcmdWith3VectorAndUnit("/macro/det/size", this);
    fpSampleXYZCmd->SetGuidance("Detector half sizes: x y z unit");
    fpSampleXYZCmd->SetParameterName("xHalfSize", "yHalfSize", "zHalfSize", false);
    fpSampleXYZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSamplePosCmd = new G4UIcmdWith3VectorAndUnit("/macro/det/position", this);
    fpSamplePosCmd->SetGuidance("Detector centre position: x y z unit");
    fpSamplePosCmd->SetParameterName("x", "y", "z", false);
    fpSamplePosCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSampleRadCmd = new G4UIcmdWithADoubleAndUnit("/macro/det/rad", this);
    fpSampleRadCmd->SetGuidance("sample radius");
    fpSampleRadCmd->SetParameterName("radius", false);
    fpSampleRadCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSampleRotAngCmd = new G4UIcmdWithADouble("/macro/det/rotAngle", this);
    fpSampleRotAngCmd->SetGuidance("roation angle in degrees");
    fpSampleRotAngCmd->SetParameterName("angle", false);
    fpSampleRotAngCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSampleRotVecCmd = new G4UIcmdWith3Vector("/macro/det/rotVector", this);
    fpSampleRotVecCmd->SetGuidance("vector about which to rotate geometry");
    fpSampleRotVecCmd->SetParameterName("x", "y", "z", false);
    fpSampleRotVecCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpSampleMaterialCmd = new G4UIcmdWithAString("/macro/det/sampleMat", this);
    fpSampleMaterialCmd->SetGuidance("Sample Material");
    fpSampleMaterialCmd->SetParameterName("sampleMat", false);
    fpSampleMaterialCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpWorldMaterialCmd = new G4UIcmdWithAString("/macro/det/worldMat", this);
    fpWorldMaterialCmd->SetGuidance("World material");
    fpWorldMaterialCmd->SetParameterName("worldMat", false);
    fpWorldMaterialCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpWorldSizeCmd = new G4UIcmdWith3VectorAndUnit("/macro/det/worldSize", this);
    fpWorldSizeCmd->SetGuidance("World Half Size: x y z unit");
    fpWorldSizeCmd->SetParameterName("xHalfWorldSize", "yHalfWorldSize", "zHalfWorldSize", false);
    fpWorldSizeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpEnThresholdCmd = new G4UIcmdWithADoubleAndUnit("/macro/det/energyThreshold", this);
    fpEnThresholdCmd->SetGuidance("Min energy in sensitive detector");
    fpEnThresholdCmd->SetParameterName("EnThreshold", false);
    fpEnThresholdCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fpUpdateCmd = new G4UIcmdWithoutParameter("/micro/det/update", this);
    fpUpdateCmd->SetGuidance("Update geometry.");
    fpUpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
    fpUpdateCmd->SetGuidance("if you changed geometrical value(s).");
    fpUpdateCmd->AvailableForStates(G4State_Idle);


}

MacroDetectorMessenger::~MacroDetectorMessenger()
{
    delete fpUseCustomGeomCmd;
    delete fpCustomGeomNameCmd;
    delete fpSampleShapeCmd;
    delete fpSamplePosCmd;
    delete fpSampleXYZCmd;
    delete fpSampleRadCmd;
    delete fpSampleRotAngCmd;
    delete fpSampleRotVecCmd;
    delete fpSampleMaterialCmd;
    delete fpWorldMaterialCmd;
    delete fpWorldSizeCmd;
    delete fpEnThresholdCmd;
    delete fpUpdateCmd;
    delete fpDirectory;
}

void MacroDetectorMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
    if (cmd == fpUseCustomGeomCmd)
        fpDetector->SetUseCustomGeom(fpUseCustomGeomCmd->GetNewBoolValue(newValue));
    else if (cmd == fpCustomGeomNameCmd)
        fpDetector->SetCustomGeomName(newValue);
    else if (cmd == fpSampleShapeCmd)
        fpDetector->SetSampleShape(newValue);
    else if (cmd == fpSampleXYZCmd)
        fpDetector->SetSampleXYZ(fpSampleXYZCmd->GetNew3VectorValue(newValue));
    else if (cmd == fpSamplePosCmd)
        fpDetector->SetSamplePos(fpSamplePosCmd->GetNew3VectorValue(newValue));
    else if (cmd == fpSampleRadCmd)
        fpDetector->SetSampleRad(fpSampleRadCmd->GetNewDoubleValue(newValue));
    else if (cmd == fpSampleRotAngCmd)
        fpDetector->SetSampleRotAngle(fpSampleRotAngCmd->GetNewDoubleValue(newValue));
    else if (cmd == fpSampleRotVecCmd)
        fpDetector->SetSampleRotVector(fpSampleRotVecCmd->GetNew3VectorValue(newValue));
    else if (cmd == fpSampleMaterialCmd)
        fpDetector->SetSampleMat(newValue);
    else if (cmd == fpWorldMaterialCmd)
        fpDetector->SetWorldMat(newValue);
    else if (cmd == fpWorldSizeCmd)
        fpDetector->SetWorldSize(fpWorldSizeCmd->GetNew3VectorValue(newValue));
    else if (cmd == fpEnThresholdCmd)
        fpDetector->SetEnThreshold(fpEnThresholdCmd->GetNewDoubleValue(newValue));
    else if (cmd == fpUpdateCmd)
        fpDetector->UpdateGeometry();
}
