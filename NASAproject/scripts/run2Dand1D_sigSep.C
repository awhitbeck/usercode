

void run2Dand1D_sigSep(double intLumi=20.0){

  using namespace RooFit;

  gSystem->Load("../PDFs/RooNASAModelZeroPlus_sigSep_cc.so");
  gSystem->Load("../PDFs/RooNASAModelZeroMinus_sigSep_cc.so");
  gSystem->Load("../PDFs/RooNASAModelBkg_sigSep_cc.so");
  gSystem->Load("../PDFs/RooMzzModelBkg_cc.so"); 
  gSystem->Load("../src/NASA2dSigSepPdfFactory_cc.so");
  gSystem->Load("../src/NASA1dPdfFactory_cc.so");
  gSystem->Load("../src/statsFactory_cc.so");
  
  RooRealVar* zzmass = new RooRealVar("zzmass","m_{ZZ}",110,140);
  RooRealVar* sigSepLD = new RooRealVar("sigSepLD","LD",0.,1.);
  RooArgSet* obs2d = new RooArgSet(*zzmass,*sigSepLD);
  RooArgSet* obs1d = new RooArgSet(*zzmass);

  TChain *tplus = new TChain("angles");
  tplus->Add("../datafiles/SMHiggs_125_JHU_v2_wResolution_withDiscriminants_withCuts.root");
  tplus->Add("../datafiles/SMHiggs_125_JHU_wResolution_withDiscriminants_withCuts.root");

  TChain *tminus = new TChain("angles");
  tminus->Add("../datafiles/PSHiggs_125_JHU_v2_wResolution_withDiscriminants_withCuts.root");
  tminus->Add("../datafiles/PSHiggs_125_JHU_wResolution_withDiscriminants_withCuts.root");

  TFile* bkgFile = new TFile("../datafiles/EWKZZ4l_Powheg_wResolution_withDiscriminants_withCuts.root");

  RooDataSet *bkgData2D = new RooDataSet("bkgData2D","bkgData2D",(TTree*)bkgFile->Get("angles"),*obs2d);
  RooDataSet *zeroPlusData2D = new RooDataSet("sigData2D","sigData2D",tplus,*obs2d);
  RooDataSet *zeroMinusData2D = new RooDataSet("sigData2D","sigData2D",tminus,*obs2d);
  RooDataSet *bkgData1D = new RooDataSet("bkgData1D","bkgData1D",(TTree*)bkgFile->Get("angles"),*obs1d);
  RooDataSet *zeroPlusData1D = new RooDataSet("sigData1D","sigData1D",tplus,*obs1d);
  RooDataSet *zeroMinusData1D = new RooDataSet("sigData1D","sigData1D",tminus,*obs1d);

  NASA2dSigSepPdfFactory my2dZeroMinusModel(zzmass,sigSepLD,true);
  NASA2dSigSepPdfFactory my2dZeroPlusModel(zzmass,sigSepLD,false);
  NASA1dPdfFactory my1dModel(zzmass);

  // - - - - - - - - - - - - - - - - - - - - - 

  my1dModel.sigPDF->fitTo(*zeroPlusData1D);
  my1dModel.bkgPDF->fitTo(*bkgData1D);


  RooPlot* plot1 = zzmass->frame(20);
  //RooPlot* plot2 = zzmass->frame(20);
  RooPlot* plot3 = sigSepLD.frame(20);
  //RooPlot* plot4 = sigSepLD.frame(20);
  
  zeroPlusData2D->plotOn(plot1,MarkerColor(2));
  my2dZeroPlusModel.zeroPlusPDF->plotOn(plot1,LineColor(2));
  zeroMinusData2D->plotOn(plot1,MarkerColor(3));
  my2dZeroMinusModel.zeroMinusPDF->plotOn(plot1,LineColor(3),LineStyle(2));
  bkgData2D->plotOn(plot1,MarkerColor(4));
  my2dZeroPlusModel.bkgPDF->plotOn(plot1,LineColor(4)); 

  zeroPlusData2D->plotOn(plot3,MarkerColor(2));
  my2dZeroPlusModel.zeroPlusPDF->plotOn(plot3,LineColor(2));
  zeroMinusData2D->plotOn(plot3,MarkerColor(3));
  my2dZeroMinusModel.zeroMinusPDF->plotOn(plot3,LineColor(3),LineStyle(2));
  bkgData2D->plotOn(plot3,MarkerColor(4));
  my2dZeroPlusModel.bkgPDF->plotOn(plot3,LineColor(4)); 


  TCanvas* fitCan = new TCanvas("fitCan","fitCan",400,400);
  plot1->Draw();
  TCanvas* fitCan2 = new TCanvas("fitCan2","fitCan2",400,400);
  plot3->Draw();

  /*
  fitCan->cd(3);
  plot2->Draw();
  fitCan->cd(4);
  plot4->Draw();
  */
  // - - - - - - - - - - - - - - - - - - - - - 

  /*

  my2dModel.makeParamsConst(true);
  my1dModel.makeParamsConst(true);
  statsFactory *my2dStats = new statsFactory(obs2d,my2dModel.sigPDF,my2dModel.bkgPDF,"2DStats_Feb8.root");
  statsFactory *my1dStats = new statsFactory(obs1d,my1dModel.sigPDF,my1dModel.bkgPDF,"1DStats_Feb8.root");
  
 
  my1dStats->runSignificance(10.75*intLumi/20.0,15.61*intLumi/20.,400,sigData1D,bkgData1D);
  my1dStats->runUpperLimit(10.75*intLumi/20.0,15.61*intLumi/20.0,400);
  my2dStats->runSignificance(10.75*intLumi/20.,15.61*intLumi/20.,400,sigData2D,bkgData2D);
  my2dStats->runUpperLimit(10.75*intLumi/20.,15.61*intLumi/20.,400);
 

  delete my2dStats;
  delete my1dStats;

  */

}
