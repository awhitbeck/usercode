#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TChain.h"
#include "TCanvas.h"
#include <vector>
#include <algorithm>

using namespace RooFit;

void angularDistributions_bkg(int plotIndex=0, int binning=80){

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

  RooRealVar* mzz = new RooRealVar("zzmass","m_{4l}",125,110,140);

  RooRealVar* z1mass = new RooRealVar("z1mass","m_{1} [GeV]",40,110);
  RooRealVar* z2mass = new RooRealVar("z2mass","m_{2} [GeV]",0,65);
  RooRealVar* hs = new RooRealVar("costhetastar","cos#theta*",-1,1);
  RooRealVar* h1 = new RooRealVar("costheta1","cos#theta_{1}",-1,1);
  RooRealVar* h2 = new RooRealVar("costheta2","cos#theta_{2}",-1,1);
  RooRealVar* Phi = new RooRealVar("phi","#Phi",-TMath::Pi(),TMath::Pi());
  RooRealVar* Phi1 = new RooRealVar("phistar1","#Phi_{1}",-TMath::Pi(),TMath::Pi());
  RooRealVar*  l1m_pT = new RooRealVar("l1m_pT","l1m_pT",10,10000);
  RooRealVar*  l1p_pT = new RooRealVar("l1p_pT","l1p_pT",10,10000); 
  RooRealVar*  l2m_pT = new RooRealVar("l2m_pT","l2m_pT",10,10000);  
  RooRealVar*  l2p_pT = new RooRealVar("l2p_pT","l2p_pT",10,10000); 

  RooRealVar*  l1m_eta = new RooRealVar("l1m_eta","l1m_eta",-2.4,2.4);
  RooRealVar*  l1p_eta = new RooRealVar("l1p_eta","l1p_eta",-2.4,2.4); 
  RooRealVar*  l2m_eta = new RooRealVar("l2m_eta","l2m_eta",-2.4,2.4); 
  RooRealVar*  l2p_eta = new RooRealVar("l2p_eta","l2p_eta",-2.4,2.4);

  RooArgSet obs(*z1mass,*z2mass,*hs,*h1,*h2,*Phi,*Phi1);

  obs->add(*mzz);

  /*
    obs->add(*l1m_pT);
    obs->add(*l1p_pT);
    obs->add(*l2m_pT);
    obs->add(*l2p_pT);
    
    obs->add(*l1m_eta);
    obs->add(*l1p_eta);
    obs->add(*l2m_eta);
    obs->add(*l2p_eta);
  */

  vector<RooRealVar*> measureables;
  measureables.push_back(z1mass);
  measureables.push_back(z2mass);
  measureables.push_back(hs);
  measureables.push_back(h1);
  measureables.push_back(h2);
  measureables.push_back(Phi);
  measureables.push_back(Phi1);

  TChain* tree = new TChain("angles");
  tree->Add("PowhegFiles/4GeVcutPowheg/pwgevents_*9.root");
  RooDataSet* data = new RooDataSet("data","data",tree,obs,"z2mass>10");

  RooPlot* plot = measureables[plotIndex]->frame(binning);
  plot->GetXaxis()->CenterTitle();
  plot->GetYaxis()->CenterTitle();
  plot->GetYaxis()->SetTitle(" ");
  plot->GetXaxis()->SetNdivisions(-505);

  data->plotOn(plot,MarkerColor(kBlack),MarkerStyle(8),MarkerSize(1.5),XErrorSize(0),Rescale(.001));  

  TGaxis::SetMaxDigits(3);

  TCanvas* can =new TCanvas("can","can",600,600);

  gStyle->SetPadLeftMargin(0.05);

  char temp[150];
  sprintf(temp,"%s>>histo(%i,%i,%i)",measureables[plotIndex]->GetName(),binning,(int)measureables[plotIndex]->getMin(),(int)measureables[plotIndex]->getMin());
  tree->Draw(temp);
  TH1F* histo = (TH1F*) gDirectory->Get("histo");

  if(!strcmp(measureables[plotIndex]->GetName(),"z1mass"))
    plot->GetYaxis()->SetRangeUser(0,histo->GetMaximum()*3./100000.);
  else if(!strcmp(measureables[plotIndex]->GetName(),"z2mass"))
    plot->GetYaxis()->SetRangeUser(0,histo->GetMaximum()*3./1000000.);
  else
    plot->GetYaxis()->SetRangeUser(0,histo->GetMaximum()*3./100000.);
  
  cout << histo->GetMaximum()*1.3/1000. << endl;
 
  plot->Draw();

  char temp[150];
  sprintf(temp,"epsfiles/%s_125GeV_bkg.eps",measureables[plotIndex]->GetName());
  can->SaveAs(temp);
  sprintf(temp,"pngfiles/%s_125GeV_bkg.png",measureables[plotIndex]->GetName());
  can->SaveAs(temp);

}
