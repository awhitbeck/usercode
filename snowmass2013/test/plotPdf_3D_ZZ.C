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

    gROOT->ProcessLine(".L ../src/RooSpinZero_3D_ZH.cc+");
    gSystem->Load("../src/RooSpinZero_3D_ZH.cc");

    // these values define the generator couplings
    bool withAcc = false;
    TString accName = "false";
    if ( withAcc ) 
      accName = "true";
    TString modeName = "ee_ZZ_llbb_250GeV_25M";
    TString fileName = Form("bkgData/%s_%s.root", modeName.Data(), accName.Data());

    bool generateToy = false;
    bool drawbkg = true;
    // 
    // Read input file
    // 
    TFile *fin = new TFile(fileName);
    TTree* tin = (TTree*) fin->Get("SelectedTree");
    
    // Observables (5D)
    RooRealVar* h1 = new RooRealVar("costheta1","h1",0, -1,1);
    RooRealVar* h2 = new RooRealVar("costheta2","h2",0, -1,1);
    RooRealVar* Phi = new RooRealVar("phi","Phi",0, -TMath::Pi(),TMath::Pi());

    float h1pol2Val = 5.43096e-01;
    float h1pol4Val = 1.05202e+00;
    float h1pol6Val = 1.61973e-02;
    float h2pol2Val = 0.246235;
    float phiconstVal = -1.96298e-02;
    float twophiconstVal =-1.37763e-01;

    if ( withAcc ) {
      h1pol2Val = 7.31558e-01;
      h1pol4Val = 3.65903e-01;
      h1pol6Val = 5.59806e-01;
      h2pol2Val = 2.15611e-01;
      phiconstVal = -1.98335e-02;
      twophiconstVal =-2.00570e-01;
    }

    RooRealVar* h1pol2  = new RooRealVar("h1pol2","h1pol2", h1pol2Val, -10, 10);
    RooRealVar* h1pol4  = new RooRealVar("h1pol4","h1pol4", h1pol4Val, -10, 10);
    RooRealVar* h1pol6  = new RooRealVar("h1pol6","h1pol6", h1pol6Val, -10, 10);
    RooRealVar* h2pol2  = new RooRealVar("h2pol2","h2pol2", h2pol2Val, -1, 1);
    RooRealVar* phiconst  = new RooRealVar("phicons","phiconst", phiconstVal, -10, 10);
    RooRealVar* twophiconst  = new RooRealVar("twophicons","twophiconst", twophiconstVal, -10, 10);

    h1pol2->setConstant(kTRUE);
    h1pol4->setConstant(kTRUE);
    h1pol6->setConstant(kTRUE);
    h2pol2->setConstant(kTRUE);
    phiconst->setConstant(kTRUE);
    twophiconst->setConstant(kTRUE);


    // set the PDF
    RooZZ_3D *bkgPdf = new RooZZ_3D("bkgPdf","bkgPdf", *h1,*h2,*Phi,
				   *h1pol2,*h1pol4,*h1pol6,*h2pol2,*phiconst,*twophiconst,withAcc);
    
    RooDataSet bkgData = RooDataSet("bkgData","bkgData",tin,RooArgSet(*h1,*h2,*Phi));

    RooFitResult* toyfitresults =  bkgPdf->fitTo(bkgData);
	
    
    //
    // Signal files
    // 

    // generated values
    double g1Gen =   1;
    double g1ImGen = 0.;
    double g2Gen =   0.;
    double g2ImGen = 0.;
    double g3Gen =   0.;
    double g3ImGen = 0.;
    double g4Gen =  0.117316;
    double g4ImGen = 0.;
    double fa2Gen = 0.;
    double phia2Gen = 0.;
    double fa3Gen = 0.;
    double phia3Gen = 0.;

    calcfractionphase(sqrtsVal, g1Gen, g1ImGen, g2Gen, g2ImGen, g4Gen, g4ImGen, fa2Gen, fa3Gen, phia2Gen, phia3Gen);
    
    // 
    // Read input file
    // 
    TFile *fin_signal = new TFile("Events_20130620/unweighted_unpol_model6_250GeV_1M_false.root");
    TTree* tin_signal = (TTree*) fin_signal->Get("SelectedTree");

    // additional variables
    // event weight
    RooRealVar* wt = new RooRealVar("wt", "wt", 0.0, 50);
    // Parameters
    RooRealVar* sqrts= new RooRealVar("sqrts","sqrts", 250.);
    RooRealVar* mX = new RooRealVar("mX","mX", 125.);
    RooRealVar* mZ = new RooRealVar("mZ","mZ", 91.1876);
    RooRealVar* gamZ = new RooRealVar("gamZ","gamZ",2.4952);
    RooRealVar* R1Val = new RooRealVar("R1Val","R1Val", 0.15);
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
    RooSpinZero_3D_ZH *sigPdf = new RooSpinZero_3D_ZH("sigPdf","sigPdf",
						     *h1,*h2,*Phi,
						     *sqrts, *mX, *mZ, *R1Val, *R2Val, para, 
						     *a1Val, *phi1Val, *a2Val, *phi2Val,*a3Val, *phi3Val, 
						     *g1Val, *g2Val, *g3Val, *g4Val, *g1ValIm, *g2ValIm, *g3ValIm, *g4ValIm,
						      *fa2, *fa3, *phia2, *phia3, withAcc);
    
    RooDataSet sigData = RooDataSet("sigData","sigData",tin_signal,RooArgSet(*h1,*h2,*Phi));


    // 
    // toyData generation
    // 
    RooRealVar* nsig = new RooRealVar("nsig","number of signal events",  1e+6);
    RooRealVar* nbkg = new RooRealVar("nbkg","number of background events", 1e+5);
    // RooDataSet* sigToyData = sigPdf->generate(RooArgSet(*h1,*h2,*Phi), nsig->getVal());
    // RooDataSet* bkgToyData = bkgPdf->generate(RooArgSet(*h1,*h2,*Phi), nbkg->getVal());
    RooAddPdf* totalPdf = new RooAddPdf("totalPdf","totalPdf",RooArgList(*sigPdf,*bkgPdf),RooArgList(*nsig,*nbkg));    

    if ( generateToy ) {
      RooDataSet* toyData = totalPdf->generate(RooArgSet(*h1,*h2,*Phi), nsig->getVal()+nbkg->getVal());
    }


    // 
    // Plotting frames
    // 
    
    RooPlot* h1frame =  h1->frame(20);
    
    if ( drawbkg ) {
      if ( generateToy ) 
	bkgToyData->plotOn(h1frame, LineColor(kBlack), MarkerStyle(24));
      else 
	bkgData->plotOn(h1frame, LineColor(kBlack), MarkerStyle(24));
      bkgPdf->plotOn(h1frame, LineColor(kBlack));
    }

    RooPlot* h2frame =  h2->frame(20);
    if ( drawbkg ) {
      if ( generateToy ) 
	bkgData->plotOn(h2frame, LineColor(kBlack), MarkerStyle(24)); 
      else 
	bkgData->plotOn(h2frame, LineColor(kBlack), MarkerStyle(24));
      bkgPdf->plotOn(h2frame, LineColor(kBlack));
    }
    
    
    RooPlot* phiframe =  Phi->frame(20);
    if ( drawbkg ) {
      if ( generateToy ) 
	toyBkgData->plotOn(phiframe, LineColor(kBlack), MarkerStyle(24));
      else 
	bkgData->plotOn(phiframe, LineColor(kBlack), MarkerStyle(24));
      bkgPdf->plotOn(phiframe, LineColor(kBlack));
    }
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

