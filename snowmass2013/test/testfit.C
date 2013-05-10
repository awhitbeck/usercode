//
// This script makes a fit and draws the projections after fit when required
// run by root -l -b testfit.C
// 

void testfit(bool pureToys=true, int ntoysperjob = 10, int seed_index=2) {

  gROOT->ProcessLine(".x  loadLib.C");
  
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
  bool dopuretoy = true;

  float mH = 126.;
  float phase0m=0;
  float phase0p=270;
  
  TString modeName = Form("0Mf01ph%.0fHiggs0PHf01ph%.0f", phase0m, phase0p);
  TString fileName = Form("POWHEG_JHUGen_2e2mu/Higgs%sToZZTo4L_M-126_8TeV_POWHEG-JHUgenV3_false_2e2mu.root", modeName.Data());
  TString treeName = "SelectedTree";
  
  //cout << "initial values" << endl;
  // these are based on the default file listed above

  double fa2Val = 0.1;
  double phia2Val = -TMath::Pi()/2.;
  double fa3Val = 0.1;
  double phia3Val = 0;
  
  double g2Re = 0.;
  double g2Im = -0.57;
  double g4Re = 0.;
  double g4Im = 0.88;
  
  // 
  //cout << "Get Playground class started" << endl;
  // 
  int parameterization = 2; 
  Playground test(mH, debug, parameterization);
  if ( loadData ) 
    test.loadTree(fileName, treeName);
  
  if(debug) cout << test.data << endl;
  ScalarPdfFactory *scalar = test.scalar;

  // 
  //cout << "define the parameters to be fitted and the initial values" << endl;
  // 


  if ( parameterization == 2 ) {
    
    scalar->fa2->setConstant(kFALSE);
    scalar->fa2->setVal(fa2Val);
    
    scalar->phia2->setConstant(kFALSE);
    scalar->phia2->setVal(phia2Val);
    scalar->phia2->setRange(-2*TMath::Pi(), 2*TMath::Pi());
    
    scalar->fa3->setConstant(kFALSE);
    scalar->fa3->setVal(fa3Val);

    scalar->phia3->setVal(phia3Val);    
    scalar->phia3->setConstant(kFALSE);
    scalar->phia3->setRange(-2*TMath::Pi(), 2*TMath::Pi());

  }

  if ( parameterization == 1 ) {
    
    scalar->g2Val->setConstant(kFALSE);
    scalar->g2Val->setVal(g2Re);

    scalar->g2ValIm->setConstant(kFALSE);
    scalar->g2ValIm->setVal(g2Im);

    scalar->g4Val->setConstant(kFALSE);
    scalar->g4Val->setVal(g4Re);

    scalar->g4ValIm->setConstant(kFALSE);
    scalar->g4ValIm->setVal(g2Im);
  }
  
  if ( fitData ) { 
    RooFitResult *fitresults = test.fitData();
  }

  // 
  // Do puretoy experiments 
  // 
  if ( dopuretoy ) {
    
    float lumi = 30; // in unit of fb
    float nsignalperfb = 1; 
    
    TFile *toyresults = new TFile(Form("toyresults_%i.root", random_seed), "RECREATE");
    gROOT->cd();
    
    TH1F *h_fa2 = new TH1F("h_fa2", "h_fa2", 100, 0, 1);
    TH1F *h_fa2_pull = new TH1F("h_fa2_pull", "h_fa2_pull", 100, -5, 5);

    TH1F *h_fa3 = new TH1F("h_fa3", "h_fa3", 100, 0, 1);
    TH1F *h_fa3_pull = new TH1F("h_fa3_pull", "h_fa3_pull", 100, -5, 5);

    TH1F *h_phia2 = new TH1F("h_phia2", "h_phia2", 100, -TMath::Pi(), TMath::Pi());
    TH1F *h_phia2_pull = new TH1F("h_phia2_pull", "h_phia2_pull", 100, -5, 5);

    TH1F *h_phia3 = new TH1F("h_phia3", "h_phia3", 100, -TMath::Pi(), TMath::Pi());
    TH1F *h_phia3_pull = new TH1F("h_phia3_pull", "h_phia3_pull", 100, -5, 5);
    
    // do toys
    for (int i = 0; i < ntoysperjob; i++) {
      
      if  ( i%100 == 0 )    
	std::cout << "doing toy " << i << "\n";

      // set initial values
      scalar->fa2->setVal(fa2Val);
      scalar->fa3->setVal(fa3Val);
      scalar->phia2->setVal(phia2Val);
      scalar->phia3->setVal(phia3Val);

      test.generate(lumi*nsignalperfb, pureToys);

      cout << "fitting data" << endl;

      RooFitResult *toyfitresults = test.fitData(true);
      RooRealVar *r_fa2 = (RooRealVar *) toyfitresults->floatParsFinal().find("fa2");
      RooRealVar *r_fa3 = (RooRealVar *) toyfitresults->floatParsFinal().find("fa3");
      RooRealVar *r_phia2 = (RooRealVar *) toyfitresults->floatParsFinal().find("phia2");
      RooRealVar *r_phia3 = (RooRealVar *) toyfitresults->floatParsFinal().find("phia3");

      if ( debug ) {
	std::cout << "toy trial " << i << "\n"; 
	std::cout << r_fa2->getVal() << " +/- " << r_fa2->getError() << "\n";
	std::cout << r_fa3->getVal() << " +/- " << r_fa3->getError() << "\n";
	std::cout << r_phia2->getVal() << " +/- " << r_phia2->getError() << "\n";
	std::cout << r_phia3->getVal() << " +/- " << r_phia3->getError() << "\n";
      }

      h_fa2->Fill(r_fa2->getVal());
      h_fa2_pull->Fill( (r_fa2->getVal() - fa2Val) / r_fa2->getError());
      h_fa3->Fill(r_fa3->getVal());
      h_fa3_pull->Fill( (r_fa3->getVal() - fa3Val) / r_fa3->getError());
      h_phia2->Fill(r_phia2->getVal());
      h_phia2_pull->Fill( (r_phia2->getVal() - phia2Val) / r_phia2->getError());
      h_phia3->Fill(r_phia3->getVal());
      h_phia3_pull->Fill( (r_phia3->getVal() - phia3Val) / r_phia3->getError());

    }

    // Save toy results
    toyresults->cd();
    h_fa2->Write();
    h_fa2_pull->Write();
    h_fa3->Write();
    h_fa3_pull->Write();
    h_phia2->Write();
    h_phia2_pull->Write();
    h_phia3->Write();
    h_phia3_pull->Write();

  }

  // 
  // Draw projetions of the loaded data
  // 

  if ( drawprojections ) {
    
    TCanvas *c1 = new TCanvas("c1","c1",1500, 800);
    c1->Divide(4,2);

    c1->cd(1);
    test.projectPDF(Playground::kz1mass);
    
    c1->cd(2);
    test.projectPDF(Playground::kz2mass);

    c1->cd(3);
    test.projectPDF(Playground::kcosthetastar);
    
    c1->cd(4);
    test.projectPDF(Playground::kphi1);
    
    c1->cd(6);
    test.projectPDF(Playground::kcostheta1);
    
    c1->cd(7);
    test.projectPDF(Playground::kcostheta2);
    
    c1->cd(8);
    test.projectPDF(Playground::kphi);

    c1->SaveAs(Form("projection_afterfit_%s.eps", modeName.Data()));
    c1->SaveAs(Form("projection_afterfit_%s.png", modeName.Data()));

    delete c1; 
  }

}
