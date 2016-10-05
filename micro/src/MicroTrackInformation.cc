#include "MicroTrackInformation.hh"

//G4ThreadLocal G4Allocator<MicroTrackInformation> * aTrackInformationAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroTrackInformation::MicroTrackInformation(G4int x, G4int y, G4int z):
    G4VUserTrackInformation(),
    xrpt(x),
    yrpt(y),
    zrpt(z),
    fEnteringCell(false),
    fpEnteringTrackIDs(0)
{
    fpEnteringTrackIDs = new std::vector<G4int>();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroTrackInformation::MicroTrackInformation(MicroTrackInformation* info):
    xrpt(info->getX()),
    yrpt(info->getY()),
    zrpt(info->getZ()),
    fEnteringCell(false),
    fpEnteringTrackIDs(0)
{
    fpEnteringTrackIDs = new std::vector<G4int>();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroTrackInformation::~MicroTrackInformation()
{
    delete fpEnteringTrackIDs;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool MicroTrackInformation::operator==(const MicroTrackInformation& right) const
{
    return (this->getX() == right.getX()
         && this->getY() == right.getY()
         && this->getZ() == right.getZ());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool MicroTrackInformation::operator!=(const MicroTrackInformation& right) const
{
    return !(*this == right);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector MicroTrackInformation::subtract(const MicroTrackInformation& right) const
{
    return G4ThreeVector(this->getX() - right.getX(),
                         this->getY() - right.getY(),
                         this->getZ() - right.getZ());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroTrackInformation::Print() const
{
    G4cout << "x: " << xrpt << " y: " << yrpt << " z: " << zrpt << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
