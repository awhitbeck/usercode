/// This script makes a fit and draws the projections after fit when required
// run by root -l -b loadlib.C testfitilc.C
// 
#include "../src/PlaygroundZH.cc"

using namespace PlaygroundZHhelpers;

void testfitilc(bool pureToys=false, int ntoysperjob = 1, int seed_index = 1) {

  //   gROOT->ProcessLine(".x  loadLib.C");
  
  int random_seed = seed_index+487563; 
  RooRandom::randomGenerator()->SetSeed(random_seed);
  bool debug = false;
  float mH = 125.;
  float sqrtsVal = 250;
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
  bool drawprojections = false;
  bool drawbkg = false;
  bool dotoys = true;

  float lumi = 250; // in unit of fb
  if ( sqrtsVal == 350. ) lumi = 350; 
  if ( sqrtsVal == 500. ) lumi = 500; 
  if ( sqrtsVal == 1000. ) lumi = 1000; 

  float nsigperfb = 8;
  float nbkgperfb = 0.8;
  
  if ( sqrtsVal == 350. ) {
    nsigperfb = 1500./350.;
    nbkgperfb = 150./350.;
  }

  if ( sqrtsVal == 500. ) {
    nsigperfb = 2.;
    nbkgperfb = 0.2;
  }

  if ( sqrtsVal == 1000. ) {
    nsigperfb = 0.5;
    nbkgperfb = 0.05;
  }
  
  float nsigEvents = lumi*nsigperfb; 
  float nbkgEvents = lumi*nbkgperfb; 

  if ( debug ) {
    std::cout << "nsigEvents = " << nsigEvents << ",\t nbkgEvents = " << nbkgEvents << "\n";
  }

  
  TString modeName = Form("f_3_%.0fGeV_5M", sqrtsVal);
  //TString modeName = Form("fa3_%.0fGeV_5M", sqrtsVal);
  TString fileName = Form("Events_20130626/unweighted_unpol_%s_%s.root", modeName.Data(), accName.Data());
  if ( sqrtsVal == 350. ) {
    modeName = Form("model8_2M", sqrtsVal);
    fileName = Form("Events_20130701/unweighted_unpol_%s_%s.root", modeName.Data(), accName.Data());
  }
  if ( sqrtsVal == 1000. ) {
    modeName = Form("model9_1M", sqrtsVal);
    fileName = Form("Events_20130701/unweighted_unpol_%s_%s.root", modeName.Data(), accName.Data());
  }
  TString treeName = "SelectedTree";
  
  double g1Re = 1;
  double g1Im = 0.;
  double g2Re = 0.;
  double g2Im = 0.;
  double g3Re = 0.;
  double g3Im = 0.;
  double g4Re = 0.117316;
  if ( sqrtsVal == 350.)
    g4Re = 0.046863;
  if ( sqrtsVal == 500 ) 
    g4Re = 2.62636E-02;
  if ( sqrtsVal == 1000.) 
    g4Re = 0.011296;
  double g4Im = 0.; //

  // below will be recalculated once Playground is defined
  double fa2Val = 0.;
  double phia2Val = 0;
  double fa3Val = 0.;
  double phia3Val = 0;
  int parameterization = 2; 
  
  // 
  // start PlaygroundZH
  // 1) load signal and backgrounds
  // 2) Set the correct signal parameters
  // 
  // 
  
  if ( debug ) 
    std::cout << "Get Playground class started\n";
  
  srand (time(NULL));
  unsigned int random_seed_events = rand() %100000;
  
  PlaygroundZH test(mH, nsigEvents, nbkgEvents, random_seed_events, debug, parameterization, withAcceptance);
  if ( loadSigData ) 
    test.loadSigTree(fileName, treeName);
  if(debug) cout << test.data << endl;
  if ( loadBkgData ) {
    test.loadBkgTree(Form("bkgData/ee_ZZ_llbb_%.0fGeV_25M_%s.root", sqrtsVal, accName.Data()), treeName);
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
  

  double b2Val = -7.41520e-02;
  double cgausVal = -3.04678e-01;
  double sgausVal =  4.97867e-02;
  
  test.scalar->b2->setVal(b2Val);
  test.scalar->cgaus->setVal(cgausVal);
  test.scalar->sgaus->setVal(sgausVal);

  test.scalar->b2->setConstant(kTRUE);
  test.scalar->cgaus->setConstant(kTRUE);
  test.scalar->sgaus->setConstant(kTRUE);
  // 
  // set up background PDF
  // 

  // directly from 3D Histogram
  RooRealVar* costheta1 = new RooRealVar("costheta1","cos#theta_{1}",-1.,1.);
  RooRealVar* costheta2 = new RooRealVar("costheta2","cos#theta_{2}",-1.,1.);
  RooRealVar* phi = new RooRealVar("phi","#Phi", -TMath::Pi(),TMath::Pi());

  int nbins_h1 = 20;
  int nbins_h2 = 20;
  int nbins_phi = 15;
  
  if ( sqrtsVal > 250. && sqrtsVal < 1000.) {
    nbins_h1 = 50;
    nbins_h2 = 10;
    nbins_phi = 10;
  }

  if ( sqrtsVal == 1000. ) {
    nbins_h1 = 100;
    nbins_h2 = 10;
    nbins_phi = 10;
  }
  costheta1->setBins(nbins_h1);
  costheta2->setBins(nbins_h2);
  phi->setBins(nbins_phi);

  // From RooDataHist
  TChain *bkgTree = new TChain("SelectedTree");
  bkgTree->Add(Form("bkgData/ee_ZZ_llbb_%.0fGeV_25M_%s.root", sqrtsVal, accName.Data()));
  RooDataSet *bkgData = new RooDataSet("bkgData","bkgData",bkgTree,RooArgSet(*costheta1, *costheta2, *phi));
  RooDataHist *bkgHist = bkgData->binnedClone(0);
  RooHistPdf* bkgPdf = new RooHistPdf("bkgPdf", "bkgPdf", RooArgSet(*costheta1, *costheta2, *phi), *bkgHist);
  /*

  // From emphirical fit
  float h1pol2Val = 5.43096e-01;
  float h1pol4Val = 1.05202e+00;
  float h1pol6Val = 1.61973e-02;
  float h1pol8Val = 0.;
  float h2pol2Val = 0.246235;
  float phiconstVal = -1.96298e-02;
  float twophiconstVal =-1.37763e-01;
  
  if ( withAcceptance ) {
      h1pol2Val = 7.31558e-01;
      h1pol4Val = 3.65903e-01;
      h1pol6Val = 5.59806e-01;
      h2pol2Val = 2.15611e-01;
      phiconstVal = -1.98335e-02;
      twophiconstVal =-2.00570e-01;
  }

  // 500 GeV values  
  if ( sqrtsVal == 500. ) {
   h1pol2Val = 2.21788e+00;
   h1pol4Val = 2.61190e+00;
   h1pol6Val = -3.31434e+01;
   h1pol8Val = 5.19676e+01;
   h2pol2Val = 8.39666e-01;
   phiconstVal = 4.71922e-03;
   twophiconstVal = -5.36814e-02;
   
   if ( withAcceptance ) {
     h1pol2Val = 2.43918e+00;
     h1pol4Val = 9.76446e+00;
     h1pol6Val = -51;
     h1pol8Val = 61;
     h2pol2Val = 5.93641e-01;
     phiconstVal = 1.26742e-02;
     twophiconstVal = -1.82704e-01;
   }
 }
 
  RooRealVar* h1pol2  = new RooRealVar("h1pol2","h1pol2", -10, 10);
  RooRealVar* h1pol4  = new RooRealVar("h1pol4","h1pol4", -10, 10);
  RooRealVar* h1pol6  = new RooRealVar("h1pol6","h1pol6", -100, 100);
  RooRealVar* h1pol8  = new RooRealVar("h1pol8","h1pol8", -100, 100);
  RooRealVar* h2pol2  = new RooRealVar("h2pol2","h2pol2",  -10, 10);
  RooRealVar* phiconst  = new RooRealVar("phicons","phiconst",  -10, 10);
  RooRealVar* twophiconst  = new RooRealVar("twophicons","twophiconst", -10, 10);

  h1pol2->setVal(h1pol2Val);
  h1pol4->setVal(h1pol4Val);
  h1pol6->setVal(h1pol6Val);
  h1pol8->setVal(h1pol8Val);
  h2pol2->setVal(h2pol2Val);
  phiconst->setVal(phiconstVal);
  twophiconst->setVal(twophiconstVal);

  h1pol2->setConstant(kTRUE);
  h1pol4->setConstant(kTRUE);
  h1pol6->setConstant(kTRUE);
  h1pol8->setConstant(kTRUE);
  h2pol2->setConstant(kTRUE);
  phiconst->setConstant(kTRUE);
  twophiconst->setConstant(kTRUE);
  
  RooZZ_3D*  bkgPdf = new RooZZ_3D("bkgPdf","bkgPdf", *(test.costheta1), *(test.costheta2), *(test.phi),
				   *h1pol2,*h1pol4,*h1pol6,*h1pol8,*h2pol2,*phiconst,*twophiconst,withAcceptance);
	
  */
  if ( fitData ) { 
    RooFitResult *fitresults = test.fitData(test.scalar->PDF, bkgPdf);
  }
  
  // 
  // Do puretoy experiments 
  // 

  if ( dotoys ) {
    
    TFile *toyresults = new TFile(Form("toyresults_%s_acc%s_%i.root", isPureName.Data(), accName.Data(), random_seed), "RECREATE");
    gROOT->cd();
    
    TTree *tree_fit = new TTree("fittree", "fittree");
    
    Float_t m_fa2, m_fa2_err, m_fa2_pull;
    Float_t m_fa3, m_fa3_err, m_fa3_pull;
    Float_t m_phia2, m_phia2_err, m_phia2_pull;
    Float_t m_phia3, m_phia3_err, m_phia3_pull;
    
    Float_t m_g2Re, m_g2Re_err, m_g2Re_pull;
    Float_t m_g2Im, m_g2Im_err, m_g2Im_pull;
    Float_t m_g4Re, m_g4Re_err, m_g4Re_pull;
    Float_t m_g4Im, m_g4Im_err, m_g4Im_pull;

    Float_t m_nsig, m_nsig_err, m_nsig_pull;
    Float_t m_nbkg, m_nbkg_err, m_nbkg_pull;

    int m_status;
    
	
    tree_fit->Branch("fa2",        &m_fa2,        "fa2/F");
    tree_fit->Branch("fa2_err",    &m_fa2_err,    "fa2_err/F");
    tree_fit->Branch("fa2_pull",   &m_fa2_pull,   "fa2_pull/F");

    tree_fit->Branch("fa3",        &m_fa3,        "fa3/F");
    tree_fit->Branch("fa3_err",    &m_fa3_err,    "fa3_err/F");
    tree_fit->Branch("fa3_pull",   &m_fa3_pull,   "fa3_pull/F");

    tree_fit->Branch("phia2",      &m_phia2,      "phia2/F");
    tree_fit->Branch("phia2_err",  &m_phia2_err,  "phia2_err/F");
    tree_fit->Branch("phia2_pull", &m_phia2_pull, "phia2_pull/F");

    tree_fit->Branch("phia3",      &m_phia3,      "phia3/F");
    tree_fit->Branch("phia3_err",  &m_phia3_err,  "phia3_err/F");
    tree_fit->Branch("phia3_pull", &m_phia3_pull, "phia3_pull/F");

    tree_fit->Branch("g2Re",       &m_g2Re,       "g2Re/F");
    tree_fit->Branch("g2Re_err",   &m_g2Re_err,   "g2Re_err/F");
    tree_fit->Branch("g2Re_pull",  &m_g2Re_pull,  "g2Re_pull/F");

    tree_fit->Branch("g2Im",       &m_g2Im,       "g2Im/F");
    tree_fit->Branch("g2Im_err",   &m_g2Im_err,   "g2Im_err/F");
    tree_fit->Branch("g2Im_pull",  &m_g2Im_pull,  "g2Im_pull/F");
    
    tree_fit->Branch("g4Re",       &m_g4Re,       "g4Re/F");
    tree_fit->Branch("g4Re_err",   &m_g4Re_err,   "g4Re_err/F");
    tree_fit->Branch("g4Re_pull",  &m_g4Re_pull,  "g4Re_pull/F");

    tree_fit->Branch("g4Im",       &m_g4Im,       "g4Im/F");
    tree_fit->Branch("g4Im_err",   &m_g4Im_err,   "g4Im_err/F");
    tree_fit->Branch("g4Im_pull",  &m_g4Im_pull,  "g4Im_pull/F");

    tree_fit->Branch("nsig",       &m_nsig,       "nsig/F");
    tree_fit->Branch("nsig_err",   &m_nsig_err,   "nsig_err/F");
    tree_fit->Branch("nsig_pull",  &m_nsig_pull,  "nsig_pull/F");

    tree_fit->Branch("nbkg",       &m_nbkg,       "nbkg/F");
    tree_fit->Branch("nbkg_err",   &m_nbkg_err,   "nbkg_err/F");
    tree_fit->Branch("nbkg_pull",  &m_nbkg_pull,  "nbkg_pull/F");

    tree_fit->Branch("status",     &m_status,        "status/I");
    
    // do toys
    for (int i = 0; i < ntoysperjob; i++) {
      if  ( i%100 == 0 ) {
	std::cout << "doing toy " << i << "\n";
      }
      // set initial values
      test.scalar->fa2->setVal(fa2Val);
      test.scalar->fa3->setVal(fa3Val);
      test.scalar->phia2->setVal(phia2Val);
      test.scalar->phia3->setVal(phia3Val);
      test.nsig->setVal(nsigEvents);
      test.nbkg->setVal(nbkgEvents);

      //test.scalar->fa3->setConstant(kTRUE);
      //test.scalar->phia3->setConstant(kTRUE);
      test.nsig->setConstant(kTRUE);
      test.nbkg->setConstant(kTRUE);




      int toy_index = i + seed_index * ntoysperjob;

      if(test.generate(test.scalar->PDF, bkgPdf, toy_index, pureToys)!=kNoError) break;

      RooFitResult *toyfitresults = test.fitData(test.scalar->PDF, bkgPdf, true, 1);

      if ( debug ) {
	std::cout << "toy trial " << i << "\n"; 
	if ( parameterization == 2 ) {
	  std::cout << "fa2 = " << test.scalar->fa2->getVal() << " +/- " << test.scalar->fa2->getError() << "\n";
	  std::cout << "phia2 = " << test.scalar->phia2->getVal() << " +/- " << test.scalar->phia2->getError() << "\n";
	  std::cout << "fa3 = " << test.scalar->fa3->getVal() << " +/- " << test.scalar->fa3->getError() << "\n";
	  std::cout << "phia3 = " << test.scalar->phia3->getVal() << " +/- " << test.scalar->phia3->getError() << "\n";
	}
	if ( parameterization == 1 ) {
	  std::cout << "Re(g2) = " << test.scalar->g2Val->getVal() << "+/-" << test.scalar->g2Val->getError() << "\n";
	  std::cout << "Im(g2) = " << test.scalar->g2ValIm->getVal() << "+/-" << test.scalar->g2ValIm->getError() << "\n";
	  std::cout << "Re(g4) = " << test.scalar->g4Val->getVal() << "+/-" << test.scalar->g4Val->getError() << "\n";
	  std::cout << "Im(g4) = " << test.scalar->g4ValIm->getVal() << "+/-" << test.scalar->g4ValIm->getError() << "\n";
	}
	std::cout << "nsig = " << test.nsig->getVal() << " +/- " << test.nsig->getError() << "\n";
	std::cout << "nbkg = " << test.nbkg->getVal() << " +/- " << test.nbkg->getError() << "\n";
	std::cout << "fit status " << toyfitresults->status() << "\n";
      }
      
      m_fa2 = test.scalar->fa2->getVal();
      m_fa2_err = test.scalar->fa2->getError();
      m_fa2_pull = m_fa2_err > 0. ? (test.scalar->fa2->getVal() - fa2Val ) / test.scalar->fa2->getError() : 0.;

      m_phia2 = test.scalar->phia2->getVal();
      m_phia2_err = test.scalar->phia2->getError();
      m_phia2_pull = m_phia2_err > 0. ? (test.scalar->phia2->getVal() - phia2Val ) / test.scalar->phia2->getError() : 0.;

      m_fa3 = test.scalar->fa3->getVal();
      m_fa3_err = test.scalar->fa3->getError();
      m_fa3_pull = m_fa3_err > 0. ? (test.scalar->fa3->getVal() - fa3Val ) / test.scalar->fa3->getError() : 0.;

      m_phia3 = test.scalar->phia3->getVal();
      m_phia3_err = test.scalar->phia3->getError();
      m_phia3_pull = m_phia3_err > 0. ? (test.scalar->phia3->getVal() - phia3Val ) / test.scalar->phia3->getError() : 0.;
      
      m_g2Re = test.scalar->g2Val->getVal();
      m_g2Re_err = test.scalar->g2Val->getError();
      m_g2Re_pull = m_g2Re_err >0. ? (test.scalar->g2Val->getVal() - g2Re ) / test.scalar->g2Val->getError() : 0.;

      m_g2Im = test.scalar->g2ValIm->getVal();
      m_g2Im_err = test.scalar->g2ValIm->getError();
      m_g2Im_pull = (test.scalar->g2ValIm->getVal() - g2Im ) / test.scalar->g2ValIm->getError();

      m_g4Re = test.scalar->g4Val->getVal();
      m_g4Re_err = test.scalar->g4Val->getError();
      m_g4Re_pull = (test.scalar->g4Val->getVal() - g4Re ) / test.scalar->g4Val->getError();

      m_g4Im = test.scalar->g4ValIm->getVal();
      m_g4Im_err = test.scalar->g4ValIm->getError();
      m_g4Im_pull = (test.scalar->g4ValIm->getVal() - g4Im ) / test.scalar->g4ValIm->getError();

      m_nsig = test.nsig->getVal();
      m_nsig_err = test.nsig->getError();
      m_nsig_pull = (test.nsig->getVal() - nsigEvents ) / test.nsig->getError();

      m_nbkg = test.nbkg->getVal();
      m_nbkg_err = test.nbkg->getError();
      m_nbkg_pull = (test.nbkg->getVal() - nbkgEvents ) / test.nbkg->getError();

      m_status = toyfitresults->status(); 
      
      tree_fit->Fill();
    }
    
    // Save toy results
    toyresults->cd();
    tree_fit->Write();
    toyresults->Close();
  }
  
  // 
  // Draw projetions of the loaded data
  // 

  if ( drawprojections ) {

    int nbins = 20;
    if ( drawbkg ) nbins = 10; 

    TCanvas *c1 = new TCanvas("c1","c1",1200, 500);
    c1->Divide(3);
    
    c1->cd(1);
    test.projectPDF(kcostheta1, test.scalar->PDF, bkgPdf, nbins, dotoys, drawbkg);
    
    c1->cd(2);
    test.projectPDF(kcostheta2, test.scalar->PDF, bkgPdf, nbins, dotoys, drawbkg);
    
    c1->cd(3);
    test.projectPDF(kphi, test.scalar->PDF, bkgPdf, nbins, dotoys, drawbkg);

    TString fitName = "nofit";
    if ( fitData ) 
      fitName = "afterfit";
    if ( dotoys ) 
      fitName = "toy";

    c1->SaveAs(Form("ilcplots/projection_%s_%s.eps", fitName.Data(), modeName.Data()));
    c1->Print(Form("ilcplots/projection_%s_%s.png", fitName.Data(), modeName.Data()));

    delete c1; 
  }


}
