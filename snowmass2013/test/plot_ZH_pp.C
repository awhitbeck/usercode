#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif

#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
using namespace RooFit ;

bool weightedevents = true;
bool paperplots = false;

typedef enum{ZEROPLUS, ZEROMINUS, ZEROMIX, ZEROMIXPHASE, ALL} PlotLevel; 

void plot_ZH_pp() {
    
  gROOT->ProcessLine(".L tdrstyle.C");
  setTDRStyle();
  gStyle->SetPadLeftMargin(0.16);
  
  // Declaration of the PDFs to use
  gROOT->ProcessLine(".L ../src/RooSpinZero_3D_ZH_pp.cc++");
  gSystem->Load("../src/RooSpinZero_3D_ZH_pp_cc.so");
  
  // these values define the generator couplings
  bool withAcc = false;
  TString accName = "false";
  if ( withAcc ) 
    accName = "true";
  
  PlotLevel plot = ZEROPLUS;

  /*
  TString modeName_0p = "pp_ZH_llbb_0p_toydata";
  TString modeName_0m = "pp_ZH_llbb_0m_toydata";
  TString modeName_0mix= "pp_ZH_llbb_0mix_f3p5_toydata";
  TString modeName_0mixphase= "pp_ZH_llbb_0mix_f3p5_phi3piover2_toydata";
  TString plotAppendix= "pp_ZH_llbb_toydata";
  */

  TString modeName_0p = "pp_ZH_llbb_g1_1M";
  TString modeName_0m = "pp_ZH_llbb_g4_1M";
  TString modeName_0mix= "pp_ZH_llbb_g1_p_g4_1M";
  TString modeName_0mixphase= "pp_ZH_llbb_g1_p_ig4_1M";
  TString plotAppendix= "pp_ZH_llbb";



  if ( plot == ZEROPLUS  )  plotAppendix = modeName_0p;
  if ( plot == ZEROMINUS ) plotAppendix = modeName_0m;
  if ( plot == ZEROMIX   )   plotAppendix = modeName_0mix;
  if ( plot == ZEROMIXPHASE   )   plotAppendix = modeName_0mixphase;
  
  
  //
  // Signal files
  // 
  
  // generated values
  double g1Gen    = 1.;
  double g1ImGen  = 0.;
  double g2Gen    = 0.;
  double g2ImGen  = 0.;
  double g3Gen    = 0.;
  double g3ImGen  = 0.;
  double g4Gen    = 0.;
  double g4ImGen  = 0.;
  double fa2Gen   = 0.;
  double phia2Gen = 0.;
  double fa3Gen   = 0.;
  double phia3Gen = 0.;
  
  // Observables (5D)
  RooRealVar* h1 = new RooRealVar("costheta1","cos#theta_{1}",-1,1);
  RooRealVar* h2 = new RooRealVar("costheta2","cos#theta_{2}",-1,1);
  RooRealVar* Phi = new RooRealVar("phi","#Phi",-TMath::Pi(),TMath::Pi());
  RooRealVar* hs = new RooRealVar("costhetastar","cos#theta^{*}",-1,1);
  RooRealVar* Phi1 = new RooRealVar("phistar1","#Phi_{1}",-TMath::Pi(),TMath::Pi());
  RooRealVar* m= new RooRealVar("m","m_{VH} [GeV]", 200, 1000);
  RooRealVar* Y= new RooRealVar("Y","Rapidity (VH)", -4, 4);
  
  // additional variables
  // event weight
  RooRealVar* wt = new RooRealVar("wt", "wt", 0.0, 50);
  // Parameters
  RooRealVar* sqrts= new RooRealVar("sqrts","sqrts", 14000.);
  RooRealVar* mX = new RooRealVar("mX","mX", 125.);
  RooRealVar* mZ = new RooRealVar("mZ","mZ", 91.1876);
  RooRealVar* gamZ = new RooRealVar("gamZ","gamZ",2.4952);
  RooRealVar* R1Val = new RooRealVar("R1Val","R1Val", 0.);
  //RooRealVar* R1Val = new RooRealVar("R1Val","R1Val", 0.67);  // for uu
  //RooRealVar* R1Val = new RooRealVar("R1Val","R1Val", 0.94);     // for dd
  RooRealVar* R2Val = new RooRealVar("R2Val","R2Val", 0.15);
  
  // amplitude parameters
  int para = 1;
  
  RooRealVar* a1Val  = new RooRealVar("a1Val","a1Val",0.);
  RooRealVar* phi1Val= new RooRealVar("phi1Val","phi1Val",0.);
  RooRealVar* a2Val  = new RooRealVar("a2Val","a2Val",0.);
  RooRealVar* phi2Val= new RooRealVar("phi2Val","phi2Val",0.);
  RooRealVar* a3Val  = new RooRealVar("a3Val","a3Val",0.);
  RooRealVar* phi3Val= new RooRealVar("phi3Val","phi3Val",0.);
  
  RooRealVar* g1Val  = new RooRealVar("g1Val","g1Val",g1Gen);
  RooRealVar* g2Val  = new RooRealVar("g2Val","g2Val",g2Gen);
  RooRealVar* g3Val  = new RooRealVar("g3Val","g3Val",g3Gen);
  RooRealVar* g4Val  = new RooRealVar("g4Val","g4Val",g4Gen);
  
  RooRealVar* g1ValIm  = new RooRealVar("g1ValIm","g1ValIm",g1ImGen);
  RooRealVar* g2ValIm  = new RooRealVar("g2ValIm","g2ValIm",g2ImGen);
  RooRealVar* g3ValIm  = new RooRealVar("g3ValIm","g3ValIm",g3ImGen);
  RooRealVar* g4ValIm  = new RooRealVar("g4ValIm","g4ValIm",g4ImGen);
  
  RooRealVar* fa2  = new RooRealVar("fa2","f_{g2}",fa2Gen,0.,1.0);
  RooRealVar* fa3  = new RooRealVar("fa3","f_{g4}",fa3Gen,0.,1.0);
  RooRealVar* phia2  = new RooRealVar("phia2","#phi_{g2}",phia2Gen,-2.*TMath::Pi(),2*TMath::Pi());
  RooRealVar* phia3  = new RooRealVar("phia3","#phi_{g4}",phia3Gen,-2.*TMath::Pi(),2*TMath::Pi());
  
  
  // set the PDF
  RooSpinZero_3D_ZH_pp *sigPdf_pp = new RooSpinZero_3D_ZH_pp("sigPdf_pp","sigPdf_pp",
							     *h1,*h2,*Phi, *m, *Y,
							     *sqrts, *mX, *mZ, *R1Val, *R2Val, para, 
							     *a1Val, *phi1Val, *a2Val, *phi2Val,*a3Val, *phi3Val, 
							     *g1Val, *g2Val, *g3Val, *g4Val, *g1ValIm, *g2ValIm, *g3ValIm, *g4ValIm,
							     *fa2, *fa3, *phia2, *phia3, withAcc);
  //
  // set the PDF for 0-
  //
  RooRealVar* g1Val0m  = new RooRealVar("g1Val0m","g1Val0m",0.);
  RooRealVar* g2Val0m  = new RooRealVar("g2Val0m","g2Val0m",0.);
  RooRealVar* g3Val0m  = new RooRealVar("g3Val0m","g3Val0m",0.);
  RooRealVar* g4Val0m  = new RooRealVar("g4Val0m","g4Val0m",1.);
  
  RooRealVar* g1Val0mIm  = new RooRealVar("g1Val0mIm","g1Val0mIm", 0.);
  RooRealVar* g2Val0mIm  = new RooRealVar("g2Val0mIm","g2Val0mIm", 0.);
  RooRealVar* g3Val0mIm  = new RooRealVar("g3Val0mIm","g3Val0mIm", 0.);
  RooRealVar* g4Val0mIm  = new RooRealVar("g4Val0mIm","g4Val0mIm", 0.);


  
  
  RooSpinZero_3D_ZH_pp *sigPdf_pp0m = new RooSpinZero_3D_ZH_pp("sigPdf_pp","sigPdf_pp",
							       *h1,*h2,*Phi, *m, *Y,
							       *sqrts, *mX, *mZ, *R1Val, *R2Val, para, 
							       *a1Val, *phi1Val, *a2Val, *phi2Val,*a3Val, *phi3Val, 
							       *g1Val0m, *g2Val0m, *g3Val0m, *g4Val0m, *g1Val0mIm, *g2Val0mIm, *g3Val0mIm, *g4Val0mIm,
							       *fa2, *fa3, *phia2, *phia3, withAcc);
  //
  // set the PDF for 0 mix
  //
  RooRealVar* g1Val0mix  = new RooRealVar("g1Val0mix","g1Val0mix",1.);
  RooRealVar* g2Val0mix  = new RooRealVar("g2Val0mix","g2Val0mix",0.);
  RooRealVar* g3Val0mix  = new RooRealVar("g3Val0mix","g3Val0mix",0.);
  RooRealVar* g4Val0mix  = new RooRealVar("g4Val0mix","g4Val0mix",0.144708);//0.144639);
  
  RooRealVar* g1Val0mixIm  = new RooRealVar("g1Val0mixIm","g1Val0mixIm", 0.);
  RooRealVar* g2Val0mixIm  = new RooRealVar("g2Val0mixIm","g2Val0mixIm", 0.);
  RooRealVar* g3Val0mixIm  = new RooRealVar("g3Val0mixIm","g3Val0mixIm", 0.);
  RooRealVar* g4Val0mixIm  = new RooRealVar("g4Val0mixIm","g4Val0mixIm", 0.);
  
  
  RooSpinZero_3D_ZH_pp *sigPdf_pp0mix = new RooSpinZero_3D_ZH_pp("sigPdf_pp0mix","sigPdf_pp0mix",
								 *h1,*h2,*Phi, *m, *Y,
								 *sqrts, *mX, *mZ, *R1Val, *R2Val, para, 
								 *a1Val, *phi1Val, *a2Val, *phi2Val,*a3Val, *phi3Val, 
								 *g1Val0mix, *g2Val0mix, *g3Val0mix, *g4Val0mix, 
								 *g1Val0mixIm, *g2Val0mixIm, *g3Val0mixIm, *g4Val0mixIm,
								 *fa2, *fa3, *phia2, *phia3, withAcc);
  
  //
  // set the PDF for 0 mix with phase
  //
  RooRealVar* g1Val0mixphase  = new RooRealVar("g1Val0mixphase","g1Val0mixphase",1.);
  RooRealVar* g2Val0mixphase  = new RooRealVar("g2Val0mixphase","g2Val0mixphase",0.);
  RooRealVar* g3Val0mixphase  = new RooRealVar("g3Val0mixphase","g3Val0mixphase",0.);
  RooRealVar* g4Val0mixphase  = new RooRealVar("g4Val0mixphase","g4Val0mixphase",0.);
  
  RooRealVar* g1Val0mixphaseIm  = new RooRealVar("g1Val0mixphaseIm","g1Val0mixphaseIm", 0.);
  RooRealVar* g2Val0mixphaseIm  = new RooRealVar("g2Val0mixphaseIm","g2Val0mixphaseIm", 0.);
  RooRealVar* g3Val0mixphaseIm  = new RooRealVar("g3Val0mixphaseIm","g3Val0mixphaseIm", 0.);
  RooRealVar* g4Val0mixphaseIm  = new RooRealVar("g4Val0mixphaseIm","g4Val0mixphaseIm", 0.144708);
  
  
  RooSpinZero_3D_ZH_pp *sigPdf_pp0mixphase = new RooSpinZero_3D_ZH_pp("sigPdf_pp0mixphase","sigPdf_pp0mixphase",
								 *h1,*h2,*Phi, *m, *Y,
								 *sqrts, *mX, *mZ, *R1Val, *R2Val, para, 
								 *a1Val, *phi1Val, *a2Val, *phi2Val,*a3Val, *phi3Val, 
								 *g1Val0mixphase, *g2Val0mixphase, *g3Val0mixphase, *g4Val0mixphase, 
								 *g1Val0mixphaseIm, *g2Val0mixphaseIm, *g3Val0mixphaseIm, *g4Val0mixphaseIm,
								 *fa2, *fa3, *phia2, *phia3, withAcc);

  // 
  // Read 0+ input file
  // 
  TFile *fin_signal = new TFile(Form("samples/pp_ZH/%s_%s.root", modeName_0p.Data(),accName.Data()));
  TTree* tin_signal = (TTree*) fin_signal->Get("SelectedTree");
  if ( weightedevents ) {
    RooDataSet dataTMP("dataTMP","dataTMP", tin_signal, RooArgSet(*h1,*h2,*Phi,*m,*Y, *wt));
    RooDataSet sigData_pp0p = RooDataSet("sigData_pp0p","sigData_pp0p",RooArgList(*h1,*h2,*Phi,*m, *Y, *wt), WeightVar("wt"), Import(dataTMP));
  } else {
    RooDataSet sigData_pp0p = RooDataSet("sigData_pp0p","sigData_pp0p",RooArgSet(*h1,*h2,*Phi,*m,*Y),Import(*tin_signal));    
  }
  
  
  // 
  // Read 0- input file
  // 
  TFile *fin_signal_0m = new TFile(Form("samples/pp_ZH/%s_%s.root", modeName_0m.Data(),accName.Data()));
  TTree* tin_signal_0m = (TTree*) fin_signal_0m->Get("SelectedTree");
  if ( weightedevents ) {
    RooDataSet dataTMP("dataTMP","dataTMP", tin_signal_0m, RooArgSet(*h1,*h2,*Phi,*m,*Y, *wt));
    RooDataSet sigData_pp0m = RooDataSet("sigData_pp0m","sigData_pp0m",RooArgList(*h1,*h2,*Phi,*m, *Y, *wt), WeightVar("wt"), Import(dataTMP));
  } else {
    RooDataSet sigData_pp0m = RooDataSet("sigData_pp0m","sigData_pp0m",RooArgSet(*h1,*h2,*Phi,*m,*Y),Import(*tin_signal_0m));    
  }
  
  
  // 
  // Read 0 mix input file
  // 
  TFile *fin_signal_0mix = new TFile(Form("samples/pp_ZH/%s_%s.root", modeName_0mix.Data(),accName.Data()));
  TTree* tin_signal_0mix = (TTree*) fin_signal_0mix->Get("SelectedTree");
  if ( weightedevents ) {
    RooDataSet dataTMP("dataTMP","dataTMP", tin_signal_0mix, RooArgSet(*h1,*h2,*Phi,*m,*Y, *wt));
    RooDataSet sigData_pp0mix = RooDataSet("sigData_pp0mix","sigData_pp0mix",RooArgList(*h1,*h2,*Phi,*m, *Y, *wt), WeightVar("wt"), Import(dataTMP));
  } else {
    RooDataSet sigData_pp0mix = RooDataSet("sigData_pp0mix","sigData_pp0mix",RooArgSet(*h1,*h2,*Phi,*m,*Y),Import(*tin_signal_0mix));    
  }

  // 
  // Read 0 mixphase input file
  // 
  TFile *fin_signal_0mixphase = new TFile(Form("samples/pp_ZH/%s_%s.root", modeName_0mixphase.Data(),accName.Data()));
  TTree* tin_signal_0mixphase = (TTree*) fin_signal_0mixphase->Get("SelectedTree");
  if ( weightedevents ) {
    RooDataSet dataTMP("dataTMP","dataTMP", tin_signal_0mixphase, RooArgSet(*h1,*h2,*Phi,*m,*Y, *wt));
    RooDataSet sigData_pp0mixphase = RooDataSet("sigData_pp0mixphase","sigData_pp0mixphase",RooArgList(*h1,*h2,*Phi,*m, *Y, *wt), WeightVar("wt"), Import(dataTMP));
  } else {
    RooDataSet sigData_pp0mixphase = RooDataSet("sigData_pp0mixphase","sigData_pp0mixphase",RooArgSet(*h1,*h2,*Phi,*m,*Y),Import(*tin_signal_0mixphase));    
  }


  // 
  //  h1 frame
  // 

  int nbins_h1 = 20;
  TH1F *h1_test_0m = new TH1F("h1_test_0m", "h1_test_0m", nbins_h1, -1, 1);
  tin_signal_0m->Project("h1_test_0m", "costheta1","wt");
  double rescale_h1_0m = 1./h1_test_0m->Integral();
  h1_test_0m->Scale(rescale_h1_0m);
  double ymax_h1 = h1_test_0m->GetMaximum();


  TH1F *h1_test_0p = new TH1F("h1_test_0p", "h1_test_0p", nbins_h1, -1, 1);
  tin_signal->Project("h1_test_0p", "costheta1","wt");
  double rescale_h1_0p = 1./h1_test_0p->Integral();
  h1_test_0p->Scale(rescale_h1_0p);
  ymax_h1 = h1_test_0p->GetMaximum() > ymax_h1 ? h1_test_0p->GetMaximum() : ymax_h1;

  TH1F *h1_test_0mix = new TH1F("h1_test_0mix", "h1_test_0mix", nbins_h1, -1, 1);
  tin_signal_0mix->Project("h1_test_0mix", "costheta1","wt");
  double rescale_h1_0mix = 1./h1_test_0mix->Integral();
  h1_test_0mix->Scale(rescale_h1_0mix);
  ymax_h1 = h1_test_0mix->GetMaximum() > ymax_h1 ? h1_test_0mix->GetMaximum() : ymax_h1;


  TH1F *h1_test_0mixphase = new TH1F("h1_test_0mixphase", "h1_test_0mixphase", nbins_h1, -1, 1);
  tin_signal_0mixphase->Project("h1_test_0mixphase", "costheta1","wt");
  double rescale_h1_0mixphase = 1./h1_test_0mixphase->Integral();
  h1_test_0mixphase->Scale(rescale_h1_0mixphase);
  ymax_h1 = h1_test_0mixphase->GetMaximum() > ymax_h1 ? h1_test_0mixphase->GetMaximum() : ymax_h1;
  


  RooPlot* h1frame =  h1->frame(nbins_h1);
  h1frame->GetXaxis()->CenterTitle();
  h1frame->GetYaxis()->CenterTitle();
  h1frame->GetYaxis()->SetTitle(" ");

  if ( plot == ZEROPLUS || plot == ALL ) {
    sigData_pp0p.plotOn(h1frame, MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_h1_0p));
    sigPdf_pp->plotOn(h1frame, LineColor(kRed),LineWidth(2),Normalization(rescale_h1_0p));
  }
  if ( plot == ZEROMINUS || plot == ALL ) {
    sigData_pp0m.plotOn(h1frame, MarkerColor(kBlue),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_h1_0m));
    sigPdf_pp0m->plotOn(h1frame, LineColor(kBlue),LineWidth(2), Normalization(rescale_h1_0m));
  }
  
  if ( plot == ZEROMIX || plot == ALL) {
    sigData_pp0mix.plotOn(h1frame, MarkerColor(kGreen+3),MarkerStyle(25),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_h1_0mix));
    sigPdf_pp0mix->plotOn(h1frame, LineColor(kGreen+3),LineWidth(2), Normalization(rescale_h1_0mix));
  }

  if ( plot == ZEROMIXPHASE || plot == ALL) {
    sigData_pp0mixphase.plotOn(h1frame, MarkerColor(kMagenta+1),MarkerStyle(20),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_h1_0mixphase));
    sigPdf_pp0mixphase->plotOn(h1frame, LineColor(kMagenta+1),LineWidth(2), Normalization(rescale_h1_0mixphase));
  }

  h1frame->SetMaximum(ymax_h1*1.2);  

  
  //
  // draw h2
  // 
  
  int nbins_h2 = 20;
  TH1F *h2_test_0m = new TH1F("h2_test_0m", "h2_test_0m", nbins_h2, -1, 1);
  tin_signal_0m->Project("h2_test_0m", "costheta2","wt");
  double rescale_h2_0m = 1./h2_test_0m->Integral();
  h2_test_0m->Scale(rescale_h2_0m);
  double ymax_h2 = h2_test_0m->GetMaximum();


  TH1F *h2_test_0p = new TH1F("h2_test_0p", "h2_test_0p", nbins_h2, -1, 1);
  tin_signal->Project("h2_test_0p", "costheta2","wt");
  double rescale_h2_0p = 1./h2_test_0p->Integral();
  h2_test_0p->Scale(rescale_h2_0p);
  ymax_h2 = h2_test_0p->GetMaximum() > ymax_h2 ? h2_test_0p->GetMaximum() : ymax_h2;

  TH1F *h2_test_0mix = new TH1F("h2_test_0mix", "h2_test_0mix", nbins_h2, -1, 1);
  tin_signal_0mix->Project("h2_test_0mix", "costheta2","wt");
  double rescale_h2_0mix = 1./h2_test_0mix->Integral();
  h2_test_0mix->Scale(rescale_h2_0mix);
  ymax_h2 = h2_test_0mix->GetMaximum() > ymax_h2 ? h2_test_0mix->GetMaximum() : ymax_h2;


  TH1F *h2_test_0mixphase = new TH1F("h2_test_0mixphase", "h2_test_0mixphase", nbins_h2, -1, 1);
  tin_signal_0mixphase->Project("h2_test_0mixphase", "costheta2","wt");
  double rescale_h2_0mixphase = 1./h2_test_0mixphase->Integral();
  h2_test_0mixphase->Scale(rescale_h2_0mixphase);
  ymax_h2 = h2_test_0mixphase->GetMaximum() > ymax_h2 ? h2_test_0mixphase->GetMaximum() : ymax_h2;
  


  RooPlot* h2frame =  h2->frame(20);
  h2frame->GetXaxis()->CenterTitle();
  h2frame->GetYaxis()->CenterTitle();
  h2frame->GetYaxis()->SetTitle(" ");

  
  if ( plot == ZEROPLUS || plot == ALL ) {
    sigData_pp0p.plotOn(h2frame, MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_h2_0p));
    sigPdf_pp->plotOn(h2frame, LineColor(kRed),LineWidth(2), Normalization(rescale_h2_0p));
  }
  if ( plot == ZEROMINUS || plot == ALL ) {
    sigData_pp0m.plotOn(h2frame, MarkerColor(kBlue),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_h2_0m));
    sigPdf_pp0m->plotOn(h2frame, LineColor(kBlue),LineWidth(2), Normalization(rescale_h2_0m));
  }
  if ( plot == ZEROMIX || plot == ALL) {
    sigData_pp0mix.plotOn(h2frame, MarkerColor(kGreen+3),MarkerStyle(25),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_h2_0mix));
    sigPdf_pp0mix->plotOn(h2frame, LineColor(kGreen+3),LineWidth(2), Normalization(rescale_h2_0mix));
  }

  if ( plot == ZEROMIXPHASE || plot == ALL) {
    sigData_pp0mixphase.plotOn(h2frame, MarkerColor(kMagenta+1),MarkerStyle(20),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_h2_0mixphase));
    sigPdf_pp0mixphase->plotOn(h2frame, LineColor(kMagenta+1),LineWidth(2), Normalization(rescale_h2_0mixphase));
  }

  h2frame->SetMaximum(ymax_h2) * 1.2;

  


  //
  // draw phi
  // 
  
  int nbins_phi = 20;
  TH1F *phi_test_0m = new TH1F("phi_test_0m", "phi_test_0m", nbins_phi, -TMath::Pi(), TMath::Pi());
  tin_signal_0m->Project("phi_test_0m", "phi","wt");
  double rescale_phi_0m = 1./phi_test_0m->Integral();
  phi_test_0m->Scale(rescale_phi_0m);
  double ymax_phi = phi_test_0m->GetMaximum();


  TH1F *phi_test_0p = new TH1F("phi_test_0p", "phi_test_0p", nbins_phi, -TMath::Pi(), TMath::Pi());
  tin_signal->Project("phi_test_0p", "phi","wt");
  double rescale_phi_0p = 1./phi_test_0p->Integral();
  phi_test_0p->Scale(rescale_phi_0p);
  ymax_phi = phi_test_0p->GetMaximum() > ymax_phi ? phi_test_0p->GetMaximum() : ymax_phi;

  TH1F *phi_test_0mix = new TH1F("phi_test_0mix", "phi_test_0mix", nbins_phi, -TMath::Pi(), TMath::Pi());
  tin_signal_0mix->Project("phi_test_0mix", "phi","wt");
  double rescale_phi_0mix = 1./phi_test_0mix->Integral();
  phi_test_0mix->Scale(rescale_phi_0mix);
  ymax_phi = phi_test_0mix->GetMaximum() > ymax_phi ? phi_test_0mix->GetMaximum() : ymax_phi;


  TH1F *phi_test_0mixphase = new TH1F("phi_test_0mixphase", "phi_test_0mixphase", nbins_phi, -TMath::Pi(), TMath::Pi());
  tin_signal_0mixphase->Project("phi_test_0mixphase", "phi","wt");
  double rescale_phi_0mixphase = 1./phi_test_0mixphase->Integral();
  phi_test_0mixphase->Scale(rescale_phi_0mixphase);
  ymax_phi = phi_test_0mixphase->GetMaximum() > ymax_phi ? phi_test_0mixphase->GetMaximum() : ymax_phi;
  
  RooPlot* phiframe =  Phi->frame(20);
  phiframe->GetXaxis()->CenterTitle();
  phiframe->GetYaxis()->CenterTitle();
  phiframe->GetYaxis()->SetTitle(" ");
 
  
  if ( plot == ZEROPLUS || plot == ALL ) {
    sigData_pp0p.plotOn(phiframe, MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_phi_0p));
    sigPdf_pp->plotOn(phiframe, LineColor(kRed),LineWidth(2), Normalization(rescale_phi_0p));
  }
  if ( plot == ZEROMINUS || plot == ALL ) {
    sigData_pp0m.plotOn(phiframe, MarkerColor(kBlue),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_phi_0m));
    sigPdf_pp0m->plotOn(phiframe, LineColor(kBlue),LineWidth(2), Normalization(rescale_phi_0m));
  }
  
  if ( plot == ZEROMIX || plot == ALL) {
    sigData_pp0mix.plotOn(phiframe, MarkerColor(kGreen+3),MarkerStyle(25),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_phi_0mix));
    sigPdf_pp0mix->plotOn(phiframe, LineColor(kGreen+3),LineWidth(2), Normalization(rescale_phi_0mix));
  }

  if ( plot == ZEROMIXPHASE || plot == ALL) {
    sigData_pp0mixphase.plotOn(phiframe, MarkerColor(kMagenta+1),MarkerStyle(20),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_phi_0mixphase));
    sigPdf_pp0mixphase->plotOn(phiframe, LineColor(kMagenta+1),LineWidth(2), Normalization(rescale_phi_0mixphase));
  }
  
  phiframe->SetMaximum(ymax_phi*1.2);
  
  //
  // draw m
  // 
  
  int nbins_m = 20;
  TH1F *m_test_0m = new TH1F("m_test_0m", "m_test_0m", nbins_m, 200, 1000.);
  tin_signal_0m->Project("m_test_0m", "m","wt");
  double rescale_m_0m = 1./m_test_0m->Integral();
  m_test_0m->Scale(rescale_m_0m);
  double ymax_m = m_test_0m->GetMaximum();


  TH1F *m_test_0p = new TH1F("m_test_0p", "m_test_0p", nbins_m, 200, 1000.);
  tin_signal->Project("m_test_0p", "m","wt");
  double rescale_m_0p = 1./m_test_0p->Integral();
  m_test_0p->Scale(rescale_m_0p);
  ymax_m = m_test_0p->GetMaximum() > ymax_m ? m_test_0p->GetMaximum() : ymax_m;

  TH1F *m_test_0mix = new TH1F("m_test_0mix", "m_test_0mix", nbins_m, 200, 1000.);
  tin_signal_0mix->Project("m_test_0mix", "m","wt");
  double rescale_m_0mix = 1./m_test_0mix->Integral();
  m_test_0mix->Scale(rescale_m_0mix);
  ymax_m = m_test_0mix->GetMaximum() > ymax_m ? m_test_0mix->GetMaximum() : ymax_m;


  TH1F *m_test_0mixphase = new TH1F("m_test_0mixphase", "m_test_0mixphase", nbins_m, 200, 1000.);
  tin_signal_0mixphase->Project("m_test_0mixphase", "m","wt");
  double rescale_m_0mixphase = 1./m_test_0mixphase->Integral();
  m_test_0mixphase->Scale(rescale_m_0mixphase);
  ymax_m = m_test_0mixphase->GetMaximum() > ymax_m ? m_test_0mixphase->GetMaximum() : ymax_m;

  RooPlot* mframe =  m->frame(40);
  mframe->GetXaxis()->CenterTitle();
  mframe->GetYaxis()->CenterTitle();
  mframe->GetYaxis()->SetTitle(" ");


  if ( plot == ZEROPLUS || plot == ALL ) {
    sigData_pp0p.plotOn(mframe, MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_m_0p));
    sigPdf_pp->plotOn(mframe, LineColor(kRed),LineWidth(2), Normalization(rescale_m_0p));
  }
  if ( plot == ZEROMINUS || plot == ALL ) {
    sigData_pp0m.plotOn(mframe, MarkerColor(kBlue),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_m_0m));
    sigPdf_pp0m->plotOn(mframe, LineColor(kBlue),LineWidth(2), Normalization(rescale_m_0m));
  }
  if ( plot == ZEROMIX || plot == ALL) {
    sigData_pp0mix.plotOn(mframe, MarkerColor(kGreen+3),MarkerStyle(25),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_m_0mix));
    sigPdf_pp0mix->plotOn(mframe, LineColor(kGreen+3),LineWidth(2), Normalization(rescale_m_0mix));
  }

  if ( plot == ZEROMIXPHASE || plot == ALL) {
    sigData_pp0mixphase.plotOn(mframe, MarkerColor(kMagenta+1),MarkerStyle(20),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_m_0mixphase));
    sigPdf_pp0mixphase->plotOn(mframe, LineColor(kMagenta+1),LineWidth(2), Normalization(rescale_m_0mixphase));
  }
  mframe->SetMaximum(ymax_m*0.7);


  //
  // draw Y
  // 
  
  int nbins_Y = 20;
  TH1F *Y_test_0m = new TH1F("Y_test_0m", "Y_test_0m", nbins_Y, -4, 4);
  tin_signal_0m->Project("Y_test_0m", "Y","wt");
  double rescale_Y_0m = 1./Y_test_0m->Integral();
  Y_test_0m->Scale(rescale_Y_0m);
  double ymax_Y = Y_test_0m->GetMaximum();


  TH1F *Y_test_0p = new TH1F("Y_test_0p", "Y_test_0p", nbins_Y, -4, 4);
  tin_signal->Project("Y_test_0p", "Y","wt");
  double rescale_Y_0p = 1./Y_test_0p->Integral();
  Y_test_0p->Scale(rescale_Y_0p);
  ymax_Y = Y_test_0p->GetMaximum() > ymax_Y ? Y_test_0p->GetMaximum() : ymax_Y;

  TH1F *Y_test_0mix = new TH1F("Y_test_0mix", "Y_test_0mix", nbins_Y, -4, 4);
  tin_signal_0mix->Project("Y_test_0mix", "Y","wt");
  double rescale_Y_0mix = 1./Y_test_0mix->Integral();
  Y_test_0mix->Scale(rescale_Y_0mix);
  ymax_Y = Y_test_0mix->GetMaximum() > ymax_Y ? Y_test_0mix->GetMaximum() : ymax_Y;


  TH1F *Y_test_0mixphase = new TH1F("Y_test_0mixphase", "Y_test_0mixphase", nbins_Y, -4, 4);
  tin_signal_0mixphase->Project("Y_test_0mixphase", "Y","wt");
  double rescale_Y_0mixphase = 1./Y_test_0mixphase->Integral();
  Y_test_0mixphase->Scale(rescale_Y_0mixphase);
  ymax_Y = Y_test_0mixphase->GetMaximum() > ymax_Y ? Y_test_0mixphase->GetMaximum() : ymax_Y;
  
  RooPlot* Yframe =  Y->frame(20);
  Yframe->GetXaxis()->CenterTitle();
  Yframe->GetYaxis()->CenterTitle();
  Yframe->GetYaxis()->SetTitle(" ");
  
  if ( plot == ZEROPLUS || plot == ALL ) {
    sigData_pp0p.plotOn(Yframe, MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_Y_0p));
    sigPdf_pp->plotOn(Yframe, LineColor(kRed),LineWidth(2), Normalization(rescale_Y_0p));
  }
  if ( plot == ZEROMINUS || plot == ALL ) {
    sigData_pp0m.plotOn(Yframe, MarkerColor(kBlue),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_Y_0m));
    sigPdf_pp0m->plotOn(Yframe, LineColor(kBlue),LineWidth(2), Normalization(rescale_Y_0m));
  }
  if ( plot == ZEROMIX || plot == ALL) {
    sigData_pp0mix.plotOn(Yframe, MarkerColor(kGreen+3),MarkerStyle(25),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_Y_0mix));
    sigPdf_pp0mix->plotOn(Yframe, LineColor(kGreen+3),LineWidth(2), Normalization(rescale_Y_0mix));
  }

  if ( plot == ZEROMIXPHASE || plot == ALL) {
    sigData_pp0mixphase.plotOn(Yframe, MarkerColor(kMagenta+1),MarkerStyle(20),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_Y_0mixphase));
    sigPdf_pp0mixphase->plotOn(Yframe, LineColor(kMagenta+1),LineWidth(2), Normalization(rescale_Y_0mixphase));
  }
  Yframe->SetMaximum(ymax_Y*1.1);
  

  if ( paperplots ) {
    TCanvas* czz = new TCanvas( "czz", "czz", 600, 600); 

    h1frame->Draw();
    czz->SaveAs(Form("paperplots/h1_ppzh_%s_acc%s.eps", plotAppendix.Data(), accName.Data()));
    czz->SaveAs(Form("paperplots/h1_ppzh_%s_acc%s.eps", plotAppendix.Data(), accName.Data()));


    h2frame->Draw();
    czz->SaveAs(Form("paperplots/h2_ppzh_%s_acc%s.eps", plotAppendix.Data(), accName.Data()));
    czz->SaveAs(Form("paperplots/h2_ppzh_%s_acc%s.png", plotAppendix.Data(), accName.Data()));
    
    phiframe->Draw();
    czz->SaveAs(Form("paperplots/phi_ppzh_%s_acc%s.eps", plotAppendix.Data(), accName.Data()));
    czz->SaveAs(Form("paperplots/phi_ppzh_%s_acc%s.png", plotAppendix.Data(), accName.Data()));
  
    mframe->Draw();
    czz->SaveAs(Form("paperplots/mvh_ppzh_%s_acc%s.eps", plotAppendix.Data(), accName.Data()));
    czz->SaveAs(Form("paperplots/mvh_ppzh_%s_acc%s.png", plotAppendix.Data(), accName.Data()));

    Yframe->Draw();
    czz->SaveAs(Form("paperplots/Yvh_ppzh_%s_acc%s.eps", plotAppendix.Data(), accName.Data()));
    czz->SaveAs(Form("paperplots/Yvh_ppzh_%s_acc%s.png", plotAppendix.Data(), accName.Data()));

  }  else {
      TCanvas* czz = new TCanvas( "czz", "czz", 1200, 800 );
      czz->Divide(3,2);
      
      czz->cd(1);
      h1frame->Draw();
      
      czz->cd(2);
      h2frame->Draw();
      
      czz->cd(3);
      phiframe->Draw();
      
      czz->cd(4);
      mframe->Draw();
      
      czz->cd(5);
      Yframe->Draw();
      
      TString plotName = Form("plots_ppzh/%s_acc%s", plotAppendix.Data(), accName.Data());
      
      czz->SaveAs(Form("%s.eps", plotName.Data()));
      czz->SaveAs(Form("%s.png", plotName.Data()));
    } 
    
}


void calcfractionphase(double sqrts, double g1Re,  double g1Im,  double g2Re,   double g2Im,  double g4Re,  double g4Im, 
                       double & fa2, double & fa3, double & phia2, double & phia3) 
{
  // LHC numbers at mH= 125 GeV (JHUGen) based on Decay
  Double_t sigma1_p = 1.860351; // was 2.03971 at 126 GeV
  Double_t sigma2_p = 0.672859; // was 0.77517 at 126 GeV
  Double_t sigma4_p = 0.284353; // was 0.32689 at 126 GeV
  
  Double_t g1 = sqrt(g1Re*g1Re + g1Im*g1Im);
  Double_t g2 = sqrt(g2Re*g2Re + g2Im*g2Im);
  Double_t g4 = sqrt(g4Re*g4Re + g4Im*g4Im);
  
  fa2 = sigma2_p*g2*g2 / (  sigma1_p*g1*g1 + sigma2_p*g2*g2 + sigma4_p*g4*g4 );
  phia2 = atan2(g2Im, g2Re);
  fa3 = sigma4_p*g4*g4 / (  sigma1_p*g1*g1 + sigma2_p*g2*g2 + sigma4_p*g4*g4 );
  phia3 = atan2(g4Im, g4Re);
  
  std::cout << "fa2 = " << fa2 << "\t with phase " << phia2 << "\n"; 
  std::cout << "fa3 = " << fa3 << "\t with phase " << phia3 << "\n"; 
}

