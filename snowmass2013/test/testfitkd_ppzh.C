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

void testfitkd_ppzh(bool pureToys = true, int ntoysperjob = 2000 ) {
 
  float sqrtsVal = 14000;
  float mH=125.;
  bool withAcc = true;
  VerbosityLevel verb = ERROR;
  PlotLevel plot = ALL;
  ToyLevel toy = NOTOYS;
  
  // double fa3Val = 0.385797;
  double fa3Val = 0.5;
  TString accName = "false";
  if ( withAcc ) 
    accName = "true";
  
  float lumi =300; // in unit of fb
  
  float nsigperfb = 0.23;
  float nbkgperfb = nsigperfb * 5.;
  
  float nsigEvents = lumi*nsigperfb; 
  float nbkgEvents = lumi*nbkgperfb; 
  
  //
  //  1D KD observable
  // 
  int nbins = 10;
  double xMin = 0; 
  double xMax = 1.;
  RooRealVar* kd = new RooRealVar("pseudoMELA","D^{0-}", xMin, xMax);
  kd->setBins(nbins);
  
  //
  // Background
  // 
  TChain *bkgTree = new TChain("SelectedTree");
  TString bkgModeName = Form("pp_ZZ_llbb_25M");
  bkgTree->Add(Form("samples/pp_ZH/%s_%s.root", bkgModeName.Data(), accName.Data()));
  RooDataSet *bkgData = new RooDataSet("bkgData","bkgData",bkgTree,RooArgSet(*kd));
  RooDataHist *bkgHist = bkgData->binnedClone(0);
  RooHistPdf* bkgPdf = new RooHistPdf("bkgPdf", "bkgPdf", RooArgSet(*kd), *bkgHist);
  
  // 
  // 0+
  // 
  
  TChain *zeroplusTree = new TChain("SelectedTree");
  //  TString zeroplusFileName = Form("samples/pp_ZH/pp_ZH_llbb_0p_toydata_%s.root", accName.Data());
  TString zeroplusFileName = Form("samples/pp_ZH/pp_ZH_llbb_g1_1M_%s.root", accName.Data());
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
  // TString zerominusFileName = Form("samples/pp_ZH/pp_ZH_llbb_0m_toydata_%s.root", accName.Data());
  TString zerominusFileName = Form("samples/pp_ZH/pp_ZH_llbb_g4_1M_%s.root", accName.Data());
  zerominusTree->Add(zerominusFileName);
  std::cout << "Reading " << zerominusFileName << "\n";
  assert(zerominusTree);
  RooDataSet *zerominusData = new RooDataSet("zerominusData","zerominusData",zerominusTree,RooArgSet(*kd));
  RooDataHist *zerominusHist = zerominusData->binnedClone(0);
  RooHistPdf* zerominusPdf = new RooHistPdf("zerominusPdf", "zerominusPdf", RooArgSet(*kd), *zerominusHist);
  
  //
  // Signal Model
  // 

  TChain *sigTree = new TChain("SelectedTree");
  // TString sigFileName = Form("samples/pp_ZH/pp_ZH_llbb_0mix_toydata_%s.root", accName.Data());
  TString sigFileName = Form("samples/pp_ZH/pp_ZH_llbb_g1_p_g4_1M_%s.root", accName.Data());
  sigTree->Add(sigFileName);
  std::cout << "Reading " << sigFileName << "\n";
  assert(sigTree);
  RooDataSet *sigData = new RooDataSet("sigData","sigData",sigTree,RooArgSet(*kd));
  RooDataHist *sigHist = sigData->binnedClone(0);
  RooHistPdf* sigPdf_MC = new RooHistPdf("sigPdf_MC", "sigPdf_MC", RooArgSet(*kd), *sigHist);
  //Define signal model with 0+, 0- mixture
  RooRealVar rrv_fa3("fa3","fa3",fa3Val,0.,1.);  //free parameter of the model
  // eff(0+) / eff(0-)
  double eff_0plus_vs_0minus = 1.;
  //  if ( withAcc ) eff_0plus_vs_0minus = 0.176893;
  char *formula = Form("1/ (1 + (1/@0 - 1)*%.6f)", eff_0plus_vs_0minus);
  RooFormulaVar rfv_fa3Obs("fa3obs",formula,RooArgList(rrv_fa3));
  std::cout << "f3 (observed) = " << rfv_fa3Obs.getVal() << "\n";
  RooAddPdf* sigPdf = new RooAddPdf("sigPdf","ps+sm",*zerominusPdf, *zeroplusPdf, rfv_fa3Obs);  
  
  // RooFitResult* toyfitresults_sig =  sigPdf->fitTo(*sigData, RooFit::PrintLevel(1), RooFit::Save(true)); 

  //
  // Perform Toy Experiments
  // 
  
  if ( toy > NOTOYS ) {
    
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
    
    TString nbkgName = "";
    if ( nbkgperfb == 0. ) nbkgName = "_sigonly";

    TFile *toyresults = new TFile(Form("toyresults_ppzh/toyresults_KD_%s_%.0fGeV_acc%s%s_%.0fifb.root", 
				       isPureName.Data(), sqrtsVal, accName.Data(), nbkgName.Data(), lumi), "RECREATE");
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
      int printlevel =  -1; // set to -1 to suppress all outputs
      
      if ( verb == INFO) {
	std::cout << "-----------toy trial " << i << " Generated values------ \n"; 
	std::cout << "f3 = " << rrv_fa3.getVal() << ",\t nsig = " << nsig->getVal() << ",\t nbkg = " << nbkg->getVal() << "\n";
	std::cout << "--------------------------------------------------\n";
      	printlevel  = 1; 
      }
      
      RooAddPdf* totalPdf = new RooAddPdf("totalPdf","totalPdf",RooArgList(*sigPdf,*bkgPdf),RooArgList(*nsig,*nbkg));   
      RooDataSet* toyData = totalPdf->generate(RooArgSet(*kd), nsigEvents+nbkgEvents);

      RooFitResult* toyfitresults =  totalPdf->fitTo(*toyData, RooFit::PrintLevel(printlevel), RooFit::Save(true), RooFit::Extended(kTRUE) ); 
      
      if ( verb == INFO) {
	std::cout << "-----------toy trial " << i << ": Fit values-------------\n"; 
	std::cout << "fa3 = " << rrv_fa3.getVal() << " +/- " << rrv_fa3.getError() << "\n";
	std::cout << "nsig = " << nsig->getVal() << " +/- " << nsig->getError() << "\n";
	std::cout << "nbkg = " << nbkg->getVal() << " +/- " << nbkg->getError() << "\n";
	std::cout << "fit status " << toyfitresults->status() << "\n";
	std::cout << "--------------------------------------------------\n";
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
    
    gROOT->ProcessLine(".L tdrstyle.C");
    setTDRStyle();
    gStyle->SetPadLeftMargin(0.16);
    TGaxis *gaxis = new TGaxis();
    gaxis->SetMaxDigits(3);
    
    double rescale = 0.00001;
    double rescale_0m = rescale;
    if ( withAcc ) rescale_0m = rescale*0.18;
    
    TH1F *kd_test = new TH1F("kd_test", "kd_test", nbins, xMin, xMax);
    sigTree->Project("kd_test", "pseudoMELA");
    double ymax_kd = kd_test->GetMaximum(); 

    RooPlot* kdframe =  kd->frame(nbins);
    kdframe->GetXaxis()->CenterTitle();
    kdframe->GetYaxis()->CenterTitle();
    kdframe->GetYaxis()->SetTitle(" ");
    
    if ( plot == ALL ) {
      zeroplusData->plotOn(kdframe, MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale));
      zeroplusPdf->plotOn(kdframe,  LineColor(kRed),LineWidth(2), Normalization(rescale));
      
      zerominusData->plotOn(kdframe, MarkerColor(kBlue),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale_0m));
      zerominusPdf->plotOn(kdframe, LineColor(kBlue),LineWidth(2), Normalization(rescale_0m));

      double bkgscale = 6.;
      if ( withAcc ) bkgscale = 20.;
      bkgData->plotOn(kdframe, LineColor(kBlack), Rescale(bkgscale*rescale));
      bkgPdf->plotOn(kdframe, LineColor(kBlack), Normalization(bkgscale*rescale));
    }

    if ( plot == SIG || plot == ALL  ) {
      sigData->plotOn(kdframe, MarkerColor(kGreen+2), MarkerStyle(25),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None), Rescale(rescale/10.));
      sigPdf->plotOn(kdframe,  LineColor(kGreen+2), Normalization(rescale/10.));
	// sigPdf_MC->plotOn(kdframe,  LineColor(kBlack));
    }

    
    TCanvas *c1 = new TCanvas("c1","c1",600,600);
    kdframe->SetMaximum(ymax_kd * 1.2 * rescale / 10.);
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
  delete zerominusTree;
  delete zerominusData;
  delete zerominusPdf;
  delete sigTree;
  delete sigData;
  delete sigPdf;

}
