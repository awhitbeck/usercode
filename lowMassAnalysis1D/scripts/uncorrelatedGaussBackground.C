#include <cstdlib>
#include <iostream>
#include "RooRealVar.h"
#include "TTree.h"
#include "TFile.h"
#include "RooPlot.h"
#include "RooDataSet.h"
//#include "../PDFs/Roo2l2jMasses2D_Bkg.h"
#include "TCanvas.h"

using namespace RooFit;

void testBackground(char* fileName="../dataFiles/forMatthew/summer11_data_lowmass.root",double lowMassCut=120, double highMassCut=170, int mH=150){

  gROOT->ProcessLine(".L ../PDFs/Roo2l2jMasses2D_Bkg.cc+");

  //================== measurables

  cout << "initializing measurables..." <<endl;

  RooRealVar mLL("mLL","mZ*",20,80);
  RooRealVar mZZ("mZZ","m_{ZZ}",lowMassCut,highMassCut);
  RooRealVar mJJ("mJJ","m_{jj}",50,150);
  RooRealVar nBTags("nBTags","nBTags",-1,3);
  RooRealVar met("met","met",0,1000);

  //================== P_1(m'Z*,mZZ) ==================
	//=============================

  cout << "initializing parameters... " << endl;

  RooRealVar mZ("mZ","m_{Z}",60,120);
  RooRealVar Gamma("Gamma","#Gamma",0,100);
  
  mZ.setVal(91.2);   mZ.setConstant(kTRUE);
  Gamma.setVal(0.); Gamma.setConstant(kTRUE);

  RooRealVar p1("p1","p1",1.54516e+01.,0,1000);
  //p1.setConstant(kTRUE);
  RooRealVar p2("p2","p2",1.54516e+01.,0,1000);
  //p2.setConstant(kTRUE);

  RooPolynomial P1("P1","P1",mLL,RooArgList(p1),0);
  RooPolynomial P2("P2","P2",mZZ,RooArgList(p2),0);

  RooProdPdf totalPDF_bkg("totalPDF_bkg","totalPDF_bkg",P1,P2);

  //============== load data ========================
	//============================
  
  cout << "loading data..." << endl;

  TFile *file = new TFile(fileName);
  char cutString_SB[100], cutString_0btag[100],cutString_1btag[100],cutString_2btag[100];
  char cutString_SB_0btag[100],cutString_SB_1btag[100],cutString_SB_2btag[100];

  sprintf(cutString_SB_0btag,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<80&&(mJJ<75||mJJ>105)&&nBTags==0",highMassCut,lowMassCut);
  sprintf(cutString_SB_1btag,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<80&&(mJJ<75||mJJ>105)&&nBTags==1",highMassCut,lowMassCut);
  sprintf(cutString_SB_2btag,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<80&&(mJJ<75||mJJ>105)&&nBTags==2&&met<50",highMassCut,lowMassCut);
  sprintf(cutString_SB,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<80&&((mJJ>60&&mJJ<75)||(mJJ<130&&mJJ>105))",highMassCut,lowMassCut);  
  sprintf(cutString_0btag,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<80&&mJJ>75&&mJJ<105&&nBTags==0",highMassCut,lowMassCut);
  sprintf(cutString_1btag,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<80&&mJJ>75&&mJJ<105&&nBTags==1",highMassCut,lowMassCut);
  sprintf(cutString_2btag,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<80&&mJJ>75&&mJJ<105&&nBTags==2&&met<50",highMassCut,lowMassCut);

  cout << "check1" << endl;

  RooDataSet data_SB_0btag("data_SB_0btag","data_SB_0btag",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags),cutString_SB_0btag);
  RooDataSet data_SB_1btag("data_SB_1btag","data_SB_1btag",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags),cutString_SB_1btag);
  RooDataSet data_SB_2btag("data_SB_2btag","data_SB_2btag",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags,met),cutString_SB_2btag);
  RooDataSet data_SB("data_SB","data_SB",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags),cutString_SB);
  RooDataSet data_0btag("data_0btag","data_0btag",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags),cutString_0btag);
  RooDataSet data_0b=(RooDataSet)data_0btag.reduce(RooArgSet(mZZ,mLL));
  RooDataSet data_1btag("data_1btag","data_1btag",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags),cutString_1btag);
  RooDataSet data_1b=(RooDataSet)data_1btag.reduce(RooArgSet(mZZ,mLL));
  RooDataSet data_2btag("data_2btag","data_2btag",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags,met),cutString_2btag);
  RooDataSet data_2b=(RooDataSet)data_2btag.reduce(RooArgSet(mZZ,mLL));
      
  
  cout << "fitting data..." << endl;

  totalPDF_bkg.fitTo(data_SB);
  
  p1.setConstant(kTRUE);
  p2.setConstant(kTRUE);

  cout << "plotting data/fits..." << endl;

  RooPlot *plot=mLL.frame(20,80,40);
  RooPlot *plot2=mZZ.frame(lowMassCut,highMassCut,40);
  data_SB.plotOn(plot2);
  totalPDF_bkg.plotOn(plot2);//DrawOption("F"),FillColor(3));
  data_SB.plotOn(plot);
  totalPDF_bkg.plotOn(plot);//,DrawOption("F"),FillColor(3));

  cout << "drawing..." << endl;

  TCanvas*c = new TCanvas("c","c",800,400);
  c->Divide(2,1);
  c->cd(1);
  plot->Draw();
  c->cd(2);  
  plot2->Draw();	

  for (int i=0; i<plot2->numItems(); i++) {
    TString obj_name=plot2->nameOf(i); if (obj_name=="") continue;
    cout << Form("%d. '%s'\n",i,obj_name.Data());
  }

  TLegend *leg = new TLegend(.2,.8,.6,.6);
  leg->SetTextSize(0.036);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry("totalPDF_bkg_Int[mLL]_Norm[mLL,mZZ]","Background Fit","l");
  leg->AddEntry("h_data_SB","Inclusive Sideband","p");
  leg->Draw();

  char inWorkspace[100];
  sprintf(inWorkspace,"2DWorkspace_%i.root",mH);
  char outWorkspace[100];
  sprintf(outWorkspace,"Final2DWorkspace_%i.root",mH);

  TFile *workspaceFile = new TFile(inWorkspace);
  RooWorkspace* w = (RooWorkspace*) workspaceFile->Get("w");

  w->import(totalPDF_bkg);
  w->import(data_0b);
  w->import(data_1b);
  w->import(data_2b);
  
  TFile *newFile = new TFile(outWorkspace,"RECREATE");
  w->Write("w");

}
