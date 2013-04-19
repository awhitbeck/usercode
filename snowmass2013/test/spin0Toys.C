#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TChain.h"
#include "TCanvas.h"
#include <vector>

using namespace RooFit;

void spin0Toys(){

  gSystem->AddIncludePath("-I$ROOFITSYS/include/");
  gROOT->ProcessLine(".L ../src/RooSpinZero_7DComplex.cc+");
  gROOT->ProcessLine(".L ../src/AngularPdfFactory.cc+");
  gROOT->ProcessLine(".L ../src/ScalarPdfFactory.cc+");

  gROOT->ProcessLine(".L ~whitbeck/tdrstyle.C");
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

  RooRealVar* mzz = new RooRealVar("mzz","mzz",125,100,1000);
  mzz->setConstant(kTRUE);

  ScalarPdfFactory* SMHiggs = new ScalarPdfFactory(z1mass,z2mass,hs,h1,h2,Phi,Phi1,mzz);
  SMHiggs->makeSMHiggs();
  SMHiggs->makeParamsConst(false);
  SMHiggs->R2Val->setConstant(kTRUE);
  SMHiggs->R1Val->setConstant(kTRUE);
  SMHiggs->gamZ->setConstant(kTRUE);
  SMHiggs->mZ->setConstant(kTRUE);
  
  SMHiggs->a1Val->setConstant(kTRUE);
  SMHiggs->a2Val->setConstant(kTRUE);
  SMHiggs->a3Val->setConstant(kTRUE);
  SMHiggs->phi1Val->setConstant(kTRUE);
  SMHiggs->phi2Val->setConstant(kTRUE);
  SMHiggs->phi3Val->setConstant(kTRUE);
  SMHiggs->useGTerm->setConstant(kTRUE);
  SMHiggs->g1Val->setConstant(kTRUE);
  //SMHiggs->g2Val->setConstant(kTRUE);
  SMHiggs->g3Val->setConstant(kTRUE);
  //SMHiggs->g4Val->setConstant(kTRUE);
  SMHiggs->g1ValIm->setVal(0.0);
  SMHiggs->g1ValIm->setConstant(kTRUE);
  //SMHiggs->g2ValIm->setConstant(kTRUE);
  SMHiggs->g3ValIm->setConstant(kTRUE);
  //SMHiggs->g4ValIm->setConstant(kTRUE);

  // --------------------
  // start toys
  // --------------------

  RooDataSet* myToy;
  TH1F* g2rHist = new TH1F("g2rHist","g2rHist;Re(g_{2}/g_{1});Number of Toys",25,-2,2);
  TH1F* g4rHist = new TH1F("g4rHist","g4rHist;Re(g_{4}/g_{1});Number of Toys",25,-2,2);
  TH1F* g2iHist = new TH1F("g2iHist","g2iHist;Im(g_{2}/g_{1});Number of Toys",25,-2,2);
  TH1F* g4iHist = new TH1F("g4iHist","g4iHist;Im(g_{4}/g_{1});Number of Toys",25,-2,2);

  for(int iToy = 0 ; iToy<100 ; iToy++){

    cout << "////////////////////////////////////////" << endl;
    cout << "              STARTING TOY " << iToy << endl;
    cout << "////////////////////////////////////////" << endl;

    SMHiggs->makeSMHiggs();

    myToy = (RooDataSet*)SMHiggs->PDF->generate(RooArgSet(*z1mass,*z2mass,*hs,*h1,*h2,*Phi,*Phi1),100);
    
    SMHiggs->PDF->fitTo(*myToy);    
    g2rHist->Fill(SMHiggs->g2Val->getVal());
    g4rHist->Fill(SMHiggs->g4Val->getVal());
    g2iHist->Fill(SMHiggs->g2ValIm->getVal());
    g4iHist->Fill(SMHiggs->g4ValIm->getVal());

    delete myToy;

  }

  TCanvas* can = new TCanvas("can","can",800,800);
  can->Divide(2,2);
  can->cd(1);
  g2rHist->Draw();
  can->cd(2);
  g4rHist->Draw();
  can->cd(3);
  g2iHist->Draw();
  can->cd(4);
  g4iHist->Draw();

  can->SaveAs("SMHiggsToys_woIntegral.eps");
  can->SaveAs("SMHiggsToys_woIntegral.png");

}
