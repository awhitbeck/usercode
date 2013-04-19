#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TChain.h"
#include "TCanvas.h"
#include <vector>
#include <algorithm>

using namespace RooFit;

void angularDistributions_spin0(int plotIndex=0, int binning=80){

  // observables
  RooRealVar* z1mass = new RooRealVar("z1mass","m_{1} [GeV]",40,110);
  RooRealVar* z2mass = new RooRealVar("z2mass","m_{2} [GeV]",1e-09,65);
  RooRealVar* hs = new RooRealVar("costhetastar","cos#theta*",-1,1);
  RooRealVar* h1 = new RooRealVar("costheta1","cos#theta_{1}",-1,1);
  RooRealVar* h2 = new RooRealVar("costheta2","cos#theta_{2}",-1,1);
  RooRealVar* Phi = new RooRealVar("phi","#Phi",-TMath::Pi(),TMath::Pi());
  RooRealVar* Phi1 = new RooRealVar("phistar1","#Phi_{1}",-TMath::Pi(),TMath::Pi());

  vector<RooRealVar*> measureables;
  measureables.push_back(z1mass);
  measureables.push_back(z2mass);
  measureables.push_back(hs);
  measureables.push_back(h1);
  measureables.push_back(h2);
  measureables.push_back(Phi);
  measureables.push_back(Phi1);

  RooRealVar* mzz = new RooRealVar("mzz","mzz",125,100,1000);

  ScalarPdfFactory* SMHiggs = new ScalarPdfFactory(z1mass,z2mass,hs,h1,h2,Phi,Phi1,mzz);
  SMHiggs->makeSMHiggs();
  SMHiggs->makeParamsConst(true);

  TChain* treeSM = new TChain("angles");
  treeSM->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/SMHiggs_store/SMHiggs_125GeV.root");
  RooDataSet* dataSM = new RooDataSet("dataSM","dataSM",treeSM,RooArgSet(*z1mass,*z2mass,*hs,*h1,*h2,*Phi,*Phi1));

  RooPlot* plot = measureables[plotIndex]->frame(binning);
  plot->GetXaxis()->CenterTitle();
  plot->GetYaxis()->CenterTitle();
  plot->GetYaxis()->SetTitle(" ");
  plot->GetXaxis()->SetNdivisions(-505);

  dataSM->plotOn(plot,MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),LineWidth(0),XErrorSize(0),DataError(RooAbsData::None));
  SMHiggs.PDF->plotOn(plot,LineColor(kRed),LineWidth(2));

  TGaxis::SetMaxDigits(3);

  TCanvas* can =new TCanvas("can","can",600,600);

  gStyle->SetPadLeftMargin(0.05);

  plot->Draw();

  char temp[150];
  sprintf(temp,"epsfiles/%s_125GeV_spin0_3in1_wIntegral.eps",measureables[plotIndex]->GetName());
  can->SaveAs(temp);
  sprintf(temp,"pngfiles/%s_125GeV_spin0_3in1_wIntegral.png",measureables[plotIndex]->GetName());
  can->SaveAs(temp);

  delete SMHiggs;

}
