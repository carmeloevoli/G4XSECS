#ifndef HISTO_H
#define HISTO_H

#include <TChain.h>
#include <TFile.h>
#include <TROOT.h>

// Header file for the classes stored in the TTree if any.
#include "TString.h"

class HISTO {
   public:
    TTree *fChain;   //! pointer to the analyzed TTree or TChain
    Int_t fCurrent;  //! current Tree number in a TChain

    // Fixed size dimensions of array or collections stored in the TTree if any.

    // Declaration of leaf types
    TString *G4version;
    Int_t Run;
    Int_t EventNumber;
    TString *PrimaryName;
    Double_t PrimaryE;
    Int_t nSec;
    TString *ParticleName;
    TString *ParticleType;
    Double_t Ek;
    Int_t A;
    Int_t Z;
    Double_t ExcitationE;
    Bool_t IsShortLived;
    Bool_t IsStable;
    Double_t LifeTime;
    Int_t NDecayChannels;
    Double_t theta;
    Double_t phi;
    TString *PhysicsList;

    // List of branches
    TBranch *b_G4version;
    TBranch *b_Run;
    TBranch *b_EventNumber;
    TBranch *b_PrimaryName;
    TBranch *b_PrimaryE;
    TBranch *b_nSec;
    TBranch *b_ParticleName;
    TBranch *b_ParticleType;
    TBranch *b_Ek;
    TBranch *b_A;
    TBranch *b_Z;
    TBranch *b_ExcitationE;
    TBranch *b_IsShortLived;
    TBranch *b_IsStable;
    TBranch *b_LifeTime;
    TBranch *b_NDecayChannels;
    TBranch *b_theta;
    TBranch *b_phi;
    TBranch *b_PhysicsList;

    HISTO(const std::string fname);
    HISTO(TTree *tree = 0);
    virtual ~HISTO();
    virtual Int_t Cut(Long64_t entry);
    virtual Int_t GetEntry(Long64_t entry);
    virtual Long64_t LoadTree(Long64_t entry);
    virtual void Init(TTree *tree);
    virtual void Loop(const Bool_t drawExp = false);
    virtual Bool_t Notify();
    virtual void Show(Long64_t entry = -1);
};

#endif
