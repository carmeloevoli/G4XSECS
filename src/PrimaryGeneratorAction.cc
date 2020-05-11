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
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class
//
//
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"
#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#ifndef __WITHOUT_ROOT__
#include "TreeManager-XS4GCR.hh"
#endif
#include "PrimaryGeneratorMessenger-XS4GCR.hh"

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* det)
    : G4VUserPrimaryGeneratorAction(), fParticleGun(0), fDetector(det) {
    fParticleGun = new G4ParticleGun(1);
    G4ParticleDefinition* particle =
        G4ParticleTable::GetParticleTable()->FindParticle("proton");  // XS4GCR
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(1 * MeV);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1., 0., 0.));
    messenger = new PrimaryGeneratorMessenger(this);  // XS4GCR
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
    delete messenger;
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    // this function is called at the begining of event
    //
    G4double halfSize = 0.5 * (fDetector->GetSize());
    G4double z0 = -halfSize;

    // randomize (y0,z0)
    //
    G4double beam = 0.8 * halfSize;
    G4double y0 = (2 * G4UniformRand() - 1.) * beam;
    // G4double z0 = (2 * G4UniformRand() - 1.) * beam;
    G4double x0 = (2 * G4UniformRand() - 1.) * beam;  // XS4GCR

    G4double E = fE;
    if (fGenerateUniformEnergyDistribution) {  // XS4GCR TODO understand this
        G4double minE = 10.0 * MeV;
        G4double logE =
            log10(minE) + G4UniformRand() * (log10(fE) - log10(minE));
        E = pow(10., logE);
    } else if (fGenerateExponentialEnergyDistribution) {
        G4double minE = 10.0 * MeV;
        G4double logE =
            log10(minE) + G4UniformRand() * (log10(fE) - log10(minE));
        E = pow(10., logE);
    }

    fParticleGun->SetParticleEnergy(E);  // XS4GCR
    fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));

#ifndef __WITHOUT_ROOT__
    TreeManager::Instance()->PrimaryE =
        fParticleGun->GetParticleEnergy() / MeV;  // XS4GCR
    TreeManager::Instance()->PrimaryName =
        fParticleGun->GetParticleDefinition()->GetParticleName();
#endif

    fParticleGun->GeneratePrimaryVertex(anEvent);
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
