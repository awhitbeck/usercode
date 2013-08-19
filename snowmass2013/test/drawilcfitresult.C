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

bool debug = false;


typedef enum {kILC, kLHC} colliderType;
typedef enum{kF3, kFA3} varLevel; 

void drawilcfitresult()
{

  int nBins = 40;
  colliderType collider = kILC; 
  bool withAcceptance = false;
  TString accName = "false";
  if (withAcceptance )  accName = "true";

  // set the f3 value 
  double f2gen = 0.;
  double f3gen = 0.;
  
  if ( collider == kILC ) {
    f2gen = 0.;
    f3gen = 0.1;
  }
  
  vector<float> energies;
  energies.push_back(250.);
  energies.push_back(350.);
  energies.push_back(500.);
  energies.push_back(1000.);
  
  //
  // 1D Plots
  // 
  
  for ( int i = 0; i < energies.size() ; i ++ ) {
    float sqrtsVal = energies.at(i);
    TString toyName = Form("embd_f_3_%.0fGeV_acc%s", sqrtsVal, accName.Data());
    drawsingle(sqrtsVal, f2gen, f3gen, toyName, kF3, collider, Form("f_{3}"), nBins);
    drawsingle(sqrtsVal, f2gen, f3gen, toyName, kFA3, collider, Form("f_{a3}"), nBins);
  }
  
  // 2D plots
  draw2dsingle("pure_f_2_p15_f_3_250GeV_accfalse", "fa2", "fa3", "f_{2}", "f_{3}", 100, 0, 0.5, 100, 0, 0.5);
  draw2dsingle("pure_f_3_250GeV_accfalse", "fa3", "phia3",  "f_{3}", "#phi_{3}", 100, 0, 0.5, 100, -3.14, 3.14);
    
  
}


void drawsingle(float sqrtsVal, float f2gen, float f3gen, TString toyName, int variable, colliderType collider, TString varName, int nBins)
{

  gROOT->ProcessLine(".L tdrstyle.C");
  gROOT->ProcessLine("setTDRStyle();");
  gROOT->ForceStyle();
  
  float fa2gen(0.);
  float fa3gen(0.);
  calcgcoup(f2gen, f3gen, sqrtsVal, collider, fa2gen, fa3gen);

  float xgen = 0.;
  float xerr = 0.;
  TString var; 

  if ( variable == kF3 ) {
    var = "f3";
    xgen = f3gen;    
  }
  
  if ( variable == kFA3 ) {
    var = "fa3";
    xgen = fa3gen;
  }
  
  // assume the expected err is 1/3 of the generated values
  xerr = xgen / 3.;
  float xMin = TMath::Max(xgen-4.*xerr,0.);
  float xMax = TMath::Min(xgen+4.*xerr,1.);
  

  TH1F *h_var = new TH1F("h_var", "h_var", nBins, xMin, xMax);
  TH1F *h_var_pull = new TH1F("h_var_pull", "h_var_pull", 40, -4, 4);

  TString fileName = Form("toyresults_ilc/toyresults_%s.root", toyName.Data());
  TFile *file = new TFile(fileName, "READ");
  gROOT->cd();
  std::cout << "Opening " << fileName << "\n";
  TTree *tree = (TTree*)file->Get("fittree");
  if ( ! tree ) return;

  // do an event loop
  float f2_ = 0.;
  float f2_err_ = 0.;
  float f2_pull_ = 0.;
  float f3_ = 0.;
  float f3_err_ = 0.;
  float f3_pull_ = 0.;
  float phi3_ = 0.;
  float phi3_err_ = 0.;
  float phi3_pull_ = 0.;

  tree->SetBranchAddress("fa2"        , &f2_        );
  tree->SetBranchAddress("fa2_err"    , &f2_err_    );
  tree->SetBranchAddress("fa2_pull"   , &f2_pull_   );
  tree->SetBranchAddress("fa3"        , &f3_        );
  tree->SetBranchAddress("fa3_err"    , &f3_err_    );
  tree->SetBranchAddress("fa3_pull"   , &f3_pull_   );
  tree->SetBranchAddress("phia3"      , &phi3_      );
  tree->SetBranchAddress("phia3_err"  , &phi3_err_  );
  tree->SetBranchAddress("phia3_pull" , &phi3_pull_ );
  
  for(int ievt = 0; ievt < tree->GetEntries() ;ievt++){
    tree->GetEntry(ievt); 
    
    if ( variable == kF3 ) {
      h_var->Fill(f3_);
      h_var_pull->Fill(f3_pull_);
    }

    if ( variable == kFA3 ) {
      float fa2_ = 0.;
      float fa3_ = 0.;
      calcgcoup(f2_, f3_, sqrtsVal, kILC, fa2_, fa3_);
      h_var->Fill(fa3_);
      h_var_pull->Fill(f3_pull_);
    }
  }

  h_var->SetXTitle(varName);
  h_var->GetXaxis()->CenterTitle(true);
  h_var->GetYaxis()->CenterTitle(true);

  h_var_pull->SetXTitle(Form("%s(fit)-%s(in)/#sigma(%s)", varName.Data(), varName.Data(), varName.Data()));
  h_var_pull->GetXaxis()->CenterTitle(true);
  h_var_pull->GetYaxis()->CenterTitle(true);
  

  TCanvas *c1 = new TCanvas("c1", "c1", 600, 600);
  c1->cd();
  c1->SetRightMargin(0.11);
  h_var->Draw("hist");
  // h_var->Fit("gaus");
  c1->SaveAs(Form("toyresults_ilc/plots/%s_fitresults_%s.eps", var.Data(), toyName.Data()));
  c1->SaveAs(Form("toyresults_ilc/plots//%s_fitresults_%s.png", var.Data(), toyName.Data()));

  c1->Clear();
  h_var_pull->Draw("hist");
  //h_var_pull->Fit("gaus");
  
  c1->SaveAs(Form("toyresults_ilc/plots/%s_pull_fitresults_%s.eps", var.Data(), toyName.Data()));
  c1->SaveAs(Form("toyresults_ilc/plots/%s_pull_fitresults_%s.png", var.Data(), toyName.Data()));

  delete h_var;
  delete h_var_pull;
  delete c1;
  file->Close();
  
}


void draw2dsingle(TString toyName, TString xVar, TString yVar, TString xTitle, TString yTitle, 
		  int nBinsX, double xMin, double xMax, int nBinsY, double yMin, double yMax) 
{

  gROOT->ProcessLine(".L tdrstyle.C");
  gROOT->ProcessLine("setTDRStyle();");
  gROOT->ForceStyle();

  TString fileName = Form("toyresults_ilc/toyresults_%s.root", toyName.Data());
  TFile *file = new TFile(fileName, "READ");
  gROOT->cd();
  std::cout << "Opening " << fileName << "\n";
  TTree *tree = (TTree*)file->Get("fittree");
  if ( ! tree ) return;
  
  TString cut = "1";
   
  TH2F *h_var = new TH2F("h_var", "h_var", nBinsX, xMin, xMax, nBinsY, yMin, yMax); 
  tree->Project("h_var", Form("%s:%s", yVar.Data(), xVar.Data()), cut);
  h_var->SetXTitle(xTitle);
  h_var->SetYTitle(yTitle);
  h_var->GetXaxis()->CenterTitle(true);
  h_var->GetYaxis()->CenterTitle(true);
  
  TH2F *h_var_clone= (h_var->Clone("clone"));
  h_var_clone->SetXTitle(xTitle);
  h_var_clone->SetYTitle(yTitle);

  //code for 2D fancy plot
  int nx = h_var->GetNbinsX();
  int ny = h_var->GetNbinsY();

  Double_t Sum = h_var->Integral();
  h_var->Scale(1./Sum);
 
  vector<Double_t> conf; 
  conf.push_back(0.9999);
  conf.push_back(0.999);
  conf.push_back(0.997);
  conf.push_back(0.955);
  conf.push_back(0.683);

  Double_t level[5];
  Double_t area[5];
  for (int j=0; j<5; j++) {
    level[j] = 0.;
    area[j] = 0.;
  }

  vector< Double_t > OrderedGArray;
  for (int ix=1; ix<=nx; ix++) {
    for (int iy=1; iy<=ny; iy++) {
      OrderedGArray.push_back(h_var->GetCellContent(ix,iy));
    }
  }
  
  std::cout<<"before sort"<<std::endl;
  std::sort( OrderedGArray.begin(), OrderedGArray.end());
  std::reverse(OrderedGArray.begin(),OrderedGArray.end());
  std::cout<<"after sort"<<std::endl;

  vector< Double_t> SumArray;
  SumArray.push_back(OrderedGArray[0]);
  cout<<"ordered g array"<<endl;
  for (int i=1; i<= (int) OrderedGArray.size(); i++) {
    if((fabs(OrderedGArray[i])>0) && ((i%100)==1))
      cout<<OrderedGArray[i]<<endl;;
    SumArray.push_back(SumArray[i-1] + OrderedGArray[i]);
    for (int j=0; j<5; j++) {
      if (SumArray[i] <= conf[j]) {
	level[j] = OrderedGArray[i];
	area[j] = SumArray[i];
      }
    }
  }

  h_var->Smooth(2);

  for (int j=0; j<5; j++) {
    std::cout << "level: " << level[j] << std::endl;
    std::cout << "area: " << area[j] << std::endl;
  }

  TH2F *h_clone= (h_var->Clone("clone"));

  h_clone->SetContour(1);
  h_clone->SetContourLevel(0,level[3]);

  h_var->SetContour(1);
  h_var->SetContourLevel(0,level[4]);

  Int_t n=35;
  Int_t *colors = new Int_t[n];
  for (Int_t i =0; i < n; i++) {
    colors[i] = i+63;
  }
  gStyle->SetPalette(n, colors); 


  TCanvas *c1 = new TCanvas("c1", "c1", 600, 600);
  c1->cd();
  c1->SetLeftMargin(0.17);
  c1->SetRightMargin(0.14);
  h_var_clone->Draw("COLZ");
  h_var->SetLineColor(1);
  h_var->SetLineWidth(2);
  h_var->Draw("cont3same");
  h_clone->SetLineColor(1);
  h_clone->SetLineWidth(2);
  h_clone->SetLineStyle(2);
  h_clone->Draw("cont3same");
  
  c1->SaveAs(Form("toyresults_ilc/plots/%s_vs_%s_fitresults_%s.eps", yVar.Data(), xVar.Data(), toyName.Data()));
  c1->SaveAs(Form("toyresults_ilc/plots/%s_vs_%s_fitresults_%s.png", yVar.Data(), xVar.Data(), toyName.Data()));

  delete h_var;
  file->Close();
  delete c1;
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
