#include <cstdlib>
#include <iostream>
#include "RooRealVar.h"
#include "TTree.h"
#include "TFile.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TCanvas.h"

using namespace RooFit;

void testSignal(int mH=150, double low_M=120,double high_M=170, int btag=0){

  gSystem->Load("libRooFit");  
  gROOT->ProcessLine(".L ../PDFs/Roo2l2jMasses2D.cc+");
  gROOT->ProcessLine(".L ../PDFs/Roo2l2jMasses2D_DoubleCB.cc+");

  //================ Parameters for Signal PDF ===============
            //=====================================

 char* name="";

  //================== measurables

  cout << "initializing measurables..." <<endl;

  RooRealVar mLL("mLL","mZ*",20,80);
  RooRealVar mZZ("mZZ","m_{ZZ}",low_M,high_M);
  RooRealVar mJJ("mJJ","m_{jj}",75,105);
  RooRealVar nBTags("nBTags","nBTags",-1,2);  
  RooRealVar weight("weight","weight",0,10);
  RooRealVar MCmatch("MCmatch","MCmatch",0,10);
  //================== P_1(m'Z*,mZZ) ==================
	//=============================

  cout << "initializing parameters... " << endl;

  RooRealVar mZ("mZ","m_{Z}",60,120);
  RooRealVar Gamma("Gamma","#Gamma",0,100);
  
  mZ.setVal(91.2);   mZ.setConstant(kTRUE);
  Gamma.setVal(0.); Gamma.setConstant(kTRUE);

  RooRealVar CMS_zz4l_mean_sig("CMS_zz4l_mean_sig","CMS_zz4l_mean_sig",mH,0,1000);
  //CMS_zz4l_mean_sig.setConstant(kTRUE);
  RooRealVar CMS_zz4l_sigma_sig("CMS_zz4l_sigma_sig","CMS_zz4l_sigma_sig",1.95714,0.,100);
  //CMS_zz4l_sigma_sig.setConstant(kTRUE);

  RooRealVar unmatched_mean("unmatched_mean","unmatched_mean",141.037,0,300);
  //unmatched_mean.setConstant(kTRUE);
  RooRealVar unmatched_width("unmatched_width","unmatched_width",14.7561,0,1000);  
  //unmatched_width.setConstant(kTRUE);

  cout << "defining PDF..." << endl;

  RooGaussian G1("G1","G1",mZZ,CMS_zz4l_mean_sig,CMS_zz4l_sigma_sig);
  RooGaussian G2("G2","G2",mLL,unmatched_mean,unmatched_width);

  RooProdPdf totalPDF_sig("totalPDF_sig","totalPDF_sig",G1,G2);

  //============== load data ========================
	//============================
  
  cout << "loading data..." << endl;

  char fileName[100];
  if(mH==130||mH==150)
    sprintf(fileName,"../dataFiles/withHLTweights/summer11_SMHiggs_%iGeV_lowmass.root",mH);
  if(mH==140||mH==160)
    sprintf(fileName,"../dataFiles/withHLTweights/spring11_SMHiggs_%iGeV_lowmass.root",mH);

  TFile *file = new TFile(fileName);
  char cutString_matched[100];
  char cutString_unmatched[100];
  char cutString[100];

  sprintf(cutString_matched,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<80&&mJJ>75&&mJJ<105&&nBTags==%i&&MCmatch==1",high_M,low_M,btag);
  sprintf(cutString_unmatched,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<80&&mJJ>75&&mJJ<105&&nBTags==%i&&MCmatch==0",high_M,low_M,btag);
  sprintf(cutString,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<80&&mJJ>75&&mJJ<105&&nBTags==%i",high_M,low_M,btag);

  //sprintf(cutString_matched,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<80&&mJJ>75&&mJJ<105&&MCmatch==1",high_M,low_M);
  //sprintf(cutString_unmatched,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<80&&mJJ>75&&mJJ<105&&MCmatch==0",high_M,low_M);
  //sprintf(cutString,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<80&&mJJ>75&&mJJ<105",high_M,low_M);

  cout << "check1" << endl;

  TTree* t;
  if(file)
    t = (TTree*) file->Get("AngularInfo");
  else{
    cout << "ERROR: could not load file..." << endl;
    return;
  }

  cout << "check2" << endl;

  if(!t){
    cout << "ERROR: could not load tree..." << endl;
    return;
  }
  
  RooDataSet data("data","data",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags,weight,MCmatch),cutString,"weight");

  cout << "fitting data..." << endl;

  totalPDF_sig.fitTo(data,SumW2Error(kTRUE));

  CMS_zz4l_mean_sig.setConstant(kTRUE);
  CMS_zz4l_sigma_sig.setConstant(kTRUE);
  unmatched_mean.setConstant(kTRUE);
  unmatched_width.setConstant(kTRUE);

  cout << "plotting data/fits..." << endl;

  RooPlot *plotmLL=mLL.frame(20,80,40);
  RooPlot *plotmZZ=mZZ.frame(low_M,high_M,50);

  data.plotOn(plotmZZ,DataError(RooAbsData::SumW2));
  totalPDF_sig.plotOn(plotmZZ);//,FillColor(3),DrawOption("F"));
  data.plotOn(plotmLL,DataError(RooAbsData::SumW2));
  totalPDF_sig.plotOn(plotmLL);//,FillColor(3),DrawOption("F"));

  cout << "drawing..." << endl;

  TCanvas*c = new TCanvas("c","c",800,400);
  c->Divide(2,1);
  c->cd(1);
  plotmLL->Draw();
  c->cd(2);  
  plotmZZ->Draw();	

  for (int i=0; i<plotmZZ->numItems(); i++) {
    TString obj_name=plotmZZ->nameOf(i); if (obj_name=="") continue;
    cout << Form("%d. '%s'\n",i,obj_name.Data());
  }

  TLegend *leg = new TLegend(.2,.8,.6,.6);
  leg->SetTextSize(0.036);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry("totalPDF_sig_Int[mLL]_Norm[mLL,mZZ]","Signal Fit","l");
  leg->AddEntry("h_data","0 b-tag","p");
  leg->Draw();

  RooWorkspace* w = new RooWorkspace("w","w");
  w->addClassDeclImportDir("/afs/cern.ch/cms/slc5_amd64_gcc434/lcg/roofit/5.28.00a-cms12/include/");
  w->import(totalPDF_sig);

  char outWorkspace[100];
  sprintf(outWorkspace,"2DWorkspace_%i.root",mH);

  TFile *workspaceFile = new TFile(outWorkspace,"RECREATE");
  w->Write("w");

}
