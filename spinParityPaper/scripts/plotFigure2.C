#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TChain.h"
#include "TCanvas.h"
#include <vector>
#include <algorithm>

using namespace RooFit;

void plotFigure2(int plotIndex=0, int binning=50){

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
  RooRealVar* z1mass = new RooRealVar("z1mass","m_{1} [GeV]",30,100);
  RooRealVar* z2mass = new RooRealVar("z2mass","m_{2} [GeV]",1e-09,65);
  RooRealVar* hs = new RooRealVar("costhetastar","cos#theta*",-1,1);
  RooRealVar* h1 = new RooRealVar("costheta1","cos#theta_{1}",-1,1);
  RooRealVar* h2 = new RooRealVar("costheta2","cos#theta_{2}",-1,1);
  RooRealVar* Phi = new RooRealVar("phi","#Phi",-TMath::Pi(),TMath::Pi());
  RooRealVar* Phi1 = new RooRealVar("phistar1","#Phi_{1}",-TMath::Pi(),TMath::Pi());

  vector<RooRealVar*> measureables;
  measureables.push_back(z1mass);

  RooRealVar* mzz = new RooRealVar("mzz","mzz",125,100,1000);

  ScalarPdfFactory* SMHiggs = new ScalarPdfFactory(z1mass,z2mass,h1,h2,Phi,mzz);
  SMHiggs->makeSMHiggs();
  SMHiggs->makeParamsConst(true);

  ScalarPdfFactory* PSHiggs = new ScalarPdfFactory(z1mass,z2mass,h1,h2,Phi,mzz);
  PSHiggs->makePSHiggs();
  PSHiggs->makeParamsConst(true);

  TensorPdfFactory* minGrav = new TensorPdfFactory(z1mass,z2mass,hs,h1,h2,Phi,Phi1,mzz);
  minGrav->makeMinGrav();
  minGrav->makeParamsConst(true);

  TensorPdfFactory* TwohPlus = new TensorPdfFactory(z1mass,z2mass,hs,h1,h2,Phi,Phi1,mzz);
  TwohPlus->make2hPlus();
  TwohPlus->makeParamsConst(true);

  TChain* treeSM = new TChain("angles");
  treeSM->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/SMHiggs_store/SMHiggs_125GeV.root");
  RooDataSet* dataSM = new RooDataSet("dataSM","dataSM",treeSM,RooArgSet(*z1mass,*z2mass,*hs,*h1,*h2,*Phi,*Phi1));

  TChain* treePS = new TChain("angles");
  treePS->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/psScalar_store/psScalar_125GeV.root");
  RooDataSet* data = new RooDataSet("dataPS","dataPS",treePS,RooArgSet(*z1mass,*z2mass,*hs,*h1,*h2,*Phi,*Phi1));

  TChain* treeMinGrav = new TChain("angles");
  treeMinGrav->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/minGrav_store/minGrav_125GeV.root");
  RooDataSet* dataMinGrav = new RooDataSet("dataMinGrav","dataMinGrav",treeMinGrav,RooArgSet(*z1mass,*z2mass,*hs,*h1,*h2,*Phi,*Phi1));

  TChain* tree2hPlus = new TChain("angles");
  tree2hPlus->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/2hPlus_store/2hPlus_125GeV_round*.root");
  RooDataSet* data2hPlus = new RooDataSet("data2hPlus","data2hPlus",tree2hPlus,RooArgSet(*z1mass,*z2mass,*hs,*h1,*h2,*Phi,*Phi1));

  RooPlot* plot = measureables[plotIndex]->frame(binning);
  plot->GetXaxis()->CenterTitle();
  plot->GetYaxis()->CenterTitle();
  plot->GetYaxis()->SetTitle(" ");
  plot->GetXaxis()->SetNdivisions(-505);

  dataSM->plotOn(plot,MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),LineWidth(0),XErrorSize(0),Rescale(.001),DataError(RooAbsData::None));
  SMHiggs.PDF->plotOn(plot,LineColor(kRed),LineWidth(2),Normalization( .001 ));

  dataPS->plotOn(plot,MarkerColor(kMagenta+1),MarkerStyle(25),MarkerSize(1.5),XErrorSize(0),Rescale(.001),DataError(RooAbsData::None));
  PSHiggs.PDF->plotOn(plot,LineColor(kMagenta+1),LineWidth(2),Normalization( .001 ));

  dataMinGrav->plotOn(plot,MarkerColor(kBlue),MarkerStyle(32),MarkerSize(1.5),XErrorSize(0),Rescale(.001),DataError(RooAbsData::None));
  minGrav.PDF->plotOn(plot,LineColor(kBlue),LineWidth(2),Normalization( .001 ));

  data2hPlus->plotOn(plot,MarkerColor(kGreen+3),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),Rescale(.001*15./2.),DataError(RooAbsData::None));
  TwohPlus.PDF->plotOn(plot,LineColor(kGreen+3),LineWidth(2),Normalization( .001*15./2. ));  

  TGaxis::SetMaxDigits(3);

  TCanvas* can =new TCanvas("can","can",600,600);

  gStyle->SetPadLeftMargin(0.05);

  /*
  char temp[150];
  sprintf(temp,"%s>>SM_histo(%i,%i,%i)",measureables[plotIndex]->GetName(),binning,(int)measureables[plotIndex]->getMin(),(int)measureables[plotIndex]->getMin());
  treeSM->Draw(temp);
  TH1F* SM_histo = (TH1F*) gDirectory->Get("SM_histo");
  sprintf(temp,"%s>>PS_histo(%i,%i,%i)",measureables[plotIndex]->GetName(),binning,(int)measureables[plotIndex]->getMin(),(int)measureables[plotIndex]->getMin());
  treePS->Draw(temp);
  TH1F* PS_histo = (TH1F*) gDirectory->Get("PS_histo");
  sprintf(temp,"%s>>LG_histo(%i,%i,%i)",measureables[plotIndex]->GetName(),binning,(int)measureables[plotIndex]->getMin(),(int)measureables[plotIndex]->getMin());
  treeLG->Draw(temp);
  TH1F* LG_histo = (TH1F*) gDirectory->Get("LG_histo");

  if(!strcmp(measureables[plotIndex]->GetName(),"z1mass"))
    plot->GetYaxis()->SetRangeUser(0,max(max(LG_histo->GetMaximum(),PS_histo->GetMaximum()),SM_histo->GetMaximum())*.8/1000.);
  else
    plot->GetYaxis()->SetRangeUser(0,max(max(LG_histo->GetMaximum(),PS_histo->GetMaximum()),SM_histo->GetMaximum())*1.3/1000.);

  //cout << "max range: " << max(max(LG_histo->GetMaximum(),PS_histo->GetMaximum()),SM_histo->GetMaximum())*.8/1000. << endl;
  */

  plot->GetYaxis()->SetRangeUser(0.1,200);
  can->SetLogy();
  plot->Draw();

  char temp[150];
  sprintf(temp,"epsfiles/figure2.eps",measureables[plotIndex]->GetName());
  can->SaveAs(temp);
  sprintf(temp,"pngfiles/figure2.png",measureables[plotIndex]->GetName());
  can->SaveAs(temp);

  delete SMHiggs;
  delete PSHiggs;
  delete minGrav;
  delete TwohPlus;

}
