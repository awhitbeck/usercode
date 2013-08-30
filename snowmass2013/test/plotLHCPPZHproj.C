#include <vector>
#include "RooRealVar.h"
#include "../src/ScalarPdfFactoryPPZH.cc"
#include "RooPlot.h"
#include "TChain.h"
#include "RooDataSet.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TH1F.h"

// 
// Run by  root -l -b loadLib.C  plotLHCPPZHproj.C+
// 
using namespace RooFit;
using namespace std;

enum variables{kcostheta1,kcostheta2,kphi,km,kY};

void plotLHCPPZHproj(variables var=kcostheta2){

  RooRealVar* costheta1 = new RooRealVar("costheta1","cos#theta_{1}",-1.,1.);
  RooRealVar* costheta2 = new RooRealVar("costheta2","cos#theta_{2}",-1.,1.);
  RooRealVar* phi = new RooRealVar("phi","#Phi",-TMath::Pi(),TMath::Pi());
  RooRealVar* m= new RooRealVar("m","m_{VH} [GeV]", 200, 1000);
  RooRealVar* Y= new RooRealVar("Y","Rapidity (VH)", -4, 4);
  // additional variables
  // event weight
  RooRealVar* wt = new RooRealVar("wt", "wt", 0.0, 50);

  vector<RooRealVar*> meas;
  meas.push_back(costheta1);
  meas.push_back(costheta2);
  meas.push_back(phi);
  meas.push_back(m);
  meas.push_back(Y);
  meas.push_back(wt);

  
  int parameterization = 1; 

  RooRealVar* mH = new RooRealVar("mH","m_{H}",125.);
  ScalarPdfFactoryPPZH SMHiggs(costheta1,costheta2,phi,m,Y,mH,parameterization,false);

  ScalarPdfFactoryPPZH psScalar(costheta1,costheta2,phi,m,Y,mH,parameterization,false);
  psScalar.g1Val->setVal(0.0);
  psScalar.g4Val->setVal(1.0);

  ScalarPdfFactoryPPZH f3p5Scalar(costheta1,costheta2,phi,m,Y,mH,parameterization,false);
  f3p5Scalar.g1Val->setVal(1.0);
  f3p5Scalar.g4Val->setVal(0.144708);

  ScalarPdfFactoryPPZH f3p5_90_Scalar(costheta1,costheta2,phi,m,Y,mH,parameterization,false);
  f3p5_90_Scalar.g1Val->setVal(1.0);
  f3p5_90_Scalar.g4ValIm->setVal(0.144708);


  //
  //  Read datasets
  // 
  TChain* SMHtree = new TChain("SelectedTree");
  SMHtree->Add("samples/pp_ZH/pp_ZH_llbb_g1_1M_false.root");
  RooDataSet dataTMPSMH("dataTMPSMH","dataTMPSMH", SMHtree, RooArgSet(*costheta1,*costheta2,*phi,*m,*Y, *wt));
  RooDataSet SMHdata = RooDataSet("SMHData","SMHData",RooArgList(*costheta1,*costheta2,*phi,*m, *Y, *wt), WeightVar("wt"), Import(dataTMPSMH));

  TChain* PStree = new TChain("SelectedTree");
  PStree->Add("samples/pp_ZH/pp_ZH_llbb_g4_1M_false.root");
  RooDataSet dataTMPPS("dataTMPPS","dataTMPPS", PStree, RooArgSet(*costheta1,*costheta2,*phi,*m,*Y, *wt));
  RooDataSet PSdata = RooDataSet("PSData","PSData",RooArgList(*costheta1,*costheta2,*phi,*m, *Y, *wt), WeightVar("wt"), Import(dataTMPPS));

  TChain* f3p5tree = new TChain("SelectedTree");
  f3p5tree->Add("samples/pp_ZH/pp_ZH_llbb_g1_p_g4_1M_false.root");
  RooDataSet dataTMPf3p5("dataTMPf3p5","dataTMPf3p5", f3p5tree, RooArgSet(*costheta1,*costheta2,*phi,*m,*Y, *wt));
  RooDataSet f3p5data = RooDataSet("f3p5Data","f3p5Data",RooArgList(*costheta1,*costheta2,*phi,*m, *Y, *wt), WeightVar("wt"), Import(dataTMPf3p5));

  TChain* f3p5_90_tree = new TChain("SelectedTree");
  f3p5_90_tree->Add("samples/pp_ZH/pp_ZH_llbb_g1_p_ig4_1M_false.root");
  RooDataSet dataTMPf3p5_90("dataTMPf3p5_90","dataTMPf3p5_90", f3p5_90_tree, RooArgSet(*costheta1,*costheta2,*phi,*m,*Y, *wt));
  RooDataSet f3p5_90_data = RooDataSet("f3p5_90Data","f3p5_90Data",RooArgList(*costheta1,*costheta2,*phi,*m, *Y, *wt), WeightVar("wt"), Import(dataTMPf3p5_90));
  
  TString varName = "costheta1";
  const int nbins = 20;
  double xMin =  -1; 
  double xMax = 1.;
  
  if ( var == kcostheta2 ) {
    varName = "costheta2";
  } else if( var == kphi)  {
    varName = "phi";
    xMin = -TMath::Pi();
    xMax = TMath::Pi();
  } else if ( var == km)   {
    varName = "m";
    xMin = 200;
    xMax = 1000.;
  } else if ( var == kY)  {
    varName = "Y";
    xMin = -4;
    xMax = 4;
  }
  // 
  // Do some gymnastics about the plots
  // normalize to 1
  // 
  
  TH1F *h_0p = new TH1F("h_0p", "h_0p", nbins, xMin, xMax);
  SMHtree->Project("h_0p", varName,"wt");
  double rescale_0p = 1./h_0p->Integral();
  h_0p->Scale(rescale_0p);
  // std::cout << "rescale_0p = " << rescale_0p << "\n";
  double ymax = h_0p->GetMaximum();
  
  TH1F *h_0m = new TH1F("h_0m", "h_0m", nbins, xMin, xMax); 
  PStree->Project("h_0m", varName,"wt");
  double rescale_0m = 1./h_0m->Integral();
  // std::cout << "rescale_0m = " << rescale_0m << "\n";
  h_0m->Scale(rescale_0m);
  ymax = h_0m->GetMaximum() > ymax ? h_0m->GetMaximum() : ymax;

  TH1F *h_0mix = new TH1F("h_0mix", "h_0mix", nbins, xMin, xMax);
  f3p5tree->Project("h_0mix", varName,"wt");
  double rescale_0mix = 1./h_0mix->Integral();
  // std::cout << "rescale_0mix = " << rescale_0mix << "\n";
  h_0mix->Scale(rescale_0mix);
  ymax = h_0mix->GetMaximum() > ymax ? h_0mix->GetMaximum() : ymax;

  TH1F *h_0mixphase = new TH1F("h_0mixphase", "h_0mixphase", nbins, xMin, xMax);
  f3p5_90_tree->Project("h_0mixphase", varName,"wt");
  double rescale_0mixphase = 1./h_0mixphase->Integral();
  // std::cout << "rescale_0mixphase = " << rescale_0mixphase << "\n";
  h_0mixphase->Scale(rescale_0mixphase);
  ymax = h_0mixphase->GetMaximum() > ymax ? h_0mixphase->GetMaximum() : ymax;
  

  // create rooplot and plot data/pdfs
  RooPlot* plot = (meas[var])->frame(nbins);
  
  SMHdata.plotOn(plot,MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_0p));
  SMHiggs.PDF->plotOn(plot,LineColor(kRed),LineWidth(2), Normalization(rescale_0p));

  PSdata.plotOn(plot,MarkerColor(kBlue),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_0m));
  psScalar.PDF->plotOn(plot,LineColor(kBlue),LineWidth(2), Normalization(rescale_0m));

  f3p5data.plotOn(plot,MarkerColor(kGreen+3),MarkerStyle(25),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_0mix));
  f3p5Scalar.PDF->plotOn(plot,LineColor(kGreen+3),LineWidth(2), Normalization(rescale_0mix));

  f3p5_90_data.plotOn(plot,MarkerColor(kMagenta+1),MarkerStyle(20), MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_0mixphase));
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

  TString plotName = "h1";
  if ( var == kcostheta2) plotName = "h2";
  if ( var == kphi) plotName = "phi";
  if ( var == km) plotName = "mvh";
  if ( var == kY) plotName = "Yvh";
  
  can->SaveAs(Form("paperplots/%s_ppzh_pp_ZH_llbb_accfalse.eps", plotName.Data()));
  can->SaveAs(Form("paperplots/%s_ppzh_pp_ZH_llbb_accfalse.png", plotName.Data()));

  delete h_0p;
  delete h_0m;
  delete h_0mix;
  delete h_0mixphase;
  delete plot;
  delete can;
  delete SMHtree;
  delete PStree;
  delete f3p5tree;  
  delete f3p5_90_tree;
}
