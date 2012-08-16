#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TChain.h"
#include "TCanvas.h"
#include <vector>

using namespace RooFit;

void angularDistributions_spin1(int plotIndex=0, int binning=80){

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

  VectorPdfFactory* Vec = new VectorPdfFactory(z1mass,z2mass,hs,h1,h2,Phi,Phi1,mzz);
  Vec->makeZprime();
  Vec->makeParamsConst(true);

  VectorPdfFactory* psVec = new VectorPdfFactory(z1mass,z2mass,hs,h1,h2,Phi,Phi1,mzz);
  psVec->makePseudoZprime();
  psVec->makeParamsConst(true);

  TChain* treeVec = new TChain("angles");
  treeVec->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/vector_store/vector_125GeV_round*.root");
  RooDataSet* dataVec = new RooDataSet("dataVec","dataVec",treeVec,RooArgSet(*z1mass,*z2mass,*hs,*h1,*h2,*Phi,*Phi1));

  TChain* treepsVec = new TChain("angles");
  treepsVec->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/psVector_store/psVector_125GeV_round*.root");
  RooDataSet* datapsVec = new RooDataSet("datapsVec","datapsVec",treepsVec,RooArgSet(*z1mass,*z2mass,*hs,*h1,*h2,*Phi,*Phi1));

  RooPlot* plot = measureables[plotIndex]->frame(binning);
  plot->GetXaxis()->CenterTitle();
  plot->GetYaxis()->CenterTitle();
  plot->GetYaxis()->SetTitle(" ");
  plot->GetXaxis()->SetNdivisions(-505);

  dataVec->plotOn(plot,MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),LineWidth(0),XErrorSize(0),Rescale(.001),DataError(RooAbsData::None));
  Vec.PDF->plotOn(plot,LineColor(kRed),LineWidth(2),Normalization(.001));
  datapsVec->plotOn(plot,MarkerColor(kBlue),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),Rescale(.001),DataError(RooAbsData::None));
  psVec.PDF->plotOn(plot,LineColor(kBlue),LineWidth(2),Normalization(.001));

  TGaxis::SetMaxDigits(3);

  TCanvas* can =new TCanvas("can","can",600,600);

  gStyle->SetPadLeftMargin(0.05);

  char temp[150];
  sprintf(temp,"%s>>vec_histo(%i,%i,%i)",measureables[plotIndex]->GetName(),binning,(int)measureables[plotIndex]->getMin(),(int)measureables[plotIndex]->getMin());
  treeVec->Draw(temp);
  TH1F* vec_histo = (TH1F*) gDirectory->Get("vec_histo");
  sprintf(temp,"%s>>psVec_histo(%i,%i,%i)",measureables[plotIndex]->GetName(),binning,(int)measureables[plotIndex]->getMin(),(int)measureables[plotIndex]->getMin());
  treepsVec->Draw(temp);
  TH1F* psvec_histo = (TH1F*) gDirectory->Get("psVec_histo");

  if(!strcmp(measureables[plotIndex]->GetName(),"z1mass"))
    plot->GetYaxis()->SetRangeUser(0,max(vec_histo->GetMaximum(),psVec_histo->GetMaximum())*.8/1000.);
  else
    plot->GetYaxis()->SetRangeUser(0,max(vec_histo->GetMaximum(),psVec_histo->GetMaximum())*1.3/1000.);

  //cout << vec_histo->GetMaximum()*.8/1000. << endl;
  //cout << psVec_histo->GetMaximum()*.8/1000. << endl;

  //cout << "max range: " << max(vec_histo->GetMaximum(),psVec_histo->GetMaximum())*.8/1000. << endl;

  plot->Draw();

  char temp[150];
  sprintf(temp,"epsfiles/%s_125GeV_spin1_2in1.eps",measureables[plotIndex]->GetName());
  can->SaveAs(temp);
  sprintf(temp,"pngfiles/%s_125GeV_spin1_2in1.png",measureables[plotIndex]->GetName());
  can->SaveAs(temp);

  delete Vec;
  delete psVec;

}
