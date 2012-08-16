#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TChain.h"
#include "TCanvas.h"
#include <vector>
#include <algorithm>

using namespace RooFit;

void angularDistributions_spin0(int plotIndex=0, int binning=80){

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

  ScalarPdfFactory* SMHiggs = new ScalarPdfFactory(z1mass,z2mass,h1,h2,Phi,mzz);
  SMHiggs->makeSMHiggs();
  SMHiggs->makeParamsConst(true);

  ScalarPdfFactory* PSHiggs = new ScalarPdfFactory(z1mass,z2mass,h1,h2,Phi,mzz);
  PSHiggs->makePSHiggs();
  PSHiggs->makeParamsConst(true);

  ScalarPdfFactory* LGHiggs = new ScalarPdfFactory(z1mass,z2mass,h1,h2,Phi,mzz);
  LGHiggs->makeLGHiggs();
  LGHiggs->makeParamsConst(true);

  ScalarPdfFactory* A2Higgs = new ScalarPdfFactory(z1mass,z2mass,h1,h2,Phi,mzz);
  A2Higgs->makeCustom(0.0,1.0,0.0,0.0,0.0,0.0);
  A2Higgs->makeParamsConst(true);

  TChain* treeSM = new TChain("angles");
  treeSM->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/SMHiggs_store/SMHiggs_125GeV.root");
  RooDataSet* dataSM = new RooDataSet("dataSM","dataSM",treeSM,RooArgSet(*z1mass,*z2mass,*hs,*h1,*h2,*Phi,*Phi1));

  TChain* treePS = new TChain("angles");
  treePS->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/psScalar_store/psScalar_125GeV.root");
  RooDataSet* data = new RooDataSet("dataPS","dataPS",treePS,RooArgSet(*z1mass,*z2mass,*hs,*h1,*h2,*Phi,*Phi1));

  TChain* treeLG = new TChain("angles");
  treeLG->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/0hPlus_store/0hPlus_125GeV.root");
  RooDataSet* dataLG = new RooDataSet("dataLG","dataLG",treeLG,RooArgSet(*z1mass,*z2mass,*hs,*h1,*h2,*Phi,*Phi1));

  double normFix;
  if( ! strcmp( measureables[plotIndex]->GetName() , "phistar1" ) ) normFix=.001/(2.*3.1415);
  else if( ! strcmp( measureables[plotIndex]->GetName() , "costhetastar" ) ) normFix=.001/2.0;
  else normFix=.001;
  
  RooPlot* plot = measureables[plotIndex]->frame(binning);
  plot->GetXaxis()->CenterTitle();
  plot->GetYaxis()->CenterTitle();
  plot->GetYaxis()->SetTitle(" ");
  plot->GetXaxis()->SetNdivisions(-505);

  cout << 1./dataSM->sumEntries() << endl;
  double rescale=(double)(1./dataSM->sumEntries());
  dataSM->plotOn(plot,MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),LineWidth(0),XErrorSize(0),Rescale(.001),DataError(RooAbsData::None));
  SMHiggs.PDF->plotOn(plot,LineColor(kRed),LineWidth(2),Normalization( normFix ));
  cout << 1./dataPS->sumEntries() << endl;
  rescale=(double)(1./dataPS->sumEntries());
  dataPS->plotOn(plot,MarkerColor(kBlue),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),Rescale(.001),DataError(RooAbsData::None));
  PSHiggs.PDF->plotOn(plot,LineColor(kBlue),LineWidth(2),Normalization( normFix ));
  cout << 1./dataLG->sumEntries() << endl;
  rescale=(double)(1./dataLG->sumEntries());
  dataLG->plotOn(plot,MarkerColor(kGreen+3),MarkerStyle(25),MarkerSize(1.5),XErrorSize(0),Rescale(.001),DataError(RooAbsData::None));
  LGHiggs.PDF->plotOn(plot,LineColor(kGreen+3),LineWidth(2),Normalization( normFix ));

  TGaxis::SetMaxDigits(3);

  TCanvas* can =new TCanvas("can","can",600,600);

  gStyle->SetPadLeftMargin(0.05);

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

  plot->Draw();

  char temp[150];
  sprintf(temp,"epsfiles/%s_125GeV_spin0_3in1.eps",measureables[plotIndex]->GetName());
  can->SaveAs(temp);
  sprintf(temp,"pngfiles/%s_125GeV_spin0_3in1.png",measureables[plotIndex]->GetName());
  can->SaveAs(temp);

  delete SMHiggs;
  delete PSHiggs;
  delete LGHiggs;

}
