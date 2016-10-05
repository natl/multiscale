#ifndef SENSITIVE_DETECTOR_HH
#define SENSITIVE_DETECTOR_HH

// Parent includes
#include "G4VSensitiveDetector.hh"

// G4 includes
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
// external includes
#include <vector>
#include <stdio.h>
// Project includes
#include "MacroAccumulatorHit.hh"
#include "MacroEnergyDepoHit.hh"
#include "MacroSecondaryHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4Event;


class MacroSensitiveDetector : public G4VSensitiveDetector
{
public:
    MacroSensitiveDetector(const G4String&, G4double);

    virtual ~MacroSensitiveDetector();

    // methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);



private:
    MacroAccumulatorHitsCollection* fAccumulatorHitsCollection;
    MacroSecondaryHitsCollection*   fSecondaryHitsCollection;
    MacroEnergyDepoHitsCollection*  fEnergyDepoHitsCollection;
    G4double fEnThreshold;

    void SaveTrack(G4Step*, const G4Track*, G4ThreeVector);

};



#endif // SENSITIVE_DETECTOR_HH
