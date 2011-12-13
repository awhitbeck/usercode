#include <iostream>
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "TFile.h"
#include "TTree.h"
#include "RooPlot.h"
#include "TCanvas.h"
#include "RooPolynomial.h"

using namespace RooFit;

void fitCosThetaStar(char* fileName="../dataFiles/summer11_SMHiggs_150GeV_lowmass.root",double lowMassCut=200, double highMassCut=250){

  // ============= measurables =================
  // ========= NOTE: name must match branch name in tree...

  RooRealVar cosThetaStar("cosThetaStar","cosThetaStar",-1,1);
  RooRealVar mZZ("mZZ","mZZ",100,1000);
  RooRealVar weight("weight","weight",0,10000);

  // ============= Parameters for PDF ===============

  RooRealVar a1("a1","a1",0,-100,100);
  RooRealVar a2("a2","a2",0,-100,100);
  RooRealVar a3("a3","a3",0,-100,100);
  
  // ============= Define PDF ==================

  RooPolynomial poly("poly","poly",cosThetaStar,RooArgList(a1,a2,a3));

  // ============= convert cuts to char* ==============

  char cutString[100];
  sprintf(cutString,"mZZ>%f&&mZZ<%f",lowMassCut,highMassCut);

  // ============= Load Data ==================

  TFile *file = new TFile(fileName);
  RooDataSet data("data","data",(TTree*)file->Get("AngularInfo"),RooArgSet(cosThetaStar,mZZ,weight),cutString,"weight");

  // ============= fit data ===================

  poly.fitTo(data,DataError(RooAbsData::SumW2));

  // ============= Plot =============

  RooPlot *hsPlot = cosThetaStar.frame(-1,1,20);

  data.plotOn(hsPlot,DataError(RooAbsData::SumW2));
  poly.plotOn(hsPlot);

  TCanvas *cFits = new TCanvas("c","c",600,600);
  hsPlot->Draw();
  
  
}
