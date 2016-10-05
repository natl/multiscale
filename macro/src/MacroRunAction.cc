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
//
/// \file MacroRunAction.cc
/// \brief Implementation of the MacroRunAction class

#include "MacroRunAction.hh"
#include "MacroAnalysis.hh"

#include "G4RootAnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MacroRunAction::MacroRunAction(MacroHistoManager* histo)
: G4UserRunAction(),
fHistoManager(histo)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MacroRunAction::~MacroRunAction()
{
    delete fHistoManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroRunAction::BeginOfRunAction(const G4Run*)
{
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  fHistoManager->Initialise();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MacroRunAction::EndOfRunAction(const G4Run*)
{

    fHistoManager->Close();

    if (IsMaster())
    {
        // Load Analysis Data and save as a Csv phase space
        G4int nworkers;
        if (G4RunManager::GetRunManager()->GetRunManagerType()
            == G4RunManager::sequentialRM)
        {
            nworkers = -1;
        }
        else
        {
            nworkers = ((G4MTRunManager*) G4RunManager::GetRunManager())
                ->GetNumberOfThreads();
        }

        G4AnalysisReader* analysisReader = G4AnalysisReader::Instance();
        analysisReader->SetVerboseLevel(1);

        G4double posx, posy, posz;
        G4double dirx, diry, dirz;
        G4double energy;
        G4String ptype;
        std::vector<G4double> posXs = std::vector<G4double>();
        std::vector<G4double> posYs = std::vector<G4double>();
        std::vector<G4double> posZs = std::vector<G4double>();
        std::vector<G4double> dirXs = std::vector<G4double>();
        std::vector<G4double> dirYs = std::vector<G4double>();
        std::vector<G4double> dirZs = std::vector<G4double>();
        std::vector<G4double> energies = std::vector<G4double>();
        std::vector<G4String> ptypes = std::vector<G4String>();

        G4String filename = "macro.root";
        if (nworkers > 0)
        {
            for (G4int ii = 0; ii < nworkers; ii++)
            {
                G4String localfile = filename;
                localfile = GetThreadedFileName(localfile, ii);
                G4cout << "Reading " << localfile << G4endl;
                G4int ntupleID = analysisReader->GetNtuple("SecondarySpectrum",
                    localfile);

                analysisReader->SetNtupleDColumn(ntupleID, "PosX", posx);
                analysisReader->SetNtupleDColumn(ntupleID, "PosY", posy);
                analysisReader->SetNtupleDColumn(ntupleID, "PosZ", posz);
                analysisReader->SetNtupleDColumn(ntupleID, "DirX", dirx);
                analysisReader->SetNtupleDColumn(ntupleID, "DirY", diry);
                analysisReader->SetNtupleDColumn(ntupleID, "DirZ", dirz);
                analysisReader->SetNtupleDColumn(ntupleID, "KineticEnergy",
                    energy);
                analysisReader->SetNtupleSColumn(ntupleID, "ParticleType",
                    ptype);
                while (analysisReader->GetNtupleRow(ntupleID) )
                {
                    posXs.push_back(posx);
                    posYs.push_back(posy);
                    posZs.push_back(posz);
                    dirXs.push_back(dirx);
                    dirYs.push_back(diry);
                    dirZs.push_back(dirz);
                    energies.push_back(energy);
                    ptypes.push_back(ptype);
                }
            }
        }
        else
        {
            G4cout << "Reading " << filename << G4endl;
            G4int ntupleID = analysisReader->GetNtuple("SecondarySpectrum",
                filename);

            analysisReader->SetNtupleDColumn(ntupleID, "PosX", posx);
            analysisReader->SetNtupleDColumn(ntupleID, "PosY", posy);
            analysisReader->SetNtupleDColumn(ntupleID, "PosZ", posz);
            analysisReader->SetNtupleDColumn(ntupleID, "DirX", dirx);
            analysisReader->SetNtupleDColumn(ntupleID, "DirY", diry);
            analysisReader->SetNtupleDColumn(ntupleID, "DirZ", dirz);
            analysisReader->SetNtupleDColumn(ntupleID, "KineticEnergy", energy);
            analysisReader->SetNtupleSColumn(ntupleID, "ParticleType",  ptype);
            while (analysisReader->GetNtupleRow(ntupleID) )
            {
                    posXs.push_back(posx);
                    posYs.push_back(posy);
                    posZs.push_back(posz);
                    dirXs.push_back(dirx);
                    dirYs.push_back(diry);
                    dirZs.push_back(dirz);
                    energies.push_back(energy);
                    ptypes.push_back(ptype);
            }
        }
        delete analysisReader;

        G4cout << "Writing Phase Space File for Secondaries" << G4endl;
        G4RootAnalysisManager* csvWriter = G4RootAnalysisManager::Instance();
        csvWriter->SetVerboseLevel(1);
        csvWriter->OpenFile("secondary_ps.root");

        G4int secondaries   = csvWriter->CreateNtuple("SecPS",
            "SecondaryPhaseSpace");
        G4int secondaryPosX = csvWriter->CreateNtupleDColumn(secondaries,
            "PosX");
        G4int secondaryPosY = csvWriter->CreateNtupleDColumn(secondaries,
            "PosY");
        G4int secondaryPosZ = csvWriter->CreateNtupleDColumn(secondaries,
            "PosZ");
        G4int secondaryDirX = csvWriter->CreateNtupleDColumn(secondaries,
            "DirX");
        G4int secondaryDirY = csvWriter->CreateNtupleDColumn(secondaries,
            "DirY");
        G4int secondaryDirZ = csvWriter->CreateNtupleDColumn(secondaries,
            "DirZ");
        G4int secondaryKE   = csvWriter->CreateNtupleDColumn(secondaries,
            "KineticEnergy");
        G4int particleType  = csvWriter->CreateNtupleSColumn(secondaries,
            "ParticleType");
        csvWriter->FinishNtuple(secondaries);


        for (G4int ii = 0; ii != (G4int) posXs.size(); ii++)
        {
            csvWriter->FillNtupleDColumn(secondaries, secondaryPosX, posXs[ii]);
            csvWriter->FillNtupleDColumn(secondaries, secondaryPosY, posYs[ii]);
            csvWriter->FillNtupleDColumn(secondaries, secondaryPosZ, posZs[ii]);
            csvWriter->FillNtupleDColumn(secondaries, secondaryDirX, dirXs[ii]);
            csvWriter->FillNtupleDColumn(secondaries, secondaryDirY, dirYs[ii]);
            csvWriter->FillNtupleDColumn(secondaries, secondaryDirZ, dirZs[ii]);
            csvWriter->FillNtupleDColumn(secondaries, secondaryKE,
                energies[ii]);
            csvWriter->FillNtupleSColumn(secondaries, particleType,
                ptypes[ii]);
            csvWriter->AddNtupleRow(secondaries);
        }

        csvWriter->Write();
        csvWriter->CloseFile();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

 G4String MacroRunAction::GetThreadedFileName(const G4String& baseFileName,
          G4int threadID) const
 {
    G4String name(baseFileName);
    G4String extension;

    extension = name.substr(name.find("."));
    name = name.substr(0, name.find("."));

    std::ostringstream os;
    os << threadID;
    name.append("_t");
    name.append(os.str());

    // Add (back if it was present) file extension
    name.append(extension);

    return name;
 }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
