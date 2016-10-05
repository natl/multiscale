#ifndef MICRO_TRACK_INFORMATION_HH
#define MICRO_TRACK_INFORMATION_HH

#include "G4VUserTrackInformation.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MicroTrackInformation : public G4VUserTrackInformation
{
    public:
        MicroTrackInformation(G4int, G4int, G4int);
        MicroTrackInformation(MicroTrackInformation*);
        virtual ~MicroTrackInformation();

        // inline void *operator new(size_t);
        // inline void operator delete(void*);

        virtual void Print() const;

        bool operator==(const MicroTrackInformation&) const;
        bool operator!=(const MicroTrackInformation&) const;

        G4ThreeVector subtract(const MicroTrackInformation&) const;

        G4int getX() const {return xrpt;};
        G4int getY() const {return yrpt;};
        G4int getZ() const {return zrpt;};

        G4int decX() {return xrpt--;};
        G4int incX() {return xrpt++;};
        G4int decY() {return yrpt--;};
        G4int incY() {return yrpt++;};
        G4int decZ() {return zrpt--;};
        G4int incZ() {return zrpt++;};

        /*
        Note: once called, sets fEnteringCell to false
        This is bad code design.
        */
        G4bool IsEnteringCell() { return fEnteringCell; };
        void SetEnteringCell() {fEnteringCell = true; };
        void UnsetEnteringCell() {fEnteringCell = false; };

        void AddEnteringSecondaryTrackID (G4int id)
        {
            fpEnteringTrackIDs->push_back(id);
        };

        std::vector<G4int>* GetEnteringTrackIDs() const
        {
            return fpEnteringTrackIDs;
        };

    protected:
    private:
        G4int xrpt, yrpt, zrpt;
        G4bool fEnteringCell;
        std::vector<G4int>* fpEnteringTrackIDs;
};
/*
extern G4ThreadLocal G4Allocator<MicroTrackInformation>*
    aTrackInformationAllocator;

inline void* MicroTrackInformation::operator new(size_t)
{
  if(!aTrackInformationAllocator)
    aTrackInformationAllocator = new G4Allocator<MicroTrackInformation>;
  return (void*)aTrackInformationAllocator->MallocSingle();
 }

 inline void MicroTrackInformation::operator delete(void* aTrackInfo)
 { aTrackInformationAllocator->FreeSingle((MicroTrackInformation*) aTrackInfo);}
*/
#endif // MICRO_TRACK_INFORMATION_HH
