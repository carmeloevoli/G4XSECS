#ifndef TreeManager_h
#define TreeManager_h

#include <iostream>
#include <string>
#include <vector>

#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TreeManagerMessenger-XS4GCR.hh"

class TreeManager {
   public:
    static TreeManager* Instance();
    virtual ~TreeManager();

    inline TTree* GetTree() const { return fTree; };
    inline TString GetName() const { return fName; };
    Int_t Fill();
    Bool_t Write();
    void Clear();
    void SetOutputFilename(const std::string);

    // Declaration of data types for the DataTree
    TString G4version;
    Int_t Run;
    Int_t EventNumber;
    TString PrimaryName;
    Double_t PrimaryE;
    Double_t inelXsec;
    Int_t nSec;
    TString ParticleName;
    TString ParticleType;
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
    TString PhysicsList;

   private:
    TreeManager();
    static TreeManager* fInstance;

    TTree* fTree;
    TString fName;
    TFile* fFile;
    TreeManagerMessenger* messenger;
};

#endif
