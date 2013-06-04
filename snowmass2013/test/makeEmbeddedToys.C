/// This script makes a fit and draws the projections after fit when required
// run by root -l -b testfit.C
// 
#include "../src/Playground.cc"

using namespace PlaygroundHelpers;

void makeEmbeddedToys() {

  gROOT->ProcessLine(".x  loadLib.C");
  TFile *toyresults = new TFile("toyresults_2e2mu_62.root", "RECREATE");
  TH1F *h_fa2 = new TH1F("h_fa2", "h_fa2", 100, 0, 1);
  TH1F *h_fa2_pull = new TH1F("h_fa2_pull", "h_fa2_pull", 100, -5, 5);
  
  TH1F *h_fa3 = new TH1F("h_fa3", "h_fa3", 100, 0, 1);
  TH1F *h_fa3_pull = new TH1F("h_fa3_pull", "h_fa3_pull", 100, -5, 5);
  
  TH1F *h_phia2 = new TH1F("h_phia2", "h_phia2", 100, -TMath::Pi(), TMath::Pi());
  TH1F *h_phia2_pull = new TH1F("h_phia2_pull", "h_phia2_pull", 100, -5, 5);
  
  TH1F *h_phia3 = new TH1F("h_phia3", "h_phia3", 100, -TMath::Pi(), TMath::Pi());
  TH1F *h_phia3_pull = new TH1F("h_phia3_pull", "h_phia3_pull", 100, -5, 5);
 
  Playground test(126, true);
  for (int i=62; i<136; i++){
  //for (int i=1; i<68; i++){
    cout<<"------------------ "<<i<<" ------------------"<<endl;
    double fa2Val = 0.;
    double phia2Val = 0.;
    double fa3Val = 0.5;
    double phia3Val = 0;
  
    ScalarPdfFactory *scalar = test.scalar;
    scalar->fa2->setVal(fa2Val);
    scalar->fa2->setConstant(kFALSE);
  
    scalar->phia2->setVal(phia2Val);
    scalar->phia2->setConstant(kFALSE);
  
    scalar->fa3->setVal(fa3Val);
    scalar->fa3->setConstant(kFALSE);
  
    scalar->phia3->setVal(phia3Val);    
    scalar->phia3->setConstant(kFALSE);
  
    test.loadTree(Form("/tmp/sbologne/Higgs0Mf05ph0ToZZTo4L_M-126_8TeV_POWHEG-JHUgenV3-pythia6_false_2e2mu_withDiscriminants_%i.root", i), "SelectedTree");
  
    RooFitResult *toyfitresults = test.fitData();
    RooRealVar *r_fa2 = (RooRealVar *) toyfitresults->floatParsFinal().find("fa2");
    RooRealVar *r_fa3 = (RooRealVar *) toyfitresults->floatParsFinal().find("fa3");
    RooRealVar *r_phia2 = (RooRealVar *) toyfitresults->floatParsFinal().find("phia2");
    RooRealVar *r_phia3 = (RooRealVar *) toyfitresults->floatParsFinal().find("phia3");
  
    std::cout << "toy trial " << i << "\n"; 
    std::cout << r_fa2->getVal() << " +/- " << r_fa2->getError() << "\n";
    std::cout << r_fa3->getVal() << " +/- " << r_fa3->getError() << "\n";
    std::cout << r_phia2->getVal() << " +/- " << r_phia2->getError() << "\n";
    std::cout << r_phia3->getVal() << " +/- " << r_phia3->getError() << "\n";

  
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
