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
/// \file PhysicsList.cc
/// \brief Implementation of the PhysicsList class
//

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PhysicsList.hh"

#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsINCLXX.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"  // XS4GCR
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4IonBinaryCascadePhysics.hh"  // XS4GCR
#include "G4IonElasticPhysics.hh"
#include "G4IonINCLXXPhysics.hh"
#include "G4IonPhysicsPHP.hh"
#include "G4IonPhysicsXS.hh"
#include "G4IonQMDPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "GammaNuclearPhysics.hh"
#include "HadronPhysicsINCLXX-XS4GCR.hh"       // XS4GCR
#include "IonQMDPhysics-XS4GCR.hh"             // XS4GCR
#include "IonWilsonAbrasionPhysics-XS4GCR.hh"  // XS4GCR

// particles

#include "G4BaryonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// PhysicsList::PhysicsList() : G4VModularPhysicsList() {
//    G4int verb = 0;
//    SetVerboseLevel(verb);
//
//    // add new units for cross sections
//    //
//    new G4UnitDefinition("mm2/g", "mm2/g", "Surface/Mass", mm2 / g);
//    new G4UnitDefinition("um2/mg", "um2/mg", "Surface/Mass", um * um / mg);
//
//    // Hadron Elastic scattering
//    //
//    RegisterPhysics(new G4HadronElasticPhysicsHP(verb));
//    // RegisterPhysics( new G4HadronElasticPhysicsXS(verb));
//
//    // Hadron Inelastic physics
//    //
//    // RegisterPhysics( new G4HadronPhysicsFTFP_BERT_HP(verb));
//    RegisterPhysics(new G4HadronPhysicsQGSP_BIC_HP(verb));
//    // RegisterPhysics( new G4HadronPhysicsQGSP_BIC_AllHP(verb));
//    // RegisterPhysics( new G4HadronInelasticQBBC(verb));
//    // RegisterPhysics( new G4HadronPhysicsINCLXX(verb));
//
//    // Ion Elastic scattering
//    //
//    RegisterPhysics(new G4IonElasticPhysics(verb));
//
//    // Ion Inelastic physics
//    //
//    RegisterPhysics(new G4IonPhysicsXS(verb));
//    // RegisterPhysics( new G4IonPhysicsPHP(verb));
//    // RegisterPhysics( new G4IonQMDPhysics(verb));
//    // RegisterPhysics( new G4IonINCLXXPhysics(verb));
//
//    // Gamma physics
//    //
//    RegisterPhysics(new GammaNuclearPhysics("gamma"));
//}

PhysicsList::PhysicsList() : G4VModularPhysicsList() {  // XS4GCR
    // add new units for cross sections
    //
    new G4UnitDefinition("mm2/g", "mm2/g", "Surface/Mass", mm2 / g);
    new G4UnitDefinition("um2/mg", "um2/mg", "Surface/Mass", um * um / mg);

    messenger = new PhysicsListMessenger(this);
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList() {}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle() {
    G4BosonConstructor pBosonConstructor;
    pBosonConstructor.ConstructParticle();

    G4LeptonConstructor pLeptonConstructor;
    pLeptonConstructor.ConstructParticle();

    G4MesonConstructor pMesonConstructor;
    pMesonConstructor.ConstructParticle();

    G4BaryonConstructor pBaryonConstructor;
    pBaryonConstructor.ConstructParticle();

    G4IonConstructor pIonConstructor;
    pIonConstructor.ConstructParticle();

    G4ShortLivedConstructor pShortLivedConstructor;
    pShortLivedConstructor.ConstructParticle();
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts() { SetCutValue(0. * mm, "proton"); }

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess() {  // XS4GCR
    // Transportation
    //
    AddTransportation();

    for (size_t i = 0; i < hadronPhys.size(); i++) {
        hadronPhys[i]->ConstructProcess();
    }
}

void PhysicsList::AddPhysicsList(const G4String& name) {  // XS4GCR
    G4int verb = 2;
    SetVerboseLevel(verb);

    if (verb > 1) {
        G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
    }

    if (name == "def" || name == "default") {
        // hadronPhys.push_back(new G4HadronElasticPhysicsHP(verb));
        // hadronPhys.push_back(new G4IonElasticPhysics(verb));
        hadronPhys.push_back(new G4IonPhysics(verb));
        // RegisterPhysics( new GammaPhysics("gamma"));
    } else if (name == "bert") {
        // non ha interazioni fra ioni a quest'energia!
        hadronPhys.push_back(new G4HadronPhysicsFTFP_BERT_HP(verb));
    } else if (name == "bic") {
        hadronPhys.push_back(new G4IonBinaryCascadePhysics(verb));
        hadronPhys.push_back(new G4HadronPhysicsQGSP_BIC_HP(verb));
    } else if (name == "incl") {
        hadronPhys.push_back(new G4HadronPhysicsINCLXX(verb));
        hadronPhys.push_back(new G4IonINCLXXPhysics(verb));
    } else if (name == "incl2") {
        hadronPhys.push_back(new MyHadronPhysicsINCLXX(verb));
        hadronPhys.push_back(new G4IonINCLXXPhysics(verb));
    } else if (name == "incl-ftfp") {
        G4HadronPhysicsINCLXX* hp = new G4HadronPhysicsINCLXX(
            "hInelastic INCLXX with FTFP", true, false, true);
        hp->SetVerboseLevel(verb);
        hadronPhys.push_back(hp);
        hadronPhys.push_back(new G4IonINCLXXPhysics(verb));
    } else if (name == "qmd") {
        hadronPhys.push_back(new MyIonQMDPhysics(verb));
    } else if (name == "php") {
        // non ha interazioni fra ioni a quest'energia
        hadronPhys.push_back(new G4IonPhysicsPHP(verb));
    } else if (name == "allHP" || name == "AllHP") {
        hadronPhys.push_back(new G4HadronPhysicsQGSP_BIC_AllHP(verb));
    } else if (name == "wilson" || name == "abrasion") {
        hadronPhys.push_back(new MyIonWilsonAbrasionPhysics(verb));
    } else if (name == "qgsp") {
        hadronPhys.push_back(new G4HadronPhysicsQGSP_BERT(verb));
    } else if (name == "ftfp") {
        hadronPhys.push_back(new G4HadronPhysicsFTFP_BERT(verb));
    } else {
        G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
               << " is not defined" << G4endl;
    }
}
