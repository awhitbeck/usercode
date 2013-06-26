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


void plotPdf_3D_ZZ(float sqrtsVal = 250.) {
    
    gROOT->ProcessLine(".L ~/tdrstyle.C");
    setTDRStyle();
    gStyle->SetPadLeftMargin(0.16);
    
    // Declaration of the PDFs to use
    gROOT->ProcessLine(".L ../src/RooZZ_3D.cc+");
    gSystem->Load("../src/RooZZ_3D.cc");

    // these values define the generator couplings
    bool withAcc = false;
    TString accName = "false";
    if ( withAcc ) 
      accName = "true";
    TString modeName = "ee_ZZ_llbb_250GeV_25M";
    TString fileName = Form("bkgData/%s_%s.root", modeName.Data(), accName.Data());

    
    // 
    // Read input file
    // 
    TFile *fin = new TFile(fileName);
    TTree* tin = (TTree*) fin->Get("SelectedTree");
    
    // Observables (5D)
    RooRealVar* h1 = new RooRealVar("costheta1","h1",-1,1);
    RooRealVar* h2 = new RooRealVar("costheta2","h2",-1,1);
    RooRealVar* Phi = new RooRealVar("phi","Phi",-TMath::Pi(),TMath::Pi());

    float h1pol2Val = 5.43096e-01;
    float h1pol4Val = 1.05202e+00;
    float h1pol6Val = 1.61973e-02;
    float h2pol2Val = 0.246235;
    float phiconstVal = -1.96298e-02;
    float twophiconstVal =-1.37763e-01;

    if ( withAcc ) {
      h1pol2Val = -0.18576;
      h1pol4Val = 2.8289;
      h1pol6Val = -1.20225;
      h2pol2Val = 0.2531;
      phiconstVal = -0.0206;
      twophiconstVal = -0.1956;
    }

    RooRealVar* h1pol2  = new RooRealVar("h1pol2","h1pol2", h1pol2Val, -10, 10);
    RooRealVar* h1pol4  = new RooRealVar("h1pol4","h1pol4", h1pol4Val, -10, 10);
    RooRealVar* h1pol6  = new RooRealVar("h1pol6","h1pol6", h1pol6Val, -10, 10);
    RooRealVar* h2pol2  = new RooRealVar("h2pol2","h2pol2", h2pol2Val, -1, 1);
    RooRealVar* phiconst  = new RooRealVar("phicons","phiconst", phiconstVal, -1, 1);
    RooRealVar* twophiconst  = new RooRealVar("twophicons","twophiconst", twophiconstVal, -1, 1);

    h1pol2->setConstant(kTRUE);
    h1pol4->setConstant(kTRUE);
    h1pol6->setConstant(kTRUE);
    h2pol2->setConstant(kTRUE);
    phiconst->setConstant(kTRUE);
    twophiconst->setConstant(kTRUE);


    // set the PDF
    RooZZ_3D *myPDF = new RooZZ_3D("myPDF","myPDF", *h1,*h2,*Phi,*h1pol2,*h1pol4,*h1pol6,*h2pol2,*phiconst,*twophiconst,withAcc);
    
    RooDataSet data = RooDataSet("data","data",tin,RooArgSet(*h1,*h2,*Phi));

    RooFitResult* toyfitresults =  myPDF->fitTo(data);
    
    
    // 
    // Plotting frames
    // 
    
    RooPlot* h1frame =  h1->frame(20);
    data.plotOn(h1frame, LineColor(kBlack), MarkerStyle(24));
    myPDF->plotOn(h1frame, LineColor(kBlack));

    RooPlot* h2frame =  h2->frame(20);
    data.plotOn(h2frame, LineColor(kBlack), MarkerStyle(24));
    myPDF->plotOn(h2frame, LineColor(kBlack));

    RooPlot* phiframe =  Phi->frame(20);
    data.plotOn(phiframe, LineColor(kBlack), MarkerStyle(24));
    myPDF->plotOn(phiframe, LineColor(kBlack));
        
    TCanvas* czz = new TCanvas( "czz", "czz", 1200, 400 );
    czz->Divide(3,1);
    
    czz->cd(1);
    h1frame->Draw();
    
    czz->cd(2);
    h2frame->Draw();

    czz->cd(3);
    phiframe->Draw();

    TString plotName = Form("plots/%s_acc%s", modeName.Data(), accName.Data());
    
    czz->SaveAs(Form("%s.eps", plotName.Data()));
    czz->SaveAs(Form("%s.png", plotName.Data()));
    
}


void calcfractionphase(double sqrts, double g1Re,  double g1Im,  double g2Re,   double g2Im,  double g4Re,  double g4Im, 
		       double & fa2, double & fa3, double & phia2, double & phia3) 
{

  // ILC numbers at 250 GeV at mH= 125 GeV (narrow Z width approximation)
  Double_t sigma1_e = 0.981396; // was 0.94696 at 126 GeV
  Double_t sigma2_e = 33.4674;  // was 32.1981 at 126 GeV
  Double_t sigma4_e = 7.9229;   // was 7.45502 at 126 GeV
  
  // ILC nubmers at 500 GeV at mH = 125 GeV
  if ( sqrts == 500. ) {
    sigma1_e = 2.57246; 
    sigma2_e = 516.556;  
    sigma4_e = 414.378;
  }

  
  Double_t g1 = sqrt(g1Re*g1Re + g1Im*g1Im);
  Double_t g2 = sqrt(g2Re*g2Re + g2Im*g2Im);
  Double_t g4 = sqrt(g4Re*g4Re + g4Im*g4Im);

  fa2 = sigma2_e*g2*g2 / (  sigma1_e*g1*g1 + sigma2_e*g2*g2 + sigma4_e*g4*g4 );
  phia2 = atan2(g2Im, g2Re);
  fa3 = sigma4_e*g4*g4 / (  sigma1_e*g1*g1 + sigma2_e*g2*g2 + sigma4_e*g4*g4 );
  phia3 = atan2(g4Im, g4Re);
  
  std::cout << "fa2 = " << fa2 << "\t with phase " << phia2 << "\n"; 
  std::cout << "fa3 = " << fa3 << "\t with phase " << phia3 << "\n"; 
  
}

