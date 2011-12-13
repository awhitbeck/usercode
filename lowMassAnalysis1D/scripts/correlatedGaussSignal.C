#include <cstdlib>
#include <iostream>
#include "RooRealVar.h"
#include "TTree.h"
#include "TFile.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TCanvas.h"

using namespace RooFit;

void correlatedGaussSignal(int mH=150, double low_M=120,double high_M=170, int btag=0){

  gSystem->Load("libRooFit");  
  gROOT->ProcessLine(".L ../PDFs/Roo2DGauss.cc+");
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

  RooRealVar mean("mean","mean",150,0,1000);
  //mean.setConstant(kTRUE);
  RooRealVar mean2("mean2","mean2",50,0,1000);
  //mean2.setConstant(kTRUE);
  RooRealVar sigma("sigma","sigma",5,0.,100);
  //sigma.setConstant(kTRUE);
  RooRealVar sigma2("sigma2","sigma2",1.95714,0.,100);
  //sigma2.setConstant(kTRUE);
  RooRealVar sigma3("sigma3","sigma3",2.,0.,10000);
  sigma3.setConstant(kTRUE);

  cout << "defining PDF..." << endl;

  Roo2DGauss totalPDF_sig("totalPDF_sig","totalPDF_sig",mLL,mZZ,mean,mean2,sigma,sigma2,sigma3);

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

  mean.setConstant(kTRUE);
  mean2.setConstant(kTRUE);
  sigma.setConstant(kTRUE);
  sigma2.setConstant(kTRUE);
  sigma3.setConstant(kTRUE);

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
  w->addClassDeclImportDir("../PDFs/");

  w->importClassCode(Roo2DGauss::Class(),kTRUE);
  w->importClassCode("Roo2DGauss",kTRUE);

  w->import(totalPDF_sig);

  char outWorkspace[100];
  sprintf(outWorkspace,"Test_v2_Workspace_%i.root",mH);

  TFile *workspaceFile = new TFile(outWorkspace,"RECREATE");
  w->Write("w");

}
