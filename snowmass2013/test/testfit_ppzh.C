/// This script makes a fit and draws the projections after fit when required
// run by root -l -b loadlib.C testfitilc.C
// 
#include "../src/PlaygroundPPZH.cc"

using namespace PlaygroundPPZHhelpers;

void testfit_ppzh(bool pureToys=true, int ntoysperjob = 1, int seed_index = 0) {

  //   gROOT->ProcessLine(".x  loadLib.C");
  
  int random_seed = seed_index+487563; 
  RooRandom::randomGenerator()->SetSeed(random_seed);
  bool debug = true;
  float mH = 125.;
  float sqrtsVal = 14000;
  bool withAcceptance = false;

  TString accName = "false";
  if (withAcceptance )  accName = "true";

  TString isPureName = "embd";
  if ( pureToys ) isPureName = "pure";
    
  // 
  // specify inputs 
  // you need to go through these following settings and verify all the 
  // initial values
  // 
  bool loadSigData = true;
  bool loadBkgData = true;
  bool fitData = false;
  bool drawprojections = true;
  bool drawbkg = false;
  bool dotoys = false;

  float lumi = 3000; // in unit of fb
  float nsigperfb = 0.1;
  float nbkgperfb = 0.;
  float nsigEvents = lumi*nsigperfb; 
  float nbkgEvents = lumi*nbkgperfb; 


  if ( debug ) {
    std::cout << "nsigEvents = " << nsigEvents << ",\t nbkgEvents = " << nbkgEvents << "\n";
  }

  
  TString modeName = Form("pp_ZH_llbb_14TeV_1M");
  TString fileName = Form("samples/pp_ZH/%s_%s.root", modeName.Data(), accName.Data());
  TString treeName = "SelectedTree";
  
  double g1Re = 1;
  double g1Im = 0.;
  double g2Re = 0.;
  double g2Im = 0.;
  double g3Re = 0.;
  double g3Im = 0.;
  // double g4Re = 0.852604;
  double g4Re = 0.;
  double g4Im = 0.; //

  // below will be recalculated once Playground is defined
  double fa2Val = 0.;
  double phia2Val = 0;
  double fa3Val = 0.1;
  double phia3Val = 0;
  int parameterization = 2; 
  
  // 
  // start PlaygroundPPZH
  // 1) load signal and backgrounds
  // 2) Set the correct signal parameters
  // 
  // 
  
  if ( debug ) 
    std::cout << "Get Playground class started\n";
  
  srand (time(NULL));
  unsigned int random_seed_events = rand() %100000;
  
  PlaygroundPPZH test(mH, nsigEvents, nbkgEvents, random_seed_events, debug, parameterization, withAcceptance);
  if ( loadSigData ) 
    test.loadSigTree(fileName, treeName);
  if(debug) cout << test.data << endl;
  if ( loadBkgData ) {
    test.loadBkgTree(Form("samples/pp_ZH/unweighted_pp_ZZ_llbb_%s.root", accName.Data()), treeName);
  }
  
  // 
  // setting the siganl pdf
  // 
  
  // ScalarPdfFactoryZH *scalar = test.scalar;
  test.scalar->sqrts->setVal(sqrtsVal);
  test.scalar->sqrts->setConstant(kTRUE);
  test.calcfractionphase(sqrtsVal, g1Re, g1Im, g2Re, g2Im, g4Re, g4Im, fa2Val, fa3Val, phia2Val, phia3Val);
  
  if ( parameterization == 2 ) {
    
    test.scalar->fa2->setVal(fa2Val);
    test.scalar->fa2->setConstant(kTRUE);

    test.scalar->phia2->setVal(phia2Val);
    test.scalar->phia2->setRange(-2*TMath::Pi(), 2*TMath::Pi());
    test.scalar->phia2->setConstant(kTRUE);
    
    test.scalar->fa3->setVal(fa3Val);
    //test.scalar->fa3->setConstant(kTRUE);
    
    test.scalar->phia3->setVal(phia3Val);    
    test.scalar->phia3->setRange(-2*TMath::Pi(), 2*TMath::Pi());
    //test.scalar->phia3->setConstant(kTRUE);


  }

  if ( parameterization == 1 ) {
    
    test.scalar->g2Val->setVal(g2Re);
    // test.scalar->g2Val->setConstant(kTRUE);
 
    test.scalar->g2ValIm->setVal(g2Im);
    // test.scalar->g2ValIm->setConstant(kTRUE);

    test.scalar->g4Val->setConstant(kFALSE);
    test.scalar->g4Val->setVal(g4Re);

    test.scalar->g4ValIm->setVal(g4Im);
    // test.scalar->g4ValIm->setConstant(kTRUE);
    
  }
  

  // 
  // set up background PDF
  // 

  // directly from 3D Histogram
  RooRealVar* costheta1 = new RooRealVar("costheta1","cos#theta_{1}",-1.,1.);
  RooRealVar* costheta2 = new RooRealVar("costheta2","cos#theta_{2}",-1.,1.);
  RooRealVar* phi = new RooRealVar("phi","#Phi", -TMath::Pi(),TMath::Pi());
  RooRealVar* m= new RooRealVar("m","m", 150, 1000);
  RooRealVar* Y= new RooRealVar("Y","Y", -4, 4);
  
  int nbins_h1 = 5;
  int nbins_h2 = 5;
  int nbins_phi = 5;
  int nbins_m = 2;
  int nbins_Y = 2;

  costheta1->setBins(nbins_h1);
  costheta2->setBins(nbins_h2);
  phi->setBins(nbins_phi);
  m->setBins(nbins_m);
  Y->setBins(nbins_Y);
  
  // From RooDataHist
  TChain *bkgTree = new TChain("SelectedTree");
  bkgTree->Add(Form("samples/pp_ZH/unweighted_pp_ZZ_llbb_%s.root", accName.Data()));
  RooDataSet *bkgData = new RooDataSet("bkgData","bkgData",bkgTree,RooArgSet(*costheta1, *costheta2, *phi, *m, *Y));
  RooDataHist *bkgHist = bkgData->binnedClone(0);
  RooHistPdf* bkgPdf = new RooHistPdf("bkgPdf", "bkgPdf", RooArgSet(*costheta1, *costheta2, *phi, *m, *Y), *bkgHist);

  if ( fitData ) { 
    RooFitResult *fitresults = test.fitData(test.scalar->PDF, bkgPdf);
  }
  
  // 
  // Draw projetions of the loaded data
  // 

  if ( drawprojections ) {

    int nbins = 20;
    if ( drawbkg ) nbins = 10; 

    TCanvas *c1 = new TCanvas("c1","c1",1200, 800);
    c1->Divide(3,2);
    
    c1->cd(1);
    test.projectPDF(kcostheta1, test.scalar->PDF, bkgPdf, nbins, dotoys, drawbkg);
    
    c1->cd(2);
    test.projectPDF(kcostheta2, test.scalar->PDF, bkgPdf, nbins, dotoys, drawbkg);
    
    c1->cd(3);
    test.projectPDF(kphi, test.scalar->PDF, bkgPdf, nbins, dotoys, drawbkg);

    c1->cd(4);
    test.projectPDF(km, test.scalar->PDF, bkgPdf, nbins, dotoys, drawbkg);

    c1->cd(5);
    test.projectPDF(kY, test.scalar->PDF, bkgPdf, nbins, dotoys, drawbkg);

    TString fitName = "nofit";
    if ( fitData ) 
      fitName = "afterfit";
    if ( dotoys ) 
      fitName = "toy";

    c1->SaveAs(Form("plots_ppVH/projection_%s_%s.eps", fitName.Data(), modeName.Data()));
    c1->Print(Form("plots_ppVH/projection_%s_%s.png", fitName.Data(), modeName.Data()));

    delete c1; 
  }

  
  delete bkgTree;
  
}
