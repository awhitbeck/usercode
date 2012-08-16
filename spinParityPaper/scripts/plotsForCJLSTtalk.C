#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TChain.h"
#include "TCanvas.h"
#include <vector>
#include <algorithm>

using namespace RooFit;

void plotsForCJLSTtalk(int plotIndex=0, int binning=80){

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
  RooRealVar* z1mass = new RooRealVar("z1mass","m_{Z1}",40,110);
  RooRealVar* z2mass = new RooRealVar("z2mass","m_{Z2}",1e-09,65);
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

  RooRealVar* mzz = new RooRealVar("mzz","mzz",125,100,1000);

  TensorPdfFactory* MinGrav = new TensorPdfFactory(z1mass,z2mass,hs,h1,h2,Phi,Phi1,mzz);
  MinGrav->makeMinGrav();
  MinGrav->makeParamsConst(true);

  ScalarPdfFactory* SMHiggs = new ScalarPdfFactory(z1mass,z2mass,h1,h2,Phi,mzz);
  SMHiggs->makeSMHiggs();
  SMHiggs->makeParamsConst(true);

  TChain* treeMinGrav = new TChain("angles");
  treeMinGrav->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/minGrav_store/minGrav_125GeV.root");
  //treeMinGrav->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/minGrav_store/minGrav_125GeV_wResolution_withDiscriminants.root");
  if(treeMinGrav->GetEntries()<=0){ cout << "couldn't load minGrav data" << endl; return;}
  RooDataSet* dataSM = new RooDataSet("dataMinGrav","dataMinGrav",treeMinGrav,RooArgSet(*z1mass,*z2mass,*hs,*h1,*h2,*Phi,*Phi1));

  TChain* treeSMHiggs = new TChain("angles");
  treeSMHiggs->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/SMHiggs_store/SMHiggs_125GeV.root");
  //treeSMHiggs->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/SMHiggs_store/SMHiggs_125GeV_wResolution_withDiscriminants.root");
  if(treeSMHiggs->GetEntries()<=0){ cout << "couldn't load SMHiggs data" << endl; return;}
  RooDataSet* dataSMHiggs = new RooDataSet("dataSMHiggs","dataSMHiggs",treeSMHiggs,RooArgSet(*z1mass,*z2mass,*hs,*h1,*h2,*Phi,*Phi1));

  TChain* treeBkg = new TChain("angles");
  treeBkg->Add("/scratch0/hep/whitbeck/4lHelicity/datafiles/7TeV/training/pwgevents_*44.root");
  //treeBkg->Add("/scratch0/hep/whitbeck/4lHelicity/datafiles/7TeV/training/pwgevents_*_wResolution_withDiscriminants.root");
  if(treeBkg->GetEntries()<=0){ cout << "couldn't load Bkg data" << endl; return;}
  RooDataSet* dataBkg = new RooDataSet("dataBkg","dataBkg",treeBkg,RooArgSet(*z1mass,*z2mass,*hs,*h1,*h2,*Phi,*Phi1));

  RooPlot* plot = measureables[plotIndex]->frame(binning);
  cout << 1./dataMinGrav->sumEntries() << endl;
  double rescale=(double)(1./dataMinGrav->sumEntries());
  dataMinGrav->plotOn(plot,MarkerColor(kRed),MarkerStyle(4)); //,Rescale(14./6.));
  MinGrav->PDF->plotOn(plot,LineColor(kRed),LineWidth(2));
  cout << 1./dataSMHiggs->sumEntries() << endl;
  rescale=(double)(1./dataSMHiggs->sumEntries());
  dataSMHiggs->plotOn(plot,MarkerColor(kBlue),MarkerStyle(27)); //,Rescale(14./6.));
  SMHiggs->PDF->plotOn(plot,LineColor(kBlue),LineWidth(2),Normalization(1./(2.*3.1415)));
  cout << 1./dataBkg->sumEntries() << endl;
  rescale=(double)(1./dataBkg->sumEntries());
  //dataBkg->plotOn(plot,MarkerColor(kGreen+3),MarkerStyle(25)); //Rescale(6/14));

  TGaxis::SetMaxDigits(3);

  TCanvas* can =new TCanvas("can","can",600,600);
  plot->Draw();

  delete MinGrav;
  delete SMHiggs ;

  char temp[150];
  //sprintf(temp,"forCJLSTtalk/%s_125GeV_minGrav_vs_SMHiggs_vs_ZZ_wDetectorEffects.eps",measureables[plotIndex]->GetName());
  sprintf(temp,"forCJLSTtalk/%s_125GeV_minGrav_vs_SMHiggs_vs_ZZ.eps",measureables[plotIndex]->GetName());
  can->SaveAs(temp);
  //sprintf(temp,"forCJLSTtalk/%s_125GeV_minGrav_vs_SMHiggs_vs_ZZ_wDetectorEffects.png",measureables[plotIndex]->GetName());
  sprintf(temp,"forCJLSTtalk/%s_125GeV_minGrav_vs_SMHiggs_vs_ZZ.png",measureables[plotIndex]->GetName());
  can->SaveAs(temp);
}


void graviMELAdistribution(){

  TChain* treeMinGrav = new TChain("angles");
  treeMinGrav->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/minGrav_store/minGrav_125GeV_wResolution_withDiscriminants.root");
  if(treeMinGrav->GetEntries()<=0){ cout << "couldn't load minGrav data" << endl; return;}

  TChain* treeSMHiggs = new TChain("angles");
  treeSMHiggs->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/SMHiggs_store/SMHiggs_125GeV_wResolution_withDiscriminants.root");
  if(treeSMHiggs->GetEntries()<=0){ cout << "couldn't load SMHiggs data" << endl; return;}

  TChain* treeBkg = new TChain("angles");
  treeBkg->Add("/scratch0/hep/whitbeck/4lHelicity/datafiles/7TeV/training/pwgevents_*_wResolution_withDiscriminants.root");
  if(treeBkg->GetEntries()<=0){ cout << "couldn't load Bkg data" << endl; return;}

  TCanvas* can = new TCanvas("can","can",600,600);

  treeBkg->Draw("gravimelaLD>>bkg_histo(30,0,1)","zzmass>120&&zzmass<130");
  treeSMHiggs->Draw("gravimelaLD>>higgs_histo(30,0,1)","zzmass>120&&zzmass<130");
  treeMinGrav->Draw("gravimelaLD>>grav_histo(30,0,1)","zzmass>120&&zzmass<130");

  TH1F* grav_histo = (TH1F*) gDirectory->Get("grav_histo");
  grav_histo->SetLineColor(2);
  grav_histo->SetLineWidth(2);
  grav_histo->Scale(1/grav_histo->Integral());
  TH1F* higgs_histo = (TH1F*) gDirectory->Get("higgs_histo");
  higgs_histo->SetLineColor(4);
  higgs_histo->SetLineWidth(2);
  higgs_histo->Scale(1/higgs_histo->Integral());
  TH1F* bkg_histo = (TH1F*) gDirectory->Get("bkg_histo");
  bkg_histo->SetLineColor(kGreen+3);  
  bkg_histo->SetLineWidth(2);
  bkg_histo->Scale(1/bkg_histo->Integral());

  double yScale=max(max(bkg_histo->GetMaximum(),higgs_histo->GetMaximum()),grav_histo->GetMaximum())*1.3;

  grav_histo->GetYaxis()->SetRangeUser(0,yScale);
  grav_histo->GetXaxis()->SetTitle("gravi-MELA");
  grav_histo->GetYaxis()->SetTitle("normalized to unity");

  grav_histo->Draw();
  higgs_histo->Draw("SAME");
  bkg_histo->Draw("SAME");

  TLegend* leg = new TLegend(.6,.6,.9,.9);
  leg->SetFillColor(0);
  leg->AddEntry(bkg_histo,"qqZZ (POWHEG)","l");
  leg->AddEntry(higgs_histo,"SM Higgs (JHUgen)","l");
  leg->AddEntry(grav_histo,"min Grav (JHUgen)","l");

  leg->Draw();

  can->SaveAs("forCJLSTtalk/graviMELAdistribution.eps");
  can->SaveAs("forCJLSTtalk/graviMELAdistribution.png");

}






TGraph* makeROCcurve(char* drawVar="gravimelaLD", 
		     const int bins=30, float start=0, float end=1,
		     int lineColor=1, int lineStyle=1, int lineWidth=2){

  TChain* model_tree = new TChain("angles");
  model_tree->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/SMHiggs_store/SMHiggs_125GeV_wResolution_withDiscriminants.root");

  TChain* altModel_tree = new TChain("angles");
  altModel_tree->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/minGrav_store/minGrav_125GeV_wResolution_withDiscriminants.root");
  
  TH1F *model_histo, *altModel_histo;
  
  char drawString[150];

  sprintf(drawString,"%s>>model_histo(%i,%f,%f)",drawVar,bins,start,end);
  model_tree->Draw(drawString,"(zzmass>100)");
  sprintf(drawString,"%s>>altModel_histo(%i,%f,%f)",drawVar,bins,start,end);
  altModel_tree->Draw(drawString,"(zzmass>100)");
  
  model_histo = (TH1F*) gDirectory->Get("model_histo");
  model_histo->Scale(1/model_histo->Integral());
  altModel_histo = (TH1F*) gDirectory->Get("altModel_histo");
  altModel_histo->Scale(1/altModel_histo->Integral());

  double effmodel_[bins],effaltModel_[bins];

  for(int i=0; i<bins; i++){

    effmodel_[i] = model_histo->Integral(i+1, bins);
    effaltModel_[i] = altModel_histo->Integral(i+1, bins);

  }

  TGraph* ROC = new TGraph(bins,effmodel_,effaltModel_);
  ROC->SetLineColor(lineColor);
  ROC->SetLineStyle(lineStyle);
  ROC->SetLineWidth(lineWidth);
  ROC->GetXaxis()->SetTitle("#epsilon_{SM Higgs}");
  ROC->GetYaxis()->SetTitle("#epsilon_{min Grav}");
  delete model_tree;
  delete altModel_tree;

  return ROC;

}

/*
void graviMELArocCurve(){

  TCanvas* can = new TCanvas("can","can",600,600);

  TGraph* graviMELA = makeROCcurve("gravimelaLD",30,0,1,                 1,1);
  TGraph* z1mass = makeROCcurve("z1mass",100,60,120,                     2,2);
  TGraph* z2mass = makeROCcurve("-z2mass",100,-80,-12,                   3,3);
  TGraph* costheta1 = makeROCcurve("-abs(costheta1)",30,-1,0,            4,4);
  TGraph* costheta2 = makeROCcurve("-abs(costheta2)",30,-1,0,            6,6);
  TGraph* costhetastar= makeROCcurve("-abs(costhetastar)",30,-1,0,       7,7);
  TGraph* phi = makeROCcurve("abs(abs(phi)-3.1415/2.)",30,0,2,           2,1);
  TGraph* phistar1 = makeROCcurve("-abs(abs(phistar1)-3.1415/2.)",30,0,2,3,1);
  
  graviMELA->Draw("AC");
  z1mass->Draw("sameC");
  z2mass->Draw("sameC");
  costheta1->Draw("sameC");
  costheta2->Draw("sameC");
  costhetastar->Draw("sameC");
  phi->Draw("sameC");
  phistar1->Draw("sameC");

  TLegend* leg = new TLegend(.2,.65,.5,.9);
  leg->SetFillColor(0);
  
  leg->AddEntry(graviMELA,"gravi-MELA","l");
  leg->AddEntry(z1mass,"m_{Z1}","l");
  leg->AddEntry(z2mass,"m_{Z2}","l");
  leg->AddEntry(costheta1,"cos#theta_{1}","l");
  leg->AddEntry(costheta2,"cos#theta_{2}","l");
  leg->AddEntry(costhetastar,"cos#theta^{*}","l");
  leg->AddEntry(phi,"#Phi","l");
  leg->AddEntry(phistar1,"#Phi_{1}","l");

  leg->Draw();

  can->SaveAs("forCJLSTtalk/graviMELArocCurve.eps");
  can->SaveAs("forCJLSTtalk/graviMELArocCurve.png");

}

void plotHypSep(){
root [2] TTree* t = (TTree*) _file0->Get("hypTuple_em_wBkg")
  root [3] t->Draw("S_H0>>h0(20,-20,20)","S_H0<100")
  (Long64_t)997
root [4] t->Draw("S_H0>>h0(30,-20,20)","S_H0<100")
  (Long64_t)997
root [5] t->Draw("S_H1>>h1(30,-20,20)","S_H0<100")
  (Long64_t)997
root [6] h1->SetLineColor(4)
  root [7] h2->SetLineColor(2)
  Error: Symbol h2 is not defined in current scope  (tmpfile):1:
  Error: Failed to evaluate h2->SetLineColor(2)
*** Interpreter error recovered ***
  root [8] h0->SetLineColor(2)
    root [9] TCanvas* can = new TCanvas("can","can",600,600)
    root [10] h0->GetYaxis()->SetTitle("-2#times ln(L_{0}/L_{1})")
    root [11] h0->Draw()
    root [12] h1->Draw("SAME")
    root [13] h1->GetYaxis()->SetTitle("-2#times ln(L_{0}/L_{1})")
root [14] .L ~/tdrstyle.C
  root [15] setTDRStyle()
^[[A^[[A^[
root [16] 
root [16] 
root [16] TCanvas* can = new TCanvas("can","can",600,600)
root [17] h1->Draw()
root [18] h1->GetYaxis()->SetTitle(" ")
root [19] h1->GetXaxis()->SetTitle("-2#times ln(L_{0}/L_{1})")
root [20] h1->Draw()
root [21] h1->GetYaxis()->SetRangeUser(0,200)
root [22] h1->Draw()
root [23] h0->Draw("SAME")
root [24] gStyle->SetOptStat(0)
root [25] h1->Draw()
root [26] h0->Draw("SAME")
root [27] TLegend* leg = new TLegend(.7,.7,.9,.9)
root [28] leg->SetBorderSize(0)
root [29] leg->SetFillColor(0)
root [30] leg->AddEntry(h0,"SM Higgs","l")
(class TLegendEntry*)0x1901f6a0
root [31] leg->AddEntry(h1,"Min Grav","l")
(class TLegendEntry*)0x1904b440
root [32] leg->Draw()
root [33] can->SaveAs("SMHiggs_vs_minGrav_35fb_1ktoys_firstAttempt.eps")
Info in <TCanvas::Print>: eps file SMHiggs_vs_minGrav_35fb_1ktoys_firstAttempt.eps has been created
root [34] can->SaveAs("SMHiggs_vs_minGrav_35fb_1ktoys_firstAttempt.png")
Info in <TCanvas::Print>: file SMHiggs_vs_minGrav_35fb_1ktoys_firstAttempt.png has been created
root [35]  FCN=12.8281 FROM MIGRAD    STATUS=CONVERGED      62 CALLS          63 TOTAL
                     EDM=1.31335e-10    STRATEGY= 1      ERROR MATRIX ACCURATE 
  EXT PARAMETER                                   STEP         FIRST   
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
   1  Constant     1.34204e+02   5.43993e+00   7.76752e-03   5.65660e-08
   2  Mean         3.76471e+00   1.25200e-01   2.26514e-04  -1.16631e-04
   3  Sigma        3.90251e+00   9.82383e-02   1.18579e-05  -6.85221e-04

root [35] 
root [35] h2->Draw()
Error: Symbol h2 is not defined in current scope  (tmpfile):1:
Error: Failed to evaluate h2->Draw()
*** Interpreter error recovered ***
root [36] h0->Draw()
root [37]  FCN=956 FROM MIGRAD    STATUS=CONVERGED     127 CALLS         128 TOTAL
                     EDM=4.07362e-10    STRATEGY= 1  ERROR MATRIX UNCERTAINTY   0.6 per cent
  EXT PARAMETER                                   STEP         FIRST   
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
   1  Constant     1.86783e+02   3.76182e+01   9.46852e-04   6.29742e-07
   2  Mean         3.93557e+00   2.55586e-02   1.80458e-07  -1.19325e-04
   3  Sigma        3.12023e-01   1.90506e-02  -6.27412e-07   1.05004e-02

root [37] 
root [37] h1->Draw()
root [38] h0->Draw("SAME")
root [39] h0->Integral(16,30)
(const Double_t)1.67000000000000000e+02
root [40] h0->Integral(16,30)/h0->Integral()
(const double)1.67502507522567706e-01
root [41] h1->Integral(1,15)/h1->Integral()
(const double)1.53460381143430302e-01
root [42] M
MemInfo_t
Mult
Mult
MK_MSG
root [42] Math::
class "Math" not defined.
root [42] TMath::N
NextPrime
Na
NaUncertainty
Nint
Nint
Normalize
Normalize
NormQuantile
NormCross
NormCross
Normal2Plane
Normal2Plane
root [42] TMath::NormQuantile(.16)
(Double_t)(-9.94457883209752813e-01)
}
*/
