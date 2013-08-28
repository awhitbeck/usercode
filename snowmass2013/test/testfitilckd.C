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
// Run by root -l -q -b testfitilckd.C
// 

void testfitilckd(bool pureToys = false, int ntoysperjob = 1000 ) {
 
  float sqrtsVal = 250;
  float mH=125.;
  bool withAcc = false;
  VerbosityLevel verb = ERROR;
  PlotLevel plot = ALL;
  ToyLevel toy = NOTOYS;
  
  double fa3Val = 0.5;
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
  
  RooRealVar* kd = new RooRealVar("pseudoMELA","D^{0-}", xMin, xMax);
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
  RooRealVar rrv_fa3("fa3","fa3",0.1,0.,1.);  //free parameter of the model
  RooFormulaVar rfv_fa3Obs("fa3obs","1/ (1 + (1/@0 - 1)*1)",RooArgList(rrv_fa3));
  if ( withAcc ) {
    // fa3Obs = 1/1 / ( 1 + ( -1 +1 / fa3) * e1 / e3 ) 
    // e1 and e3 are the efficiency  of your cuts on SM and P
    if ( sqrtsVal == 250. ) 
      rfv_fa3Obs = RooFormulaVar("fa3obs","1/ (1 + (1/@0 - 1)*0.985732)",RooArgList(rrv_fa3));
    if ( sqrtsVal == 350. ) 
      rfv_fa3Obs = RooFormulaVar("fa3obs","1/ (1 + (1/@0 - 1)*1.009)",RooArgList(rrv_fa3));
    if ( sqrtsVal == 500. ) 
      rfv_fa3Obs = RooFormulaVar("fa3obs","1/ (1 + (1/@0 - 1)*1.0302)",RooArgList(rrv_fa3));
    if ( sqrtsVal == 1000. ) 
      rfv_fa3Obs = RooFormulaVar("fa3obs","1/ (1 + (1/@0 - 1)*1.0575)",RooArgList(rrv_fa3));
  }

  RooAddPdf* sigPdf = new RooAddPdf("sigPdf","ps+sm",*zerominusPdf, *zeroplusPdf, rfv_fa3Obs);  
    
  
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
    
    TFile *toyresults = new TFile(Form("toyresults_eezh/toyresults_KD_%s_%.0fGeV_acc%s.root", isPureName.Data(), sqrtsVal, accName.Data()), "RECREATE");
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
	  return kNotEnoughEvents;
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
      
      if ( verb == DEBUG ) {
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
    gROOT->ProcessLine(".L tdrstyle.C");
    setTDRStyle();
    TGaxis *gaxis = new TGaxis();
    gaxis->SetMaxDigits(3);
    
    RooPlot* kdframe =  kd->frame(nbins);
    kdframe->GetXaxis()->CenterTitle();
    kdframe->GetYaxis()->CenterTitle();
    kdframe->GetYaxis()->SetTitle(" ");

    TH1F *kd_test = new TH1F("kd_test", "kd_test", nbins, xMin, xMax);                                                                  
    zerominusTree->Project("kd_test", "pseudoMELA");                                                                                         
    double ymax_kd = kd_test->GetMaximum(); 
    
    double rescale = 0.00001;

    if ( plot == ALL ) {
      zeroplusData->plotOn(kdframe, MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale));
      zeroplusPdf->plotOn(kdframe,  LineColor(kRed),LineWidth(2), Normalization(rescale));
      zerominusData->plotOn(kdframe, MarkerColor(kBlue),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale));
      zerominusPdf->plotOn(kdframe, LineColor(kBlue),LineWidth(2), Normalization(rescale));
      
      double bkgscale = 3.;
      bkgData->plotOn(kdframe, LineColor(kBlack), Rescale(rescale*bkgscale));
      bkgPdf->plotOn(kdframe, LineColor(kBlack), Normalization(rescale*bkgscale));
    }
    
    if ( plot == SIG || plot == ALL ) {
      sigData->plotOn(kdframe, MarkerColor(kGreen+2), MarkerStyle(25),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale*0.2));
      sigPdf->plotOn(kdframe,  LineColor(kGreen+2), Normalization(rescale*0.2));
    }
    
    if ( plot == ALL ) {
      kdframe->SetMaximum(ymax_kd * 1.2 * rescale);
    }

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
