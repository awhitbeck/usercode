
// Before you run, make sure, you have 
// 1. ln -s /export/d1/scratch/ygao/samples/ samples
// 2. mkdir -p plots_eezh/
// 3. mkdir -p toyresults_eezh/plots/
// 
// Run by root -l -b -n loadLib.C testfitilckd.C+
// 


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
#include "../src/../src/RooSpinZero_KD_ZH.cc"
#include "../src/ScalarPdfFactoryZH.cc"
#include "../src/../src/PlaygroundZH.cc"

using namespace RooFit ;

void testfitilckd(int ntoysperjob = 1 ) {
  
  typedef enum{ERROR, INFO, DEBUG} VerbosityLevel; 
  typedef enum{NOPLOTS, SIG, BKG, ALL} PlotLevel;
  typedef enum{NOTOYS, EMBED, PURE} ToyLevel; 
  

  float sqrtsVal = 250;
  float mH=125.;
  bool withAcc = false;
  VerbosityLevel verb = ERROR;
  PlotLevel plot = NOPLOTS;
  ToyLevel toy = EMBED;
  
  double fa3Val = 0.1;
  TString accName = "false";
  if ( withAcc ) 
    accName = "true";
  
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
  
  //
  //  1D KD observable
  // 
  int nbins = 100;
  double xMin = 0.3;
  double xMax = 1.;

  if ( sqrtsVal == 350. ) {
    xMin = 0.1;
  } 

  if ( sqrtsVal == 500. ) {
    xMin = 0.2;
  } 

  if ( sqrtsVal == 1000. ) {
    xMin = 0.;
  } 
  
  RooRealVar* kd = new RooRealVar("pseudoMELA","D_{0-}", xMin, xMax);
  kd->setBins(nbins);
  
  
  //
  // Background
  // 
  TChain *bkgTree = new TChain("SelectedTree");
  TString bkgModeName = Form("ee_ZZ_llbb_%.0fGeV_25M", sqrtsVal);
  bkgTree->Add(Form("samples/ee_ZH/%s_%s.root", bkgModeName.Data(), accName.Data()));
  RooDataSet *bkgData = new RooDataSet("bkgData","bkgData",bkgTree,RooArgSet(*kd));
  RooDataHist *bkgHist = bkgData->binnedClone(0);
  RooHistPdf* bkgPdf = new RooHistPdf("bkgPdf", "bkgPdf", RooArgSet(*kd), *bkgHist);
  
  
  // 
  // 0+
  // 
  
  TChain *zeroplusTree = new TChain("SelectedTree");
  TString zeroplusFileName = Form("samples/ee_ZH/unweighted_unpol_g1_1M_%s.root", accName.Data());
  if ( sqrtsVal == 350. ) 
    zeroplusFileName = Form("samples/ee_ZH/unweighted_unpol_g1_350GeV_2M_%s.root", accName.Data());
  if ( sqrtsVal == 500. ) 
    zeroplusFileName = Form("samples/ee_ZH/unweighted_unpol_g1_500GeV_2M_%s.root", accName.Data());
  if ( sqrtsVal == 1000. ) 
    zeroplusFileName = Form("samples/ee_ZH/unweighted_unpol_g1_1TeV_1M_%s.root", accName.Data());
  zeroplusTree->Add(zeroplusFileName);
  std::cout << "Reading " << zeroplusFileName << "\n";
  assert(zeroplusTree);
  RooDataSet *zeroplusData = new RooDataSet("zeroplusData","zeroplusData",zeroplusTree,RooArgSet(*kd));
  RooDataHist *zeroplusHist = zeroplusData->binnedClone(0);
  RooHistPdf* zeroplusPdf = new RooHistPdf("zeroplusPdf", "zeroplusPdf", RooArgSet(*kd), *zeroplusHist);
  
  // 
  // 0-
  // 
  
  TChain *zerominusTree = new TChain("SelectedTree");
  TString zerominusFileName = Form("samples/ee_ZH/unweighted_unpol_g4_1M_%s.root", accName.Data());
  if ( sqrtsVal == 350. ) 
    zerominusFileName = Form("samples/ee_ZH/unweighted_unpol_g4_350GeV_2M_%s.root", accName.Data());
  if ( sqrtsVal == 500. ) 
    zerominusFileName = Form("samples/ee_ZH/unweighted_unpol_g4_500GeV_2M_%s.root", accName.Data());
  if ( sqrtsVal == 1000. ) 
    zerominusFileName = Form("samples/ee_ZH/unweighted_unpol_g4_1TeV_1M_%s.root", accName.Data());
  zerominusTree->Add(zerominusFileName);
  RooDataSet *zerominusData = new RooDataSet("zerominusData","zerominusData",zerominusTree,RooArgSet(*kd));
  RooDataHist *zerominusHist = zerominusData->binnedClone(0);
  RooHistPdf* zerominusPdf = new RooHistPdf("zerominusPdf", "zerominusPdf", RooArgSet(*kd), *zerominusHist);
  
  //
  // Signal model
  // 
  
  TChain *sigTree = new TChain("SelectedTree");
  // TString sigFileName = Form("samples/ee_ZH/ee_ZH_f3_p5_phi_0_250GeV_1M_false.root"); 
  TString sigFileName = Form("samples/ee_ZH/unweighted_unpol_f_3_250GeV_5M_%s.root", accName.Data());
  if ( sqrtsVal == 350. ) 
    sigFileName = Form("samples/ee_ZH/unweighted_unpol_model8_2M_%s.root", accName.Data());
  if ( sqrtsVal == 500. ) 
    sigFileName = Form("samples/ee_ZH/unweighted_unpol_f_3_500GeV_5M_%s.root", accName.Data());
  if ( sqrtsVal == 1000. ) 
    sigFileName = Form("samples/ee_ZH//unweighted_unpol_model9_1M_%s.root", accName.Data());
  sigTree->Add(sigFileName);
  RooDataSet *sigData = new RooDataSet("sigData","sigData",sigTree,RooArgSet(*kd));
  //Define signal model with 0+, 0- mixture
  RooRealVar rrv_fa3("fa3","fa3",fa3Val ,0.,1.);  //free parameter of the model
  double eff_0plus_vs_0minus = 1.;
  if ( withAcc ) { 
    if ( sqrtsVal == 250. )   eff_0plus_vs_0minus = 0.985732; 
    if ( sqrtsVal == 350. )   eff_0plus_vs_0minus = 1.009;
    if ( sqrtsVal == 500. )   eff_0plus_vs_0minus = 1.0302;
    if ( sqrtsVal == 1000. )   eff_0plus_vs_0minus =1.0575;
  }
  char *formula = Form("1/ (1 + (1/@0 - 1)*%.6f)", eff_0plus_vs_0minus);
  RooFormulaVar rfv_fa3Obs("fa3obs",formula,RooArgList(rrv_fa3));
  // -------------------------------------------------------------- 
  //	Use this PDF to reproduce the old results in the paper 								
  // RooAddPdf* sigPdf = new RooAddPdf("sigPdf","ps+sm",*zerominusPdf, *zeroplusPdf, rfv_fa3Obs);  
  // -------------------------------------------------------------- 

  // 
  // Use the histogram based 
  // 

  TH1F *hist_zeroplus = new TH1F("hist_zeroplus", "hist_zeroplus", nbins, xMin, xMax);
  zeroplusTree->Project("hist_zeroplus", "pseudoMELA");
  hist_zeroplus->Scale(1./hist_zeroplus->Integral());
  std::cout << "hist_zeroplus->Integral() = " << hist_zeroplus->Integral() << "\n";

  TH1F *hist_zerominus = new TH1F("hist_zerominus", "hist_zerominus", nbins, xMin, xMax);
  zerominusTree->Project("hist_zerominus", "pseudoMELA");
  hist_zerominus->Scale(1./hist_zerominus->Integral());
  std::cout << "hist_zerominus->Integral() = " << hist_zerominus->Integral() << "\n";


  TChain *f3p5Tree = new TChain("SelectedTree");
  TString f3p5FileName = Form("samples/ee_ZH/ee_ZH_f3_p5_phi_0_250GeV_1M_false.root"); 
  f3p5Tree->Add(f3p5FileName);
  double g4mix = 0.351949; // this is the g4 value correspoinding to f3 = 0.5

  TH1F *hist_f3p5 = new TH1F("hist_f3p5", "hist_f3p5", nbins, xMin, xMax);
  f3p5Tree->Project("hist_f3p5", "pseudoMELA");
  hist_f3p5->Scale(2./hist_f3p5->Integral());
  std::cout << "hist_f3p5->Integral() = " << hist_f3p5->Integral() << "\n";

  
  TH1F *hist_zerointerf = new TH1F("hist_zerointerf", "hist_zerointerf", nbins, xMin, xMax);
  
  //  Fill in the interference term
  for ( int i = 1 ; i < nbins + 1 ; i++ ) {
    double SMHiggsProb = hist_zeroplus->GetBinContent(i);
    double altSignalProb = hist_zerominus->GetBinContent(i);
    double mixSignalProb = hist_f3p5->GetBinContent(i);
    double intProb = mixSignalProb - SMHiggsProb - altSignalProb; 
    intProb = intProb;
    hist_zerointerf->SetBinContent(i, intProb); 
  }
  
  vector<TH1F*> histos;
  histos.push_back(hist_zeroplus);
  histos.push_back(hist_zerominus);
  histos.push_back(hist_zerointerf);

  RooAbsPdf *sigPdf = new RooSpinZero_KD_ZH("sigPdf", "sigPdf", *kd,  rfv_fa3Obs, histos, withAcc);
  

  //
  // Signal Model with phase
  // 

  TChain *sigPhaseTree = new TChain("SelectedTree");
  TString sigPhaseFileName = Form("samples/ee_ZH/ee_ZH_f3_p5_phi_pi2_250GeV_1M_false.root"); 
  sigPhaseTree->Add(sigPhaseFileName);
  std::cout << "Reading " << sigPhaseFileName << "\n";
  assert(sigPhaseTree);
  RooDataSet *sigPhaseData = new RooDataSet("sigPhaseData","sigPhaseData",sigPhaseTree,RooArgSet(*kd));

  //
  // start toy MC
  // 

  if ( toy > NOTOYS  ) {
    
    TString isPureName = "embd";
    if ( toy == PURE ) isPureName = "pure";
    // 
    // toy generation
    // 
    RooRealVar* nsig = new RooRealVar("nsig","number of signal events",  0, 100000);
    RooRealVar* nbkg = new RooRealVar("nbkg","number of background events", 0, 1000000);
    
    // 
    // Fit the dataset
    // 
    
    TFile *toyresults = new TFile(Form("toyresults_eezh/toyresults_KD_%s_%.0fGeV_acc%s_new.root", isPureName.Data(), sqrtsVal, accName.Data()), "RECREATE");
    
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

      if ( toy == PURE ) 
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
	  return; // kNotEnoughEvents;
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

    TH1F *kd_test_0m = new TH1F("kd_test_0m", "kd_test_0m", nbins, xMin, xMax);
    zerominusTree->Project("kd_test_0m", "pseudoMELA"); 
    double rescale_0m = 1./kd_test_0m->Integral();
    kd_test_0m->Scale(rescale_0m);
    double ymax_kd = kd_test_0m->GetMaximum();
    std::cout << "rescale_0m = " << rescale_0m << "\n";
    
    TH1F *kd_test_0p = new TH1F("kd_test_0p", "kd_test_0p", nbins, xMin, xMax);
    zeroplusTree->Project("kd_test_0p", "pseudoMELA"); 
    double rescale_0p = 1./kd_test_0p->Integral();
    kd_test_0p->Scale(rescale_0p);
    ymax_kd = kd_test_0p->GetMaximum() > ymax_kd ? kd_test_0p->GetMaximum() : ymax_kd;
    std::cout << "rescale_0p = " << rescale_0p << "\n";

    TH1F *kd_test_0mix = new TH1F("kd_test_0mix", "kd_test_0mix", nbins, xMin, xMax);
    sigTree->Project("kd_test_0mix", "pseudoMELA"); // "wt");
    double rescale_0mix = 1./kd_test_0mix->Integral();
    kd_test_0mix->Scale(rescale_0mix);
    std::cout << "rescale_0mix = " << rescale_0mix << "\n";
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
      

    TCanvas *c1 = new TCanvas("c1", "c1", 600, 600);
    kdframe->Draw();
    
    TString plotName = Form("plots_eezh/KD_mX%.0f_sqrts%.0f_acc%s", mH, sqrtsVal, accName.Data());
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
  delete zerominusTree;
  delete zerominusData;
  delete zerominusPdf;
  
}
