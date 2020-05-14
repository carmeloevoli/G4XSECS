#include <TSystem.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
// #include <TArrow.h>
// #include <TGaxis.h>
#include <TString.h>
// #include <TGraph.h>
// #include <TGraphAsymmErrors.h>
// #include <TGraphErrors.h>
// #include <TRandom.h>
// #include <TMarker.h>
// #include <TColor.h>
// #include <TLatex.h>
// #include <TLegend.h>

#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

int np = 3;
bool DEBUG=true;

void analysis(char* filename) {

  TFile* fileIn= TFile::Open(filename);
  TTree* theTree=(TTree*)fileIn->Get("tree");

  int run, eventNumber, nSec, A, Z;
  double primaryE;
  TString* particleName = new TString();
  
  theTree->SetBranchAddress("Run",&run);
  theTree->SetBranchAddress("EventNumber",&eventNumber);
  theTree->SetBranchAddress("PrimaryE",&primaryE);
  theTree->SetBranchAddress("nSec",&nSec);
  theTree->SetBranchAddress("ParticleName",&particleName);
  theTree->SetBranchAddress("A",&A);
  theTree->SetBranchAddress("Z",&Z);

  int nentries= theTree->GetEntries();
 
  vector<int> xSec(np, 0);

  int nev = 0;
  for (Int_t i=0; i < nentries; i++) { 
    theTree->GetEntry(i);
    if (i==nentries-1) nev = eventNumber+1;

    
    if(A==10) {
      xSec[0]++;
      if(DEBUG) cout << A << " " <<  *particleName << endl;
    }
    if(A==11) {
      xSec[1]++;
      if(DEBUG) cout << A << " " <<  *particleName << endl;
    }
    if(A==12) {
      xSec[2]++;
      if(DEBUG) cout << A << " " <<  *particleName << endl;
    }
  }

  cout << "Simulated events:" << nev << endl;
  cout << (double)xSec[0]/(double)nev << " " << (double)xSec[1]/(double)nev << " " << (double)xSec[2]/(double)nev << endl;

  delete particleName;
  fileIn->Close();  
}
