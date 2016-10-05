//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// Note: Histogram methods from eventgenerator example
//
/// \file MacroPrimaryGeneratorAction.cc
/// \brief Implementation of the MacroPrimaryGeneratorAction class

#include "MacroPrimaryGeneratorAction.hh"

#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"
#include "G4RandomDirection.hh"
#include "G4PhysicalConstants.hh"
#include "G4RotationMatrix.hh"

#include "MacroPrimaryGeneratorMessenger.hh"

#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MacroPrimaryGeneratorAction::MacroPrimaryGeneratorAction():
    G4VUserPrimaryGeneratorAction(),
    fUseHisto(false),
    fUseHalfDir(false),
    fUseIsoDir(false),
    fUseCosDir(false),
    fFixedDirection(false),
    fIsSphere(false),
    fIsPlane(false),
    fIsBox(false),
    fIsPoint(false),
    fIsCircle(false),
    fIsMicroplate(false),
    fIsMuon(false),
    fParticleGun(0)
{
    fParticleGun = new G4ParticleGun();
    fMessenger = new MacroPrimaryGeneratorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MacroPrimaryGeneratorAction::~MacroPrimaryGeneratorAction()
{
    delete fMessenger;
    delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    if (fIsMuon) {
        // Use muon ratio at 1 GeV/c
        if (G4UniformRand() < 0.54)
        {
            fParticleGun->SetParticleDefinition(G4MuonPlus::Definition());
        }
        else
        {
            fParticleGun->SetParticleDefinition(G4MuonMinus::Definition());
        }
    }
    fParticleGun->SetParticleEnergy(GetEnergy());
    G4ThreeVector pos = GetPosition();
    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(GetEmissionDirection(pos));
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroPrimaryGeneratorAction::Initialise()
{
    G4ParticleTable* ptable = G4ParticleTable::GetParticleTable();

    // if muon flag is set, particle is determined on a per-run basis
    if (!fIsMuon)
    {
        fParticleGun->SetParticleDefinition(
            ptable->FindParticle(fParticleType));
    }
    // shape
    if (fGeometry.compare("sphere") == 0)
    {
        fIsSphere = true;
        G4cout << "Spherical Source Selected" << G4endl;
    }
    else if (fGeometry.compare("plane") == 0)
    {
        fIsPlane  = true;
        G4cout << "Planar Source Selected" << G4endl;
        fSurfaceNormal = G4ThreeVector(0,0,1);
    }
    else if (fGeometry.compare("box") == 0)
    {
        fIsBox    = true;
        G4cout << "Volumetric Box Source Selected" << G4endl;
    }
    else if (fGeometry.compare("point") == 0)
    {
        fIsPoint  = true;
        G4cout << "Point Source Selected" << G4endl;
    }
    else if (fGeometry.compare("circle") == 0)
    {
        fIsCircle = true;
        G4cout << "Circular Source Selected" << G4endl;
        fSurfaceNormal = G4ThreeVector(0,0,1);
    }
    else if (fGeometry.compare("microplate") == 0)
    {
        fIsMicroplate = true;
        G4cout << "Microplate Source Selected" << G4endl;
    }
    else
    {
        G4cout << "No Source geometry defined" << G4endl;
    }

    // Is a surface
    if (fIsSphere || fIsPlane || fIsCircle) fUseHalfDir = true;

    if ( (fIsCircle || fIsPlane) && fRotAngle)
        fSurfaceNormal = fSurfaceNormal.rotate(fRotAngle, fRotDirection);

    // angle
    if (fAngleDist.compare("uniform") == 0) fFixedDirection = true;
    else if (fAngleDist.compare("iso") == 0) fUseIsoDir = true;
    else if (fAngleDist.compare("cos") == 0) fUseCosDir = true;

    //histo
    if (fHistEns.size() > 0)
    {
        fUseHisto = true;
        DoHisto();
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroPrimaryGeneratorAction::DoHisto()
{
    fNPoints = (G4int) fHistCnts.size();
    G4double total_counts;

    //copy arrays in std::vector and compute fMax
    //
    xx.resize(fNPoints); ff.resize(fNPoints);
    for (G4int jj=0; jj<fNPoints; jj++)
    {
        xx[jj] = fHistEns[jj];
        total_counts = total_counts + fHistCnts[jj];
    }

    xx.resize(fNPoints); ff.resize(fNPoints);
    for (G4int jj=0; jj<fNPoints; jj++)
    {
        ff[jj] = fHistCnts[jj]/total_counts;
    }
/*
    //compute slopes
    //
    aa.resize(fNPoints);
    for (G4int jj=0; jj<fNPoints-1; jj++)
    {
        aa[jj] = (ff[jj+1] - ff[jj])/(xx[jj+1] - xx[jj]);
    };
*/

    //compute cumulative function
    //
    fc.resize(fNPoints);
    fc[0] = 0.;
    for (G4int jj=1; jj<fNPoints; jj++)
    {
        fc[jj] = fc[jj-1] + ff[jj];
    };
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double MacroPrimaryGeneratorAction::GetEnergy()
{
    if (fUseHisto) return GetHistoEnergy();
    if (fUseAngleEnergyFile) return GetAngEnSpecifiedEnergy();
    else return fEnergy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double MacroPrimaryGeneratorAction::GetHistoEnergy()
{
    //choose y randomly
    G4double y_rndm = G4UniformRand();
    //find bin
    G4int jj = fNPoints-2;
    while ((fc[jj] > y_rndm) && (jj > 0)) jj--;

    G4double x_rndm = xx[jj];
    G4double hh = xx[jj+1]-xx[jj];
    x_rndm = x_rndm + (y_rndm - fc[jj])/(fc[jj+1] - fc[jj])*hh;

    return x_rndm;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector MacroPrimaryGeneratorAction::GetPosition()
{
    if (fIsPlane) return GetPlanePosition();
    else if (fIsSphere) return GetSpherePosition();
    else if (fIsBox) return GetBoxPosition();
    else if (fIsMicroplate) return GetMicroplatePosition();
    else if (fIsCircle) return GetCirclePosition();
    else if (fIsPoint) return fCentre;
    else
    {
        G4cout << "Error: Source position is unclear" << G4endl;
        return G4ThreeVector(0,0,0);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector MacroPrimaryGeneratorAction::GetPlanePosition()
{
    G4double x = (2*G4UniformRand()-1)*fHalfX;
    G4double y = (2*G4UniformRand()-1)*fHalfY;
    G4double z = 0;
    G4ThreeVector pos = G4ThreeVector(x,y,z);
    pos.rotate(fRotAngle, fRotDirection);
    pos = pos + fCentre;
    return pos;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector MacroPrimaryGeneratorAction::GetCirclePosition()
{
    G4double theta = 2*pi*G4UniformRand();
    G4double r = std::pow(G4UniformRand(), 0.5)*fRad;
    G4double x = r*std::cos(theta);
    G4double y = r*std::sin(theta);
    G4double z = 0;
    G4ThreeVector pos = G4ThreeVector(x,y,z);
    pos.rotate(fRotAngle, fRotDirection);
    pos = pos + fCentre;
    return pos;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector MacroPrimaryGeneratorAction::GetSpherePosition()
{
    return fCentre + G4RandomDirection()*fRad;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector MacroPrimaryGeneratorAction::GetBoxPosition()
{
    G4double x = (2*G4UniformRand()-1)*fHalfX;
    G4double y = (2*G4UniformRand()-1)*fHalfY;
    G4double z = (2*G4UniformRand()-1)*fHalfZ;
    G4ThreeVector pos(x,y,z);
    pos.rotate(fRotAngle, fRotDirection);
    pos = pos + fCentre;
    return pos;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector MacroPrimaryGeneratorAction::GetMicroplatePosition()
{
    G4double halfx = 5*mm;
    G4double halfy = 5*mm;
    G4double halfz = 7.5*mm;
    G4double gapbig = 22*mm;
    G4double gaplittle = 11*mm;
    G4ThreeVector centre(-5.5*mm, 5.5*mm, -2.5*mm);

    G4int well = std::floor(G4UniformRand()*7.);
    G4ThreeVector sourceCentre;
    if (well == 0)
    {
        sourceCentre = centre;

    }
    else if (well == 1){
        sourceCentre = centre;
        sourceCentre.setX(sourceCentre.getX() + gapbig);
    }
    else if (well == 2){
        sourceCentre = centre;
        sourceCentre.setX(sourceCentre.getX() - gapbig);
    }
    else if (well == 3){
        sourceCentre = centre;
        sourceCentre.setX(sourceCentre.getX() + gaplittle);
        sourceCentre.setY(sourceCentre.getY() + gapbig);
        sourceCentre.setZ(sourceCentre.getZ());
    }
    else if (well == 4){
        sourceCentre = centre;
        sourceCentre.setX(sourceCentre.getX() - gaplittle);
        sourceCentre.setY(sourceCentre.getY() + gapbig);
        sourceCentre.setZ(sourceCentre.getZ());
    }
    else if (well == 5){
        sourceCentre = centre;
        sourceCentre.setX(sourceCentre.getX() + gaplittle);
        sourceCentre.setY(sourceCentre.getY() - gapbig);
        sourceCentre.setZ(sourceCentre.getZ());
    }
    else if (well == 6){
        sourceCentre = centre;
        sourceCentre.setX(sourceCentre.getX() - gaplittle);
        sourceCentre.setY(sourceCentre.getY() - gapbig);
        sourceCentre.setZ(sourceCentre.getZ());
    }

    G4ThreeVector pos(sourceCentre.getX() + 2*(G4UniformRand()-0.5)*halfx,
                      sourceCentre.getY() + 2*(G4UniformRand()-0.5)*halfy,
                      sourceCentre.getZ() + 2*(G4UniformRand()-0.5)*halfz);
    return pos;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector MacroPrimaryGeneratorAction::GetEmissionDirection(
    G4ThreeVector pos)
{
    if (fUseIsoDir)
    {
        if (fUseHalfDir) return GetHalfIsoDirection(pos);
        else return G4RandomDirection();
    }
    else if (fUseCosDir) return GetHalfCosineDirection(pos);
    else if (fFixedDirection) return fEmitDirection;
    else if (fUseAngleEnergyFile) return GetAngEnSpecifiedDirection();
    else
    {
        G4cout << "Error: Output direction unclear" << G4endl;
        return G4ThreeVector(1,0,0);
    }


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector MacroPrimaryGeneratorAction::GetHalfIsoDirection(G4ThreeVector pos)
{
    // Generate the random direction, across the top half of the unit sphere
    G4double u = G4UniformRand();
    G4double v = 0.5+0.5*(G4UniformRand());
    G4double theta = 2*pi*u;
    G4double phi = std::acos(2*v-1);

    G4double cosphi = std::cos(phi);
    G4double randx = std::pow(1-cosphi*cosphi, 0.5)*std::cos(theta);
    G4double randy = std::pow(1-cosphi*cosphi, 0.5)*std::sin(theta);
    G4double randz = cosphi;

    G4ThreeVector vec_rand = G4ThreeVector(randx, randy, randz);

    // Need to do the co-ordinate transform now
    // Find the surface normal
    G4ThreeVector norm;
    if (fIsSphere)
    {
        norm = fCentre - pos; // inward facing normal
        norm = norm/norm.mag();
    }
    else
    {
        norm = fSurfaceNormal;
    }

    // Now build a reference frame around this.
    G4ThreeVector chosenVector = G4ThreeVector(0.1, 0.4, 0.7);
    chosenVector = chosenVector/chosenVector.mag();
    if (norm.dot(chosenVector) < 0.001)
    {
        chosenVector = G4ThreeVector(-0.1, 0.8, -0.2);
        chosenVector = chosenVector/chosenVector.mag();
    }
    G4ThreeVector perp_vector = norm.cross(chosenVector);
    G4ThreeVector new_x = norm.cross(perp_vector);
    G4ThreeVector new_y = norm.cross(new_x);
    // Now we have the co-ordinate system new_x, new_y, norm
    G4RotationMatrix rot(new_x, new_y, norm);

    return rot(vec_rand);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector MacroPrimaryGeneratorAction::GetHalfCosineDirection(
    G4ThreeVector pos)
{
    // Generate the cos distributed direction
    // See Greenwood (2002), Vacuum, 67(2) 217-222.
    G4double u = G4UniformRand();
    G4double v = std::pow(G4UniformRand(), 0.5);
    G4double theta = 2*pi*u;
    G4double phi = std::asin(v);

    G4double cosphi = std::cos(phi);
    G4double randx = std::pow(1-cosphi*cosphi, 0.5)*std::cos(theta);
    G4double randy = std::pow(1-cosphi*cosphi, 0.5)*std::sin(theta);
    G4double randz = cosphi;

    G4ThreeVector vec_rand = G4ThreeVector(randx, randy, randz);

    // Need to do the co-ordinate transform now
    // Find the surface normal
    G4ThreeVector norm;
    if (fIsSphere)
    {
        norm = fCentre - pos; // inward facing normal
        norm = norm/norm.mag();
    }
    else
    {
        norm = fSurfaceNormal;
    }

    // Now build a reference frame around this.
    G4ThreeVector chosenVector = G4ThreeVector(0.1, 0.4, 0.7);
    chosenVector = chosenVector/chosenVector.mag();
    if (norm.dot(chosenVector) < 0.001)
    {
        chosenVector = G4ThreeVector(-0.1, 0.8, -0.2);
        chosenVector = chosenVector/chosenVector.mag();
    }
    G4ThreeVector perp_vector = norm.cross(chosenVector);
    G4ThreeVector new_x = norm.cross(perp_vector);
    G4ThreeVector new_y = norm.cross(new_x);
    // Now we have the co-ordinate system new_x, new_y, norm
    G4RotationMatrix rot(new_x, new_y, norm);

    return rot(vec_rand);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroPrimaryGeneratorAction::LoadSpecifiedAngleEnergyFile()
{
    //Load file
    std::ifstream infile (fAngleEnergyFilename, std::ifstream::in);
    G4String currentline;

    if (infile.is_open())
    {
        while (getline(infile, currentline))
        {
            if (currentline[0] != '#')
            {
                fSpecifiedEnsAngs.push_back(std::make_pair(
                    std::stod(split(currentline, ' ').at(0))*MeV,
                    std::stod(split(currentline, ' ').at(1))*deg));
            }
        }
        infile.close();
    }
    else
    {
        G4cerr << "Histogram file read error" << G4endl;
        G4cerr << "Format should be <energy (MeV)> <angle (deg)>" << G4endl;
        G4cerr << "Separator is space, lines with '#' as the first "
               << "character are commented" << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroPrimaryGeneratorAction::PrepareNextSpecifiedEnergyAngle()
{
    if (fSpecifiedEnergyIsUnused || fSpecifiedAngleIsUnused) return;

    if (fCurrentAngleEnergyIdx == (G4int) fSpecifiedEnsAngs.size())
    {
        fCurrentAngleEnergyIdx = 0;
        // Randomize events Vector
        std::random_shuffle(fSpecifiedEnsAngs.begin(), fSpecifiedEnsAngs.end(),
            fRandDist);
    }
    fCurrentAngleEnergyIdx++;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector MacroPrimaryGeneratorAction::GetAngEnSpecifiedDirection()
{
    G4double phi = pi - fSpecifiedEnsAngs[fCurrentAngleEnergyIdx].second;
    fSpecifiedAngleIsUnused = false;
    G4double theta = 2*pi*G4UniformRand();

    G4double cosphi = std::cos(phi);
    G4double dirx = std::pow(1-cosphi*cosphi, 0.5)*std::cos(theta);
    G4double diry = std::pow(1-cosphi*cosphi, 0.5)*std::sin(theta);
    G4double dirz = cosphi;

    PrepareNextSpecifiedEnergyAngle();
    return G4ThreeVector(dirx, diry, dirz);
}


G4double MacroPrimaryGeneratorAction::GetAngEnSpecifiedEnergy()
{
    G4double energy = fSpecifiedEnsAngs[fCurrentAngleEnergyIdx].first;
    fSpecifiedEnergyIsUnused = false;

    PrepareNextSpecifiedEnergyAngle();
    return energy;
}
