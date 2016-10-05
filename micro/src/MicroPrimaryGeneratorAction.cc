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
// This example is provided by the Geant4-DNA collaboration
// Any report or published results obtained using the Geant4-DNA software
// shall cite the following Geant4-DNA collaboration publication:
// Med. Phys. 37 (2010) 4692-4708
// The Geant4-DNA web site is available at http://geant4-dna.org
//
// $Id$
//
/// \file MicroPrimaryGeneratorAction.cc
/// \brief Implementation of the MicroPrimaryGeneratorAction class

#include "MicroPrimaryGeneratorAction.hh"

#include "MicroPrimaryGeneratorMessenger.hh"
#include "MicroEventInformation.hh"
#include "MicroDetectorConstruction.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4RandomDirection.hh"
#include "G4PrimaryVertex.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"

#include "G4Geantino.hh"

#include "G4RootAnalysisReader.hh"
#include "G4RunManager.hh"

#include <algorithm>
#include <math.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroPrimaryGeneratorAction::MicroPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fpParticleGun(0),
   fUseRandomPosition(false),
   fUseRandomDirection(false),
   fUserPosition(G4ThreeVector(0,0,0)),
   fUserDirection(G4ThreeVector(1,0,0)),
   fUseHisto(false),
   fUseRootFile(false),
   fRootFilename(""),
   fpRootEvents(0),
   fpRootPositions(0),
   fpRootDirections(0),
   fpRootEnergies(0),
   fCurrentEventIndex(0),
   fNRootEvents(0)
{
    fpParticleGun = new G4ParticleGun();
    fpGeneratorMessenger = new MicroPrimaryGeneratorMessenger(this);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MicroPrimaryGeneratorAction::~MicroPrimaryGeneratorAction()
{
    delete fpParticleGun;
    delete fpRootEvents;
    delete fpRootPositions;
    delete fpRootDirections;
    delete fpRootEnergies;
    delete fpRootPTypes;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    if (fUseRootFile)
        DoNextRootEvent(anEvent);
    else
        DoNextEvent(anEvent);
}

void MicroPrimaryGeneratorAction::DoNextEvent(G4Event* anEvent)
{

    G4ThreeVector nboxes=  (static_cast<const MicroDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction()))->GetNBoxes();

    // If the position is set, then we start in the middlemost box
    // A custom position in the macro-domain can be implenented using the
    // Generate Position method
    G4int x, y, z;

    if (fUseRandomPosition){
        x = std::floor(G4UniformRand()*nboxes.getX());
        y = std::floor(G4UniformRand()*nboxes.getY());
        z = std::floor(G4UniformRand()*nboxes.getZ());
    }
    else {
        x = std::floor(nboxes.getX()/2);
        y = std::floor(nboxes.getY()/2);
        z = std::floor(nboxes.getZ()/2);
    }

    MicroEventInformation* eventInfo = new MicroEventInformation(x, y, z);
    anEvent->SetUserInformation(eventInfo);

    fpParticleGun->SetParticleMomentumDirection(GenerateDirection());
    fpParticleGun->SetParticlePosition(GeneratePosition());
    if (fUseHisto) fpParticleGun->SetParticleEnergy(GetHistoEnergy());
    fpParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroPrimaryGeneratorAction::DoNextRootEvent(G4Event* anEvent)
{
    if (fCurrentEventIndex == fNRootEvents)
    {
        fCurrentEventIndex = 0;
        // Randomize events Vector
        std::random_shuffle(fpRootEvents->begin(), fpRootEvents->end(), fRandDist);
    }

    G4ThreeVector halfLengths =
        0.5*(static_cast<const MicroDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction()))
        ->GetParaWorldSize();
    G4ThreeVector nboxes =
        (static_cast<const MicroDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction()))
        ->GetNBoxes();

    G4int idx = fpRootEvents->at(fCurrentEventIndex++);

    G4ThreeVector currentPosition = fpRootPositions->at(idx);
    G4ThreeVector currentDirection = fpRootDirections->at(idx);
    G4double currentEnergy = fpRootEnergies->at(idx)*MeV;
    G4String currentParticle = fpRootPTypes->at(idx);

    G4int xbox = std::floor((currentPosition.getX()*mm)/(2*halfLengths.getX())
                            + 0.5*nboxes.getX());
    G4int ybox = std::floor((currentPosition.getY()*mm)/(2*halfLengths.getY())
                            + 0.5*nboxes.getY());
    G4int zbox = std::floor((currentPosition.getZ()*mm)/(2*halfLengths.getZ())
                            + 0.5*nboxes.getZ());

    // Add this bit because we prefer to create at the +ve edge of a valid box
    // than the -ve edge of an invalid box
    if (xbox == nboxes.getX()) xbox --;
    if (ybox == nboxes.getY()) ybox --;
    if (zbox == nboxes.getZ()) zbox --;
    MicroEventInformation* eventInfo =
        new MicroEventInformation(xbox, ybox, zbox);
    anEvent->SetUserInformation(eventInfo);

    G4double x = currentPosition.getX() + nboxes.getX()*halfLengths.getX()
                 - 2*halfLengths.getX()*xbox;
    G4double y = currentPosition.getY() + nboxes.getY()*halfLengths.getY()
                 - 2*halfLengths.getY()*ybox;
    G4double z = currentPosition.getZ() + nboxes.getZ()*halfLengths.getZ()
                 - 2*halfLengths.getZ()*zbox;

    fpParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));
    fpParticleGun->SetParticleMomentumDirection(currentDirection);
    fpParticleGun->SetParticleEnergy(currentEnergy);

    G4ParticleDefinition* thisParticleDefn = G4ParticleTable::GetParticleTable()->FindParticle(currentParticle);

    if (!thisParticleDefn)
    {
        //Need to make the ion
        G4String chemicalSymbol;
        G4String atomicMass;
        G4String chars[] = {'0','1', '2', '3', '4', '5', '6', '7', '8', '9'};

        for (unsigned ii = 0; ii != currentParticle.length(); ii++)
        {
            G4bool isDigit = std::find(std::begin(chars), std::end(chars),
                                       currentParticle[ii]) != std::end(chars);
            if (isDigit) atomicMass.append(currentParticle[ii]);
            // else if (currentParticle[ii] == "[") break;
            else chemicalSymbol.append(currentParticle[ii]);
        }
        G4int mass = std::stoi(atomicMass);

        const G4String* it = std::find(G4IonTable::elementName,
                                       std::end(G4IonTable::elementName),
                                       chemicalSymbol);

        G4int number = std::distance(G4IonTable::elementName, it) + 1;
        G4cout << "Adding an ion of " << chemicalSymbol
               << " (number " <<  number << " mass " << atomicMass
               << ") To the particle table" << G4endl;

        if (it != std::end(G4IonTable::elementName))
        {

            thisParticleDefn =
                G4IonTable::GetIonTable()->GetIon(number, mass, 0);
        }
        else
        {
            G4cerr << "Element Not Found: " << currentParticle << G4endl;
            thisParticleDefn = G4Geantino::Geantino();
        }
    }
    fpParticleGun->SetParticleDefinition(thisParticleDefn);

    fpParticleGun->GeneratePrimaryVertex(anEvent);
}

void MicroPrimaryGeneratorAction::SetupRootFile()
{
    G4RootAnalysisReader* analysisReader = G4RootAnalysisReader::Instance();
    analysisReader->SetVerboseLevel(1);

    G4int ntupleID = analysisReader->GetNtuple("SecPS", fRootFilename);

    G4ThreeVector macroLengths =
        0.5*(static_cast<const MicroDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction()))
        ->GetMacroWorldSize();

    macroLengths = 0.5*macroLengths;

    if (ntupleID >= 0)
    {
        // Read in tuple
        G4double posx, posy, posz;
        G4double dirx, diry, dirz;
        G4double energy;
        G4String ptype;
        fpRootEvents = new std::vector<int>();
        fpRootPositions = new std::vector<G4ThreeVector>();
        fpRootDirections = new std::vector<G4ThreeVector>();
        fpRootEnergies = new std::vector<G4double>();
        fpRootPTypes = new std::vector<G4String>();
        fCurrentEventIndex = 0;
        analysisReader->SetNtupleDColumn(ntupleID, "PosX", posx);
        analysisReader->SetNtupleDColumn(ntupleID, "PosY", posy);
        analysisReader->SetNtupleDColumn(ntupleID, "PosZ", posz);
        analysisReader->SetNtupleDColumn(ntupleID, "DirX", dirx);
        analysisReader->SetNtupleDColumn(ntupleID, "DirY", diry);
        analysisReader->SetNtupleDColumn(ntupleID, "DirZ", dirz);
        analysisReader->SetNtupleDColumn(ntupleID, "KineticEnergy", energy);
        analysisReader->SetNtupleSColumn(ntupleID, "ParticleType", ptype);

        G4int nInvalidEvents = 0;
        G4bool isValidParticle;
        while (analysisReader->GetNtupleRow(ntupleID) )
        {
            isValidParticle = true;
            // !(ptype.compareTo("alpha")*ptype.compareTo("e-")
            //    *ptype.compareTo("mu-")*ptype.compareTo("proton"));
            if (isValidParticle)
            {
                fpRootEvents->push_back(fCurrentEventIndex - nInvalidEvents);
                fpRootPositions->push_back(G4ThreeVector(posx/mm, posy/mm, posz/mm));
                fpRootDirections->push_back(G4ThreeVector(dirx, diry, dirz));
                fpRootEnergies->push_back(energy/MeV);
                fpRootPTypes->push_back(ptype);
            }
            else
            {
                nInvalidEvents++;
            }
            fCurrentEventIndex++;
        }

        if (nInvalidEvents>0)
        {
            G4cerr << nInvalidEvents << " of " << fCurrentEventIndex
                   << " total events were ignored as the particle has not"
                   << " been defined." << G4endl;
        }

        fNRootEvents = fCurrentEventIndex - nInvalidEvents;
        fCurrentEventIndex = fNRootEvents;
    }
    else
    {
        G4cerr << "Did not identify correct tuple in root file" << G4endl;
        G4cerr << "Expect a segmentation fault" << G4endl;
    }

    delete analysisReader;
}

G4ThreeVector MicroPrimaryGeneratorAction::GeneratePosition()
{
    if (!fUseRandomPosition) return fUserPosition;

    G4ThreeVector halfLengths =
        0.5*(static_cast<const MicroDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction()))
        ->GetParaWorldSize();

    // Set Box Boundaries in contructor
    G4double x = (1. - 2.*G4UniformRand())*halfLengths.getX();
    G4double y = (1. - 2.*G4UniformRand())*halfLengths.getY();
    G4double z = (1. - 2.*G4UniformRand())*halfLengths.getZ();

    G4ThreeVector pos(x,y,z);

    return pos;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector MicroPrimaryGeneratorAction::GenerateDirection()
{
    if (!fUseRandomDirection) return fUserDirection;

    G4ThreeVector dir = G4RandomDirection();
    return dir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MicroPrimaryGeneratorAction::DoHisto()
{
    if (fHistEns.size() >= 2)
    {
        fNPoints = (G4int) fHistCnts.size();
        //copy arrays in std::vector and compute fMax
        xx.resize(fNPoints); ff.resize(fNPoints);
        for (G4int jj=0; jj<fNPoints; jj++)
        {
            xx[jj] = fHistEns[jj]; ff[jj] = fHistCnts[jj];
        }

        //compute slopes
        aa.resize(fNPoints);
        for (G4int jj=0; jj<fNPoints-1; jj++)
        {
            aa[jj] = (ff[jj+1] - ff[jj])/(xx[jj+1] - xx[jj]);
        };

        //compute cumulative function
        fc.resize(fNPoints);
        fc[0] = 0.;
        for (G4int jj=1; jj<fNPoints; jj++)
        {
            fc[jj] = fc[jj-1] + 0.5*(ff[jj] + ff[jj-1])*(xx[jj] - xx[jj-1]);
        };
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double MicroPrimaryGeneratorAction::GetHistoEnergy()
{
    // tabulated function
    // f is assumed positive, linear per segment, continuous
    // --> cumulative function is second order polynomial

    //choose y randomly
    G4double y_rndm = G4UniformRand()*fc[fNPoints-1];
    //find bin
    G4int jj = fNPoints-2;
    while ((fc[jj] > y_rndm) && (jj > 0)) jj--;
    //y_rndm --> x_rndm :  Fc(x) is second order polynomial
    G4double x_rndm = xx[jj];
    G4double hh = aa[jj];
    if (hh != 0.)
    {
        G4double b = ff[jj]/hh, c = 2*(y_rndm - fc[jj])/hh;
        G4double delta = b*b + c;
        G4int sign = 1;
        if (hh < 0.) sign = -1;
        x_rndm += sign*std::sqrt(delta) - b;
    }
    else if (ff[jj] > 0.)
    {
        x_rndm += (y_rndm - fc[jj])/ff[jj];
    };
    return x_rndm;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
