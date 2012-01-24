#include "../../PDFs/LikelihoodToolbox.cc"
#include "RooGaussian.h"
#include "RooExponential.h"
#include "RooPlot.h"
#include "RooHistPdf.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "TCanvas.h"
#include "TFile.h"

using namespace std;

void testSeparation(){

  cout << "load files and trees for data" << endl;

  TFile* fileSMHiggs = new TFile("/scratch/hep/ntran/HZZ_materials/datafiles/SMHiggs_125_JHU.root");
  TTree* treeSMHiggs = (TTree*) fileSMHiggs->Get("angles");
  TFile* filePSHiggs = new TFile("/scratch/hep/ntran/HZZ_materials/datafiles/PSHiggs_125_JHU.root");
  TTree* treePSHiggs = (TTree*) filePSHiggs->Get("angles");
  TFile* fileEWKZZ4l = new TFile("/scratch/hep/ntran/HZZ_materials/datafiles/EWKZZ4l_Powheg.root");
  TTree* treeEWKZZ4l = (TTree*) fileEWKZZ4l->Get("angles");

  RooRealVar* costhetastar = new RooRealVar("costhetastar","cos#theta*",-1,1);
  costhetastar->setBins(10);
  RooRealVar* costheta1 = new RooRealVar("costheta1","cos#theta_{1}",-1,1);
  costheta1->setBins(10);

  RooArgSet argSet(*costheta1,*costhetastar);

  char cuts[50]="";

  cout << "load datasets" << endl;
  

  RooDataSet SMdataSet("SMdataSet","SMdataSet",treeSMHiggs,argSet,cuts);  
  RooDataSet PSdataSet("PSdataSet","PSdataSet",treePSHiggs,argSet,cuts);
  RooDataSet EWKdataSet("EWKdataSet","EWKdataSet",treeEWKZZ4l,argSet,cuts);

  cout << "datasets done" << endl;
  cout << "load histograms from datasets" << endl;

  RooDataHist SMdataHist("SMdataHist","SMdataHist",argSet,SMdataSet);
  RooDataHist PSdataHist("PSdataHist","PSdataHist",argSet,PSdataSet);
  RooDataHist EWKdataHist("EWKdataHist","EWKdataHist",argSet,EWKdataSet);
 
  cout << "histograms done" << endl;
  cout << "initialize binned PDFs" << endl;

  RooHistPdf* SMHiggsPdf = new RooHistPdf("SMHiggsPdf","SMHiggsPdf",argSet,SMdataHist);
  RooHistPdf* PSHiggsPdf = new RooHistPdf("PSHiggsPdf","PSHiggsPdf",argSet,PSdataHist);
  RooHistPdf* EWKZZ4lPdf = new RooHistPdf("EWKZZ4lPdf","EWKZZ4lPdf",argSet,EWKdataHist);

  cout << "PDFs done" << endl;

  //---------------- fill vector of measurables -------------------------

  vector<RooRealVar*> meas;
  meas.push_back(costhetastar);
  meas.push_back(costheta1);

  //------------------- Initialize toolbox: -----------------------------------------
  //                    signal, alternative signal, background, vector of measurables
  //---------------------------------------------------------------------------------


  LikelihoodToolbox<RooHistPdf,RooHistPdf> Toolbox(SMHiggsPdf,PSHiggsPdf,EWKZZ4lPdf,meas);

  //------------- adjust parametrs for plotting and calculation LnL ratio

  Toolbox.sepParams_nToys=1;
  Toolbox.sepParams_numEvents=1;
  Toolbox.sepParams_numSignal=40;
  Toolbox.sepParams_rangeLow=-40;
  Toolbox.sepParams_rangeHigh=40;
  Toolbox.sepParams_bins=80;

  //----------------------------------------------------------
  // plot log likelihood ratio using toys generated from 
  // EWKZZ4lPdf*f+SMHiggsPdf
  //----------------------------------------------------------
  Toolbox.makeLnLRPlot(true)->Draw();
  //----------------------------------------------------------
  // plot log likelihood ratio using toys generated from 
  // EWKZZ4lPdf*f+PSHiggsPdf
  //----------------------------------------------------------
  Toolbox.makeLnLRPlot(false)->Draw("SAME");

}

