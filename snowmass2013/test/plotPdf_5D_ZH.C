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
bool weightedevents = false;
bool drawsm = true;
bool drawhminus = false;
bool drawhplus = false;


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
    if ( drawsm ) 
      fileName = "lhefiles/unweighted_unpol_0plus_2M_false.root";
    if ( drawhminus )
      fileName = "lhefiles/unweighted_unpol_0minus_2M_false.root";
    if ( drawhplus ) 
      fileName = "lhefiles/unweighted_unpol_g02_2M_false.root";
    if ( drawbkg ) {
      fileName = ("lhefiles/ee_ZZ_llbb_false.root");
    } 
    
    /*
    else {
      if ( r1val == 1. ) 
	fileName = "lhefiles/ee_ZsmH_llbb_LR_false.root";
      if ( r1val == -1. ) 
	fileName = "lhefiles/ee_ZsmH_llbb_RL_false.root";
    }
    */

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
    int para = 2; 
    RooRealVar* a1Val  = new RooRealVar("a1Val","a1Val",0.);
    RooRealVar* phi1Val= new RooRealVar("phi1Val","phi1Val",0.);
    RooRealVar* a2Val  = new RooRealVar("a2Val","a2Val",0.);
    RooRealVar* phi2Val= new RooRealVar("phi2Val","phi2Val",0.);
    RooRealVar* a3Val  = new RooRealVar("a3Val","a3Val",0.);
    RooRealVar* phi3Val= new RooRealVar("phi3Val","phi3Val",0.);
           
    RooRealVar* g1Val  = new RooRealVar("g1Val","g1Val",1.);
    RooRealVar* g2Val  = new RooRealVar("g2Val","g2Val",0.);
    RooRealVar* g3Val  = new RooRealVar("g3Val","g3Val",0.);
    RooRealVar* g4Val  = new RooRealVar("g4Val","g4Val",0.);

    RooRealVar* g1ValIm  = new RooRealVar("g1ValIm","g1ValIm",0.);
    RooRealVar* g2ValIm  = new RooRealVar("g2ValIm","g2ValIm",0.);
    RooRealVar* g3ValIm  = new RooRealVar("g3ValIm","g3ValIm",0.);
    RooRealVar* g4ValIm  = new RooRealVar("g4ValIm","g4ValIm",0.);

    RooRealVar* fa2  = new RooRealVar("fa2","f_{g2}",0.,0.,1.0);
    RooRealVar* fa3  = new RooRealVar("fa3","f_{g4}",0.,0.,1.0);
    RooRealVar* phia2  = new RooRealVar("phia2","#phi_{g2}",0.,-2.*TMath::Pi(),2*TMath::Pi());
    RooRealVar* phia3  = new RooRealVar("phia3","#phi_{g4}",0.,-2.*TMath::Pi(),2*TMath::Pi());

    // set the PDF
    RooSpinZero_5D_ZH *myPDF = new RooSpinZero_5D_ZH("myPDF","myPDF",
						     *h1,*h2,*hs,*Phi,*Phi1,
						     *sqrts, *mX, *mZ, *R1Val, *R2Val, para, 
						     *a1Val, *phi1Val, *a2Val, *phi2Val,*a3Val, *phi3Val, 
						     *g1Val, *g2Val, *g3Val, *g4Val, *g1ValIm, *g2ValIm, *g3ValIm, *g4ValIm,
						     *fa2, *fa3, *phia2, *phia3);
    
    if ( drawhminus) {
      g1Val->setVal(0.);
      g2Val->setVal(0.);
      g4Val->setVal(1.);
    }

    if ( drawhplus ) {
      g1Val->setVal(0.);
      g2Val->setVal(1.);
      g4Val->setVal(0.);
    }

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
    
    TString modeName = "sm";
    if ( drawhminus ) modeName = "hminus";
    if ( drawhplus ) modeName = "hplus";
    
    TString beamPolarName = "unpolar";
    if ( r1val < 0. ) beamPolarName = Form("RL_minus%.0f", TMath::Abs(r1val*100.));
    if ( r1val > 0. ) beamPolarName = Form("RL_plus%.0f", TMath::Abs(r1val*100.));
    


    TString plotName = Form("plots/ZX_mX%.0f_sqrts%.0f_%s_%s", mH, sqrtsVal, modeName.Data(), beamPolarName.Data());
    
    if ( drawbkg ) 
      plotName = Form("plots/ZZ_sqrts%.0f", sqrtsVal);
    
    czz->SaveAs(Form("%s.eps", plotName.Data()));
    czz->SaveAs(Form("%s.png", plotName.Data()));
    
}
