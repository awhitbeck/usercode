#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooAbsPdf.h"
#include "RooFormulaVar.h"
#include "RooAbsReal.h"

using namespace RooFit ;

typedef enum{ERROR, INFO, DEBUG} VerbosityLevel; 
typedef enum{NOPLOTS, SIG, ALL} PlotLevel;
typedef enum{NOTOYS, EMBED, PURE} ToyLevel; 

// 
// Run by root -l -n -q loadLib.C testfitkd_ppzh.C 
// 

void testfitkd_ppzh(bool pureToys = true, int ntoysperjob = 1000 ) {
 
  float sqrtsVal = 14000;
  
  float mH=125.;
  bool withAcc = true;
  VerbosityLevel verb = INFO;
  PlotLevel plot = NOPLOTS;
  ToyLevel toy = PURE; 
  
  double fa3Val = 0.5;
  TString accName = "false";
  if ( withAcc ) 
    accName = "true";
  
  float lumi = 3000; // in unit of fb
  
  float nsigperfb = 0.1;
  float nbkgperfb = 0;
  
  float nsigEvents = lumi*nsigperfb; 
  float nbkgEvents = lumi*nbkgperfb; 
  
  if ( nbkgperfb == 0. ) 
    fa3Val = 0.15;

  //
  //  1D KD observable
  // 
  int nbins = 10;
  double xMin = 0.; 
  double xMax = 1.;
  RooRealVar* kd = new RooRealVar("pseudoMELA","D^{0-}", xMin, xMax);
  kd->setBins(nbins);
  
  //
  // Background
  // 
  TChain *bkgTree = new TChain("SelectedTree");
  TString bkgModeName = Form("pp_ZZ_llbb_25M");
  bkgTree->Add(Form("samples/pp_ZH/%s_%s_withKD.root", bkgModeName.Data(), accName.Data()));
  RooDataSet *bkgData = new RooDataSet("bkgData","bkgData",bkgTree,RooArgSet(*kd));
  RooDataHist *bkgHist = bkgData->binnedClone(0);
  RooHistPdf* bkgPdf = new RooHistPdf("bkgPdf", "bkgPdf", RooArgSet(*kd), *bkgHist);
  
  
  // 
  // 0+
  // 
  
  TChain *zeroplusTree = new TChain("SelectedTree");
  TString zeroplusFileName = Form("samples/pp_ZH/pp_ZH_llbb_14TeV_1M_%s_withKD.root", accName.Data());
  zeroplusTree->Add(zeroplusFileName);
  std::cout << "Reading " << zeroplusFileName << "\n";
  assert(zeroplusTree);
  RooDataSet *zeroplusData = new RooDataSet("zeroplusData","zeroplusData",zeroplusTree,RooArgSet(*kd));
  RooDataHist *zeroplusHist = zeroplusData->binnedClone(0);
  RooHistPdf* zeroplusPdf = new RooHistPdf("zeroplusPdf", "zeroplusPdf", RooArgSet(*kd), *zeroplusHist);
  

  // 
  // Now make a PDF based on puretoy dataset
  // 
  
  int nEventswoAcc =  1e+6;

  // define the signal model parameters
  double g1Gen    = 1.;
  double g1ImGen  = 0.;
  double g2Gen    = 0.;
  double g2ImGen  = 0.;
  double g3Gen    = 0.;
  double g3ImGen  = 0.;
  double g4Gen    = 0.;
  double g4ImGen  = 0.;
  
  // Observables (5D)
  RooRealVar* h1 = new RooRealVar("costheta1","h1",0, -1,1);
  RooRealVar* h2 = new RooRealVar("costheta2","h2",0, -1,1);
  RooRealVar* Phi = new RooRealVar("phi","Phi",0, -TMath::Pi(),TMath::Pi());
  RooRealVar* m= new RooRealVar("m","m", 775, 150, 1400);
  RooRealVar* Y= new RooRealVar("Y","Y", 0, -4, 4);
  
  // Parameters
  RooRealVar* sqrts= new RooRealVar("sqrts","sqrts", sqrtsVal);
  RooRealVar* mX = new RooRealVar("mX","mX", mH);
  RooRealVar* mZ = new RooRealVar("mZ","mZ", 91.1876);
  RooRealVar* gamZ = new RooRealVar("gamZ","gamZ",2.4952);
  RooRealVar* R1Val = new RooRealVar("R1Val","R1Val", 0.);
  RooRealVar* R2Val = new RooRealVar("R2Val","R2Val", 0.15);
    
  // amplitude parameters
  int para = 2;
  RooRealVar* a1Val  = new RooRealVar("a1Val","a1Val",0.);
  RooRealVar* phi1Val= new RooRealVar("phi1Val","phi1Val",0.);
  RooRealVar* a2Val  = new RooRealVar("a2Val","a2Val",0.);
  RooRealVar* phi2Val= new RooRealVar("phi2Val","phi2Val",0.);
  RooRealVar* a3Val  = new RooRealVar("a3Val","a3Val",0.);
  RooRealVar* phi3Val= new RooRealVar("phi3Val","phi3Val",0.);
  
  RooRealVar* g1Val  = new RooRealVar("g1Val","g1Val", 0, 100);
  RooRealVar* g2Val  = new RooRealVar("g2Val","g2Val", 0, 100);
  RooRealVar* g3Val  = new RooRealVar("g3Val","g3Val", 0, 100);
  RooRealVar* g4Val  = new RooRealVar("g4Val","g4Val", 0, 100);
  
  RooRealVar* g1ValIm  = new RooRealVar("g1ValIm","g1ValIm", -100, 100);
  RooRealVar* g2ValIm  = new RooRealVar("g2ValIm","g2ValIm", -100, 100);
  RooRealVar* g3ValIm  = new RooRealVar("g3ValIm","g3ValIm", -100, 100);
  RooRealVar* g4ValIm  = new RooRealVar("g4ValIm","g4ValIm", -100, 100);
  
  RooRealVar* fa2  = new RooRealVar("fa2","f_{g2}", 0.,1.0);
  RooRealVar* fa3  = new RooRealVar("fa3","f_{g4}", 0.,1.0);
  RooRealVar* phia2  = new RooRealVar("phia2","#phi_{g2}", -2.*TMath::Pi(),2*TMath::Pi());
  RooRealVar* phia3  = new RooRealVar("phia3","#phi_{g4}", -2.*TMath::Pi(),2*TMath::Pi());
  

  g1Val->setVal(g1Gen);
  g2Val->setVal(g2Gen);
  g3Val->setVal(g3Gen);
  g4Val->setVal(g4Gen);

  g1ValIm->setVal(g1ImGen);
  g2ValIm->setVal(g2ImGen);
  g3ValIm->setVal(g3ImGen);
  g4ValIm->setVal(g4ImGen);

  fa2->setVal(0.);
  fa3->setVal(0.0);
  phia2->setVal(0.);
  phia3->setVal(0.);

  // set the PDF
  RooSpinZero_3D_ZH_pp *sigPdf_pp = new RooSpinZero_3D_ZH_pp("sigPdf_pp","sigPdf_pp",
							     *h1,*h2,*Phi, *m, *Y,
							     *sqrts, *mX, *mZ, *R1Val, *R2Val, para, 
							     *a1Val, *phi1Val, *a2Val, *phi2Val,*a3Val, *phi3Val, 
							     *g1Val, *g2Val, *g3Val, *g4Val, *g1ValIm, *g2ValIm, *g3ValIm, *g4ValIm,
							     *fa2, *fa3, *phia2, *phia3, false);
  // 
  //  0+ from 5D phase space
  // 
  RooDataSet *zeroplusToyData = sigPdf_pp->generate(RooArgSet(*h1, *h2, *Phi, *m, *Y), nEventswoAcc);
  KDcalcPPZH test(sqrtsVal, mH);
  float KD = 0.;
  TH1D *hkd_zeroplus = new TH1D("hkd_zeropplus", "hkd_zeroplus", nbins, xMin, xMax);
  for (int i = 0; i < nEventswoAcc; i++) {
    KD = 0.;
    RooArgSet *tempEvent = (RooArgSet*) zeroplusToyData->get(i);
    float costheta1_ = ((RooRealVar*) tempEvent->find("costheta1"))->getVal();
    float costheta2_ = ((RooRealVar*) tempEvent->find("costheta2"))->getVal();
    float phi_ = ((RooRealVar*) tempEvent->find("phi"))->getVal();
    float m_ = ((RooRealVar*) tempEvent->find("m"))->getVal();
    float Y_ = ((RooRealVar*) tempEvent->find("Y"))->getVal();
    test.computeKD(mH, costheta1_, costheta2_, phi_, m_, Y_, KD, withAcc);
    hkd_zeroplus->Fill(KD);
    if ( verb == DEBUG ) {
      std::cout << "generated phase space (h1, h2, phi, m, Y): (" 
		<< costheta1_ << ", " << costheta2_ << ", " << phi_ << ", " << m_ << ", " << Y_
		<< ")\t KD = " << KD << "\n"; 
    }
  }

  hkd_zeropplus->Scale( zeroplusTree->GetEntries() / hkd_zeroplus->Integral());
  
  RooDataHist* zeroplusToyHist = new RooDataHist("zeroplusToyHist","zeroplusToyHist",RooArgSet(*kd), hkd_zeroplus);
  RooHistPdf* zeroplusToyPdf = new RooHistPdf("zeroplusToyPdf", "zeroplusToyPdf", RooArgSet(*kd), *zeroplusToyHist);
  
  // 
  //  0- from 5D phase space
  // 
  g1Val->setVal(0.);
  g4Val->setVal(1.);
  fa3->setVal(0.9999999);

  RooDataSet *zerominusToyData = sigPdf_pp->generate(RooArgSet(*h1, *h2, *Phi, *m, *Y), nEventswoAcc);
  
  TH1D *hkd_zerominus = new TH1D("hkd_zerominus", "hkd_zerominus", nbins, xMin, xMax);
  for (int i = 0; i < nEventswoAcc; i++) {
    KD = 0.;
    RooArgSet *tempEvent = (RooArgSet*) zerominusToyData->get(i);
    float costheta1_ = ((RooRealVar*) tempEvent->find("costheta1"))->getVal();
    float costheta2_ = ((RooRealVar*) tempEvent->find("costheta2"))->getVal();
    float phi_ = ((RooRealVar*) tempEvent->find("phi"))->getVal();
    float m_ = ((RooRealVar*) tempEvent->find("m"))->getVal();
    float Y_ = ((RooRealVar*) tempEvent->find("Y"))->getVal();
    test.computeKD(mH, costheta1_, costheta2_, phi_, m_, Y_, KD, withAcc);
    hkd_zerominus->Fill(KD);
    if ( verb == DEBUG ) {
      std::cout << "generated phase space (h1, h2, phi, m, Y): (" 
		<< costheta1_ << ", " << costheta2_ << ", " << phi_ << ", " << m_ << ", " << Y_
		<< ")\t KD = " << KD << "\n"; 
    }
  }

  hkd_zerominus->Scale( zeroplusTree->GetEntries() / hkd_zerominus->Integral());
  RooDataHist* zerominusToyHist = new RooDataHist("zerominusToyHist","zerominusToyHist",RooArgSet(*kd), hkd_zerominus);
  RooHistPdf* zerominusToyPdf = new RooHistPdf("zerominusToyPdf", "zerominusToyPdf", RooArgSet(*kd), *zerominusToyHist);
  
  // 
  // Define Signal PDF
  // 
  g1Val->setVal(1.);
  g4Val->setVal(0.852604);
  double fa3_suppression = 250.;
  if ( withAcc ) fa3_suppression = 750.;
  fa3->setVal(fa3Val/fa3_suppression);

  std::cout << "fa2 = " << fa2->getVal() << "\n";
  std::cout << "fa3 = " << fa3->getVal() << "\n";
  RooDataSet *sigToyData = sigPdf_pp->generate(RooArgSet(*h1, *h2, *Phi, *m, *Y), nEventswoAcc);

  TH1D *hkd_sig = new TH1D("hkd_sig", "hkd_sig", nbins, xMin, xMax);
  for (int i = 0; i < nEventswoAcc; i++) {
    KD = 0.;
    RooArgSet *tempEvent = (RooArgSet*) sigToyData->get(i);
    float costheta1_ = ((RooRealVar*) tempEvent->find("costheta1"))->getVal();
    float costheta2_ = ((RooRealVar*) tempEvent->find("costheta2"))->getVal();
    float phi_ = ((RooRealVar*) tempEvent->find("phi"))->getVal();
    float m_ = ((RooRealVar*) tempEvent->find("m"))->getVal();
    float Y_ = ((RooRealVar*) tempEvent->find("Y"))->getVal();
    test.computeKD(mH, costheta1_, costheta2_, phi_, m_, Y_, KD, withAcc);
    hkd_sig->Fill(KD);
    if ( verb == DEBUG ) {
      std::cout << "generated phase space (h1, h2, phi, m, Y): (" 
		<< costheta1_ << ", " << costheta2_ << ", " << phi_ << ", " << m_ << ", " << Y_
		<< ")\t KD = " << KD << "\n"; 
    }
  }
  RooDataHist* sigToyHist = new RooDataHist("sigToyHist","sigToyHist",RooArgSet(*kd), hkd_sig);
  RooHistPdf* sigToyPdf = new RooHistPdf("sigToyPdf", "sigToyPdf", RooArgSet(*kd), *sigToyHist);
  
  RooRealVar rrv_fa3("fa3","fa3",fa3Val,0.,1.);  //free parameter of the model
  RooFormulaVar rfv_fa3Obs("fa3obs","1/ (1 + (1/@0 - 1)*1)",RooArgList(rrv_fa3));
  /*
  if ( withAcc ) {
    rfv_fa3Obs = RooFormulaVar("fa3obs","1/ (1 + (1/@0 - 1)*0.985732)",RooArgList(rrv_fa3));
  }
  */
  RooAddPdf* sigPdf = new RooAddPdf("sigPdf","ps+sm",*zerominusToyPdf, *zeroplusToyPdf, rfv_fa3Obs);  
     
  

  //
  // Perform Toy Experiments
  // 
  
  if ( toy > NOTOYS ) {
    
    TString isPureName = "embd";
    if ( pureToys ) isPureName = "pure";
    // 
    // toy generation
    // 
    RooRealVar* nsig = new RooRealVar("nsig","number of signal events",  0, 100000);
    RooRealVar* nbkg = new RooRealVar("nbkg","number of background events", 0, 1000000);
    
    // 
    // Fit the dataset
    // 
    
    TString nbkgName = "";
    if ( nbkgperfb == 0. ) nbkgName = "_sigonly";

    TFile *toyresults = new TFile(Form("toyresults_ppzh/toyresults_KD_%s_%.0fGeV_acc%s%s.root", 
				       isPureName.Data(), sqrtsVal, accName.Data(), nbkgName.Data()), "RECREATE");
    gROOT->cd();
    
    TTree *tree_fit = new TTree("fittree", "fittree");
    
    Float_t m_fa3, m_fa3_err, m_fa3_pull;
    Float_t m_nsig, m_nsig_err, m_nsig_pull;
    Float_t m_nbkg, m_nbkg_err, m_nbkg_pull;
    int m_status;
    
    tree_fit->Branch("fa3",        &m_fa3,        "fa3/F");
    tree_fit->Branch("fa3_err",    &m_fa3_err,    "fa3_err/F");
    tree_fit->Branch("fa3_pull",   &m_fa3_pull,   "fa3_pull/F");
    tree_fit->Branch("nsig",       &m_nsig,       "nsig/F");
    tree_fit->Branch("nsig_err",   &m_nsig_err,   "nsig_err/F");
    tree_fit->Branch("nsig_pull",  &m_nsig_pull,  "nsig_pull/F");
    tree_fit->Branch("nbkg",       &m_nbkg,       "nbkg/F");
    tree_fit->Branch("nbkg_err",   &m_nbkg_err,   "nbkg_err/F");
    tree_fit->Branch("nbkg_pull",  &m_nbkg_pull,  "nbkg_pull/F");
    tree_fit->Branch("status",     &m_status,        "status/I");
    
    int toy_index; 

    for (int i = 0; i < ntoysperjob; i++) {
      
      toy_index = i; 
      nsig->setVal(nsigEvents);
      nbkg->setVal(nbkgEvents);
      rrv_fa3.setVal(fa3Val); 
      nsig->setConstant(kTRUE);
      nbkg->setConstant(kTRUE);

      int printlevel =  1; // set to -1 to suppress all outputs

      if ( verb == INFO ) 
	printlevel  = 1; 
      
      
      RooAddPdf* totalPdf = new RooAddPdf("totalPdf","totalPdf",RooArgList(*sigPdf,*bkgPdf),RooArgList(*nsig,*nbkg));   
      RooDataSet* toyData = totalPdf->generate(RooArgSet(*kd), nsigEvents+nbkgEvents);

      RooFitResult* toyfitresults =  totalPdf->fitTo(*toyData, RooFit::PrintLevel(printlevel), RooFit::Save(true), RooFit::Extended(kTRUE) ); 
      
      if ( verb == INFO) {
	std::cout << "toy trial " << i << "\n"; 
	std::cout << "fa3 = " << rrv_fa3.getVal() << " +/- " << rrv_fa3.getError() << "\n";
	std::cout << "nsig = " << nsig->getVal() << " +/- " << nsig->getError() << "\n";
	std::cout << "nbkg = " << nbkg->getVal() << " +/- " << nbkg->getError() << "\n";
	std::cout << "fit status " << toyfitresults->status() << "\n";
      }
      
      m_fa3 = rrv_fa3.getVal();
      m_fa3_err = rrv_fa3.getError();
      m_fa3_pull = m_fa3_err > 0. ? (rrv_fa3.getVal() - fa3Val ) / rrv_fa3.getError() : 0.;
      m_nsig = nsig->getVal();
      m_nsig_err = nsig->getError();
      m_nsig_pull = (nsig->getVal() - nsigEvents ) / nsig->getError();
      m_nbkg = nbkg->getVal();
      m_nbkg_err = nbkg->getError();
      m_nbkg_pull = (nbkg->getVal() - nbkgEvents ) / nbkg->getError();
      m_status = toyfitresults->status(); 
      tree_fit->Fill();
    }
    
    // Save toy results
    toyresults->cd();
    tree_fit->Write();
    toyresults->Close();
  }   


  //
  // Make Plots
  // 
  
  if ( plot >  NOPLOTS ) {
    
    gROOT->ProcessLine(".L ~/tdrstyle.C");
    setTDRStyle();
    gStyle->SetPadLeftMargin(0.16);
    TGaxis *gaxis = new TGaxis();
    gaxis->SetMaxDigits(3);
    
    RooPlot* kdframe =  kd->frame(nbins);

    if ( plot == ALL ) {
      zeroplusData->plotOn(kdframe, MarkerColor(kRed), MarkerStyle(24));
      // zeroplusPdf->plotOn(kdframe, LineColor(kRed), LineStyle(kDashed));
      zeroplusToyPdf->plotOn(kdframe, LineColor(kRed));
      zerominusToyPdf->plotOn(kdframe, LineColor(kBlue));
      double bkgscale = 6.;
      if ( withAcc ) bkgscale = 15.;
      bkgData->plotOn(kdframe, LineColor(kBlack), Rescale(bkgscale));
      bkgPdf->plotOn(kdframe, LineColor(kBlack), Normalization(bkgscale));
    }

    if ( plot == SIG ) {
      sigToyPdf->plotOn(kdframe, LineColor(kBlack), LineStyle(kDashed));
      zeroplusToyPdf->plotOn(kdframe, LineColor(kRed));
      zerominusToyPdf->plotOn(kdframe, LineColor(kBlue));
      sigPdf->plotOn(kdframe, LineColor(kBlack));
    }

    
    TCanvas *c1 = new TCanvas();
    kdframe->Draw();

    TString plotAppendix = "";
    if ( plot == SIG )  plotAppendix = "_sig";
    if ( plot == ALL )  plotAppendix = "_all";
      
    TString plotName = Form("plots_ppzh/KD_mX%.0f_sqrts%.0f_acc%s%s", mH, sqrtsVal, accName.Data(), plotAppendix.Data());
    c1->SaveAs(Form("%s.eps", plotName.Data()));
    c1->SaveAs(Form("%s.png", plotName.Data()));
    delete kdframe;
    delete c1; 
    }
  
  delete bkgTree; 
  delete bkgData;
  delete bkgPdf;
  delete zeroplusTree;
  delete zeroplusData;
  delete zeroplusPdf;
  delete zeroplusToyData;
  delete zeroplusToyHist;  
  delete zeroplusToyPdf;
  delete zerominusToyData;
  delete zerominusToyHist;  
  delete zerominusToyPdf;
  delete sigToyData;
  delete sigToyHist;  
  delete sigToyPdf;
}
