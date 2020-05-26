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
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
//
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"
#include "G4BGGNucleonInelasticXS.hh"

#include "G4HadronicProcess.hh"
#include "G4ProcessTable.hh"
#include "G4ParticleTypes.hh"
#include "G4ProcessManager.hh"
#include "G4RunManager.hh"
#include "HistoManager.hh"
#include "G4NistManager.hh"
#include "Run.hh"

#ifndef __WITHOUT_ROOT__
#include "TreeManager-XS4GCR.hh"  // XS4GCR
#endif
#include "G4DecayTable.hh"
#include "G4VDecayChannel.hh"  // XS4GCR

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction() : G4UserSteppingAction() {}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction() {}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep) {
    Run* run = static_cast<Run*>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun());
    
    
    // count processes
    //
    const G4StepPoint* endPoint = aStep->GetPostStepPoint();
    G4VProcess* process =
        const_cast<G4VProcess*>(endPoint->GetProcessDefinedStep());
    run->CountProcesses(process);

    // check that an real interaction occured (eg. not a transportation)
    G4StepStatus stepStatus = endPoint->GetStepStatus();
    G4bool transmit =
        (stepStatus == fGeomBoundary || stepStatus == fWorldBoundary);
    if (transmit) return;

    // real processes : sum track length
    //
    G4double stepLength = aStep->GetStepLength();
    run->SumTrack(stepLength);

    // energy-momentum balance initialisation
    //
    const G4StepPoint* prePoint = aStep->GetPreStepPoint();
    G4double Q = -prePoint->GetKineticEnergy();
    //std::cout << " prePoint = " << prePoint->GetKineticEnergy() << std::endl;
    G4ThreeVector Pbalance = -prePoint->GetMomentum();

    // initialisation of the nuclear channel identification
    //
    G4ParticleDefinition* particle = aStep->GetTrack()->GetDefinition();
    G4String partName = particle->GetParticleName();
        
    G4String nuclearChannel = partName;
    G4HadronicProcess* hproc = dynamic_cast<G4HadronicProcess*>(process);
    const G4Isotope* target = NULL;
    if (hproc) target = hproc->GetTargetIsotope();
    //std::cout << "target->GetName() -> " << target->GetName() << std::endl;
    G4String targetName = "XXXX";
    if (target) targetName = target->GetName();
    nuclearChannel += " + " + targetName + " --> ";
    if (targetName == "XXXX") run->SetTargetXXX(true);

    //retrieve inelastic Cross Section
    const G4DynamicParticle* dp = aStep->GetTrack()->GetDynamicParticle();
    G4ThreeVector mom = prePoint->GetMomentumDirection();
    //std::cout << mom.X << " " << mom.Y << " " << mom.Z << std::endl;
    G4DynamicParticle* ndp = new G4DynamicParticle( dp->GetParticleDefinition(), mom, prePoint->GetKineticEnergy() );
    G4Element* elm = new G4Element(target->GetName(),"",target->GetZ(), target->GetA());
    G4NistManager* nistManager = G4NistManager::Instance();
    //G4Material* g4_c = nistManager->FindOrBuildMaterial("G4_C");
    G4double xsection = hproc->GetElementCrossSection (ndp, elm);//, g4_c) ;//, const G4Material *mat=0)
#ifndef __WITHOUT_ROOT__
    TreeManager::Instance()->inelXsec = xsection / CLHEP::millibarn;  // XS4GCR
#endif

    
    //G4String bulletName = ndp->GetParticleDefinition()->GetParticleName();
    //G4double kin = ndp->GetKineticEnergy();
    //G4BGGNucleonInelasticXS* bgg = new G4BGGNucleonInelasticXS(ndp->GetParticleDefinition());
    //G4int targetZ = target->GetZ();
    //G4bool isapplicable = bgg->IsElementApplicable(ndp,targetZ,g4_c); 
    //G4double xsection = bgg->GetElementCrossSection(ndp,targetZ,g4_c);
    //G4HadronicProcess* hp = new G4HadronicProcess();
    //std::cout << "bulletName = " << bulletName << " kinetic energy = " << kin/CLHEP::MeV << " target->GetName() -> " << target->GetName() << " Z = " << targetZ << " isApplicable = " << isapplicable << " xsection = " << xsection/ CLHEP::millibarn << std::endl;
    
   
    // scattered primary particle (if any)
    //
    G4AnalysisManager* analysis = G4AnalysisManager::Instance();
    G4int ih = 1;
    if (aStep->GetTrack()->GetTrackStatus() == fAlive) {
        G4double energy = endPoint->GetKineticEnergy();
        analysis->FillH1(ih, energy);
        //
        G4ThreeVector momentum = endPoint->GetMomentum();
        Q += energy;
        Pbalance += momentum;
        //
        nuclearChannel += partName + " + ";
	
    }


    // secondaries
    //
   
    
    const std::vector<const G4Track*>* secondary =
        aStep->GetSecondaryInCurrentStep();
#ifndef __WITHOUT_ROOT__
    TreeManager::Instance()->nSec = (*secondary).size();  // XS4GCR
#endif
    for (size_t lp = 0; lp < (*secondary).size(); lp++) {
        particle = (*secondary)[lp]->GetDefinition();
        G4String name = particle->GetParticleName();
        G4String type = particle->GetParticleType();
        G4double energy = (*secondary)[lp]->GetKineticEnergy();

        G4int A = particle->GetAtomicMass();    // XS4GCR
        G4int Z = particle->GetAtomicNumber();  // XS4GCR

        run->ParticleCount(name, energy);

        // energy spectrum
        ih = 0;
        if (particle == G4Gamma::Gamma())
            ih = 2;
        else if (particle == G4Neutron::Neutron())
            ih = 3;
        else if (particle == G4Proton::Proton())
            ih = 4;
        else if (particle == G4Deuteron::Deuteron())
            ih = 5;
        else if (particle == G4Alpha::Alpha())
            ih = 6;
        else if (type == "nucleus")
            ih = 7;
        else if (type == "meson")
            ih = 8;
        else if (type == "baryon")
            ih = 9;

#ifndef __WITHOUT_ROOT__
        if (type == "nucleus") {
            TreeManager::Instance()->ExcitationE =
                ((G4Ions*)particle)->GetExcitationEnergy() / CLHEP::eV;
        }

        G4ThreeVector momentumDirection =
            (*secondary)[lp]->GetMomentumDirection();
        // G4double angle = momentumDirection.theta();
        TreeManager::Instance()->theta =
            momentumDirection.theta() / CLHEP::degree;
        TreeManager::Instance()->phi = momentumDirection.phi() / CLHEP::degree;
        TreeManager::Instance()->Ek = energy / CLHEP::MeV;
        TreeManager::Instance()->ParticleName = name;
        TreeManager::Instance()->ParticleType = type;
        TreeManager::Instance()->A = particle->GetAtomicMass();
        TreeManager::Instance()->Z = particle->GetAtomicNumber();
        TreeManager::Instance()->IsStable = particle->GetPDGStable();
        TreeManager::Instance()->IsShortLived = particle->IsShortLived();
        TreeManager::Instance()->LifeTime = particle->GetPDGLifeTime();
        G4DecayTable* decayTable = particle->GetDecayTable();
        if (decayTable) {
            TreeManager::Instance()->NDecayChannels = decayTable->entries();
        } else {
            TreeManager::Instance()->NDecayChannels = -1;
        }
#endif

        if (ih > 0) analysis->FillH1(ih, energy);
        // atomic mass
        if (type == "nucleus") {
            G4int A = particle->GetAtomicMass();
            analysis->FillH1(12, A);
        }

        // energy-momentum balance
        G4ThreeVector momentum = (*secondary)[lp]->GetMomentum();
        Q += energy;
        Pbalance += momentum;
        // count e- from internal conversion together with gamma
        if (particle == G4Electron::Electron()) particle = G4Gamma::Gamma();
        // particle flag
        fParticleFlag[particle]++;

#ifndef __WITHOUT_ROOT__
        TreeManager::Instance()->Fill();
        TreeManager::Instance()->Clear();
#endif
    }

    // energy-momentum balance
    G4double Pbal = Pbalance.mag();
    run->Balance(Pbal);
    ih = 10;
    analysis->FillH1(ih, Q);
    ih = 11;
    analysis->FillH1(ih, Pbal);

    // nuclear channel
    const G4int kMax = 16;
    const G4String conver[] = {"0",   "",    "2 ",  "3 ",  "4 ",  "5 ",
                               "6 ",  "7 ",  "8 ",  "9 ",  "10 ", "11 ",
                               "12 ", "13 ", "14 ", "15 ", "16 "};
    std::map<G4ParticleDefinition*, G4int>::iterator ip;
    for (ip = fParticleFlag.begin(); ip != fParticleFlag.end(); ip++) {
        particle = ip->first;
        G4String name = particle->GetParticleName();
        G4int nb = ip->second;
        if (nb > kMax) nb = kMax;
        G4String Nb = conver[nb];
        if (particle == G4Gamma::Gamma()) {
            run->CountGamma(nb);
            Nb = "N ";
            name = "gamma or e-";
        }
        if (ip != fParticleFlag.begin()) nuclearChannel += " + ";
        nuclearChannel += Nb + name;
    }

    /// G4cout << "\n nuclear channel: " << nuclearChannel << G4endl;
    run->CountNuclearChannel(nuclearChannel, Q);

    fParticleFlag.clear();

    // kill event after first interaction
    //
    G4RunManager::GetRunManager()->AbortEvent();
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
