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
/// \file RunAction.cc
/// \brief Implementation of the RunAction class
//
//
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"

#include <iomanip>

#include "DetectorConstruction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"  // XS4GCR
#include "G4SystemOfUnits.hh"
#include "G4Timer.hh"  // XS4GCR
#include "G4UnitsTable.hh"
#include "G4Version.hh"  // XS4GCR
#include "HistoManager.hh"
#include "PrimaryGeneratorAction.hh"
#include "Randomize.hh"
#include "Run.hh"
#include "RunMessenger.hh"

#ifndef __WITHOUT_ROOT__
#include "TreeManager-Xs4GCR.hh"  // XS4GCR
#endif

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim)
    : G4UserRunAction(),
      fDetector(det),
      fPrimary(prim),
      fRun(0),
      fHistoManager(0),
      fRunMessenger(0),
      fPrint(true) {
    timer = new G4Timer;  // XS4GCR
    fHistoManager = new HistoManager();
    fRunMessenger = new RunMessenger(this);
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction() {
    delete fHistoManager;
    delete fRunMessenger;
    delete timer;  // XS4GCR
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* RunAction::GenerateRun() {
    fRun = new Run(fDetector);
    return fRun;
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun) {
    timer->Start();
#ifndef __WITHOUT_ROOT__
    TreeManager::Instance()->Run = aRun->GetRunID();  // XS4GCR
#endif

    // show Rndm status
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);  // XS4GCR
    if (isMaster) G4Random::showEngineStatus();

    // keep run condition
    if (fPrimary) {
        G4ParticleDefinition* particle =
            fPrimary->GetParticleGun()->GetParticleDefinition();
        G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
        fRun->SetPrimary(particle, energy);
    }

    // histograms
    //
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    if (analysisManager->IsActive()) {
        analysisManager->OpenFile();
    }

    G4String version;  // XS4GCR
    // remove blank spaces
    for (size_t i = 0; i < G4Version.length(); i++)
        if (G4Version[i] != ' ' && G4Version[i] != '$') version += G4Version[i];
    // check if there is "Name" in front, if so remove 5 chars
    if (version.substr(0, 4) == "Name") {
        G4String vs = version;
        version = vs.substr(5, vs.size() - 5);
    }
#ifndef __WITHOUT_ROOT__
    TreeManager::Instance()->G4version = version;  // XS4GCR
#endif
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*) {
    if (isMaster) fRun->EndOfRun(fPrint);

    // save histograms
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    if (analysisManager->IsActive()) {
        analysisManager->Write();
        analysisManager->CloseFile();
    }

    // show Rndm status
    if (isMaster) G4Random::showEngineStatus();

    timer->Stop();
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::SetPrintFlag(G4bool flag) { fPrint = flag; }

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
