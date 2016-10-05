#include "MicroParallelWorldConstruction.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroParallelWorldConstruction::MicroParallelWorldConstruction(G4String worldName):
    G4VUserParallelWorld(worldName),
    fParaWorldSize(200.*um)
{
    //ctor
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroParallelWorldConstruction::~MicroParallelWorldConstruction()
{
    //dtor
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroParallelWorldConstruction::Construct()
{
    if (true)
    {
        G4cout << "Bulding parallel world with side length "
               << fParaWorldSize/um << " microns" <<G4endl;
    }
    G4VPhysicalVolume* ghostWorld = GetWorld();
    G4LogicalVolume* worldLogical = ghostWorld->GetLogicalVolume();

    //Create/Define repeating region here.

    G4double repeatRegionX = fParaWorldSize.getX();
    G4double repeatRegionY = fParaWorldSize.getY();
    G4double repeatRegionZ = fParaWorldSize.getZ();

    //Could be defined later using more messenger hooks
    G4ThreeVector repeatRegionOffset(0);

    G4Box* repeatingRegionPhysical = new G4Box( "ParaWorldRepeating",
        0.5*repeatRegionX,
        0.5*repeatRegionY,
        0.5*repeatRegionZ );

    G4LogicalVolume* repeatingRegionLogical =
        new G4LogicalVolume( repeatingRegionPhysical,
        NULL,
        "ParaWorldRepeating");

    new G4PVPlacement(0,
        repeatRegionOffset,
        repeatingRegionLogical,
        "ParaWorldRepeating",
        worldLogical,
        0,
        0,
        false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
