#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TChain.h"
#include "TCanvas.h"
#include <vector>

using namespace RooFit;

void spin2_fit2SMHiggs(int plotIndex=0, int binning=80){
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
  RooRealVar* z1mass = new RooRealVar("z1mass","m_{1} [GeV]",90,40,110);
  RooRealVar* z2mass = new RooRealVar("z2mass","m_{2} [GeV]",30,1e-09,65);

  //z1mass.setConstant(kTRUE);
  //z2mass.setConstant(kTRUE);
  
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

  TensorPdfFactory* MinGrav = new TensorPdfFactory(z1mass,z2mass,hs,h1,h2,Phi,Phi1,mzz);
  MinGrav->makeMinGrav();
  MinGrav->makeParamsConst(false);
  MinGrav->fz1Val->setVal(.4);
  MinGrav->fz1Val->setConstant(kTRUE);
  MinGrav->fz2Val->setVal(.4);
  MinGrav->fz2Val->setConstant(kTRUE);
  MinGrav->R2Val->setConstant(kTRUE);
  MinGrav->R1Val->setConstant(kTRUE);
  MinGrav->gamZ->setConstant(kTRUE);
  MinGrav->mZ->setConstant(kTRUE);
  
  MinGrav->c1Val->setConstant(kTRUE);
  MinGrav->c2Val->setConstant(kTRUE);
  MinGrav->c3Val->setConstant(kTRUE);
  MinGrav->c4Val->setConstant(kTRUE);
  MinGrav->c5Val->setConstant(kTRUE);
  MinGrav->c6Val->setConstant(kTRUE);
  MinGrav->c7Val->setConstant(kTRUE);
  MinGrav->useGTerm->setConstant(kTRUE);
  MinGrav->g1Val->setVal(1.0);
  MinGrav->g1Val->setConstant(kTRUE);
  //MinGrav->g2Val->setConstant(kTRUE);
  MinGrav->g3Val->setConstant(kTRUE);
  MinGrav->g4Val->setConstant(kTRUE);
  MinGrav->g5Val->setVal(300.0);
  MinGrav->g6Val->setConstant(kTRUE);
  MinGrav->g7Val->setConstant(kTRUE);
  MinGrav->g8Val->setConstant(kTRUE);
  MinGrav->g9Val->setConstant(kTRUE);
  MinGrav->g10Val->setConstant(kTRUE);

  TChain* treeMinGrav = new TChain("angles");
  treeMinGrav->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/SMHiggs_store/SMHiggs_125GeV.root");
  if(treeMinGrav->GetEntries()<=0){ cout << "couldn't load minGrav data" << endl; return;}
  RooDataSet* dataSM = new RooDataSet("dataSM","dataSM",treeMinGrav,RooArgSet(*mzz,*z1mass,*z2mass,*hs,*h1,*h2,*Phi,*Phi1),"((phi*100000)%1000)<-995");

  dataSM->getNorm

  cout << "TOTAL NUMBER OF EVENTS: " << dataSM.numEntries() << endl;

  //MinGrav->PDF->fitTo(*dataSM);

  RooPlot* plot = measureables[plotIndex]->frame(binning);
  plot->GetXaxis()->CenterTitle();
  plot->GetYaxis()->CenterTitle();
  plot->GetYaxis()->SetTitle(" ");
  plot->GetXaxis()->SetNdivisions(-505);

  //dataSM->plotOn(plot,MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),LineWidth(0),XErrorSize(0),Rescale((3./15.)*.001),DataError(RooAbsData::None)); 
  //MinGrav->PDF->plotOn(plot,LineColor(kRed),LineWidth(2),Normalization( (3./15.)*0.001 ));

  TGaxis::SetMaxDigits(3);

  TCanvas* can =new TCanvas("can","can",600,600);

  gStyle->SetPadLeftMargin(0.05);

  plot->Draw();

  /*
  char temp[150];
  sprintf(temp,"epsfiles/%s_125GeV_spin2_3in1.eps",measureables[plotIndex]->GetName());
  can->SaveAs(temp);
  sprintf(temp,"pngfiles/%s_125GeV_spin2_3in1.png",measureables[plotIndex]->GetName());
  can->SaveAs(temp);
  */

}
