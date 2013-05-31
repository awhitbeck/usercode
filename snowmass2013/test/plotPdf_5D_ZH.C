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

// 
// global variables, not very wise, will revisit later
// 
// set this to true if you want to draw the ee->ZZ 
bool drawbkg = false;
// set this to true if you are using the JHUGen one with weights
bool weightedevents = true;
// set this to true if you are drawing the 0-
bool zerominus = true;

void plotPdf_5D_ZH(float mH = 126, float sqrtsVal = 250.) {
    
    gROOT->ProcessLine(".L ~/tdrstyle.C");
    setTDRStyle();
    gStyle->SetPadLeftMargin(0.16);
    
    // Declaration of the PDFs to use
    gROOT->ProcessLine(".L ../src/RooSpinZero_5D_ZH.cc+");
    gSystem->Load("../src/RooSpinZero_5D_ZH.cc");

    // set beam parameters according to the input file
    // the number refers to the electron polarization!
    // The file is listed for e+/e- polarization
    // 0 for unpolarized beam;
    // 1 for e+_L e-_R configuration
    // -1 for e+_R e-_L configuration

    double r1val = 0;

    // 
    // Read input file
    // 
    
    TString fileName = "lhefiles/ee_ZsmH_llbb_false.root";
    if ( weightedevents ) {
      fileName = "lhefiles/100k_events_false.root";
      if ( zerominus )
	fileName = "lhefiles/puseduscalar_100k_events_false.root";
    }
    if ( drawbkg ) {
      fileName = ("lhefiles/ee_ZZ_llbb_false.root");
      //fileName = ("lhefiles/events_yfz_false.root");
    } else {
      if ( r1val == 1. ) 
	fileName = "lhefiles/ee_ZsmH_llbb_LR_false.root";
      if ( r1val == -1. ) 
	fileName = "lhefiles/ee_ZsmH_llbb_RL_false.root";
    }

    TFile *fin = new TFile(fileName);
    TTree* tin = (TTree*) fin->Get("SelectedTree");
    
    // Observables (5D)
    RooRealVar* h1 = new RooRealVar("costheta1","h1",-1,1);
    RooRealVar* h2 = new RooRealVar("costheta2","h2",-1,1);
    RooRealVar* Phi = new RooRealVar("phi","Phi",-TMath::Pi(),TMath::Pi());
    RooRealVar* hs = new RooRealVar("costhetastar","hs",-1,1);
    RooRealVar* Phi1 = new RooRealVar("phistar1","Phi1",-TMath::Pi(),TMath::Pi());
    // event weight
    RooRealVar* wt = new RooRealVar("wt", "wt", 0.0, 50);

    // Parameters
    RooRealVar* sqrts= new RooRealVar("sqrts","sqrts", sqrtsVal);
    RooRealVar* mX = new RooRealVar("mX","mX", mH);
    RooRealVar* mZ = new RooRealVar("mZ","mZ", 91.1876);
    RooRealVar* gamZ = new RooRealVar("gamZ","gamZ",2.4952);
    RooRealVar* R1Val = new RooRealVar("R1Val","R1Val", r1val);
    RooRealVar* R2Val = new RooRealVar("R2Val","R2Val", 0.15);

    // amplitude parameters
    RooRealVar* a1 = new RooRealVar("a1","a1", 1.);
    RooRealVar* a2 = new RooRealVar("a2","a2", 0.);
    RooRealVar* a3 = new RooRealVar("a3","a3", 0);
    RooRealVar* phia1 = new RooRealVar("phia1","phia1", 0.);
    RooRealVar* phia2 = new RooRealVar("phia2","phia2", 0.);
    RooRealVar* phia3 = new RooRealVar("phia3","phixa3", 0.);

    if ( zerominus ) {
      a1->setVal (0.);
      a3->setVal (1.);
      
    }

    // set the PDF
    RooSpinZero_5D_ZH *myPDF = new RooSpinZero_5D_ZH("myPDF","myPDF",
						     *h1,*h2,*hs,*Phi,*Phi1,
						     *sqrts, *mX, *mZ, 
						     *a1, *a2, *a3, *phia1, *phia2, *phia3,
						     *R1Val, *R2Val);
    
    // get the data
    if ( weightedevents ) {
      RooDataSet dataTMP("dataTMP","dataTMP",tin,RooArgSet(*h1,*h2,*Phi,*hs,*Phi1,*wt));
      RooDataSet data = RooDataSet("data","data",RooArgList(*h1,*h2,*Phi,*hs,*Phi1,*wt), WeightVar("wt"), Import(dataTMP));
    } else {
      RooDataSet data = RooDataSet("data","data",tin,RooArgSet(*h1,*h2,*Phi,*hs,*Phi1));
    }
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
    if ( weightedevents ) {
      plotName = Form("plots/ZH_mH%.0f_sqrts%.0f_unpolar_JHU", mH, sqrtsVal);
      if ( zerominus ) 
	plotName = Form("plots/ZA_mH%.0f_sqrts%.0f_unpolar_JHU", mH, sqrtsVal);
    }
    
    if ( r1val < 0. ) {
      plotName = Form("plots/ZH_mH%.0f_sqrts%.0f_RL_minus%.0f", mH, sqrtsVal, TMath::Abs(r1val*100.));
    }

    if ( r1val > 0. ) {
      plotName = Form("plots/ZH_mH%.0f_sqrts%.0f_RL_plus%.0f", mH, sqrtsVal, TMath::Abs(r1val*100.));
    }
    
    if ( drawbkg ) 
      plotName = Form("plots/ZZ_sqrts%.0f", sqrtsVal);
      // plotName = Form("plots/ZH_sqrts%.0f_YFZ", sqrtsVal);
    czz->SaveAs(Form("%s.eps", plotName.Data()));
    czz->SaveAs(Form("%s.png", plotName.Data()));
    
}
