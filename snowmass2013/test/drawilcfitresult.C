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
void drawilcfitresult()
{

  float sqrtsVal = 250;
  int nbins = 40;
  bool withAcceptance = false;
  TString accName = "false";
  if (withAcceptance )  accName = "true";
  
  TString toyName = Form("pure_f_2_f_3_%.0fGeV_acc%s", sqrtsVal, accName.Data());
  
  draw2dsingle(toyName, "fa3", "fa2", "f_{3}", "f_{2}", 100, 0, 0.5, 100, 0, 0.5);

  /*
  double fa2val = 0.15;
  if ( sqrtsVal == 350. ) fa2val = 0.05;
  if ( sqrtsVal == 1000. ) fa2val = 0.15;
  double fa2err = 0.05; 
  if ( sqrtsVal == 350. ) fa2err = 0.015;
  if ( sqrtsVal == 1000. ) fa2err = 0.05;
  
  // drawsingle(toyName, "fa2", Form("f_{2}"), nbins, TMath::Max(fa2val-5.*fa2err,0.), fa2val+5.*fa2err, 0., 2*fa2err);

  double fa3val = 0.5;
  double fa3err = 0.05; 
  drawsingle(toyName, "fa3", Form("f_{3}"), nbins, TMath::Max(fa3val-5.*fa3err,0.), fa3val+5.*fa3err, 0., 2*fa3err);

  double phia3val = 0.;
  double phia3err = 0.15; 
  drawsingle(toyName, "phia3", Form("#phi_{3}"), nbins, phia3val-5.*phia3err, phia3val+5.*phia3err, 0., 2*0.1);

  double nsigval = 2000.;
  if (sqrtsVal == 500.)     nsigval = 1000.;
  double nsigerr = 100;
  if (sqrtsVal == 500.)     nsigerr = 20.;
  // drawsingle(toyName, "nsig", Form("nSignal"), nbins, nsigval-5.*nsigerr, nsigval+5.*nsigerr, 0., 200.);

  double nbkgval = 200.;
  if (sqrtsVal == 500.)     nbkgval = 100.;
  double nbkgerr = 100;
  if (sqrtsVal == 500.)     nbkgerr = 20.;
  // drawsingle(toyName, "nbkg", Form("nBackground"), nbins, TMath::Max(nbkgval-5.*nbkgerr, 0.), nbkgval+5.*nbkgerr, 0., 200.);
  */
  
}


void drawsingle(TString toyName, TString var, TString varName, int nBins, float xMin, float xMax, float errMin, float errMax)
{
  gROOT->ProcessLine(".L tdrstyle.C");
  gROOT->ProcessLine("setTDRStyle();");
  gROOT->ForceStyle();

  TH1F *h_var = new TH1F("h_var", "h_var", nBins, xMin, xMax);
  TH1F *h_var_err = new TH1F("h_var_err", "h_var_err", nBins, errMin, errMax);
  TH1F *h_var_pull = new TH1F("h_var_pull", "h_var_pull", 40, -4, 4);

  TString fileName = Form("toyresults_%s.root", toyName.Data());
  TFile *file = new TFile(fileName, "READ");
  gROOT->cd();
  std::cout << "Opening " << fileName << "\n";
  TTree *tree = (TTree*)file->Get("fittree");
  if ( ! tree ) return;

  // TString cut = "nbkg>1.";
  TString cut = "abs(fa3_pull)<4.";

  tree->Project("h_var", var, cut);
  tree->Project("h_var_err", Form("%s_err",var.Data()), cut);;
  tree->Project("h_var_pull", Form("%s_pull",var.Data()), cut);;

  if ( debug ) {
    std::cout << "h_var = " <<  h_var << "\n";
    std::cout << "h_var_pull = "<<  h_var << "\n";
  }
  h_var->SetXTitle(varName);
  h_var->SetYTitle("toy experiments");
  
  h_var_pull->SetXTitle(Form("%s(fit)-%s(in)/#sigma(%s)", varName.Data(), varName.Data(), varName.Data()));
  h_var_pull->SetYTitle(Form("toy experiments"));

  h_var_err->SetXTitle(Form("#sigma(%s)/%s", varName.Data(), varName.Data()));
  h_var_err->SetYTitle(Form("toy experiments"));
  

  TCanvas *c1 = new TCanvas("c1", "c1", 1200, 400);
  c1->Divide(2,1);
  c1->cd(1);
  h_var->Draw("hist");
  h_var->Fit("gaus");
  c1->cd(2);
  // h_var_err->Draw("hist");
  // c1->cd(3);
  h_var_pull->Draw("hist");
  h_var_pull->Fit("gaus");
  
  c1->SaveAs(Form("fitplots/%s_fitresults_%s.eps", var.Data(), toyName.Data()));
  c1->SaveAs(Form("fitplots/%s_fitresults_%s.png", var.Data(), toyName.Data()));

  delete h_var;
  delete h_var_err;
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
  
  TString fileName = Form("toyresults_%s.root", toyName.Data());
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

  h_var->Smooth(1);

  for (int j=0; j<5; j++) {
    std::cout << "level: " << level[j] << std::endl;
    std::cout << "area: " << area[j] << std::endl;
  }

  TH2F *h_clone= (h_var->Clone("clone"));
  h_clone->SetContour(1);
  h_clone->SetContourLevel(0,level[3]);

  h_var->SetContour(1);
  h_var->SetContourLevel(0,level[4]);


  TCanvas *c1 = new TCanvas();
  c1->cd();
  h_var->Draw("COL");
  h_var->SetLineColor(1);
  h_var->SetLineWidth(2);
  h_var->Draw("cont3same");
  h_clone->SetLineColor(1);
  h_clone->SetLineWidth(2);
  h_clone->SetLineStyle(2);
  h_clone->Draw("cont3same");

  Int_t n=35;
  Int_t *colors = new Int_t[n];
  for (Int_t i =0; i < n; i++) {
    colors[i] = i+63;
  }
  gStyle->SetPalette(n, colors); 

  c1->SaveAs(Form("fitplots/%s_vs_%s_fitresults_%s.png", yVar.Data(), xVar.Data(), toyName.Data()));

  delete h_var;
  file->Close();
  delete c1;
}
