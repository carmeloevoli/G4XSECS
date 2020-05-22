#include <TSystem.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH1D.h>
#include <TStyle.h>
#include <TString.h>
#include <TMarker.h>
#include <TColor.h>
#include <TLatex.h>
#include <TPaveText.h>

#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

int np = 5;
bool DEBUG = false;
double lgEmin = 2.0; //100 MeV
double lgEmax = 6.0; //100 GeV (in MeV)
double step = 0.05; // energy step in logE
int Nbins = (lgEmax-lgEmin)/step;

void analysis(char* filename) {

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(1);
  TH1D* hBR[np];
  TH1D* hXsec[np];
  TH1D* hBRnorm
    = new TH1D("hBRnorm", "total number of events vs energy", Nbins, lgEmin, lgEmax);
  hBRnorm->GetXaxis()->SetTitle("log_{10}(E/MeV)");
  hBRnorm->Sumw2();

  TH1D* hXsecNominal
    = new TH1D("hXsecNominal", "inelastic X-section vs energy ", Nbins, lgEmin, lgEmax);
  hXsecNominal->GetXaxis()->SetTitle("log_{10}(E/MeV)");
  hXsecNominal->GetYaxis()->SetTitle("#sigma_{inel}[mb]");
  hXsecNominal->Sumw2();

  
  vector<int> xSec(np, 0);
  vector<int> first(np, 0);
  string products[] = {"C11", "B11", "Be10", "Be9", "Be7"};
  int Znuc[] = {6,  5,  4,  4, 4};
  int Anuc[] = {11, 11, 10, 9, 7};
  int color[] = {kRed,  kAzure,  kSpring,  kOrange, kMagenta};
  
  TFile* fileIn= TFile::Open(filename);
  TTree* theTree=(TTree*)fileIn->Get("tree");

  int run, eventNumber, nSec, A, Z;
  double primaryE, inelXsec;
  TString* particleName = new TString();
  
  theTree->SetBranchAddress("Run",&run);
  theTree->SetBranchAddress("EventNumber",&eventNumber);
  theTree->SetBranchAddress("PrimaryE",&primaryE);
  theTree->SetBranchAddress("inelXsec",&inelXsec);
  theTree->SetBranchAddress("nSec",&nSec);
  theTree->SetBranchAddress("ParticleName",&particleName);
  theTree->SetBranchAddress("A",&A);
  theTree->SetBranchAddress("Z",&Z);

  int nentries= theTree->GetEntries();
  if(DEBUG) cout << " Nbins -> " << Nbins << " " << lgEmin << " " << lgEmax << endl;
  
  int nev = 0;
  int nev2 = 0;
  
  int sectors = 0;
  
  for (Int_t i=0; i < nentries; i++) { 
    theTree->GetEntry(i);

    if(sectors == 0) {
      sectors = nSec;
      hBRnorm->Fill(log10(primaryE));
      hXsecNominal->Fill(log10(primaryE),inelXsec);
      nev++;
    }
    
    for (int j=0; j<np; j++) {
      if(Z == Znuc[j] && A == Anuc[j]) {
	if(!first[j]) {
	  if(DEBUG) cout << Z << " " << A << " " <<  *particleName << endl;
	  char name[10];
	  sprintf(name, "hBR[%d]",j);
	  char title[25];
	  sprintf(title, "branching ratio for %s",products[j].c_str());
	  if(DEBUG) cout << title << " "  << name << endl;
	  hBR[j] = new TH1D(name, title, Nbins, lgEmin, lgEmax);
	  hBR[j]->GetXaxis()->SetTitle("log_{10}(E/MeV)");
	  sprintf(title, "BR[%s]",products[j].c_str());
	  hBR[j]->GetYaxis()->SetTitle(title);
	  hBR[j]->Sumw2();

	  sprintf(name, "hXsec[%d]",j);
	  sprintf(title, "x-section for %s",products[j].c_str());
	  hXsec[j] = new TH1D(name, title, Nbins, lgEmin, lgEmax);
	  hXsec[j]->GetXaxis()->SetTitle("log_{10}(E/MeV)");
	  sprintf(title, "#sigma_{inel}[%s]",products[j].c_str());
	  hXsec[j]->GetYaxis()->SetTitle(title);
	  hXsec[j]->Sumw2();

	  
	  first[j] = 1;
	}
	hBR[j]->Fill(log10(primaryE));
	hXsec[j]->Fill(log10(primaryE),inelXsec);
	xSec[j]++;
      }
    }
    if(eventNumber > nev2) nev2 = eventNumber;
    --sectors;
  }

  Double_t w = 800;
  Double_t h = 800;
  //auto c = new TCanvas("c", "c", w, h);
  TCanvas* cBR = new TCanvas("cBR","cBR",w,h);
  cBR->cd();
  cBR->Divide(2,3);
  TCanvas* cXsec = new TCanvas("cXsec","cXsec",w,h);
  cXsec->cd();
  cXsec->Divide(2,3);
  
  cout << "Simulated events:" << nev << " (" << nev2+1 << ")" <<  endl;
  for (int i=0; i<np; i++) {
    cBR->cd(i+1);
    if(DEBUG) cout << (double)xSec[i] << " / " << (double)nev << endl;
    hBR[i]->SetMarkerStyle(8);
    hBR[i]->SetMarkerSize(.5);
    hBR[i]->SetMarkerColor(color[i]);
    hBR[i]->SetLineColor(color[i]);
    hBR[i]->Divide(hBRnorm);
    //hBR[i]->GetXaxis()->SetTitleOffset(0.1);
    //hBR[i]->GetYaxis()->SetTitleOffset(0.1);
    hBR[i]->Draw();

    
    cXsec->cd(i+1); 
    hXsec[i]->SetMarkerStyle(4);
    hXsec[i]->SetMarkerSize(.5);
    hXsec[i]->SetMarkerColor(color[i]);
    hXsec[i]->SetLineColor(color[i]);
    hXsec[i]->Divide(hBRnorm);
    //hXsec[i]->GetXaxis()->SetTitleOffset(0.1);
    //hXsec[i]->GetYaxis()->SetTitleOffset(0.1);
    hXsec[i]->Draw();
    
    if(DEBUG) for (int k=1; k<=Nbins; k++) cout << k << " " << hBR[i]->GetBinContent(k) << " " << hBR[i]->GetBinCenter(k) << endl;
  }
  
  if(DEBUG) {
    for (int k=1; k<=Nbins; k++) cout << k << " " << hBRnorm->GetBinContent(k) << " " << hBRnorm->GetBinCenter(k) << endl;
  }
  
  cBR->cd(6);
  hBRnorm->Draw("H");
  cBR->Update();

  hXsecNominal->Divide(hBRnorm);
  cXsec->cd(6);
  hXsecNominal->Draw("H");
  cXsec->Update();  
  
 
}



//C12 -> C11, B10, B11, Be10, Be9, Be7 
//O16 -> C11, B10, B11, Be10, Be9, Be7 
//N14 -> C11, B10, B11, Be10, Be9, Be7 
