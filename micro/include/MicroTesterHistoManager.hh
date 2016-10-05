#ifndef MICROTESTERHISTOMANAGER_HH
#define MICROTESTERHISTOMANAGER_HH

#include "MicroAnalysis.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class MicroTesterHistoManager
{
    public:
        MicroTesterHistoManager();
        virtual ~MicroTesterHistoManager();

        void Initialise();
        void Close();

        void RecordEvent(G4int, G4double, G4double);
        void RecordPositionError(G4int, G4String, G4double, G4double);
        void recordInsAndOuts(G4int, G4int, G4int, G4int, G4int, G4int, G4int,
            G4int, G4int, G4int);

        //Histos
        void AddToInitHisto(G4double);
        void AddToEdepHisto(G4double);
        void AddToRadiiHisto(G4double, G4double);
        void AddToTLHisto(G4double);

    protected:

    private:
        G4AnalysisManager* fAnalysisManager;

        //Histogram IDS
        G4double fHistoInits, fHistoDeps, fHistoERad, fHistoTrack;

        //Event Tuple
        G4int fEvt, fEvtEvtID, fEvtEInit, fEvtEDepo, fEvtEDiff;

        //Position Errors
        G4int fPos, fPosEvtID, fPosSide, fPosErr1, fPosErr2;

        //InsAndOuts
        G4int fIAO, fIAOEvtID, fIAOXI, fIAOXO, fIAOXP, fIAOYI, fIAOYO, fIAOYP;
        G4int fIAOZI, fIAOZO, fIAOZP;
};

#endif // MICROTESTERHISTOMANAGER_HH
