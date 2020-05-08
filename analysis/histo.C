#include "histo.h"
#include "utils.h"

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

HISTO::HISTO(TTree *tree) : fChain(0)
{
    // if parameter tree is not specified (or zero), connect the file
    // used to generate this class and read the Tree.
    if (tree == 0) {
        TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("incl2-melo.root");
        if (!f || !f->IsOpen()) {
            f = new TFile("incl2-melo.root");
        }
        f->GetObject("tree",tree);
        
    }
    Init(tree);
}

HISTO::HISTO(const std::string fname) : fChain(0)
{
    TTree *tree =0;
    TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fname.c_str());
    if (!f || !f->IsOpen()) {
        f = new TFile(fname.c_str());
    }
    f->GetObject("tree",tree);
    Init(tree);
}

HISTO::~HISTO()
{
    if (!fChain) return;
    delete fChain->GetCurrentFile();
}

Int_t HISTO::GetEntry(Long64_t entry)
{
    // Read contents of entry.
    if (!fChain) return 0;
    return fChain->GetEntry(entry);
}
Long64_t HISTO::LoadTree(Long64_t entry)
{
    // Set the environment to read one entry
    if (!fChain) return -5;
    Long64_t centry = fChain->LoadTree(entry);
    if (centry < 0) return centry;
    if (fChain->GetTreeNumber() != fCurrent) {
        fCurrent = fChain->GetTreeNumber();
        Notify();
    }
    return centry;
}

void HISTO::Init(TTree *tree)
{
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the branch addresses and branch
    // pointers of the tree will be set.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).
    
    // Set object pointer
    G4version = 0;
    PrimaryName = 0;
    ParticleName = 0;
    ParticleType = 0;
    PhysicsList = 0;
    // Set branch addresses and branch pointers
    if (!tree) return;
    fChain = tree;
    fCurrent = -1;
    fChain->SetMakeClass(1);
    
    fChain->SetBranchAddress("G4version", &G4version, &b_G4version);
    fChain->SetBranchAddress("Run", &Run, &b_Run);
    fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
    fChain->SetBranchAddress("PrimaryName", &PrimaryName, &b_PrimaryName);
    fChain->SetBranchAddress("PrimaryE", &PrimaryE, &b_PrimaryE);
    fChain->SetBranchAddress("nSec", &nSec, &b_nSec);
    fChain->SetBranchAddress("ParticleName", &ParticleName, &b_ParticleName);
    fChain->SetBranchAddress("ParticleType", &ParticleType, &b_ParticleType);
    fChain->SetBranchAddress("Ek", &Ek, &b_Ek);
    fChain->SetBranchAddress("A", &A, &b_A);
    fChain->SetBranchAddress("Z", &Z, &b_Z);
    fChain->SetBranchAddress("ExcitationE", &ExcitationE, &b_ExcitationE);
    fChain->SetBranchAddress("IsShortLived", &IsShortLived, &b_IsShortLived);
    fChain->SetBranchAddress("IsStable", &IsStable, &b_IsStable);
    fChain->SetBranchAddress("LifeTime", &LifeTime, &b_LifeTime);
    fChain->SetBranchAddress("NDecayChannels", &NDecayChannels, &b_NDecayChannels);
    fChain->SetBranchAddress("theta", &theta, &b_theta);
    fChain->SetBranchAddress("phi", &phi, &b_phi);
    fChain->SetBranchAddress("PhysicsList", &PhysicsList, &b_PhysicsList);
    Notify();
}

Bool_t HISTO::Notify()
{
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.
    
    return kTRUE;
}

void HISTO::Show(Long64_t entry)
{
    // Print contents of entry.
    // If entry is not specified, print current entry
    if (!fChain) return;
    fChain->Show(entry);
}

Int_t HISTO::Cut(Long64_t entry)
{
    // This function may be called from Loop.
    // returns  1 if entry is accepted.
    // returns -1 otherwise.
    return 1;
}

void HISTO::h12ascii (TH1* h)
{
    Int_t n = h->GetNbinsX();
    Double_t count = 0;
    for (Int_t i = 1; i <= n; ++i)
    {
        cout << scientific << setprecision(3) << h->GetBinLowEdge(i) << " " << h->GetBinLowEdge(i) + h->GetBinWidth(i) << " ";
        cout << fixed << setprecision(0) << h->GetBinContent(i) << "\n";
        count += h->GetBinContent(i);
    }
    cout << "histo of " << count << "\n";
}

void HISTO::print_normalized(TH1* h, TH1* hp)
{
    Int_t n = h->GetNbinsX();
    cout << scientific << setprecision(5);
    for (Int_t i = 1; i <= n; ++i) {
        double T = h->GetBinLowEdge(i) + 0.5 * h->GetBinWidth(i);
        cout << scientific << setprecision(3) << h->GetBinLowEdge(i) << " ";
        cout << h->GetBinLowEdge(i) + h->GetBinWidth(i) << " ";
        cout << h->GetBinContent(i) / hp->GetBinContent(i) << " ";
        cout << "\n";
    }
}

void HISTO::Loop(const Bool_t drawExp)
{
    //   In a ROOT session, you can do:
    //      root> .L histo.C
    //      root> histo t
    //      root> t.GetEntry(12); // Fill t data members with entry number 12
    //      root> t.Show();       // Show values of entry 12
    //      root> t.Show(16);     // Read and show values of entry 16
    //      root> t.Loop();       // Loop on all entries
    //
    //   This is the loop skeleton where:
    //    jentry is the global entry number in the chain
    //    ientry is the entry number in the current Tree
    //   Note that the argument to GetEntry must be:
    //    jentry for TChain::GetEntry
    //    ientry for TTree::GetEntry and TBranch::GetEntry
    //
    //   To read only selected branches, Insert statements like:
    //   METHOD1:
    //    fChain->SetBranchStatus("*",0);  // disable all branches
    //    fChain->SetBranchStatus("branchname",1);  // activate branchname
    //   METHOD2: replace line
    //    fChain->GetEntry(jentry);       //read all branches
    //    b_branchname->GetEntry(ientry); //read only this branch
    
    if (fChain == 0) return;
    
    //   double crossSection_per_atom =    254.87; //    219.28;// mbarn
    const Int_t nPrim = fChain->GetMaximum("EventNumber") + 1;
    const Double_t EMinPrimary = fChain->GetMinimum("PrimaryE");
    const Double_t EMaxPrimary = fChain->GetMaximum("PrimaryE");
    const Int_t nbin = 4 * 10;
    const Double_t min = 1e1;
    const Double_t max = 1e5;
    
    TH1F *h = new TH1F("histo_frag", "histo_frag", nbin, (min), (max));
    TH1F *hP = new TH1F("histo_proton", "histo_proton", nbin, (min), (max));
    BinLogX(h);
    BinLogX(hP);
    
    Long64_t nentries = fChain->GetEntriesFast();
    
    cout << nentries << " " << nPrim << " " << fChain->GetMinimum("EventNumber") << " " << EMinPrimary << " " << EMaxPrimary << "\n";
    
    Int_t prevEventNumber = -1;
    
    Long64_t nbytes = 0, nb = 0;
    Int_t counter = 0;
    Double_t Emean = 0;
    for (Long64_t jentry = 0; jentry < nentries; ++jentry) {
        if (LoadTree(jentry) < 0) {
            break;
        }
        nb = fChain->GetEntry(jentry);
        //nbytes += nb;
        const Double_t FillingE = PrimaryE;
        
        if(EventNumber != prevEventNumber)
        {
            prevEventNumber = EventNumber;
            hP->Fill(FillingE);
            //counter = counter + 1;
        }
        if(Z == 0 && A == 1)
        {
            counter += 1;
            Emean += Ek;
            h->Fill(FillingE);
            //cout << Z << " " << A << " " << Ek << " " << PrimaryE << " " << EventNumber << " " << Run << "\n";
        }
    }//end loop on entries
    
    cout << counter << " " << Emean / (1.0 * counter) << "\n";
    
    //h12ascii(h);
    //h12ascii(hP);
    
    //print_normalized(h, hP);
}
