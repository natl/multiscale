#ifndef MICRO_TESTER_SENSITIVE_DETECTOR_HH
#define MICRO_TESTER_SENSITIVE_DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

#include "MicroTesterHit.hh"
#include "G4Box.hh"

class G4Step;
class G4HCofThisEvent;
class G4Event;


class MicroTesterSensitiveDetector : public G4VSensitiveDetector
{
public:
    MicroTesterSensitiveDetector(const G4String& name, const G4String& hcname);

    virtual ~MicroTesterSensitiveDetector();

    // methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

private:
    MicroTesterHitsCollection* fMicroTesterHitsCollection;
    G4double fMinX, fMaxX, fMinY, fMaxY, fMinZ, fMaxZ;
};



#endif // SENSITIVE_DETECTOR_HH
