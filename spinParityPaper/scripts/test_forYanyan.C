#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TChain.h"
#include "TCanvas.h"
#include <vector>

using namespace RooFit;

void test_forYanyan(int plotIndex=0, int binning=80){

  gROOT->ProcessLine(".L  ../PDFs/RooXZsZs_5D.cxx+");
  gROOT->ProcessLine(".L  ../PDFs/RooSpinOne_7D.cxx+");  
  gROOT->ProcessLine(".L  ../PDFs/RooSpinTwo_7D.cxx+");  
  gROOT->ProcessLine(".L  ../src/AngularPdfFactory.cc+");
  gROOT->ProcessLine(".L  ../src/ScalarPdfFactory.cc+");
  gROOT->ProcessLine(".L  ../src/VectorPdfFactory.cc+");
  gROOT->ProcessLine(".L  ../src/TensorPdfFactory.cc+");

  gROOT->ProcessLine(".L  ~/tdrstyle.C");
  setTDRStyle();

  // observables
  RooRealVar* z1mass = new RooRealVar("z1mass","m_{Z1}",91.188,40,110);
  RooRealVar* z2mass = new RooRealVar("z2mass","m_{Z2}",91.188,40,110);
  RooRealVar* hs = new RooRealVar("costhetastar","cos#theta^{*}",-1,1);
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

  RooRealVar* mzz = new RooRealVar("mzz","mzz",250,100,1000);

  /*
  ScalarPdfFactory* SMHiggs = new ScalarPdfFactory(z1mass,z2mass,h1,h2,Phi,mzz);
  SMHiggs->makeSMHiggs();
  SMHiggs->makeParamsConst(true);
  */

  TensorPdfFactory* minGrav = new TensorPdfFactory(z1mass,z2mass,hs,h1,h2,Phi,Phi1,mzz);
  minGrav->makeMinGrav();
  minGrav->makeParamsConst(true);


  TChain* treeSM = new TChain("angles");
  //treeSM->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/SMHiggs_store/SMHiggs_250GeV_onShellZs.root");
  treeSM->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/minGrav_store/minGrav_250GeV.root");
  //RooDataSet* dataSM = new RooDataSet("dataSM","dataSM",treeSM,RooArgSet(*hs,*h1,*h2,*Phi,*Phi1));
  RooDataSet* dataSM = new RooDataSet("dataSM","dataSM",treeSM,RooArgSet(*z1mass,*z2mass,*hs,*h1,*h2,*Phi,*Phi1));

  double normFix;
  if( ! strcmp( measureables[plotIndex]->GetName() , "phistar1" ) ) normFix=1./(2.*3.1415);
  else if( ! strcmp( measureables[plotIndex]->GetName() , "costhetastar" ) ) normFix=1./2.0;
  else normFix=1.0;
  normFiz=1.0;
  
  RooPlot* plot = measureables[plotIndex]->frame(binning);
  cout << 1./dataSM->sumEntries() << endl;
  double rescale=(double)(1./dataSM->sumEntries());
  dataSM->plotOn(plot,MarkerColor(kRed),MarkerStyle(4)/*,Rescale(rescale)*/);
  //SMHiggs.PDF->plotOn(plot,LineColor(kRed),LineWidth(2),Normalization( normFix ));
  minGrav.PDF->plotOn(plot,LineColor(kRed),LineWidth(2),Normalization( normFix ));

  TGaxis::SetMaxDigits(3);

  TCanvas* can =new TCanvas("can","can",600,600);
  plot->Draw();

  //delete SMHiggs;
  delete minGrav;

  char temp[150];
  //sprintf(temp,"epsfiles/%s_250GeV_spin0_SMHiggs.eps",measureables[plotIndex]->GetName());
  sprintf(temp,"epsfiles/%s_250GeV_spin2_minGrav.eps",measureables[plotIndex]->GetName());
  can->SaveAs(temp);
  //sprintf(temp,"pngfiles/%s_250GeV_spin0_SMHiggs.png",measureables[plotIndex]->GetName());
  sprintf(temp,"pngfiles/%s_250GeV_spin2_minGrav.png",measureables[plotIndex]->GetName());
  can->SaveAs(temp);

}
