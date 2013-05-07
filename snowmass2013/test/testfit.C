//
// This script makes a fit and draws the projections after fit when required
// run by root -l -b testfit.C
// 

void testfit() {

  gROOT->ProcessLine(".x  loadLib.C");

  // 
  // inputs 
  // 
  bool dofit = true;  
  bool drawprojections = false;
  
  float mH = 126.;
  float phase0m=0;
  float phase0p=270;
  
  TString modeName = Form("0Mf01ph%.0fHiggs0PHf01ph%.0f", phase0m, phase0p);
  TString fileName = Form("datafiles/Higgs%sToZZTo4L_M-126_8TeV_POWHEG-JHUgenV3_false_2e2mu.root", modeName.Data());
  TString treeName = "SelectedTree";

  // 
  // Get Playground class started
  // 
  bool debug = false;
  int parameterization = 1; 
  Playground test(mH, debug, parameterization);
  test.loadTree(fileName, treeName);
  ScalarPdfFactory *scalar = test.scalar;

  // 
  // define the parameters to be fitted and the initial values
  // 

  if ( parameterization == 2 ) {
    
    double fg2Val = 0.2;
    double phig2Val = TMath::Pi();
    double fg4Val = 0.2;
    double phig4Val = 0;
    
    scalar->fg2->setConstant(kFALSE);
    scalar->fg2->setVal(fg2Val);
    
    scalar->phig2->setConstant(kFALSE);
    scalar->phig2->setVal(phig2Val);
    
    scalar->fg4->setConstant(kFALSE);
    scalar->fg4->setVal(fg4Val);
    
    scalar->phig4->setConstant(kFALSE);
    scalar->phig4->setVal(phig4Val);
  }

  if ( parameterization == 1 ) {

    double g2Re = 0.;
    double g2Im = 0.;
    double g4Re = 0.;
    double g4Im = 0.;

    scalar->g2Val->setConstant(kFALSE);
    scalar->g2Val->setVal(g2Re);

    scalar->g2ValIm->setConstant(kFALSE);
    scalar->g2ValIm->setVal(g2Im);

    scalar->g4Val->setConstant(kFALSE);
    scalar->g4Val->setVal(g4Re);

    scalar->g4ValIm->setConstant(kFALSE);
    scalar->g4ValIm->setVal(g2Im);
  }
  
  if ( dofit ) {
    test.fitData();
  }
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
