#include <vector>
#include "RooRealVar.h"
#include "../src/ScalarPdfFactoryZH.cc"
#include "RooPlot.h"
#include "TChain.h"
#include "RooDataSet.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TH1F.h"

// 
// Run by  root -l -b -q loadLib.C  plotILCproj.C+
// 
using namespace RooFit;
using namespace std;

enum variables{kcostheta1,kcostheta2,kphi};

void plotILCproj(variables var=kcostheta2){

  RooRealVar* costheta1 = new RooRealVar("costheta1","cos#theta_{1}",-1.,1.);
  RooRealVar* costheta2 = new RooRealVar("costheta2","cos#theta_{2}",-1.,1.);
  RooRealVar* phi = new RooRealVar("phi","#Phi",-TMath::Pi(),TMath::Pi());

  vector<RooRealVar*> meas;
  meas.push_back(costheta1);
  meas.push_back(costheta2);
  meas.push_back(phi);

  RooRealVar* mH = new RooRealVar("mH","m_{H}",125.);
 
  ScalarPdfFactoryZH SMHiggs(costheta1,costheta2,phi,mH,2,false);

  ScalarPdfFactoryZH psScalar(costheta1,costheta2,phi,mH,2,false);
  psScalar.fa2->setVal(0.0);
  psScalar.fa3->setVal(0.9999999);
  psScalar.phia2->setVal(0.0);
  psScalar.phia3->setVal(0.0);

  ScalarPdfFactoryZH f3p5Scalar(costheta1,costheta2,phi,mH,2,false);
  f3p5Scalar.fa2->setVal(0.0);
  // f3p5Scalar.fa3->setVal(0.848419);
  f3p5Scalar.fa3->setVal(0.5);
  f3p5Scalar.phia2->setVal(0.0);
  f3p5Scalar.phia3->setVal(0.0);

  ScalarPdfFactoryZH f3p5_90_Scalar(costheta1,costheta2,phi,mH,2,false);
  f3p5_90_Scalar.fa2->setVal(0.0);
  // f3p5_90_Scalar.fa3->setVal(0.848419);
  f3p5_90_Scalar.fa3->setVal(0.5);
  f3p5_90_Scalar.phia2->setVal(0.0);
  f3p5_90_Scalar.phia3->setVal(3.1415/2.);

  TChain* SMHtree = new TChain("SelectedTree");
  SMHtree->Add("samples/ee_ZH/unweighted_unpol_g1_1M_false.root");
  RooDataSet* SMHdata = new RooDataSet("SMHdata","SMHdata",SMHtree,RooArgSet(*costheta1,*costheta2,*phi));

  TChain* PStree = new TChain("SelectedTree");
  PStree->Add("samples/ee_ZH/unweighted_unpol_g4_1M_false.root");
  RooDataSet* PSdata = new RooDataSet("PSdata","PSdata",PStree,RooArgSet(*costheta1,*costheta2,*phi));
  
  
  TChain* f3p5tree = new TChain("SelectedTree");
  f3p5tree->Add("samples/ee_ZH/ee_ZH_f3_p5_phi_0_250GeV_1M_false.root");
  RooDataSet* f3p5data = new RooDataSet("f3p5data","f3p5data",f3p5tree,RooArgSet(*costheta1,*costheta2,*phi));

  TChain* f3p5_90_tree = new TChain("SelectedTree");
  f3p5_90_tree->Add("samples/ee_ZH/ee_ZH_f3_p5_phi_pi2_250GeV_1M_false.root");
  RooDataSet* f3p5_90_data = new RooDataSet("f3p5_90_data","f3p5_90_data",f3p5_90_tree,RooArgSet(*costheta1,*costheta2,*phi));
  
  TString varName = "costheta1";
  const int nbins = 20;
  double xMin =  -1; 
  double xMax = 1.;
  
  if ( var == kcostheta2 )  {
    varName = "costheta2";
  }
  if ( var == kphi)    {
    varName = "phi";
    xMin = -TMath::Pi();
    xMax = TMath::Pi();
  }
  
  // 
  // Do some gymnastics about the plots
  // normalize to 1
  // 
  
  TH1F *h_0p = new TH1F("h_0p", "h_0p", nbins, xMin, xMax);
  SMHtree->Project("h_0p", varName);
  double rescale_0p = 1./h_0p->Integral();
  h_0p->Scale(rescale_0p);
  // std::cout << "rescale_0p = " << rescale_0p << "\n";
  double ymax = h_0p->GetMaximum();
  
  TH1F *h_0m = new TH1F("h_0m", "h_0m", nbins, xMin, xMax); 
  PStree->Project("h_0m", varName);
  double rescale_0m = 1./h_0m->Integral();
  // std::cout << "rescale_0m = " << rescale_0m << "\n";
  h_0m->Scale(rescale_0m);
  ymax = h_0m->GetMaximum() > ymax ? h_0m->GetMaximum() : ymax;

  TH1F *h_0mix = new TH1F("h_0mix", "h_0mix", nbins, xMin, xMax);
  f3p5tree->Project("h_0mix", varName);
  double rescale_0mix = 1./h_0mix->Integral();
  // std::cout << "rescale_0mix = " << rescale_0mix << "\n";
  h_0mix->Scale(rescale_0mix);
  ymax = h_0mix->GetMaximum() > ymax ? h_0mix->GetMaximum() : ymax;

  TH1F *h_0mixphase = new TH1F("h_0mixphase", "h_0mixphase", nbins, xMin, xMax);
  f3p5_90_tree->Project("h_0mixphase", varName);
  double rescale_0mixphase = 1./h_0mixphase->Integral();
  // std::cout << "rescale_0mixphase = " << rescale_0mixphase << "\n";
  h_0mixphase->Scale(rescale_0mixphase);
  ymax = h_0mixphase->GetMaximum() > ymax ? h_0mixphase->GetMaximum() : ymax;

  // create rooplot and plot data/pdfs
  RooPlot* plot = (meas[var])->frame(nbins);

  SMHdata->plotOn(plot,MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_0p));
  SMHiggs.PDF->plotOn(plot,LineColor(kRed),LineWidth(2), Normalization(rescale_0p));

  PSdata->plotOn(plot,MarkerColor(kBlue),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_0m));
  psScalar.PDF->plotOn(plot,LineColor(kBlue),LineWidth(2), Normalization(rescale_0m));

  f3p5data->plotOn(plot,MarkerColor(kGreen+3),MarkerStyle(25),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_0mix));
  f3p5Scalar.PDF->plotOn(plot,LineColor(kGreen+3),LineWidth(2), Normalization(rescale_0mix));

  f3p5_90_data->plotOn(plot,MarkerColor(kMagenta+1),MarkerStyle(20), MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_0mixphase));
  f3p5_90_Scalar.PDF->plotOn(plot,LineColor(kMagenta+1),LineWidth(2), Normalization(rescale_0mixphase));
  //TGaxis::SetMaxDigits(3);
  plot->SetMaximum(ymax*1.1);
  

  TCanvas* can = new TCanvas("can","can",600,600);
  // plot styles
  // gStyle->SetPadLeftMargin(0.05);
  plot->GetXaxis()->CenterTitle();
  plot->GetYaxis()->CenterTitle();
  plot->GetYaxis()->SetTitle(" ");
  plot->GetXaxis()->SetNdivisions(505);
  plot->Draw();
  
  can->SaveAs(Form("paperplots/ILCprojections_%s.eps", varName.Data()));
  can->SaveAs(Form("paperplots/ILCprojections_%s.png", varName.Data()));


  delete h_0p;
  delete h_0m;
  delete h_0mix;
  delete h_0mixphase;
  delete SMHtree;
  delete PStree;
  delete f3p5tree;  
  delete f3p5_90_tree;
  delete plot;
  delete can;
	      
}
