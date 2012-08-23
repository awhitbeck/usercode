#include "TFile.h"
#include "RooWorkspace.h"
#include "RooPlot.h"
#include "TCanvas.h"
#include "RooRealVar.h"
#include "RooAbsData.h"
#include "RooAbsPdf.h"

using namespace RooFit;

void checkShapes(int mH=150, int btag=0, int chan=0, double signalNorm=11., char* workspaceName="w"){

  double Norm[3]={chan?647.0000:442.0000,chan?215.0000:145.0000,chan?15.0000:15.0000};
  
  char fileName[30];
  if(chan==0)  //  electrons!!!!
    sprintf(fileName,"%i/hzz2l2q_ee%ib.input.root",mH,btag);
  else         //  muons!!!!
    sprintf(fileName,"%i/hzz2l2q_mm%ib.input.root",mH,btag);


  TFile* file=new TFile(fileName);
  RooWorkspace* w=dynamic_cast<RooWorkspace*>(file->Get("w"));

  RooPlot* mZZplot = w->var("CMS_hzz2l2q_mZZ")->frame(120,170,40);  

  TCanvas* c = new TCanvas("c","c",800,400);

  w->data("dataset_obs")->plotOn(mZZplot);
  w->pdf("signal")->plotOn(mZZplot,LineColor(2));//,Normalization(signalNorm/w->data("dataset_obs")->sumEntries()));
  w->pdf("background")->plotOn(mZZplot,Normalization(Norm[btag]/w->data("dataset_obs")->sumEntries()));
  
  mZZplot->Draw();

}
