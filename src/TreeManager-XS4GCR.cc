#include "TreeManager-XS4GCR.hh"

#include <vector>

#include "G4AutoLock.hh"
#include "TreeManagerMessenger-XS4GCR.hh"

TreeManager* TreeManager::fInstance = NULL;

namespace {
// Mutex to acquire access to singleton instance check/creation
G4Mutex instanceMutex = G4MUTEX_INITIALIZER;
// Mutex to acquire accss to histograms creation/access
// It is also used to control all operations related to histos
// File writing and check analysis
G4Mutex dataManipulationMutex = G4MUTEX_INITIALIZER;
}  // namespace

TreeManager* TreeManager::Instance() {
    G4AutoLock l(&instanceMutex);
    if (fInstance == NULL) fInstance = new TreeManager();
    return fInstance;
}

TreeManager::TreeManager() : fFile(0) {
    messenger = new TreeManagerMessenger(this);

    fName = "out";

    // Booking of histograms has to be protected.
    // In addition there are issues with ROOT that is
    // heavily non thread-safe. In particular I/O related operations
    // are not thread safe. To avoid problems let's mutex everything
    // here
    G4AutoLock l(&dataManipulationMutex);

    fInstance = this;
    fTree = new TTree("tree", "name");

    fTree->Branch("G4version", &G4version);
    fTree->Branch("Run", &Run, "Run/I");
    fTree->Branch("EventNumber", &EventNumber, "EventNumber/I");
    fTree->Branch("PrimaryName", &PrimaryName);
    fTree->Branch("PrimaryE", &PrimaryE, "PrimaryE/D");
    //  fTree->Branch("xsec",&xsec,"xsec/D");
    fTree->Branch("nSec", &nSec, "nSec/I");
    fTree->Branch("ParticleName", &ParticleName);
    fTree->Branch("ParticleType", &ParticleType);
    fTree->Branch("Ek", &Ek, "Ek/D");
    fTree->Branch("A", &A, "A/I");
    fTree->Branch("Z", &Z, "Z/I");
    fTree->Branch("ExcitationE", &ExcitationE, "ExcitationE/D");
    fTree->Branch("IsShortLived", &IsShortLived, "IsShortLived/O");
    fTree->Branch("IsStable", &IsStable, "IsStable/O");
    fTree->Branch("LifeTime", &LifeTime, "LifeTime/D");
    fTree->Branch("NDecayChannels", &NDecayChannels, "NDecayChannels/I");
    fTree->Branch("theta", &theta, "theta/D");
    fTree->Branch("phi", &phi, "phi/D");
    fTree->Branch("PhysicsList", &PhysicsList);
    this->Clear();
}

TreeManager::~TreeManager() {
    // No need to mutex, this is a real singleton.
    delete messenger;
    delete fTree;
    if (fFile) delete fFile;
}

Int_t TreeManager::Fill() {
    G4AutoLock l(&dataManipulationMutex);
    fTree->Fill();
}

Bool_t TreeManager::Write() {
    // TString filename = fName;
    // filename += ".root";
    // TFile* file = new TFile(filename,"RECREATE");
    G4AutoLock l(&dataManipulationMutex);
    if (!fFile)  // file not created at all: e.g. for a vis-only execution
    {
        G4cout << "TreeManager::Write() Warning: not writing" << G4endl;
        return false;
    }
    if (!fFile->IsOpen()) {
        G4Exception("TreeManager::Write()", "Hadr02", FatalException,
                    "Trying to write on a ROOT file which is not open");
        return false;
    }
    G4cout << "... write Root file : " << fName;
    fFile->cd();

    fTree->Write();
    fFile->Write();
    G4cout << " - done" << G4endl;

    G4cout << "... write Root file : " << fName;
    fFile->Close();
    G4cout << " - done" << G4endl;

    //  delete file;
    return true;
}

void TreeManager::Clear() {
    //  Run=0;
    ExcitationE = -1;
}

void TreeManager::SetOutputFilename(const std::string fname) {
    G4AutoLock l(&dataManipulationMutex);
    fName = fname;
    fName += ".root";
    G4cout << "... open Root tree file : " << fName;
    fFile = new TFile(fName, "RECREATE");
    G4cout << " - done" << G4endl;
}
