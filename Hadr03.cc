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
/// \file Hadr03.cc
/// \brief Main program of the hadronic/Hadr03 example
//
//
//
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Types.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "PhysicsList.hh"
#include "Randomize.hh"
#include "SteppingVerbose.hh"

#ifndef __WITHOUT_ROOT__
#include "TreeManager-XS4GCR.hh"
#endif

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv) {
    // detect interactive mode (if no arguments) and define UI session
    G4UIExecutive* ui = nullptr;
    if (argc == 1) ui = new G4UIExecutive(argc, argv);

    // choose the Random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    // Construct the default run manager
#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    G4int nThreads = G4Threading::G4GetNumberOfCores();
    if (argc == 3) nThreads = G4UIcommand::ConvertToInt(argv[2]);
    runManager->SetNumberOfThreads(nThreads);
#else
    // my Verbose output class
    G4VSteppingVerbose::SetInstance(new SteppingVerbose);
    G4RunManager* runManager = new G4RunManager;
#endif

#ifndef __WITHOUT_ROOT__
    //  TreeManager *treeManager=new TreeManager();
    TreeManager* treeManager = TreeManager::Instance();  // XS4GCR
#endif

    // set mandatory initialization classes
    DetectorConstruction* det = new DetectorConstruction;
    runManager->SetUserInitialization(det);

    PhysicsList* phys = new PhysicsList;
    runManager->SetUserInitialization(phys);

    runManager->SetUserInitialization(new ActionInitialization(det));

    // initialize visualization
    G4VisManager* visManager = nullptr;

    // get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (ui) {
        // interactive mode
        visManager = new G4VisExecutive;
        visManager->Initialize();
        ui->SessionStart();
        delete ui;
    } else {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

#ifndef __WITHOUT_ROOT__
    treeManager->Write();  // XS4GCR
    delete treeManager;
#endif

    // job termination
    delete visManager;
    delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
