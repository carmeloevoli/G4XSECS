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
/// \file eventgenerator/particleGun/src/PrimaryGeneratorMessenger.cc
/// \brief Implementation of the PrimaryGeneratorMessenger class
//
// $Id: PrimaryGeneratorMessenger.cc 98734 2016-08-09 10:53:12Z gcosmo $
//
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorMessenger-XS4GCR.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIdirectory.hh"
#include "PrimaryGeneratorAction.hh"

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(
    PrimaryGeneratorAction* Gun)
    : G4UImessenger(), fAction(Gun), fDir(0), fSetEnergyCmd(0), fRandCmd(0) {
    fDir = new G4UIdirectory("/mygun/");
    fDir->SetGuidance("Set primary parameters");

    fSetEnergyCmd = new G4UIcmdWithADoubleAndUnit("/mygun/energy", this);
    fSetEnergyCmd->SetGuidance("Select primary energy");
    fSetEnergyCmd->SetParameterName("energy", true);
    fSetEnergyCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    //  fSelectActionCmd->SetRange("id>=0 && id<5");

    // fRandCmd = new G4UIcmdWithABool("/mygun/rand",this);
    // fRandCmd->SetGuidance("Set a uniform random energy distribution for
    // primaries"); fRandCmd->SetParameterName("rand",false);
    // fRandCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fRandCmd = new G4UIcmdWithAString("/mygun/rand", this);
    fRandCmd->SetGuidance(
        "Set a uniform or exponential random energy distribution for "
        "primaries");
    fRandCmd->SetParameterName("rand", false);
    fRandCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fRandCmd->SetToBeBroadcasted(false);
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger() {
    delete fSetEnergyCmd;
    delete fRandCmd;
    delete fDir;
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,
                                            G4String newValue) {
    if (command == fSetEnergyCmd) {
        G4double e = fSetEnergyCmd->GetNewDoubleValue(newValue);
        fAction->SetBeamEnergy(e);
        //      fAction->SelectAction(fSelectActionCmd->GetNewIntValue(newValue));
    }
    if (command == fRandCmd) {
      //fAction->SetRandom(fRandCmd->GetNewBoolValue(newValue));
        G4cout << "PrimaryGeneratorMessenger: setting random distribution for "
                  "primaries: "
               << newValue << G4endl;
        if (newValue == "false") {
            fAction->SetRandom(false);
            fAction->SetRandomExp(false);
        } else if (newValue == "uniform" || newValue == "unif") {
            fAction->SetRandom(true);
        } else if (newValue == "exponential" || newValue == "exp") {
            fAction->SetRandomExp(true);
        } else {
            G4cerr << "***** Command is not found !!! " << newValue << G4endl;
        }
    }
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
