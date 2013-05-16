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

bool drawbkg = false;

void plotPdf_5D_ZH(float mH = 126, float sqrtsVal = 250.) {
    
    gROOT->ProcessLine(".L ~/tdrstyle.C");
    setTDRStyle();
    gStyle->SetPadLeftMargin(0.16);
    
    // Declaration of the PDFs to use
    gROOT->ProcessLine(".L ../src/RooSpinZero_5D_ZH.cc+");
    gSystem->Load("../src/RooSpinZero_5D_ZH.cc");
    // gROOT->ProcessLine(".L ../src/RooSpinZero_5D_ZH_PRD.cc+");
    // gSystem->Load("../src/RooSpinZero_5D_ZH_PRD.cc");

    // set beam parameters according to the input file
    // 0 for unpolarized beam;
    // 1 for RL configuration
    // -1 for LR configuration

    double r2val = 1.;

    // 
    // Read input file
    // 

    TString fileName = "lhefiles/ee_ZsmH_llbb_false.root";
    if ( drawbkg ) {
      fileName = ("lhefiles/ee_ZZ_llbb_false.root");
    } else {
      if ( r2val == 1. ) 
	fileName = "lhefiles/ee_ZsmH_llbb_RL_false.root";
      if ( r2val == -1. ) 
	fileName = "lhefiles/ee_ZsmH_llbb_LR_false.root";
    }

    TFile *fin = new TFile(fileName);
    TTree* tin = (TTree*) fin->Get("SelectedTree");
    


    
    
    // Observables (5D)
    RooRealVar* h1 = new RooRealVar("costheta1","h1",-1,1);
    RooRealVar* h2 = new RooRealVar("costheta2","h2",-1,1);
    RooRealVar* Phi = new RooRealVar("phi","Phi",-TMath::Pi(),TMath::Pi());
    RooRealVar* hs = new RooRealVar("costhetastar","hs",-1,1);
    RooRealVar* Phi1 = new RooRealVar("phistar1","Phi1",-TMath::Pi(),TMath::Pi());
    
    // Parameters
    RooRealVar* sqrts= new RooRealVar("sqrts","sqrts", sqrtsVal);
    RooRealVar* mX = new RooRealVar("mX","mX", mH);
    RooRealVar* mZ = new RooRealVar("mZ","mZ", 91.1876);
    RooRealVar* gamZ = new RooRealVar("gamZ","gamZ",2.4952);
    RooRealVar* R1Val = new RooRealVar("R1Val","R1Val", 0.15);
    RooRealVar* R2Val = new RooRealVar("R2Val","R2Val", r2val);

    
    // set the PDF
    RooSpinZero_5D_ZH *myPDF = new RooSpinZero_5D_ZH("myPDF","myPDF",
						     *h1,*h2,*hs,*Phi,*Phi1,
						     *sqrts, *mX, *mZ, *R1Val, *R2Val);
    
    // get the data
    RooDataSet data("data","data",tin,RooArgSet(*h1,*h2,*Phi,*hs,*Phi1));


    // 
    // Plotting frames
    // 
    
    RooPlot* h1frame =  h1->frame(20);
    data.plotOn(h1frame, LineColor(kBlack), MarkerStyle(24));
    if ( !drawbkg ) 
      myPDF->plotOn(h1frame, LineColor(kBlack));
    
    RooPlot* h2frame =  h2->frame(20);
    data.plotOn(h2frame, LineColor(kBlack), MarkerStyle(24));
    if ( !drawbkg ) 
      myPDF->plotOn(h2frame, LineColor(kBlack));

    RooPlot* hsframe =  hs->frame(20);
    data.plotOn(hsframe, LineColor(kBlack), MarkerStyle(24));
    if ( !drawbkg )
      myPDF->plotOn(hsframe, LineColor(kBlack));

    RooPlot* phiframe =  Phi->frame(20);
    data.plotOn(phiframe, LineColor(kBlack), MarkerStyle(24));
    if ( !drawbkg ) 
      myPDF->plotOn(phiframe, LineColor(kBlack));
    
    RooPlot* phi1frame =  Phi1->frame(20);
    data.plotOn(phi1frame, LineColor(kBlack), MarkerStyle(24));
    if ( !drawbkg ) 
      myPDF->plotOn(phi1frame, LineColor(kBlack));
    
    TCanvas* czz = new TCanvas( "czz", "czz", 1000, 600 );
    czz->Divide(3,2);
    
    czz->cd(1);
    hsframe->Draw();
    
    czz->cd(2);
    phi1frame->Draw();

    czz->cd(4);
    h1frame->Draw();

    czz->cd(5);
    h2frame->Draw();

    czz->cd(6);
    phiframe->Draw();
    

    TString plotName = Form("plots/ZH_mH%.0f_sqrts%.0f_unpolar", mH, sqrtsVal);
    
    if ( r2val < 0. ) {
      plotName = Form("plots/ZH_mH%.0f_sqrts%.0f_RL_minus%.0f", mH, sqrtsVal, TMath::Abs(r2val*100.));
    }

    if ( r2val > 0. ) {
      plotName = Form("plots/ZH_mH%.0f_sqrts%.0f_RL_plus%.0f", mH, sqrtsVal, TMath::Abs(r2val*100.));
    }
    
    if ( drawbkg ) 
      plotName = Form("plots/ZZ_sqrts%.0f", sqrtsVal);
    czz->SaveAs(Form("%s.eps", plotName.Data()));
    czz->SaveAs(Form("%s.png", plotName.Data()));
    
}
