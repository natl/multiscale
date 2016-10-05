/******************************************************************************
 *
 * \file MicroCellSensitiveDetector.cc
 * \brief Declaration of MicroCellSensitiveDetector.cc
 *
 *****************************************************************************/
#ifndef MICRO_CELL_SENSITIVE_DETECTOR_HH
#define MICRO_CELL_SENSITIVE_DETECTOR_HH

#include "G4VSensitiveDetector.hh"

#include "MicroCellInteriorHit.hh"
#include "MicroCellWallHit.hh"

#include "globals.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Step;
class G4HCofThisEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MicroCellSensitiveDetector : public G4VSensitiveDetector
{
public:
    MicroCellSensitiveDetector(const G4String& name);

    virtual ~MicroCellSensitiveDetector();

    // methods from base class
    virtual void   Initialize(G4HCofThisEvent*);
    virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    virtual void   EndOfEvent(G4HCofThisEvent*);



private:
    MicroCellWallHitsCollection*   fMicroCellWallHitsCollection;
    MicroCellInteriorHitsCollection*  fMicroCellInteriorHitsCollection;
    G4ThreeVector fNBoxes;

};

#endif // MICRO_CELL_SENSITIVE_DETECTOR_HH
