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
#include "TROOT.h"
using namespace RooFit ;

void plotPdf_7D_XZZ(double mH = 125, bool draw=true) {
    
    gROOT->ProcessLine(".L ~/tdrstyle.C");
    setTDRStyle();
    gStyle->SetPadLeftMargin(0.16);
    
    // Declaration of the PDFs to use
    gROOT->ProcessLine(".L  ../PDFs/RooXZsZs_5D.cxx+");
    gROOT->ProcessLine(".L  ../PDFs/RooXZsZs_5D_form.cxx+");

    /*
    gROOT->ProcessLine(".L  ../PDFs/RooSpinOne_7D.cxx+");  
    gROOT->ProcessLine(".L  ../PDFs/RooSpinTwoXZsZs.cxx+");  
    gROOT->ProcessLine(".L  ../src/AngularPdfFactory.cc+");
    gROOT->ProcessLine(".L  ../src/ScalarPdfFactory.cc+");
    gROOT->ProcessLine(".L  ../src/VectorPdfFactory.cc+");
    gROOT->ProcessLine(".L  ../src/TensorPdfFactory.cc+");
    */


    // W/Z mass and decay width constants
    double mV = 91.1876;
    double gamV = 2.4952;
    bool offshell = false;
    if ( mH < 2 * mV ) offshell = true;

    offshell=true; // forcing mz1 and mz2 to plot...
    
    // for the pole mass and decay width of Z 
    RooRealVar* mX = new RooRealVar("mX","mX", mH);
    RooRealVar* mZ = new RooRealVar("mZ","mZ", mV);
    RooRealVar* gamZ = new RooRealVar("gamZ","gamZ",gamV);

    //
    // Observables (7D)
    // 
    RooRealVar* z1mass = new RooRealVar("z1mass","m_{Z1}",mV,1e-09,120);
    RooRealVar* z2mass = new RooRealVar("z2mass","m_{Z2}",mV,1e-09,120);
    RooRealVar* hs = new RooRealVar("costhetastar","hs",-1,1);
    RooRealVar* h1 = new RooRealVar("costheta1","h1",-1,1);
    RooRealVar* h2 = new RooRealVar("costheta2","h2",-1,1);
    RooRealVar* Phi = new RooRealVar("phi","Phi",-TMath::Pi(),TMath::Pi());
    RooRealVar* Phi1 = new RooRealVar("phistar1","Phi1",-TMath::Pi(),TMath::Pi());
    
    //
    // coupling constants for 2m+
    // See equation 5,6,7 in PRD 91, 075022
    //
    double s = (mH*mH-2*mV*mV)/2.;
    double c1 = 2*(1+mV*mV/s); // definition as in paper
    c1 = c1 * 2.; // to be consistent with the generator
    std::cout << "c1Value = " << c1 << "\n";
    RooRealVar* c1Val = new RooRealVar("c1Val", "c1Val", 1.0);
    c1Val->setConstant(kTRUE);
    RooRealVar* c2Val = new RooRealVar("c2Val", "c2Val", 0.0);
    c2Val->setConstant(kTRUE);
    RooRealVar* c3Val = new RooRealVar("c3Val", "c3Val", 0.0);
    c3Val->setConstant(kTRUE);
    RooRealVar* c4Val = new RooRealVar("c4Val", "c4Val", 0.0);
    c4Val->setConstant(kTRUE);
    RooRealVar* c5Val = new RooRealVar("c5Val", "c5Val", 0.0);
    c5Val->setConstant(kTRUE);
    RooRealVar* c6Val = new RooRealVar("c6Val", "c6Val", 0.0);
    c6Val->setConstant(kTRUE);
    RooRealVar* c7Val = new RooRealVar("c7Val", "c7Val", 0.0);
    c7Val->setConstant(kTRUE);

    // 
    // Alternative definition in terms of g1->g10
    // 
    RooRealVar* useGTerm = new RooRealVar("useGTerm", "useGTerm",1.); // set to 1 if using g couplings
    useGTerm->setConstant(kTRUE);
    RooRealVar* g1Val = new RooRealVar("g1Val", "g1Val", 1.0);
    g1Val->setConstant(kTRUE);
    RooRealVar* g2Val = new RooRealVar("g2Val", "g2Val", 0.);
    g2Val->setConstant(kTRUE);
    RooRealVar* g3Val = new RooRealVar("g3Val", "g3Val", 0.);
    g3Val->setConstant(kTRUE);
    RooRealVar* g4Val = new RooRealVar("g4Val", "g4Val", 0.);
    g4Val->setConstant(kTRUE);
    RooRealVar* g5Val = new RooRealVar("g5Val", "g5Val", 0.0);
    g5Val->setConstant(kTRUE);
    RooRealVar* g6Val = new RooRealVar("g6Val", "g6Val", 0.);
    g6Val->setConstant(kTRUE);
    RooRealVar* g7Val = new RooRealVar("g7Val", "g7Val", 0.);
    g7Val->setConstant(kTRUE);
    RooRealVar* g8Val = new RooRealVar("g8Val", "g8Val", 0.);
    g8Val->setConstant(kTRUE);
    RooRealVar* g9Val = new RooRealVar("g9Val", "g9Val", 0.);
    g9Val->setConstant(kTRUE);
    RooRealVar* g10Val = new RooRealVar("g10Val", "g10Val", 0.);
    g10Val->setConstant(kTRUE);

    // related to the gg/qq productions 
    RooRealVar* fz1Val = new RooRealVar("fz1Val", "fz1Val", 0.);
    fz1Val->setConstant(kTRUE);
    RooRealVar* fz2Val = new RooRealVar("fz2Val", "fz2Val", 1.0);
    fz2Val->setConstant(kTRUE);

    // Even more parameters, do not have to touch, based on Z couplings
    RooRealVar* R1Val = new RooRealVar("R1Val","R1Val",0.0,-1.,1.);
    R1Val->setConstant(kFALSE);
    RooRealVar* R2Val = new RooRealVar("R2Val","R2Val",0.0,-1.,1.);
    R2Val->setConstant(kFALSE);      

    // PDF definition SM Higgs (JP = 2+)
    RooXZsZs_5D *myPDF;
    RooXZsZs_5D_form *myPDF_form;
    if ( offshell ){
      myPDF = new RooXZsZs_5D("myPDF","myPDF", *z1mass, *z2mass, *h1,*h2, *Phi, 
			      *c1Val, *c2Val, *c3Val, *c4Val, *c5Val, *c6Val, 
			      *useGTerm, *g1Val, *g2Val, *g3Val, *g4Val,
			      *mZ,*gamZ,*mX,*R1Val, *R2Val);
      myPDF_form = new RooXZsZs_5D_form("myPDF_form","myPDF_form", *z1mass, *z2mass, *h1,*h2, *Phi, 
			      *c1Val, *c2Val, *c3Val, *c4Val, *c5Val, *c6Val, 
			      *useGTerm, *g1Val, *g2Val, *g3Val, *g4Val,
			      *mZ,*gamZ,*mX,*R1Val, *R2Val);      
    }else{
      myPDF = new RooXZsZs_5D("myPDF","myPDF", *mX, *mZ, *mZ, *hs, *h1,*h2, *Phi, *Phi1, 
				  *c1Val, *c2Val, *c3Val, *c4Val, *c5Val, *c6Val, *c7Val, 
				  *useGTerm, *g1Val, *g2Val, *g3Val, *g4Val, *g5Val, *g6Val, *g7Val, *g8Val, *g9Val, *g10Val,
				  *fz1Val, *fz2Val, *R1Val, *R2Val, *mZ, *gamZ);
    }
      
    // dataset for (JP = 2+)
    TString fileName;
    if ( useGTerm->getVal() > 0.) {
      fileName = Form("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/SMHiggs_store/SMHiggs_125GeV.root",mH); // Ian's file
      // fileName = Form("TZZ_%.0f_JHU_YY.root", mH);
      // fileName = Form("TZZ_%.0f_JHU_FromG1.root", mH);
    }
    else {
      fileName = Form("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/SMHiggs_store/SMHiggs_125GeV.root", mH);
      // fileName = Form("TZZ_125_JHU_GenFromC_symz1z2.root", mH);
    }
    std::cout << "Opening " << fileName << "\n";
    TFile* fin = new TFile(fileName);
    TTree* tin = (TTree*) fin->Get("angles");
    
    if ( offshell) 
      RooDataSet data("data","data",tin,RooArgSet(*z1mass, *z2mass, *hs, *h1, *h2, *Phi, *Phi1));
    else 
      RooDataSet data("data","data",tin,RooArgSet(*hs, *h1, *h2, *Phi, *Phi1));

    if ( draw ) {
      // P L O T   . . .  
      // (All parameters fixed, no fitting, just looking at the shape of the PDFs w.r.t. the data)
      TH1F* dum0 = new TH1F("dum0","dum0",1,0,1); dum0->SetLineColor(kRed); dum0->SetMarkerColor(kBlack); dum0->SetLineWidth(3); // 2m+
      
      TLegend * box3 = new TLegend(0.1,0.1,0.9,0.92);
      box3->SetFillColor(0);
      box3->SetBorderSize(0);
      box3->AddEntry(dum0,"X#rightarrow ZZ JP = 2m+","lp");

      myPDF->fitTo(data);
      
      /*
      if ( offshell ) {
      RooPlot* z1massframe =  z1mass->frame(55);
      data.plotOn(z1massframe, LineColor(kBlack));
      myPDF->plotOn(z1massframe, LineColor(kRed));
      
      RooPlot* z2massframe =  z2mass->frame(55);
      data.plotOn(z2massframe, LineColor(kBlack));
      myPDF->plotOn(z2massframe, LineColor(kRed));
      }

      RooPlot* h1frame =  h1->frame(20);
      data.plotOn(h1frame, LineColor(kBlack));
      myPDF->plotOn(h1frame, LineColor(kRed));
      
      RooPlot* h2frame =  h2->frame(20);
      data.plotOn(h2frame, LineColor(kBlack));
      myPDF->plotOn(h2frame, LineColor(kRed));

      RooPlot* hsframe =  hs->frame(20);
      data.plotOn(hsframe, LineColor(kBlack));
      myPDF->plotOn(hsframe, LineColor(kRed));

      */

      RooPlot* Phiframe =  Phi->frame(20);
      data.plotOn(Phiframe, LineColor(kBlack));
      myPDF->plotOn(Phiframe, LineColor(kRed));
      myPDF_form->plotOn(Phiframe, LineColor(kBlue), LineStyle(2));
      
      /*

      RooPlot* Phi1frame =  Phi1->frame(20);
      data.plotOn(Phi1frame, LineColor(kBlack));
      myPDF->plotOn(Phi1frame, LineColor(kRed));
      */
      TCanvas* czz = new TCanvas( "czz", "czz", 1000, 600 );
      czz->Divide(4,2);
      
      /*

      if ( offshell ) {
	czz->cd(1);
	z1massframe->Draw();

	//czz->cd(2);
	//z2massframe->Draw();
      }

      czz->cd(3);
      hsframe->Draw();

      czz->cd(4);
      box3->Draw();
      
      czz->cd(5);
      Phi1frame->Draw();
      
      czz->cd(6);
      h1frame->Draw();
      
      czz->cd(7);
      h2frame->Draw();

      */

      czz->cd(8);
      Phiframe->Draw();

      /*
      if ( useGTerm->getVal() > 0.) {
	czz->SaveAs(Form("epsfiles/angles_g5Only_125GeV_ZZ.eps", mH));
	czz->SaveAs(Form("pngfiles/angles_g5Only_125GeV_ZZ.png", mH));
      } else {
	czz->SaveAs(Form("epsfiles/angles_g5Only_125GeV_ZZ.eps", mH));
	czz->SaveAs(Form("pngfiles/angles_g5Only_125GeV_ZZ.png", mH));
      }
      */
      //delete czz;
    }
}
