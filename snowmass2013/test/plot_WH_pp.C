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
bool paperplots = true;

typedef enum{ZEROPLUS, ZEROMINUS, ALL} PlotLevel; 

void plot_WH_pp() {
    
    gROOT->ProcessLine(".L tdrstyle.C");
    setTDRStyle();
    gStyle->SetPadLeftMargin(0.16);
    
    // Declaration of the PDFs to use
    gROOT->ProcessLine(".L ../src/RooSpinZero_3D_WH_pp.cc++");
    gSystem->Load("../src/RooSpinZero_3D_WH_pp_cc.so");
    
    // these values define the generator couplings
    bool withAcc = false;
    TString accName = "false";
    if ( withAcc ) 
        accName = "true";

    PlotLevel plot = ALL;
    
    /*
    TString modeName_0p = "MG_pp_WH_jjbb_100k";
    TString modeName_0m = "MG_pp_WH_jjbb_100k";
    TString plotAppendix= "MG_pp_WH_jjbb";
    */

    TString modeName_0p = "pp_WH_jjbb_0p_toydata";
    TString modeName_0m = "pp_WH_jjbb_0m_toydata";
    TString plotAppendix= "MG_pp_WH_jjbb";
    

    if ( plot == ZEROPLUS )  plotAppendix = modeName_0p;
    if ( plot == ZEROMINUS ) plotAppendix = modeName_0m;


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
    RooRealVar* m= new RooRealVar("m","m_{VH} [GeV]", 150, 1000);
    RooRealVar* Y= new RooRealVar("Y","Rapidity (VH)", -4, 4);
    
    // additional variables
    // event weight
    RooRealVar* wt = new RooRealVar("wt", "wt", 0.0, 50);
    // Parameters
    RooRealVar* sqrts= new RooRealVar("sqrts","sqrts", 14000.);
    RooRealVar* mX = new RooRealVar("mX","mX", 125.);
    RooRealVar* mZ = new RooRealVar("mZ","mZ", 80.385);
    RooRealVar* gamZ = new RooRealVar("gamZ","gamZ",2.085);
    RooRealVar* R1Val = new RooRealVar("R1Val","R1Val", 0.);
    RooRealVar* R2Val = new RooRealVar("R2Val","R2Val", 0.);
    
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
    RooSpinZero_3D_WH_pp *sigPdf_pp = new RooSpinZero_3D_WH_pp("sigPdf_pp","sigPdf_pp",
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


    RooSpinZero_3D_WH_pp *sigPdf_pp0m = new RooSpinZero_3D_WH_pp("sigPdf_pp","sigPdf_pp",
                                                      *h1,*h2,*Phi, *m, *Y,
                                                      *sqrts, *mX, *mZ, *R1Val, *R2Val, para, 
                                                      *a1Val, *phi1Val, *a2Val, *phi2Val,*a3Val, *phi3Val, 
                                                      *g1Val0m, *g2Val0m, *g3Val0m, *g4Val0m, *g1Val0mIm, *g2Val0mIm, *g3Val0mIm, *g4Val0mIm,
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

    double rescale = 1.;

    RooPlot* h1frame =  h1->frame(20);
    h1frame->GetXaxis()->CenterTitle();
    h1frame->GetYaxis()->CenterTitle();
    h1frame->GetYaxis()->SetTitle(" ");
    
    if ( plot == ZEROPLUS || plot == ALL ) {
      sigData_pp0p.plotOn(h1frame, MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale));
      sigPdf_pp->plotOn(h1frame, LineColor(kRed),LineWidth(2), Normalization(rescale));
    }
    if ( plot == ZEROMINUS || plot == ALL ) {
      sigData_pp0m.plotOn(h1frame, MarkerColor(kBlue),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),DataError(RooAbsData::None));
      sigPdf_pp0m->plotOn(h1frame, LineColor(kBlue),LineWidth(2));
    }


    RooPlot* h2frame =  h2->frame(20);
    h2frame->GetXaxis()->CenterTitle();
    h2frame->GetYaxis()->CenterTitle();
    h2frame->GetYaxis()->SetTitle(" ");
    
    if ( plot == ZEROPLUS || plot == ALL ) {
      sigData_pp0p.plotOn(h2frame, MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale));
      sigPdf_pp->plotOn(h2frame, LineColor(kRed),LineWidth(2), Normalization(rescale));
    }
    if ( plot == ZEROMINUS || plot == ALL ) {
      sigData_pp0m.plotOn(h2frame, MarkerColor(kBlue),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),DataError(RooAbsData::None));
      sigPdf_pp0m->plotOn(h2frame, LineColor(kBlue),LineWidth(2));
    }
    
    RooPlot* phiframe =  Phi->frame(20);
    phiframe->GetXaxis()->CenterTitle();
    phiframe->GetYaxis()->CenterTitle();
    phiframe->GetYaxis()->SetTitle(" ");

    if ( plot == ZEROPLUS || plot == ALL ) {
      sigData_pp0p.plotOn(phiframe, MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale));
      sigPdf_pp->plotOn(phiframe, LineColor(kRed),LineWidth(2), Normalization(rescale));
    }
    if ( plot == ZEROMINUS || plot == ALL ) {
      sigData_pp0m.plotOn(phiframe, MarkerColor(kBlue),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),DataError(RooAbsData::None));
      sigPdf_pp0m->plotOn(phiframe, LineColor(kBlue),LineWidth(2));
    }

    RooPlot* mframe =  m->frame(40);
    mframe->GetXaxis()->CenterTitle();
    mframe->GetYaxis()->CenterTitle();
    mframe->GetYaxis()->SetTitle(" ");

    if ( plot == ZEROPLUS || plot == ALL ) {
      sigData_pp0p.plotOn(mframe, MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale));
      sigPdf_pp->plotOn(mframe, LineColor(kRed),LineWidth(2), Normalization(rescale));
    }
    if ( plot == ZEROMINUS || plot == ALL ) {
      sigData_pp0m.plotOn(mframe, MarkerColor(kBlue),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),DataError(RooAbsData::None));
      sigPdf_pp0m->plotOn(mframe, LineColor(kBlue),LineWidth(2));
    }

    RooPlot* Yframe =  Y->frame(20);
    Yframe->GetXaxis()->CenterTitle();
    Yframe->GetYaxis()->CenterTitle();
    Yframe->GetYaxis()->SetTitle(" ");
    
    if ( plot == ZEROPLUS || plot == ALL ) {
      sigData_pp0p.plotOn(Yframe, MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale));
      sigPdf_pp->plotOn(Yframe, LineColor(kRed),LineWidth(2), Normalization(rescale));
    }
    if ( plot == ZEROMINUS || plot == ALL ) {
      sigData_pp0m.plotOn(Yframe, MarkerColor(kBlue),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),DataError(RooAbsData::None));
      sigPdf_pp0m->plotOn(Yframe, LineColor(kBlue),LineWidth(2));
    }

    if ( paperplots ) {
      TCanvas* czz = new TCanvas( "czz", "czz", 600, 600); 

      h1frame->Draw();
      czz->SaveAs(Form("paperplots/h1_ppwh_%s_acc%s.eps", plotAppendix.Data(), accName.Data()));
      czz->SaveAs(Form("paperplots/h1_ppwh_%s_acc%s.eps", plotAppendix.Data(), accName.Data()));
      
      h2frame->Draw();
      czz->SaveAs(Form("paperplots/h2_ppwh_%s_acc%s.eps", plotAppendix.Data(), accName.Data()));
      czz->SaveAs(Form("paperplots/h2_ppwh_%s_acc%s.png", plotAppendix.Data(), accName.Data()));
      
      phiframe->Draw();
      czz->SaveAs(Form("paperplots/phi_ppwh_%s_acc%s.eps", plotAppendix.Data(), accName.Data()));
      czz->SaveAs(Form("paperplots/phi_ppwh_%s_acc%s.png", plotAppendix.Data(), accName.Data()));
      
      mframe->Draw();
      czz->SaveAs(Form("paperplots/mvh_ppwh_%s_acc%s.eps", plotAppendix.Data(), accName.Data()));
      czz->SaveAs(Form("paperplots/mvh_ppwh_%s_acc%s.png", plotAppendix.Data(), accName.Data()));
      
      Yframe->Draw();
      czz->SaveAs(Form("paperplots/Yvh_ppwh_%s_acc%s.eps", plotAppendix.Data(), accName.Data()));
      czz->SaveAs(Form("paperplots/Yvh_ppwh_%s_acc%s.png", plotAppendix.Data(), accName.Data()));
      


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

