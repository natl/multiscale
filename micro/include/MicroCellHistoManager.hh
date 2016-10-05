#ifndef MICRO_CELL_HISTO_MANAGER_HH
#define MICRO_CELL_HISTO_MANAGER_HH

#include "MicroAnalysis.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"


class MicroCellHistoManager
{
    public:
        MicroCellHistoManager();
        virtual ~MicroCellHistoManager();

        void Initialise();
        void Close();

        void RecordCellWallHit(G4int, G4int, G4double, G4ThreeVector,
            G4ThreeVector, G4String);
        void RecordCellInteriorHit(G4int, G4int, G4ThreeVector,
            G4double, G4double);

        void RecordEventSummary(G4int, G4double, G4String, G4ThreeVector, G4ThreeVector, G4double, G4int);

        void RecordCellDepo(G4int, G4int, G4double, G4double);


    protected:
    private:

        G4AnalysisManager* fAnalysisManager;

        //Histogram IDs
        G4double fHistoCWEl, fHistoCWGa, fHistoCellInt, fHistoEinits;

        //Accumulated Energy Tuple
        G4int fCellWall, fCWEventID, fCWCellID, fCWEnergy, fCWPosX, fCWPosY;
        G4int fCWPosZ, fCWDirX, fCWDirY, fCWDirZ, fCWParticle;

        //Energy Deposition Tuple
        G4int fCellInt, fCIEventID, fCICellID, fCIPosX, fCIPosY, fCIPosZ;
        G4int fCIEnergy, fCITLength;

        // Event Summary Tuple
        G4int fEvt, fEvtID, fEvtEinit, fEvtPType, fEvtEdepo, fEvtNCells;
        G4int fEvtPosX, fEvtPosY, fEvtPosZ, fEvtDirX, fEvtDirY, fEvtDirZ;

        // Cell Event tuples
        G4int fCDep, fCDepEvtID, fCDepCellID, fCDepEinit, fCDepEdep;
};



#endif // MICRO_CELL_HISTO_MANAGER_HH
