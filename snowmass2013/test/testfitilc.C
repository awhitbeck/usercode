/// This script makes a fit and draws the projections after fit when required
// run by root -l -b loadlib.C testfitilc.C
// 
#include "../src/PlaygroundZH.cc"

using namespace PlaygroundZHhelpers;

void testfitilc(bool pureToys=false, int ntoysperjob = 1000, int seed_index=2) {

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
  TString modeName = Form("g1_p_g2_p_g4_1M");
  TString fileName = Form("Events_20130618/unweighted_unpol_%s_false.root", modeName.Data());
  // TString fileName = Form("Events_20130618/unpol_%s_false.root", modeName.Data());
  TString treeName = "SelectedTree";
  
  double g1Re = 1;
  double g1Im = 0.;
  double g2Re = 0.54071;
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
    // scalar->fa2->setConstant(kTRUE);

    scalar->phia2->setVal(phia2Val);
    scalar->phia2->setRange(-2*TMath::Pi(), 2*TMath::Pi());
    // scalar->phia2->setConstant(kTRUE);
    
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
