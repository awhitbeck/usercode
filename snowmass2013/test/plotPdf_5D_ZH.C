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
void calcfractionphase(double sqrts, double g1Re,  double g1Im,  double g2Re,   double g2Im,  double g4Re,  double g4Im, 
		       double & fa2, double & fa3, double & phia2, double & phia3);

void plotPdf_5D_ZH(float mH = 125, float sqrtsVal =  250.) {
    
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

    double P_ele = 0.;
    TString beamPolarName = "unpol";
    if ( P_ele == -1. ) beamPolarName = Form("LL");
    if ( P_ele  == 1. ) beamPolarName = Form("RR");
    
    double r1 = 0.15; // this is related to the Z->ll decay
    double r1val = (r1 + P_ele ) / (1 + r1*P_ele); 
    
    // these values define the generator couplings
    TString modeName = Form("g1_1M", sqrtsVal);
    TString fileName = Form("Events_20130618/unweighted_%s_%s_false.root", beamPolarName.Data(), modeName.Data());
    
    double g1Gen =   1;
    double g1ImGen = 0.;
    double g2Gen =   0.; 
    double g2ImGen = 0.; 
    double g3Gen =   0.;
    double g3ImGen = 0.;
    double g4Gen =  0; //0.046863; // 0.0112957; //0.117316; 
    if ( modeName.Contains("fa3")  ) {
      g4Gen =  0.83265;
    } 
    if ( sqrtsVal == 500. && modeName.Contains("f3") ) 
      g4Gen =  2.62636E-2;

    double g4ImGen = 0.; // 0.83265;
    double fa2Gen = 0.;
    double phia2Gen = 0.;
    double fa3Gen = 0.;
    double phia3Gen = 0.;

    calcfractionphase(sqrtsVal, g1Gen, g1ImGen, g2Gen, g2ImGen, g4Gen, g4ImGen, fa2Gen, fa3Gen, phia2Gen, phia3Gen);
    
    if ( drawbkg ) {
      fileName = ("lhefiles/ee_ZZ_llbb_false.root");
    } 
    
    // 
    // Read input file
    // 
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
    int para = kFracPhase_Gs;  // chose from kMagPhase_As, kRealImag_Gs, kFracPhase_Gs;
    // int para = kRealImag_Gs;  // chose from kFracPhase_Gs, kMagPhase_As;
     
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
    RooSpinZero_5D_ZH *myPDF = new RooSpinZero_5D_ZH("myPDF","myPDF",
						     *h1,*h2,*hs,*Phi,*Phi1,
						     *sqrts, *mX, *mZ, *R1Val, *R2Val, para, 
						     *a1Val, *phi1Val, *a2Val, *phi2Val,*a3Val, *phi3Val, 
						     *g1Val, *g2Val, *g3Val, *g4Val, *g1ValIm, *g2ValIm, *g3ValIm, *g4ValIm,
						     *fa2, *fa3, *phia2, *phia3);
    
    // get the data
    if ( weightedevents ) {
      RooDataSet dataTMP("dataTMP","dataTMP",tin,RooArgSet(*h1,*h2,*Phi,*hs,*Phi1,*wt));
      RooDataSet data = RooDataSet("data","data",RooArgList(*h1,*h2,*Phi,*hs,*Phi1,*wt), WeightVar("wt"), Import(dataTMP));
    } else {
      RooDataSet data = RooDataSet("data","data",tin,RooArgSet(*h1,*h2,*Phi,*hs,*Phi1));
    }

    
    // 
    // Generate toy Data
    // 
    
    //    int nsig = 1e+6;
    // RooDataSet* sigToyData = myPDF->generate(RooArgSet(*h1,*h2,*hs,*Phi,*Phi1), nsig);

    // 
    // Plotting frames
    // 
    
    RooPlot* h1frame =  h1->frame(20);
    data.plotOn(h1frame, LineColor(kBlack), MarkerStyle(24));
    //sigToyData.plotOn(h1frame, LineColor(kBlack), MarkerStyle(24));

    if ( !drawbkg )
      myPDF->plotOn(h1frame, LineColor(kBlack));

    RooPlot* h2frame =  h2->frame(20);
    data.plotOn(h2frame, LineColor(kBlack), MarkerStyle(24));
    //sigToyData.plotOn(h2frame, LineColor(kBlack), MarkerStyle(24));
    if ( !drawbkg ) 
      myPDF->plotOn(h2frame, LineColor(kBlack));

    RooPlot* hsframe =  hs->frame(20);
    data.plotOn(hsframe, LineColor(kBlack), MarkerStyle(24));
    //sigToyData.plotOn(hsframe, LineColor(kBlack), MarkerStyle(24));
    if ( !drawbkg )
      myPDF->plotOn(hsframe, LineColor(kBlack));

    RooPlot* phiframe =  Phi->frame(20);
    data.plotOn(phiframe, LineColor(kBlack), MarkerStyle(24));
    //sigToyData.plotOn(phiframe, LineColor(kBlack), MarkerStyle(24));
    if ( !drawbkg ) 
      myPDF->plotOn(phiframe, LineColor(kBlack));
    
    RooPlot* phi1frame =  Phi1->frame(20);
    data.plotOn(phi1frame, LineColor(kBlack), MarkerStyle(24));
    //sigToyData.plotOn(phi1frame, LineColor(kBlack), MarkerStyle(24));
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
    
    
    TString plotName = Form("plots/ZX_mX%.0f_sqrts%.0f_%s_%s", mH, sqrtsVal, modeName.Data(), beamPolarName.Data());
    
    if ( drawbkg ) 
      plotName = Form("plots/ZZ_sqrts%.0f", sqrtsVal);
    
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
 
  // ILC nubmers at 350 GeV at mH = 125 GeV
  if ( sqrts == 350. ) {
    sigma1_e = 1.48872; 
    sigma2_e = 125.387;  
    sigma4_e = 75.3199;
  }

  // ILC nubmers at 500 GeV at mH = 125 GeV
  if ( sqrts == 500. ) {
    sigma1_e = 2.57246; 
    sigma2_e = 516.556;  
    sigma4_e = 414.378;
  }

  // ILC nubmers at 1000 GeV at mH = 125 GeV
  if ( sqrts == 1000. ) {
    sigma1_e = 8.95721; 
    sigma2_e = 8208.91;  
    sigma4_e = 7800.2;
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

