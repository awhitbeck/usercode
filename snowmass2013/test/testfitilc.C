/// This script makes a fit and draws the projections after fit when required
// run by root -l -b loadlib.C testfitilc.C
// 
#include "../src/PlaygroundZH.cc"

using namespace PlaygroundZHhelpers;

void testfitilc(bool pureToys=false, int ntoysperjob = 1, int seed_index=2) {

  //   gROOT->ProcessLine(".x  loadLib.C");
  
  int random_seed = seed_index+487563; 
  RooRandom::randomGenerator()->SetSeed(random_seed);
  bool debug = false;

  // 
  // specify inputs 
  // you need to go through these following settings and verify all the 
  // initial values
  // 
  bool loadData = true;
  bool fitData = false;
  bool drawprojections = false;
  bool testsingletoy = false;
  bool dotoys = true;

  float mH = 125.;
  TString modeName = Form("g1_p_g4_1M");
  TString fileName = Form("data/Events_20130618/unweighted_unpol_%s_false.root", modeName.Data());
  TString treeName = "SelectedTree";
  
  double g1Re = 1;
  double g1Im = 0.;
  double g2Re = 0.; //0.54071;
  double g2Im = 0.;
  double g3Re = 0.;
  double g3Im = 0.;
  double g4Re = 0.83265;
  double g4Im = 0.; //

  // below will be recalculated once Playground is defined
  double fa2Val = 0.;
  double phia2Val = 0;
  double fa3Val = 0.;
  double phia3Val = 0;
  
  // 
  //cout << "Get Playground class started" << endl;
  // 
  int parameterization = 2; 
  PlaygroundZH test(mH, debug, parameterization);
  if ( loadData ) 
    test.loadTree(fileName, treeName);
  if(debug) cout << test.data << endl;
  test.calcfractionphase(g1Re, g1Im, g2Re, g2Im, g4Re, g4Im, fa2Val, fa3Val, phia2Val, phia3Val);
  
  ScalarPdfFactoryZH *scalar = test.scalar;

  // 
  //cout << "define the parameters to be fitted and the initial values" << endl;
  // 

  if ( parameterization == 2 ) {
    
    scalar->fa2->setVal(fa2Val);
    scalar->fa2->setConstant(kTRUE);

    scalar->phia2->setVal(phia2Val);
    scalar->phia2->setRange(-2*TMath::Pi(), 2*TMath::Pi());
    scalar->phia2->setConstant(kTRUE);
    
    scalar->fa3->setVal(fa3Val);
    scalar->fa3->setConstant(kFALSE);
 
    scalar->phia3->setVal(phia3Val);    
    scalar->phia3->setRange(-2*TMath::Pi(), 2*TMath::Pi());
    scalar->phia3->setConstant(kFALSE);


  }

  if ( parameterization == 1 ) {
    
    scalar->g2Val->setVal(g2Re);
    // scalar->g2Val->setConstant(kTRUE);
 
    scalar->g2ValIm->setVal(g2Im);
    // scalar->g2ValIm->setConstant(kTRUE);

    scalar->g4Val->setConstant(kFALSE);
    scalar->g4Val->setVal(g4Re);

    scalar->g4ValIm->setVal(g4Im);
    // scalar->g4ValIm->setConstant(kTRUE);
    
  }
  
  if ( fitData ) { 
    RooFitResult *fitresults = test.fitData();
  }
  
  if ( testsingletoy ) {
    int nEvents = 2000;
    if(test.generate(nEvents, true)!=kNoError) break;
  }
  // 
  // Do puretoy experiments 
  // 

  if ( dotoys ) {
    
    float lumi = 2000; // in unit of fb
    float nsignalperfb = 8; 
    
    TFile *toyresults = new TFile(Form("toyresults_%i.root", random_seed), "RECREATE");
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

    // do toys
    for (int i = 0; i < ntoysperjob; i++) {
      if  ( i%100 == 0 ) {
	std::cout << "doing toy " << i << "\n";
      }
      // set initial values
      scalar->fa2->setVal(fa2Val);
      scalar->fa3->setVal(fa3Val);
      scalar->phia2->setVal(phia2Val);
      scalar->phia3->setVal(phia3Val);
      
      if(test.generate(lumi*nsignalperfb, pureToys)!=kNoError) break;

      RooFitResult *toyfitresults = test.fitData(true,-1);
      
      if ( debug ) {
	std::cout << "toy trial " << i << "\n"; 
	if ( parameterization == 2 ) {
	  std::cout << "fa2 = " << scalar->fa2->getVal() << " +/- " << scalar->fa2->getError() << "\n";
	  std::cout << "phia2 = " << scalar->phia2->getVal() << " +/- " << scalar->phia2->getError() << "\n";
	  std::cout << "fa3 = " << scalar->fa3->getVal() << " +/- " << scalar->fa3->getError() << "\n";
	  std::cout << "phia3 = " << scalar->phia3->getVal() << " +/- " << scalar->phia3->getError() << "\n";
	}
	if ( parameterization == 1 ) {
	  std::cout << "Re(g2) = " << scalar->g2Val->getVal() << "+/-" << scalar->g2Val->getError() << "\n";
	  std::cout << "Im(g2) = " << scalar->g2ValIm->getVal() << "+/-" << scalar->g2ValIm->getError() << "\n";
	  std::cout << "Re(g4) = " << scalar->g4Val->getVal() << "+/-" << scalar->g4Val->getError() << "\n";
	  std::cout << "Im(g4) = " << scalar->g4ValIm->getVal() << "+/-" << scalar->g4ValIm->getError() << "\n";
	}
      }
      
      m_fa2 = scalar->fa2->getVal();
      m_fa2_err = scalar->fa2->getError();
      m_fa2_pull = m_fa2_err > 0. ? (scalar->fa2->getVal() - fa2Val ) / scalar->fa2->getError() : 0.;

      m_phia2 = scalar->phia2->getVal();
      m_phia2_err = scalar->phia2->getError();
      m_phia2_pull = m_phia2_err > 0. ? (scalar->phia2->getVal() - phia2Val ) / scalar->phia2->getError() : 0.;

      m_fa3 = scalar->fa3->getVal();
      m_fa3_err = scalar->fa3->getError();
      m_fa3_pull = m_fa3_err > 0. ? (scalar->fa3->getVal() - fa3Val ) / scalar->fa3->getError() : 0.;

      m_phia3 = scalar->phia3->getVal();
      m_phia3_err = scalar->phia3->getError();
      m_phia3_pull = m_phia3_err > 0. ? (scalar->phia3->getVal() - phia3Val ) / scalar->phia3->getError() : 0.;
      
      m_g2Re = scalar->g2Val->getVal();
      m_g2Re_err = scalar->g2Val->getError();
      m_g2Re_pull = m_g2Re_err >0. ? (scalar->g2Val->getVal() - g2Re ) / scalar->g2Val->getError() : 0.;

      m_g2Im = scalar->g2ValIm->getVal();
      m_g2Im_err = scalar->g2ValIm->getError();
      m_g2Im_pull = (scalar->g2ValIm->getVal() - g2Im ) / scalar->g2ValIm->getError();

      m_g4Re = scalar->g4Val->getVal();
      m_g4Re_err = scalar->g4Val->getError();
      m_g4Re_pull = (scalar->g4Val->getVal() - g4Re ) / scalar->g4Val->getError();

      m_g4Im = scalar->g4ValIm->getVal();
      m_g4Im_err = scalar->g4ValIm->getError();
      m_g4Im_pull = (scalar->g4ValIm->getVal() - g4Im ) / scalar->g4ValIm->getError();
      
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

    TCanvas *c1 = new TCanvas("c1","c1",1000, 800);
    c1->Divide(3,2);

    c1->cd(1);
    test.projectPDF(kcosthetastar, 20, testsingletoy);
    
    c1->cd(2);
    test.projectPDF(kphi1, 20, testsingletoy);
    
    c1->cd(4);
    test.projectPDF(kcostheta1, 20, testsingletoy);
    
    c1->cd(5);
    test.projectPDF(kcostheta2, 20, testsingletoy);
    
    c1->cd(6);
    test.projectPDF(kphi, 20, testsingletoy);

    TString fitName = "nofit";
    if ( fitData ) 
      fitName = "afterfit";
    if ( dotoys ) 
      fitName = "toy";

    c1->SaveAs(Form("ilcplots/projection_%s_%s.eps", fitName.Data(), modeName.Data()));
    c1->SaveAs(Form("ilcplots/projection_%s_%s.png", fitName.Data(), modeName.Data()));

    delete c1; 
  }

}
