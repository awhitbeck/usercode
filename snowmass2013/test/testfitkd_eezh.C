// Root headers
#include <vector>
#include "TH2F.h"
#include "TChain.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TAxis.h"

// RooFit headers
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooAbsPdf.h"
#include "RooFormulaVar.h"
#include "RooAbsReal.h"
#include "RooHistPdf.h"
#include "RooFitResult.h"

// snowmass code headers
#include "../src/ScalarPdfFactoryPPZH.cc"
#include "../src/../src/RooSpinZero_KDInt_ZH.cc"
#include "../src/../src/PlaygroundPPZH.cc"


using namespace RooFit ;


// 
// Run by root -l -q -b testfitkd_eezh.C
// 

void testfitkd_eezh(bool pureToys = false, int ntoysperjob = 1) {


  typedef enum{ERROR, INFO, DEBUG} VerbosityLevel; 
  typedef enum{NOPLOTS, SIG, BKG, ALL} PlotLevel;
  typedef enum{NOTOYS, EMBED, PURE} ToyLevel; 
  

  float sqrtsVal = 250;
  float mH=125.;
  bool withAcc = false;
  VerbosityLevel verb = INFO;
  PlotLevel plot = NOPLOTS;
  ToyLevel toy = EMBED;
  
  double fa3Val = 0.1;
  TString accName = "false";
  if ( withAcc ) 
    accName = "true";
  
  float lumi = 25000; // in unit of fb
    
  float nsigperfb = 8;
  float nbkgperfb = 0.8;

  float nsigEvents = lumi*nsigperfb; 
  float nbkgEvents = lumi*nbkgperfb; 
  
  //
  //  1D KD observable
  // 
  int nbins = 100;
  double xMin = 0.3;
  double xMax = 1.;

  int nbins_kdint = 50;
  double xMin_kdint = -0.5;
  double xMax_kdint = 0.5;

  
  RooRealVar* kd = new RooRealVar("pseudoMELA","D_{0-}", xMin, xMax);
  kd->setBins(nbins);

  RooRealVar* kdint = new RooRealVar("pseudoMELAInt","D_{int}", xMin_kdint, xMax_kdint);
  kdint->setBins(nbins_kdint);
  
  
  //
  // Background
  // 
  TChain *bkgTree = new TChain("SelectedTree");
  TString bkgModeName = Form("ee_ZZ_llbb_%.0fGeV_25M", sqrtsVal);
  bkgTree->Add(Form("samples/ee_ZH/%s_%s.root", bkgModeName.Data(), accName.Data()));
  RooDataSet *bkgData = new RooDataSet("bkgData","bkgData",bkgTree,RooArgSet(*kd, *kdint));
  RooDataHist *bkgHist = bkgData->binnedClone(0);
  RooHistPdf* bkgPdf = new RooHistPdf("bkgPdf", "bkgPdf", RooArgSet(*kd, *kdint), *bkgHist);
  
  
  // 
  // 0+
  // 
  
  TChain *zeroplusTree = new TChain("SelectedTree");
  TString zeroplusFileName = Form("samples/ee_ZH/unweighted_unpol_g1_1M_%s.root", accName.Data());
  zeroplusTree->Add(zeroplusFileName);
  std::cout << "Reading " << zeroplusFileName << "\n";
  assert(zeroplusTree);
  RooDataSet *zeroplusData = new RooDataSet("zeroplusData","zeroplusData",zeroplusTree,RooArgSet(*kd, *kdint));
  RooDataHist *zeroplusHist = zeroplusData->binnedClone(0);
  RooHistPdf* zeroplusPdf = new RooHistPdf("zeroplusPdf", "zeroplusPdf", RooArgSet(*kd, *kdint), *zeroplusHist);
  
  // 
  // 0-
  // 
  
  TChain *zerominusTree = new TChain("SelectedTree");
  TString zerominusFileName = Form("samples/ee_ZH/unweighted_unpol_g4_1M_%s.root", accName.Data());
  zerominusTree->Add(zerominusFileName);
  RooDataSet *zerominusData = new RooDataSet("zerominusData","zerominusData",zerominusTree,RooArgSet(*kd, *kdint));
  RooDataHist *zerominusHist = zerominusData->binnedClone(0);
  RooHistPdf* zerominusPdf = new RooHistPdf("zerominusPdf", "zerominusPdf", RooArgSet(*kd, *kdint), *zerominusHist);
  
  //
  // Signal model
  // 
  
  TChain *sigTree = new TChain("SelectedTree");
  //TString sigFileName = Form("samples/ee_ZH/ee_ZH_f3_p5_phi_0_250GeV_1M_false.root"); 
  TString sigFileName = Form("samples/ee_ZH/unweighted_unpol_f_3_250GeV_5M_%s.root", accName.Data());
  //TString sigFileName = Form("samples/ee_ZH/ee_ZH_f3_p5_phi_0_250GeV_1M_false.root"); 
  sigTree->Add(sigFileName);
  RooDataSet *sigData = new RooDataSet("sigData","sigData",sigTree,RooArgSet(*kd, *kdint));
  RooDataHist *sigHist = sigData->binnedClone(0);
  RooHistPdf* sigPdf_MC = new RooHistPdf("sigPdf_MC", "sigPdf_MC", RooArgSet(*kd, *kdint), *sigHist);

  //Define signal model with 0+, 0- mixture
  RooRealVar rrv_fa3("fa3","fa3",fa3Val ,0.,1.);  //free parameter of the model
  double eff_0plus_vs_0minus = 1.;
  char *formula = Form("1/ (1 + (1/@0 - 1)*%.6f)", eff_0plus_vs_0minus);
  RooFormulaVar rfv_fa3Obs("fa3obs",formula,RooArgList(rrv_fa3));
  // RooAddPdf* sigPdf = new RooAddPdf("sigPdf","ps+sm",*zerominusPdf, *zeroplusPdf, rfv_fa3Obs);  
    

  // 
  // Use the histogram based 
  // 

  TH2F *hist_zeroplus = new TH2F("hist_zeroplus", "hist_zeroplus", nbins, xMin, xMax, nbins_kdint, xMin_kdint, xMax_kdint);
  zeroplusTree->Project("hist_zeroplus", "pseudoMELAInt:pseudoMELA"); 
  hist_zeroplus->Scale(1./hist_zeroplus->Integral());
  std::cout << "hist_zeroplus->Integral() = " << hist_zeroplus->Integral() << "\n";

  TH2F *hist_zerominus = new TH2F("hist_zerominus", "hist_zerominus", nbins, xMin, xMax, nbins_kdint, xMin_kdint, xMax_kdint);
  zerominusTree->Project("hist_zerominus", "pseudoMELAInt:pseudoMELA"); 
  hist_zerominus->Scale(1./hist_zerominus->Integral());
  std::cout << "hist_zerominus->Integral() = " << hist_zerominus->Integral() << "\n";

  TH2F *hist_zerointerf = new TH2F("hist_zerointerf", "hist_zerointerf", nbins, xMin, xMax, nbins_kdint, xMin_kdint, xMax_kdint);
  zeroplusTree->Project("hist_zerointerf", "pseudoMELAInt:pseudoMELA"); 
  hist_zerointerf->Scale(1./hist_zerointerf->Integral());
  std::cout << "hist_zerointerf->Integral() = " << hist_zerointerf->Integral() << "\n";
  

  vector<TH2F*> histos;
  histos.push_back(hist_zeroplus);
  histos.push_back(hist_zerominus);
  histos.push_back(hist_zerointerf);

  RooAbsPdf *sigPdf = new RooSpinZero_KDInt_ZH("sigPdf", "sigPdf", *kd, *kdint, rrv_fa3, histos, withAcc);

  //
  // Signal Model with phase
  // 

  TChain *sigPhaseTree = new TChain("SelectedTree");
  TString sigPhaseFileName = Form("samples/ee_ZH/ee_ZH_f3_p5_phi_pi2_250GeV_1M_false.root"); 
  sigPhaseTree->Add(sigPhaseFileName);
  std::cout << "Reading " << sigPhaseFileName << "\n";
  assert(sigPhaseTree);
  RooDataSet *sigPhaseData = new RooDataSet("sigPhaseData","sigPhaseData",sigPhaseTree,RooArgSet(*kd, *kdint));

  //
  // start toy MC
  // 

  if ( toy > NOTOYS  ) {
    
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
    
    TFile *toyresults = new TFile(Form("toyresults_eezh/toyresults_KD_%s_%.0fGeV_acc%s_new.root", isPureName.Data(), sqrtsVal, accName.Data()), "RECREATE");
    //    gROOT->cd();
    
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
    int embedTrackerSig = 0;
    int embedTrackerBkg = 0;

    for (int i = 0; i < ntoysperjob; i++) {
      
      toy_index = i; 
      // reset the events starting point
      embedTrackerSig = toy_index * nsigEvents;
      embedTrackerBkg = toy_index * nbkgEvents;
      
      nsig->setVal(nsigEvents);
      nbkg->setVal(nbkgEvents);
      rrv_fa3.setVal(fa3Val); 
      nsig->setConstant(kTRUE);
      nbkg->setConstant(kTRUE);

      int printlevel = - 1; // set to -1 to suppress all outputs

      if ( verb == DEBUG ) 
	printlevel  = 1; 
      
      
      RooAddPdf* totalPdf = new RooAddPdf("totalPdf","totalPdf",RooArgList(*sigPdf,*bkgPdf),RooArgList(*nsig,*nbkg));   
      RooDataSet* toyData; 

      if ( pureToys ) 
	toyData = totalPdf->generate(RooArgSet(*kd), nsigEvents+nbkgEvents);
      else {
	// reset the events starting point
	embedTrackerSig = toy_index * nsigEvents;
	embedTrackerBkg = toy_index * nbkgEvents;
	
	int isigEvent = toy_index * nsigEvents;
	int ibkgEvent = toy_index * nbkgEvents;
	
	RooArgSet *tempEvent;
	toyData = new RooDataSet("toyData","toyData",RooArgSet(*kd));
	
	if(  (nsigEvents+embedTrackerSig > sigData->sumEntries()) 
	     || (nbkgEvents+embedTrackerBkg > bkgData->sumEntries()) ) {
	  cout << "PlaygroundZH::generate() - ERROR!!! PlaygroundZH::data does not have enough events to fill toy!!!!  bye :) " << endl;
	  toyData = NULL;
	  return; //kNotEnoughEvents;
	}
	
	for(int iEvent=isigEvent; iEvent<isigEvent+nsigEvents; iEvent++){
	  if( verb == DEBUG ) cout << "generating signal event: " << iEvent << " embedTrackerSig: " << embedTrackerSig << endl;
	  tempEvent = (RooArgSet*) sigData->get(embedTrackerSig);
	  toyData->add(*tempEvent);
	  embedTrackerSig++;
	}

	for(int iEvent=ibkgEvent; iEvent<ibkgEvent+nbkgEvents; iEvent++){
	  if( verb == DEBUG ) cout << "generating background event: " << iEvent << " embedTrackerBkg: " << embedTrackerBkg << endl;
	  tempEvent = (RooArgSet*) bkgData->get(embedTrackerBkg);
	  toyData->add(*tempEvent);
	  embedTrackerBkg++;
	}
	
      }

      RooFitResult* toyfitresults =  totalPdf->fitTo(*toyData, RooFit::PrintLevel(printlevel), RooFit::Save(true), RooFit::Extended(kTRUE) ); 
      
      if ( verb >= INFO ) {
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
  
  if ( plot > NOPLOTS ) {

    //
    // kd frame
    // 

    TH1F *kd_test_0m = new TH1F("kd_test_0m", "kd_test_0m", nbins, xMin, xMax);
    zerominusTree->Project("kd_test_0m", "pseudoMELA"); 
    double rescale_0m = 1./kd_test_0m->Integral();
    kd_test_0m->Scale(rescale_0m);
    double ymax_kd = kd_test_0m->GetMaximum();
    
    TH1F *kd_test_0p = new TH1F("kd_test_0p", "kd_test_0p", nbins, xMin, xMax);
    zeroplusTree->Project("kd_test_0p", "pseudoMELA"); 
    double rescale_0p = 1./kd_test_0p->Integral();
    kd_test_0p->Scale(rescale_0p);
    ymax_kd = kd_test_0p->GetMaximum() > ymax_kd ? kd_test_0p->GetMaximum() : ymax_kd;

    TH1F *kd_test_0mix = new TH1F("kd_test_0mix", "kd_test_0mix", nbins, xMin, xMax);
    sigTree->Project("kd_test_0mix", "pseudoMELA"); // "wt");
    double rescale_0mix = 1./kd_test_0mix->Integral();
    kd_test_0mix->Scale(rescale_0mix);
    ymax_kd = kd_test_0mix->GetMaximum() > ymax_kd ? kd_test_0mix->GetMaximum() : ymax_kd;
    
    RooPlot* kdframe =  kd->frame(nbins);
    kdframe->GetXaxis()->CenterTitle();
    kdframe->GetYaxis()->CenterTitle();
    kdframe->GetYaxis()->SetTitle(" ");
    
    if ( plot == ALL ) {
      zeroplusData->plotOn(kdframe, MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_0p));
      zeroplusPdf->plotOn(kdframe,  LineColor(kRed),LineWidth(2), Normalization(rescale_0p));
      zerominusData->plotOn(kdframe, MarkerColor(kBlue),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_0m));
      zerominusPdf->plotOn(kdframe, LineColor(kBlue),LineWidth(2), Normalization(rescale_0m));
    }
    
    if ( plot == SIG || plot == ALL ) {
      sigData->plotOn(kdframe, MarkerColor(kGreen+2), MarkerStyle(25),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_0mix));
      sigPdf->plotOn(kdframe,  LineColor(kGreen+2), Normalization(rescale_0mix));
      // sigPhaseData->plotOn(kdframe, MarkerColor(kMagenta+1), MarkerStyle(20),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_0mix));
    }
    
    if ( plot == BKG ) {
      bkgData->plotOn(kdframe, LineColor(kBlack));
      bkgPdf->plotOn(kdframe, LineColor(kBlack));
    }
    
    kdframe->SetMaximum(ymax_kd * 1.1 );
    

    //
    // repeat for Dint
    // 

    //
    // kd frame
    // 

    TH1F *kdint_test_0m = new TH1F("kdint_test_0m", "kdint_test_0m", nbins_kdint, xMin_kdint, xMax_kdint);
    zerominusTree->Project("kdint_test_0m", "pseudoMELAInt"); 
    rescale_0m = 1./kdint_test_0m->Integral();
    kdint_test_0m->Scale(rescale_0m);
    double ymax_kdint = kdint_test_0m->GetMaximum();
    
    TH1F *kdint_test_0p = new TH1F("kdint_test_0p", "kdint_test_0p", nbins_kdint, xMin_kdint, xMax_kdint);
    zeroplusTree->Project("kdint_test_0p", "pseudoMELAInt"); 
    rescale_0p = 1./kdint_test_0p->Integral();
    kdint_test_0p->Scale(rescale_0p);
    ymax_kdint = kdint_test_0p->GetMaximum() > ymax_kdint ? kdint_test_0p->GetMaximum() : ymax_kdint;

    TH1F *kdint_test_0mix = new TH1F("kdint_test_0mix", "kdint_test_0mix", nbins_kdint, xMin_kdint, xMax_kdint);
    sigTree->Project("kdint_test_0mix", "pseudoMELAInt"); // "wt");
    rescale_0mix = 1./kdint_test_0mix->Integral();
    kdint_test_0mix->Scale(rescale_0mix);
    ymax_kdint = kdint_test_0mix->GetMaximum() > ymax_kdint ? kdint_test_0mix->GetMaximum() : ymax_kdint;
    
    RooPlot* kdintframe =  kdint->frame(nbins_kdint);
    kdintframe->GetXaxis()->CenterTitle();
    kdintframe->GetYaxis()->CenterTitle();
    kdintframe->GetYaxis()->SetTitle(" ");
    
    if ( plot == ALL ) {
      zeroplusData->plotOn(kdintframe, MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_0p));
      zeroplusPdf->plotOn(kdintframe,  LineColor(kRed),LineWidth(2), Normalization(rescale_0p));
      zerominusData->plotOn(kdintframe, MarkerColor(kBlue),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_0m));
      zerominusPdf->plotOn(kdintframe, LineColor(kBlue),LineWidth(2), Normalization(rescale_0m));
    }
    
    if ( plot == SIG || plot == ALL ) {
      sigData->plotOn(kdintframe, MarkerColor(kGreen+2), MarkerStyle(25),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_0mix));
      sigPdf->plotOn(kdintframe,  LineColor(kGreen+2), Normalization(rescale_0mix));
      // sigPhaseData->plotOn(kdintframe, MarkerColor(kMagenta+1), MarkerStyle(20),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_0mix));

    }
    
    if ( plot == BKG ) {
      bkgData->plotOn(kdintframe, LineColor(kBlack));
      bkgPdf->plotOn(kdintframe, LineColor(kBlack));
    }

    kdintframe->SetMaximum(ymax_kdint * 1.1 );
    


    TCanvas *c1 = new TCanvas("c1", "c1", 600, 600);
    kdframe->Draw();
    
    TString plotName = Form("plots_eezh/KD_mX%.0f_sqrts%.0f_acc%s", mH, sqrtsVal, accName.Data());
    c1->SaveAs(Form("%s.eps", plotName.Data()));
    c1->SaveAs(Form("%s.png", plotName.Data()));

    c1->Clear();

    kdintframe->Draw();
    plotName = Form("plots_eezh/KDInt_mX%.0f_sqrts%.0f_acc%s", mH, sqrtsVal, accName.Data());
    c1->SaveAs(Form("%s.eps", plotName.Data()));
    c1->SaveAs(Form("%s.png", plotName.Data()));

    delete kdframe;
    delete kdintframe;
    delete c1; 
    }
  
  delete bkgTree; 
  delete bkgData;
  delete bkgPdf;
  delete zeroplusTree;
  delete zeroplusData;
  delete zeroplusPdf;
  delete zerominusTree;
  delete zerominusData;
  delete zerominusPdf;
  
}
