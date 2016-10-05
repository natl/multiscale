#ifndef HISTOMANAGER_HH
#define HISTOMANAGER_HH

#include "globals.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "MacroAnalysis.hh"


class MacroHistoManager
{
    public:
        MacroHistoManager();
        virtual ~MacroHistoManager();

        void Initialise();
        void Close();

        void RecordAccumulation(G4int EventID, G4double einit, G4double edep);
        void RecordEnergyDeposit(G4int eventID, G4int trackID, G4String ptype,
            G4double edep, G4int nsecs, G4String processName,
            G4ThreeVector pos);
        void RecordSecondary(G4int eventID, G4int trackID, G4String particleName, G4double charge, G4double edep, G4double ekin, G4ThreeVector pos, G4ThreeVector dir);

        void AddToAccumulatedEdepHisto(G4double edep);
        void AddToIndividualEdepHisto(G4double edep);
        void AddToSecondaryHisto(G4double eElectron);
        void AddToInitHisto(G4double einit);


        G4double GetMeanEdep() const{return fAnalysisManager->GetH1(1)->mean();}
        G4double GetRMSEdep()  const{return fAnalysisManager->GetH1(1)->rms();}
        void PrintEdep()       const{return fAnalysisManager->GetH1(1)->hprint(std::cout);}

    protected:
    private:

        G4AnalysisManager* fAnalysisManager;
        //Accumulated Energy Tuple
        G4int fAccum, fAccumEventID, fAccumEinit, fAccumEdep;
        //Energy Deposition Tuple
        G4int fDepos, fDeposEventID, fDeposTrackID, fDeposPType, fDeposEdep;
        G4int fDeposNSavedSecs, fDeposProcess;
        G4int fDeposPosX, fDeposPosY, fDeposPosZ;
        //Secondaries Tuple
        G4int fSecondaries, fSecondaryEventID, fSecondaryTrackID, fSecondaryType, fSecondaryCharge, fSecondaryEdep, fSecondaryKEnergy;
        G4int fSecondaryPosX, fSecondaryPosY, fSecondaryPosZ, fSecondaryDirX, fSecondaryDirY, fSecondaryDirZ;
};



#endif // HISTOMANAGER_HH
