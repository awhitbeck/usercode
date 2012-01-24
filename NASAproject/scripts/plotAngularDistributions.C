#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "AngularPdfFactory.cc"

using namespace RooFit;

void plotAngularDistributions(){

  RooRealVar* z1mass = new RooRealVar("z1mass","m_{Z1}",20,120);
  RooRealVar* z2mass = new RooRealVar("z2mass","m_{Z2}",20,120);
  RooRealVar* costheta1 = new RooRealVar("costheta1","cos#theta_{1}",-1,1);  
  RooRealVar* costheta2 = new RooRealVar("costheta2","cos#theta_{2}",-1,1);
  RooRealVar* phi= new RooRealVar("phi","#Phi",-3.1415,3.1415);
  
  AngularPdfFactory SMHiggs(z1mass,z2mass,costheta1,costheta2,phi);
  SMHiggs.makeSMHiggs();
  SMHiggs.makeParamsConst(true);

  AngularPdfFactory PSHiggs(z1mass,z2mass,costheta1,costheta2,phi);
  PSHiggs.makePSHiggs();
  PSHiggs.makeParamsConst(true);

  TFile* fin = new TFile("/scratch/hep/ntran/HZZ_materials/datafiles/SMHiggs_125_JHU.root");
  TTree* tin = (TTree*) fin->Get("angles");
  RooDataSet data("data","data",tin,RooArgSet(*z1mass,*z2mass,*costheta1,*costheta2,*phi));

  TCanvas* c = new TCanvas("c","c",1200,800);
  c->Divide(3,2);

  RooPlot *plot_m1 = z1mass->frame(55);
  data.plotOn(plot_m1);
  PSHiggs.PDF->plotOn(plot_m1);
  SMHiggs.PDF->plotOn(plot_m1,LineColor(2));
  c->cd(1);
  plot_m1->Draw();

  RooPlot *plot_m2 = z2mass->frame(55);
  data.plotOn(plot_m2);
  PSHiggs.PDF->plotOn(plot_m2);
  SMHiggs.PDF->plotOn(plot_m2,LineColor(2));
  c->cd(2);
  plot_m2->Draw();

  RooPlot *plot_h1 = costheta1->frame(55);
  data.plotOn(plot_h1);
  PSHiggs.PDF->plotOn(plot_h1);
  SMHiggs.PDF->plotOn(plot_h1,LineColor(2));
  c->cd(3);
  plot_h1->Draw();
  RooPlot *plot_h2 = costheta2->frame(55);
  data.plotOn(plot_h2);
  PSHiggs.PDF->plotOn(plot_h2);
  SMHiggs.PDF->plotOn(plot_h2,LineColor(2));
  c->cd(4);
  plot_h2->Draw();
  RooPlot *plot_Phi = phi->frame(55);
  data.plotOn(plot_Phi);
  PSHiggs.PDF->plotOn(plot_Phi);
  SMHiggs.PDF->plotOn(plot_Phi,LineColor(2));
  c->cd(5);
  plot_Phi->Draw();

}
