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
    
    gROOT->ProcessLine(".L tdrstyle.C");
    setTDRStyle();
    gROOT->ForceStyle(); 
     
    gROOT->ProcessLine(".L ../src/RooSpinZero_3D_ZH_Acc.cc+");
    gSystem->Load("../src/RooSpinZero_3D_ZH_Acc.cc");

    // Declaration of the PDFs to use
    gROOT->ProcessLine(".L ../src/RooZZ_3D.cc+");
    gSystem->Load("../src/RooZZ_3D.cc");

    // these values define the generator couplings
    bool withAcc = false;
    TString accName = "false";
    if ( withAcc ) 
      accName = "true";
    TString modeName = Form("ee_ZZ_llbb_%.0fGeV_25M", sqrtsVal);
    
    // Observables (3D)
    RooRealVar* h1 = new RooRealVar("costheta1","cos#theta_{1}",0, -1,1);
    RooRealVar* h2 = new RooRealVar("costheta2","cos#theta_{2}",0, -1,1);
    RooRealVar* Phi = new RooRealVar("phi","#Phi",0, -TMath::Pi(),TMath::Pi());
    
    h1->setBins(40);
    h2->setBins(40);
    Phi->setBins(40);
    
    /*
    float h1pol2Val = 5.43096e-01;
    float h1pol4Val = 1.05202e+00;
    float h1pol6Val = 1.61973e-02;
    float h1pol8Val = 0.;
    float h2pol2Val = 0.246235;
    float phiconstVal = -1.96298e-02;
    float twophiconstVal =-1.37763e-01;

    float h1pol2Val_acc = 7.31558e-01;
    float h1pol4Val_acc = 3.65903e-01;
    float h1pol6Val_acc = 5.59806e-01;
    float h2pol2Val_acc = 2.15611e-01;
    float phiconstVal_acc = -1.98335e-02;
    float twophiconstVal_acc =-2.00570e-01;
    float h1pol8Val_acc = 0.;
    
    if ( sqrtsVal == 500. ) {
      
      h1pol2Val = 2.21788e+00;
      h1pol4Val = 2.61190e+00;
      h1pol6Val = -3.31434e+01;
      h1pol8Val = 5.19676e+01;
      h2pol2Val = 8.39666e-01;
      phiconstVal = 4.71922e-03;
      twophiconstVal = -5.36814e-02;
      
      h1pol2Val_acc = 2.43918e+00;
      h1pol4Val_acc = 9.76446e+00;
      h1pol6Val_acc = -51;
      h1pol8Val_acc = 61;
      h2pol2Val_acc = 5.93641e-01;
      phiconstVal_acc = 1.26742e-02;
      twophiconstVal_acc = -1.82704e-01;
      
    }
    
    
    RooRealVar* h1pol2  = new RooRealVar("h1pol2","h1pol2", h1pol2Val);
    RooRealVar* h1pol4  = new RooRealVar("h1pol4","h1pol4", h1pol4Val);
    RooRealVar* h1pol6  = new RooRealVar("h1pol6","h1pol6", h1pol6Val);
    RooRealVar* h1pol8  = new RooRealVar("h1pol8","h1pol8", h1pol8Val);
    RooRealVar* h2pol2  = new RooRealVar("h2pol2","h2pol2", h2pol2Val);
    RooRealVar* phiconst  = new RooRealVar("phicons","phiconst", phiconstVal);
    RooRealVar* twophiconst  = new RooRealVar("twophicons","twophiconst", twophiconstVal);

    // set the PDF
    RooZZ_3D *bkgPdf = new RooZZ_3D("bkgPdf","bkgPdf", *h1,*h2,*Phi,
				    *h1pol2,*h1pol4,*h1pol6,*h1pol8,*h2pol2,*phiconst,*twophiconst,false);
    
    RooRealVar* h1pol2_acc = new RooRealVar("h1pol2_acc","h1pol2_acc", h1pol2Val_acc);
    RooRealVar* h1pol4_acc = new RooRealVar("h1pol4_acc","h1pol4_acc", h1pol4Val_acc);
    RooRealVar* h1pol6_acc = new RooRealVar("h1pol6_acc","h1pol6_acc", h1pol6Val_acc);
    RooRealVar* h1pol8_acc = new RooRealVar("h1pol8_acc","h1pol8_acc", h1pol8Val_acc);
    RooRealVar* h2pol2_acc = new RooRealVar("h2pol2_acc","h2pol2_acc", h2pol2Val_acc);
    RooRealVar* phiconst_acc = new RooRealVar("phicons_acc","phiconst_acc", phiconstVal_acc);
    RooRealVar* twophiconst_acc = new RooRealVar("twophicons","twophiconst_acc", twophiconstVal_acc);

    // set the PDF
    RooZZ_3D *bkgPdf_acc = new RooZZ_3D("bkgPdf_acc","bkgPdf_acc", *h1,*h2,*Phi,
					*h1pol2_acc,*h1pol4_acc,*h1pol6_acc,*h1pol8_acc,*h2pol2_acc,*phiconst_acc,*twophiconst_acc,true);

    */
    
    // data points
    TChain *bkgTree = new TChain("SelectedTree");
    bkgTree->Add(Form("bkgData/%s_false.root", modeName.Data()));
    RooDataSet *bkgData = new RooDataSet("bkgData","bkgData",bkgTree,RooArgSet(*h1, *h2, *Phi));
    
    TChain *bkgTree_acc = new TChain("SelectedTree");
    bkgTree_acc->Add(Form("bkgData/%s_true.root", modeName.Data()));
    RooDataSet *bkgData_acc = new RooDataSet("bkgData_acc","bkgData_acc",bkgTree_acc,RooArgSet(*h1, *h2, *Phi));

    RooDataHist *bkgHist = bkgData->binnedClone(0);
    RooHistPdf* bkgPdf = new RooHistPdf("bkgPdf", "bkgPdf", RooArgSet(*h1, *h2, *Phi), *bkgHist);

    RooDataHist *bkgHist_acc = bkgData_acc->binnedClone(0);
    RooHistPdf* bkgPdf_acc = new RooHistPdf("bkgPdf_acc", "bkgPdf_acc", RooArgSet(*h1, *h2, *Phi), *bkgHist_acc);
    
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
    double g4Gen =  0; //0.117316;
    
    if ( sqrtsVal == 500. ) 
      g4Gen = 2.62636E-02;

    double g4ImGen = 0.;
    double fa2Gen = 0.;
    double phia2Gen = 0.;
    double fa3Gen = 0.;
    double phia3Gen = 0.;

    calcfractionphase(sqrtsVal, g1Gen, g1ImGen, g2Gen, g2ImGen, g4Gen, g4ImGen, fa2Gen, fa3Gen, phia2Gen, phia3Gen);
    

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
    int para = 2;
     
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

    fa2->setConstant(kTRUE);
    fa3->setConstant(kTRUE);
    phia2->setConstant(kTRUE);
    phia3->setConstant(kTRUE);
    g1Val->setConstant(kTRUE);
    g1ValIm->setConstant(kTRUE);
    g2Val->setConstant(kTRUE);
    g2ValIm->setConstant(kTRUE);
    g3Val->setConstant(kTRUE);
    g3ValIm->setConstant(kTRUE);
    g4Val->setConstant(kTRUE);
    g4ValIm->setConstant(kTRUE);
    a1Val->setConstant(kTRUE);
    phi1Val->setConstant(kTRUE);
    a2Val->setConstant(kTRUE);
    phi2Val->setConstant(kTRUE);
    a3Val->setConstant(kTRUE);
    phi3Val->setConstant(kTRUE);
    R1Val->setConstant(kTRUE);
    R2Val->setConstant(kTRUE);
    mX->setConstant(kTRUE);    
    mZ->setConstant(kTRUE);    
    gamZ->setConstant(kTRUE);    
    sqrts->setConstant(kTRUE);    
    
    double b2Val = -7.41520e-02;
    double cgausVal = -3.04678e-01;
    double sgausVal =  4.97867e-02;

    RooRealVar* b2  = new RooRealVar("b2","b2",b2Val,-10,10);
    RooRealVar* cgaus  = new RooRealVar("cgaus","cgaus",cgausVal, -10, 10);
    RooRealVar* sgaus  = new RooRealVar("sgaus","sgaus",sgausVal,0,1);


    b2->setConstant(kTRUE);
    cgaus->setConstant(kTRUE);
    sgaus->setConstant(kTRUE);


    // set the PDF
    RooSpinZero_3D_ZH_Acc *sigPdf = new RooSpinZero_3D_ZH_Acc("sigPdf","sigPdf",
							      *h1,*h2,*Phi,
							      *sqrts, *mX, *mZ, *R1Val, *R2Val, para, 
							      *a1Val, *phi1Val, *a2Val, *phi2Val,*a3Val, *phi3Val, 
							      *g1Val, *g2Val, *g3Val, *g4Val, *g1ValIm, *g2ValIm, *g3ValIm, *g4ValIm,
							      *fa2, *fa3, *phia2, *phia3, 
							      *b2, *cgaus, *sgaus, withAcc);


    RooSpinZero_3D_ZH_Acc *sigPdf_acc = new RooSpinZero_3D_ZH_Acc("sigPdf","sigPdf",
							      *h1,*h2,*Phi,
							      *sqrts, *mX, *mZ, *R1Val, *R2Val, para, 
							      *a1Val, *phi1Val, *a2Val, *phi2Val,*a3Val, *phi3Val, 
							      *g1Val, *g2Val, *g3Val, *g4Val, *g1ValIm, *g2ValIm, *g3ValIm, *g4ValIm,
							      *fa2, *fa3, *phia2, *phia3, 
							      *b2, *cgaus, *sgaus, true);

    
    // 
    // Read input file
    // 
    TFile *fin_signal = new TFile(Form("Events_20130618/unweighted_unpol_g1_1M_%s.root", accName.Data()));
    TTree* tin_signal = (TTree*) fin_signal->Get("SelectedTree");
    std::cout << "signal events is " << tin_signal->GetEntries() << "\n";
    RooDataSet sigData = RooDataSet("sigData","sigData",tin_signal,RooArgSet(*h1,*h2,*Phi));


    TFile *fin_signal_acc = new TFile(Form("Events_20130618/unweighted_unpol_g1_1M_true.root"));
    TTree* tin_signal_acc = (TTree*) fin_signal_acc->Get("SelectedTree");
    std::cout << "signal events is " << tin_signal_acc->GetEntries() << "\n";
    RooDataSet sigData_acc = RooDataSet("sigData_acc","sigData_acc",tin_signal_acc,RooArgSet(*h1,*h2,*Phi));

    
    
    
    // RooFitResult* sigToyfitresults =  sigPdf->fitTo(sigData);



    // 
    // total signal+bkg Pdf
    // 
    RooRealVar* nsig = new RooRealVar("nsig","number of signal events",  2e+3);
    RooRealVar* nbkg = new RooRealVar("nbkg","number of background events", 200.);
    RooAddPdf* totalPdf = new RooAddPdf("totalPdf","totalPdf",RooArgList(*sigPdf,*bkgPdf),RooArgList(*nsig,*nbkg));    
    
    // 
    // toy generation
    // 
    RooDataSet* sigToyData = sigPdf->generate(RooArgSet(*h1,*h2,*Phi), nsig->getVal());
    // RooDataSet* bkgToyData = bkgPdf->generate(RooArgSet(*h1,*h2,*Phi), nbkg->getVal());
    RooDataSet* toyData = totalPdf->generate(RooArgSet(*h1,*h2,*Phi), nsig->getVal()+nbkg->getVal());
    
    // 
    // Plotting frames
    // 
    
    double rescale = 0.05;

    
    //
    // h1 
    // 
    RooPlot* h1frame =  h1->frame(40);
    h1frame->GetXaxis()->CenterTitle();
    h1frame->GetYaxis()->CenterTitle();
    h1frame->GetYaxis()->SetTitle(" ");
    
    double ymax_h1;                                                                                                                
    TH1F *h1_test = new TH1F("h1_test", "h1_test", 40, -1, 1);                                                                  
    tin_signal->Project("h1_test", "costheta1");                                                                                         
    ymax_h1 = h1_test->GetMaximum(); 
    tin_signal->Project("h1_test", "costheta1");                                                                                         
    ymax_h1 = ymax_h1 > h1_test->GetMaximum() ? ymax_h1 : h1_test->GetMaximum();
    
    sigData.plotOn(h1frame, LineColor(kRed), MarkerColor(kRed), MarkerStyle(24), XErrorSize(0), DataError(RooAbsData::None), Rescale(rescale));
    sigPdf->plotOn(h1frame, LineColor(kRed), Normalization(rescale));

    sigData_acc.plotOn(h1frame, LineColor(kRed), MarkerColor(kRed), MarkerStyle(25), XErrorSize(0), DataError(RooAbsData::None), Rescale(rescale));
    sigPdf_acc->plotOn(h1frame, LineColor(kRed), LineStyle(kDashed), Normalization(rescale));

    bkgData->plotOn(h1frame, LineColor(kBlack), MarkerColor(kBlack), MarkerStyle(20), XErrorSize(0), DataError(RooAbsData::None), Rescale(rescale*2.5));
    bkgPdf->plotOn(h1frame, LineColor(kBlack), Normalization(rescale*2.5));

    bkgData_acc->plotOn(h1frame, LineColor(kBlack), MarkerColor(kBlack), MarkerStyle(21), XErrorSize(0), DataError(RooAbsData::None), Rescale(rescale*2.5));
    bkgPdf_acc->plotOn(h1frame, LineColor(kBlack), LineStyle(kDashed), Normalization(rescale*2.5));

    if ( rescale != 1. )                                                                                                           
      h1frame->GetYaxis()->SetRangeUser(0, ymax_h1  * rescale * 1.1);                                                                                 
    
    //
    // h2 
    // 

    RooPlot* h2frame =  h2->frame(40);
    h2frame->GetXaxis()->CenterTitle();
    h2frame->GetYaxis()->CenterTitle();
    h2frame->GetYaxis()->SetTitle(" ");
    
    double ymax_h2;                                                                                                                
    TH1F *h2_test = new TH1F("h2_test", "h2_test", 40, -1, 1);
    tin_signal->Project("h2_test", "costheta2");  
    ymax_h2 = h2_test->GetMaximum(); 
    tin_signal->Project("h2_test", "costheta2");                                                                                         
    ymax_h2 = ymax_h2 > h2_test->GetMaximum() ? ymax_h2 : h2_test->GetMaximum();
    
    sigData.plotOn(h2frame, LineColor(kRed), MarkerColor(kRed), MarkerStyle(24), XErrorSize(0), DataError(RooAbsData::None), Rescale(rescale));
    sigPdf->plotOn(h2frame, LineColor(kRed), Normalization(rescale));

    sigData_acc.plotOn(h2frame, LineColor(kRed), MarkerColor(kRed), MarkerStyle(25), XErrorSize(0), DataError(RooAbsData::None), Rescale(rescale));
    sigPdf_acc->plotOn(h2frame, LineColor(kRed), LineStyle(kDashed), Normalization(rescale));

    bkgData->plotOn(h2frame, LineColor(kBlack), MarkerColor(kBlack), MarkerStyle(20), XErrorSize(0), DataError(RooAbsData::None), Rescale(rescale*2.5));
    bkgPdf->plotOn(h2frame, LineColor(kBlack), Normalization(rescale*2.5));

    bkgData_acc->plotOn(h2frame, LineColor(kBlack), MarkerColor(kBlack), MarkerStyle(21), XErrorSize(0), DataError(RooAbsData::None), Rescale(rescale*2.5));
    bkgPdf_acc->plotOn(h2frame, LineColor(kBlack), LineStyle(kDashed), Normalization(rescale*2.5));

    if ( rescale != 1. )                                                                                                           
      h2frame->GetYaxis()->SetRangeUser(0, ymax_h2  * rescale * 1.1);   


    //
    // phi 
    // 

    RooPlot* phiframe =  Phi->frame(40);
    phiframe->GetXaxis()->CenterTitle();
    phiframe->GetYaxis()->CenterTitle();
    phiframe->GetYaxis()->SetTitle(" ");
    
    double ymax_phi;                                                                                                                
    TH1F *phi_test = new TH1F("phi_test", "phi_test", 40, -3.2, 3.2);                                                                  
    tin_signal->Project("phi_test", "phi");                                                                                         
    ymax_phi = phi_test->GetMaximum(); 
    tin_signal->Project("phi_test", "phi");                                                                                         
    ymax_phi = ymax_phi > phi_test->GetMaximum() ? ymax_phi : phi_test->GetMaximum();
    
    sigData.plotOn(phiframe, LineColor(kRed), MarkerColor(kRed), MarkerStyle(24), XErrorSize(0), DataError(RooAbsData::None), Rescale(rescale));
    sigPdf->plotOn(phiframe, LineColor(kRed), Normalization(rescale));

    sigData_acc.plotOn(phiframe, LineColor(kRed), MarkerColor(kRed), MarkerStyle(25), XErrorSize(0), DataError(RooAbsData::None), Rescale(rescale));
    sigPdf_acc->plotOn(phiframe, LineColor(kRed), LineStyle(kDashed), Normalization(rescale));

    bkgData->plotOn(phiframe, LineColor(kBlack), MarkerColor(kBlack), MarkerStyle(20), XErrorSize(0), DataError(RooAbsData::None), Rescale(rescale*2.5));
    bkgPdf->plotOn(phiframe, LineColor(kBlack), Normalization(rescale*2.5));

    bkgData_acc->plotOn(phiframe, LineColor(kBlack), MarkerColor(kBlack), MarkerStyle(21), XErrorSize(0), DataError(RooAbsData::None), Rescale(rescale*2.5));
    bkgPdf_acc->plotOn(phiframe, LineColor(kBlack), LineStyle(kDashed), Normalization(rescale*2.5));

    if ( rescale != 1. )                                                                                                           
      phiframe->GetYaxis()->SetRangeUser(0, ymax_phi  * rescale * 1.1);   


    TCanvas* czz = new TCanvas( "czz", "czz", 600, 600 );
    czz->cd();
    
    h1frame->Draw();
    czz->SaveAs(Form("paperplots/h1_ee_%.0fGeV_acc.eps", sqrtsVal));
    czz->SaveAs(Form("paperplots/h1_ee_%.0fGeV_acc.png", sqrtsVal));

    czz->Clear();
    h2frame->Draw();
    czz->SaveAs(Form("paperplots/h2_ee_%.0fGeV_acc.eps", sqrtsVal));
    czz->SaveAs(Form("paperplots/h2_ee_%.0fGeV_acc.png", sqrtsVal));
    
    czz->Clear();
    phiframe->Draw();
    czz->SaveAs(Form("paperplots/phi_ee_%.0fGeV_acc.eps", sqrtsVal));
    czz->SaveAs(Form("paperplots/phi_ee_%.0fGeV_acc.png", sqrtsVal));


    delete bkgTree;
    delete bkgTree_acc;
    
    
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

