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

bool debug = false;
void drawilcfitresult()
{
  float sqrts = 500;
  
  TString toyName = "embd_model6_fa3_p1_2000signal_200bkg_accfalse";
  double fa3val = 0.1;
  double fa3err = 0.03; 
  drawsingle(toyName, "fa3", Form("f_{3}"), 40, fa3val-5.*fa3err, fa3val+5.*fa3err, 0., 2*fa3err);

  double phia3val = 0.;
  double phia3err = 0.2; 
  drawsingle(toyName, "phia3", Form("#phi_{3}"), 40, phia3val-5.*phia3err, phia3val+5.*phia3err, 0., 2*0.1);

  double nsigval = 2000.;
  double nsigerr = 100;
  drawsingle(toyName, "nsig", Form("nSignal"), 40, nsigval-5.*nsigerr, nsigval+5.*nsigerr, 0., 200.);

  double nbkgval = 200.;
  double nbkgerr = 100;
  drawsingle(toyName, "nbkg", Form("nBackground"), 40, TMath::Max(nbkgval-5.*nbkgerr, 0.), nbkgval+5.*nbkgerr, 0., 200.);
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

  tree->Project("h_var", var);
  tree->Project("h_var_err", Form("%s_err",var.Data()));;
  tree->Project("h_var_pull", Form("%s_pull",var.Data()));;

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
  c1->cd(2);
  // h_var_err->Draw("hist");
  // c1->cd(3);
  h_var_pull->Draw("hist");
  //h_var_pull->Fit("gaus");
  
  c1->SaveAs(Form("fitplots/%s_fitresults_%s.eps", var.Data(), toyName.Data()));
  c1->SaveAs(Form("fitplots/%s_fitresults_%s.png", var.Data(), toyName.Data()));

  delete h_var;
  delete h_var_err;
  delete h_var_pull;
  delete c1;
  file->Close();
  
}
