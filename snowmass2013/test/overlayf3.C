// 
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include <iostream>
#include "TH2F.h"
#include "TH1F.h"
#include "TString.h"
#include "TRint.h"
#include "TChain.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLine.h"
#include "TLegend.h"
#include "TCut.h"
#include "THStack.h"
#include "TNtuple.h"
#include "TMath.h"

#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>


typedef enum {kILC, kLHC} colliderType;

void overlayf3() {
  
  gROOT->ProcessLine(".L tdrstyle.C");
  gROOT->ProcessLine("setTDRStyle();");
  gROOT->ForceStyle();

  vector<TString> fileNames;
  vector<int> linestyles;
  vector<Color_t> colors; 

  fileNames.push_back("toyresults_ilc/toyresults_embd_f_3_250GeV_accfalse_fixphia3.root");
  linestyles.push_back(2);
  colors.push_back(kMagenta);
  

  fileNames.push_back("toyresults_ilc/toyresults_KD_embd_f_3_250GeV_accfalse.root");
  linestyles.push_back(1);
  colors.push_back(kBlack);


  fileNames.push_back("toyresults_ilc/toyresults_embd_f_3_250GeV_accfalse.root");
  linestyles.push_back(3);
  colors.push_back(kBlue);


  const int nHist = fileNames.size();
  TH1F *hists[nHist];
  
  int nBins = 40;
  float xMin = 0.0;
  float xMax = 0.1+0.1*6./3;
  
  
  for ( int i = 0; i < fileNames.size() ; i ++ ) {
    hists[i] = new TH1F(Form("hist_%i",i), Form("hist_%i",i), nBins, xMin, xMax);
    TFile *file = new TFile(fileNames.at(i), "READ");
    gROOT->cd();
    std::cout << "Opening " << fileNames.at(i) << "\n";
    TTree *tree = (TTree*)file->Get("fittree");
    if ( ! tree ) return;
    
    float f3_ = 0.;
    tree->SetBranchAddress("fa3"        , &f3_        );
    
    for(int ievt = 0; ievt < tree->GetEntries() ;ievt++){
      tree->GetEntry(ievt); 
      float fa2_(0.), fa3_(0.);
      calcgcoup(0, f3_, 250,  kILC, fa2_, fa3_);
      hists[i]->Fill(f3_);
    }
    hists[i]->SetLineStyle(linestyles.at(i));
    hists[i]->SetLineColor(colors.at(i));
    hists[i]->Scale(1./hists[i]->Integral());
    hists[i]->SetXTitle("f_{3}");
    hists[i]->GetXaxis()->CenterTitle(true);
    hists[i]->SetLineWidth(3);
  }

  TCanvas *c1 = new TCanvas("c1", "c1", 600, 600);
  c1->SetRightMargin(0.04);
  hists[0]->Draw("hist");
  for ( int i = 1; i < fileNames.size() ; i++ ) 
    hists[i]->Draw("samehist");
  c1->SaveAs("toyresults_ilc/plots/f3_250GeV_overlay.eps");
  c1->SaveAs("toyresults_ilc/plots/f3_250GeV_overlay.png");

}


void calcgcoup(double f2, double f3, double sqrts = 250., colliderType machine = kLHC, float &fa2, float &fa3){

  Double_t sigma1;
  Double_t sigma2;
  Double_t sigma4;
  

  // ILC numbers at 250 GeV at mH= 125 GeV (narrow Z width approximation)
  Double_t sigma1_e = 0.981396; // was 0.94696 at 126 GeV
  Double_t sigma2_e = 33.4674;  // was 32.1981 at 126 GeV
  Double_t sigma4_e = 7.9229;   // was 7.45502 at 126 GeV
  
  // ILC nubmers at 350 GeV at mH = 125 GeV
  if ( sqrts == 350. ) {
    sigma1_e = 1.48872; 
    sigma2_e = 125.387;  
    sigma4_e = 75.3199;
  }
  
  // ILC nubmers at 500 GeV at mH = 125 GeV
  if ( sqrts == 500. ) {
    sigma1_e = 2.57246; 
    sigma2_e = 516.556;  
    sigma4_e = 414.378;
  }
  
  // ILC nubmers at 1000 GeV at mH = 125 GeV
  if ( sqrts == 1000. ) {
    sigma1_e = 8.95721; 
    sigma2_e = 8208.91;  
    sigma4_e = 7800.2;
  }

  if (machine == kILC) {
    sigma1 = sigma1_e;
    sigma2 = sigma2_e;
    sigma4 = sigma4_e;
  }


  // LHC numbers at mH= 125 GeV (JHUGen)
  Double_t sigma1_p = 1.860351; // was 2.03971 at 126 GeV
  Double_t sigma2_p = 0.672859; // was 0.77517 at 126 GeV
  Double_t sigma4_p = 0.284353; // was 0.32689 at 126 GeV

  if (machine == kLHC) {
    sigma1 = sigma1_p;
    sigma2 = sigma2_p;
    sigma4 = sigma4_p;
  }

  
  // get g2 and g4 

  double sigma_total = sigma1 / ( 1. - f2 - f3 ) ;
  double g1 = 1.;
  double g2 = sqrt(f2 * sigma_total  / sigma2);
  double g4 = sqrt(f3 * sigma_total  / sigma4);

  // std::cout << "--- f2 = " << f2 << ", f3 = " << f3 << "\n"; 
  // std::cout << "--- g1 = 1, g2 = " << g2 << ", g4 = " << g4 << "\n";

  // calculate fa2 and fa3
  
  Double_t fg2_p = sigma2_p*g2*g2 / (  sigma1_p*g1*g1 + sigma2_p*g2*g2 + sigma4_p*g4*g4 );
  Double_t fg4_p = sigma4_p*g4*g4 / (  sigma1_p*g1*g1 + sigma2_p*g2*g2 + sigma4_p*g4*g4 );
  
  //   std::cout << " --- LHC (mH=125 GeV):   fa2 = " << fg2_p << "   fa3 = "<< fg4_p << endl;   
  
  fa2 = fg2_p;
  fa3 = fg4_p;
  
}
