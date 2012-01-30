#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "TString.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "TFile.h"
#include "TLegend.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TString.h"
#include <sstream>
#include <string>
#include <vector>
#include "../PDFs/RooMzzModelBkg.h"
void fitmZZBkg(){

  RooRealVar* zzmass = new RooRealVar("zzmass","zzmass",110,180);
  RooRealVar* a0 = new RooRealVar("a0","a0",116.87,100,150);
  RooRealVar* a1 = new RooRealVar("a1","a1",11.65,5,15);
  RooRealVar* a2 = new RooRealVar("a2","a2",66.47,50,100);
  RooRealVar* a3 = new RooRealVar("a3","a3",0.152,0.05,0.5);
  RooRealVar* a4 = new RooRealVar("a4","a4",185,150,200);
  RooRealVar* a5 = new RooRealVar("a5","a5",11.7,5,15);
  RooRealVar* a6 = new RooRealVar("a6","a6",35.8,25,45);
  RooRealVar* a7 = new RooRealVar("a7","a7",0.13,0.05,0.2);
  RooRealVar* a8 = new RooRealVar("a8","a8",62.7,25,100);
  RooRealVar* a9 = new RooRealVar("a9","a9",0.062,0.01,0.1);
  //a0->setConstant(kTRUE);
  //a1->setConstant(kTRUE);
  //a2->setConstant(kTRUE);
  //a3->setConstant(kTRUE);
  a4->setConstant(kTRUE);
  a5->setConstant(kTRUE);
  a6->setConstant(kTRUE);
  a7->setConstant(kTRUE);
  a8->setConstant(kTRUE);
  a9->setConstant(kTRUE);

  RooMzzModelBkg* mzzPdf = new RooMzzModelBkg("PDF","PDF",*zzmass,*a0,*a1,*a2,*a3,*a4,*a5,*a6,*a7,*a8,*a9);
  
  TChain* chain = new TChain("angles");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_1.root");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_2.root");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_3.root");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_4.root");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_5.root");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_6.root");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_7.root");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_8.root");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_9.root");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_10.root");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_11.root");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_12.root");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_13.root");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_14.root");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_15.root");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_16.root");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_17.root");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_18.root");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_19.root");

  RooDataSet *data = new RooDataSet("data","data",chain,RooArgSet(*zzmass),"zzmass>110 && zzmass<180");
  //mzzPdf->fitTo(*data);
  
  TCanvas* cmzz = new TCanvas( "cmzz", "cmzz");
  RooPlot* mzzframe =  zzmass->frame(55);
  data->plotOn(mzzframe);
  mzzPdf->plotOn(mzzframe);
  mzzframe->Draw();
}
