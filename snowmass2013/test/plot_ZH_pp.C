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

void plot_ZH_pp() {
    
    gROOT->ProcessLine(".L ~/tdrstyle.C");
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

    //TString modeName = "JHUGen_weighted_pp_ZH_llbb_0m_1M";
    TString modeName = "pp_ZH_llbb_14TeV_1M";

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
        
    // 
    // Read input file
    // 
    TFile *fin_signal = new TFile(Form("samples/pp_ZH/%s_%s.root", modeName.Data(),accName.Data()));
    TTree* tin_signal = (TTree*) fin_signal->Get("SelectedTree");
    
    // Observables (5D)
    RooRealVar* h1 = new RooRealVar("costheta1","h1",-1,1);
    RooRealVar* h2 = new RooRealVar("costheta2","h2",-1,1);
    RooRealVar* Phi = new RooRealVar("phi","Phi",-TMath::Pi(),TMath::Pi());
    RooRealVar* hs = new RooRealVar("costhetastar","hs",-1,1);
    RooRealVar* Phi1 = new RooRealVar("phistar1","Phi1",-TMath::Pi(),TMath::Pi());
    RooRealVar* m= new RooRealVar("m","m", 150, 500);
    RooRealVar* Y= new RooRealVar("Y","Y", -4, 4);
    
    // additional variables
    // event weight
    RooRealVar* wt = new RooRealVar("wt", "wt", 0.0, 50);
    // Parameters
    RooRealVar* sqrts= new RooRealVar("sqrts","sqrts", 14000.);
    RooRealVar* mX = new RooRealVar("mX","mX", 125.);
    RooRealVar* mZ = new RooRealVar("mZ","mZ", 91.1876);
    RooRealVar* gamZ = new RooRealVar("gamZ","gamZ",2.4952);
    RooRealVar* R1Val = new RooRealVar("R1Val","R1Val", 0.);
    // RooRealVar* R1Val = new RooRealVar("R1Val","R1Val", 0.67);    
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

    // set the PDF for the other 
    
    RooRealVar* g1Val0m  = new RooRealVar("g1Val0m","g1Val0m",0.);
    RooRealVar* g2Val0m  = new RooRealVar("g2Val0m","g2Val0m",0.);
    RooRealVar* g3Val0m  = new RooRealVar("g3Val0m","g3Val0m",0.);
    RooRealVar* g4Val0m  = new RooRealVar("g4Val0m","g4Val0m",1.);
    
    RooRealVar* g1Val0mIm  = new RooRealVar("g1Val0mIm","g1Val0mIm", 0.);
    RooRealVar* g2Val0mIm  = new RooRealVar("g2Val0mIm","g2Val0mIm", 0.);
    RooRealVar* g3Val0mIm  = new RooRealVar("g3Val0mIm","g3Val0mIm", 0.);
    RooRealVar* g4Val0mIm  = new RooRealVar("g4Val0mIm","g4Val0mIm", 0.);


    RooSpinZero_3D_ZH_pp *sigPdf_pp_0m = new RooSpinZero_3D_ZH_pp("sigPdf_pp","sigPdf_pp",
                                                      *h1,*h2,*Phi, *m, *Y,
                                                      *sqrts, *mX, *mZ, *R1Val, *R2Val, para, 
                                                      *a1Val, *phi1Val, *a2Val, *phi2Val,*a3Val, *phi3Val, 
                                                      *g1Val0m, *g2Val0m, *g3Val0m, *g4Val0m, *g1Val0mIm, *g2Val0mIm, *g3Val0mIm, *g4Val0mIm,
                                                      *fa2, *fa3, *phia2, *phia3, withAcc);

    
    // set the data

    
    // get the data
    if ( weightedevents ) {
      RooDataSet dataTMP("dataTMP","dataTMP", tin_signal, RooArgSet(*h1,*h2,*Phi,*m,*Y, *wt));
      RooDataSet sigData_pp = RooDataSet("sigData","sigData",RooArgList(*h1,*h2,*Phi,*m, *Y, *wt), WeightVar("wt"), Import(dataTMP));
    } else {
      RooDataSet sigData_pp = RooDataSet("sigData","sigData",RooArgSet(*h1,*h2,*Phi,*m,*Y),Import(*tin_signal));    
    }
    RooPlot* h1frame =  h1->frame(20);
    sigData_pp.plotOn(h1frame, LineColor(kBlack), MarkerStyle(24));
    sigPdf_pp->plotOn(h1frame, LineColor(kBlack));
    //sigPdf_pp_0m->plotOn(h1frame, LineColor(kRed));
    
    RooPlot* h2frame =  h2->frame(20);
    sigData_pp.plotOn(h2frame, LineColor(kBlack), MarkerStyle(24));
    sigPdf_pp->plotOn(h2frame, LineColor(kBlack));
    //sigPdf_pp_0m->plotOn(h2frame, LineColor(kRed));
    
    RooPlot* phiframe =  Phi->frame(20);
    sigData_pp.plotOn(phiframe, LineColor(kBlack), MarkerStyle(24));
    sigPdf_pp->plotOn(phiframe, LineColor(kBlack));
    //sigPdf_pp_0m->plotOn(phiframe, LineColor(kRed));

    RooPlot* mframe =  m->frame(20);
    sigData_pp.plotOn(mframe, LineColor(kBlack), MarkerStyle(24));
    sigPdf_pp->plotOn(mframe, LineColor(kBlack));
    //sigPdf_pp_0m->plotOn(mframe, LineColor(kRed));

    RooPlot* Yframe =  Y->frame(20);
    sigData_pp.plotOn(Yframe, LineColor(kBlack), MarkerStyle(24));
    sigPdf_pp->plotOn(Yframe, LineColor(kBlack));
    //sigPdf_pp_0m->plotOn(Yframe, LineColor(kRed));


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
    
    TString plotName = Form("plots/%s_acc%s", modeName.Data(), accName.Data());
    
    czz->SaveAs(Form("%s.eps", plotName.Data()));
    czz->SaveAs(Form("%s.png", plotName.Data()));
    
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

