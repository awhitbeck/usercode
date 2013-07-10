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


void testfitilckd(bool pureToys = false, int ntoysperjob = 1000) {
 
  float sqrtsVal = 250.;
  float mH=125.;
  bool withAcc = false;
  bool debug = false;

  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();
  gStyle->SetPadLeftMargin(0.16);
  
  bool doplots = false;
  bool dotoys = true;
  
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
  RooRealVar* kd = new RooRealVar("pseudoMELA","kd", 0.3, 1.);
  kd->setBins(100);
  
  
  //
  // Background
  // 
  TChain *bkgTree = new TChain("SelectedTree");
  TString bkgModeName = Form("ee_ZZ_llbb_%.0fGeV_25M", sqrtsVal);
  bkgTree->Add(Form("bkgData/%s_%s_withKD.root", bkgModeName.Data(), accName.Data()));
  RooDataSet *bkgData = new RooDataSet("bkgData","bkgData",bkgTree,RooArgSet(*kd));
  RooDataHist *bkgHist = bkgData->binnedClone(0);
  RooHistPdf* bkgPdf = new RooHistPdf("bkgPdf", "bkgPdf", RooArgSet(*kd), *bkgHist);
  
  
  // 
  // 0+
  // 
  
  TChain *zeroplusTree = new TChain("SelectedTree");
  TString zeroplusModeName = "g1_1M";
  zeroplusTree->Add(Form("Events_20130618/unweighted_unpol_%s_%s_withKD.root", zeroplusModeName.Data(), accName.Data()));
  RooDataSet *zeroplusData = new RooDataSet("zeroplusData","zeroplusData",zeroplusTree,RooArgSet(*kd));
  RooDataHist *zeroplusHist = zeroplusData->binnedClone(0);
  RooHistPdf* zeroplusPdf = new RooHistPdf("zeroplusPdf", "zeroplusPdf", RooArgSet(*kd), *zeroplusHist);
  
  // 
  // 0-
  // 
  
  TChain *zerominusTree = new TChain("SelectedTree");
  TString zerominusModeName = "g4_1M";
  zerominusTree->Add(Form("Events_20130618/unweighted_unpol_%s_%s_withKD.root", zerominusModeName.Data(), accName.Data()));
  RooDataSet *zerominusData = new RooDataSet("zerominusData","zerominusData",zerominusTree,RooArgSet(*kd));
  RooDataHist *zerominusHist = zerominusData->binnedClone(0);
  RooHistPdf* zerominusPdf = new RooHistPdf("zerominusPdf", "zerominusPdf", RooArgSet(*kd), *zerominusHist);
  
  //
  // Signal model
  // 
  
  TChain *sigTree = new TChain("SelectedTree");
  TString sigModeName = "f_3_250GeV_5M";
  sigTree->Add(Form("Events_20130626/unweighted_unpol_%s_%s_withKD.root", sigModeName.Data(), accName.Data()));
  RooDataSet *sigData = new RooDataSet("sigData","sigData",sigTree,RooArgSet(*kd));
  //Define signal model with 0+, 0- mixture
  RooRealVar rrv_fa3("fa3","fa3",0.1,0.,1.);  //free parameter of the model
  //RooFormulaVar rfv_fa3Obs("fa3obs","1/ (1 + (1/@0 - 1)*0.981396)",RooArgList(rrv_fa3));
  RooFormulaVar rfv_fa3Obs("fa3obs","1/ (1 + (1/@0 - 1)*1)",RooArgList(rrv_fa3));
  RooAddPdf* sigPdf = new RooAddPdf("sigPdf","ps+sm",*zerominusPdf, *zeroplusPdf, rfv_fa3Obs);  
    
  
  if ( dotoys ) {
    
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
    
    TFile *toyresults = new TFile(Form("toyresults_KD_%s_acc%s.root", isPureName.Data(), accName.Data()), "RECREATE");
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

      int printlevel = 1; // set to -1 to suppress all outputs

      RooAddPdf* totalPdf = new RooAddPdf("totalPdf","totalPdf",RooArgList(*sigPdf,*bkgPdf),RooArgList(*nsig,*nbkg));   
      RooDataSet* toyData; 

      if ( pureToys ) 
	totalPdf->generate(RooArgSet(*kd), nsigEvents+nbkgEvents);
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
	  return kNotEnoughEvents;
	}
	
	for(int iEvent=isigEvent; iEvent<isigEvent+nsigEvents; iEvent++){
	  if(debug) cout << "generating signal event: " << iEvent << " embedTrackerSig: " << embedTrackerSig << endl;
	  tempEvent = (RooArgSet*) sigData->get(embedTrackerSig);
	  toyData->add(*tempEvent);
	  embedTrackerSig++;
	}

	for(int iEvent=ibkgEvent; iEvent<ibkgEvent+nbkgEvents; iEvent++){
	  if(debug) cout << "generating background event: " << iEvent << " embedTrackerBkg: " << embedTrackerBkg << endl;
	  tempEvent = (RooArgSet*) bkgData->get(embedTrackerBkg);
	  toyData->add(*tempEvent);
	  embedTrackerBkg++;
	}
	
      }

      RooFitResult* toyfitresults =  totalPdf->fitTo(*toyData, RooFit::PrintLevel(printlevel), RooFit::Save(true), RooFit::Extended(kTRUE) ); 
      
      if ( debug ) {
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
  
  if ( doplots ) {
    RooPlot* kdframe =  kd->frame(20);
    //toyData->plotOn(kdframe, LineColor(kBlack), MarkerStyle(24));
    //totalPdf->plotOn(kdframe, LineColor(kBlack));
    sigData->plotOn(kdframe, LineColor(kBlack), MarkerStyle(24));
    sigPdf->plotOn(kdframe, LineColor(kBlack));
    
    /*
      bkgData->plotOn(kdframe, LineColor(kBlack), MarkerStyle(24));
      bkgPdf->plotOn(kdframe,LineColor(kBlack));
      zeroplusData->plotOn(kdframe, LineColor(kRed), MarkerStyle(20));
      zeroplusPdf->plotOn(kdframe,LineColor(kRed));
      zerominusData->plotOn(kdframe, LineColor(kBlue), MarkerStyle(22));
      zerominusPdf->plotOn(kdframe,LineColor(kBlue));
    */
    
    TCanvas *c1 = new TCanvas();
    kdframe->Draw();
    
    TString plotName = Form("plots/KD_mX%.0f_sqrts%.0f", mH, sqrtsVal);
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
