#include <vector>
#include "RooRealVar.h"
#include "../src/ScalarPdfFactoryZH.cc"
#include "RooPlot.h"
#include "TChain.h"
#include "RooDataSet.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TAxis.h"

using namespace RooFit;
using namespace std;

enum variables{kcostheta1,kcostheta2,kphi};

void plotILCproj(variables var=kcostheta2){

  RooRealVar* costheta1 = new RooRealVar("costheta1","cos#theta_{1}",-1.,1.);
  RooRealVar* costheta2 = new RooRealVar("costheta2","cos#theta_{2}",-1.,1.);
  RooRealVar* phi = new RooRealVar("phi","#Phi",-TMath::Pi(),TMath::Pi());

  vector<RooRealVar*> meas;
  meas.push_back(costheta1);
  meas.push_back(costheta2);
  meas.push_back(phi);

  RooRealVar* mH = new RooRealVar("mH","m_{H}",125.);
 
  ScalarPdfFactoryZH SMHiggs(costheta1,costheta2,phi,mH,2,false);

  ScalarPdfFactoryZH psScalar(costheta1,costheta2,phi,mH,2,false);
  psScalar.fa2->setVal(0.0);
  psScalar.fa3->setVal(0.9999999);
  psScalar.phia2->setVal(0.0);
  psScalar.phia3->setVal(0.0);

  ScalarPdfFactoryZH fa3p1Scalar(costheta1,costheta2,phi,mH,2,false);
  fa3p1Scalar.fa2->setVal(0.0);
  fa3p1Scalar.fa3->setVal(0.848419);
  fa3p1Scalar.phia2->setVal(0.0);
  fa3p1Scalar.phia3->setVal(0.0);

  ScalarPdfFactoryZH fa3p1_90_Scalar(costheta1,costheta2,phi,mH,2,false);
  fa3p1_90_Scalar.fa2->setVal(0.0);
  fa3p1_90_Scalar.fa3->setVal(0.848419);
  fa3p1_90_Scalar.phia2->setVal(0.0);
  fa3p1_90_Scalar.phia3->setVal(3.1415/2.);

  TChain* SMHtree = new TChain("SelectedTree");
  SMHtree->Add("/export/d1/scratch/snowmass2013/ilcsamples/signal/spin0/Events_20130618/unweighted_unpol_g1_1M_false.root");

  RooDataSet* SMHdata = new RooDataSet("SMHdata","SMHdata",SMHtree,RooArgSet(*costheta1,*costheta2,*phi));

  TChain* PStree = new TChain("SelectedTree");
  PStree->Add("/export/d1/scratch/snowmass2013/ilcsamples/signal/spin0/Events_20130618/unweighted_unpol_g4_1M_false.root");

  RooDataSet* PSdata = new RooDataSet("PSdata","PSdata",PStree,RooArgSet(*costheta1,*costheta2,*phi));
  
  TChain* fa3p1tree = new TChain("SelectedTree");
  fa3p1tree->Add("/export/d1/scratch/snowmass2013/ilcsamples/signal/spin0/Events_20130618/unweighted_unpol_g1_p_g4_1M_false.root");

  RooDataSet* fa3p1data = new RooDataSet("fa3p1data","fa3p1data",fa3p1tree,RooArgSet(*costheta1,*costheta2,*phi));

  TChain* fa3p1_90_tree = new TChain("SelectedTree");
  fa3p1_90_tree->Add("/export/d1/scratch/snowmass2013/ilcsamples/signal/spin0/Events_20130618/unweighted_unpol_g1_p_ig4_1M_false.root");

  RooDataSet* fa3p1_90_data = new RooDataSet("fa3p1_90_data","fa3p1_90_data",fa3p1_90_tree,RooArgSet(*costheta1,*costheta2,*phi));

  // create rooplot and plot data/pdfs
  RooPlot* plot = (meas[var])->frame(20);
  SMHdata->plotOn(plot,MarkerColor(kRed),MarkerStyle(4),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None));
  SMHiggs.PDF->plotOn(plot,LineColor(kRed),LineWidth(2));
  PSdata->plotOn(plot,MarkerColor(kBlue),MarkerStyle(27),MarkerSize(1.9),XErrorSize(0),DataError(RooAbsData::None));
  psScalar.PDF->plotOn(plot,LineColor(kBlue),LineWidth(2));
  fa3p1data->plotOn(plot,MarkerColor(kGreen+3),MarkerStyle(25),MarkerSize(1.5),XErrorSize(0),DataError(RooAbsData::None));
  fa3p1Scalar.PDF->plotOn(plot,LineColor(kGreen+3),LineWidth(2));
  fa3p1_90_data->plotOn(plot,MarkerColor(kMagenta+1),MarkerStyle(20));
  fa3p1_90_Scalar.PDF->plotOn(plot,LineColor(kMagenta+1),LineWidth(2));
  //TGaxis::SetMaxDigits(3);

  TCanvas* can = new TCanvas("can","can",600,600);

  // plot styles
  gStyle->SetPadLeftMargin(0.05);
  plot->GetXaxis()->CenterTitle();
  plot->GetYaxis()->CenterTitle();
  plot->GetYaxis()->SetTitle(" ");
  plot->GetXaxis()->SetNdivisions(-505);


  plot->Draw();


}
