#include <cstdlib>
#include <iostream>
#include "RooRealVar.h"
#include "TTree.h"
#include "TFile.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TCanvas.h"
#include "RooWorkspace.h"
#include "RooPolynomial.h"
#include "RooBernstein.h"
#include "TLegend.h"

using namespace RooFit;

void fitmZZBackground(
		      int mH=150,
		      int btag=-1,
		      int leptType=0,
		      double lowMassCut=120, 
		      double highMassCut=170,
		      char* fileName="../dataFiles/4fbData/summer11_data_4600pb_lowmass.root"
		      ){

  //================== measurables

  cout << "initializing measurables..." <<endl;

  RooRealVar mLL("mLL","mZ*",0,120);
  RooRealVar mZZ("mZZ","m_{ZZ}",lowMassCut,highMassCut);
  RooRealVar mJJ("mJJ","m_{jj}",50,150);
  RooRealVar nBTags("nBTags","nBTags",-1,10);
  RooRealVar met("met","met",0,1000);
  RooRealVar lep("lep","lep",0,1000);
  
  //================== P_1(m'Z*,mZZ) ==================
	//=============================

  cout << "initializing parameters... " << endl;

  //================= G(m'Z*) ========================
 	//=============================


  // fit to the inclusive sidebands:
  //-1 p1           0.000
  //0  p2           4.34045e-01
  //1  p3           3.44987e+00   1.04942e+00   3.80696e-03  -1.53365e+00
  //2  p4           3.07162e+00   7.39223e-01   2.84360e-03  -1.53574e+00

  // fit to the include signal region:
  // 0  p1           0.00
  // 1  p2           6.04933e-01   
  // 2  p3           3.56323e+00   9.57393e-01   4.09119e-03  -1.53299e+00
  // 3  p4           3.23295e+00   6.31555e-01   2.85665e-03  -1.53506e+00

  RooRealVar p1("p1","p1",0,0.,100000);
  p1.setConstant(kTRUE);
  RooRealVar p2("p2","p2",6.04933e-01,0.,100000);
  p2.setConstant(kTRUE);
  RooRealVar p3("p3","p3",3.56323,0.,10000);
  //p3.setConstant(kTRUE);
  RooRealVar p4("p4","p4",3.23295,0.,10000);
  //p4.setConstant(kTRUE);

  RooBernstein background("background","background",mZZ,RooArgList(p1,p2,p3,p4));
  //============== load data ========================
	//============================
  
  cout << "loading data..." << endl;

  TFile *file = new TFile(fileName);
  char cutString_SB[100], cutString[100];
  char cutString_0btag[100],cutString_1btag[100],cutString_2btag[100];
  char cutString_SB0[100],cutString_SB1[100],cutString_SB2[100];

  sprintf(cutString_SB,"mZZ<%f&&mZZ>%f&&((mJJ>60&&mJJ<75)||(mJJ>105&&mJJ<130))",highMassCut,lowMassCut);
  sprintf(cutString,"mZZ<%f&&mZZ>%f&&mJJ<105&&mJJ>75",highMassCut,lowMassCut);  
  sprintf(cutString_SB0,"mZZ<%f&&mZZ>%f&&((mJJ>60&&mJJ<75)||(mJJ>105&&mJJ<130))&&nBTags==0",highMassCut,lowMassCut);
  sprintf(cutString_SB1,"mZZ<%f&&mZZ>%f&&((mJJ>60&&mJJ<75)||(mJJ>105&&mJJ<130))&&nBTags==1",highMassCut,lowMassCut);
  sprintf(cutString_SB2,"mZZ<%f&&mZZ>%f&&((mJJ>60&&mJJ<75)||(mJJ>105&&mJJ<130))&&nBTags==2&&met<50",highMassCut,lowMassCut);    
  
  sprintf(cutString_0btag,"mZZ<%f&&mZZ>%f&&mJJ>75&&mJJ<105&&nBTags==0&&lep==%i",highMassCut,lowMassCut,leptType);
  sprintf(cutString_1btag,"mZZ<%f&&mZZ>%f&&mJJ>75&&mJJ<105&&nBTags==1&&lep==%i",highMassCut,lowMassCut,leptType);
  sprintf(cutString_2btag,"mZZ<%f&&mZZ>%f&&mJJ>75&&mJJ<105&&nBTags==2&&lep==%i&&met<50",highMassCut,lowMassCut,leptType);

  cout << "check1" << endl;

  RooDataSet data_SB("data_SB","data_SB",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ),cutString_SB);
  RooDataSet data("data","data",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ),cutString);
  RooDataSet data_SB0("data_SB0","data_SB0",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags),cutString_SB0);
  RooDataSet data_SB1("data_SB1","data_SB1",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags),cutString_SB1);
  RooDataSet data_SB2("data_SB2","data_SB2",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags,met),cutString_SB2);
  RooDataSet data_0btag("data_0btag","data_0btag",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags,lep),cutString_0btag);
  RooDataSet data_0b = (RooDataSet) data_0btag.reduce(RooArgSet(mZZ,mLL));
  RooDataSet data_1btag("data_1btag","data_1btag",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags,lep),cutString_1btag);  
  RooDataSet data_1b = (RooDataSet) data_1btag.reduce(RooArgSet(mZZ,mLL));
  RooDataSet data_2btag("data_2btag","data_2btag",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags,lep,met),cutString_2btag);    
  RooDataSet data_2b = (RooDataSet) data_2btag.reduce(RooArgSet(mZZ,mLL));
  
  
  cout << "fitting data..." << endl;
  
  RooFitResult* r;
  /*
  if(btag==0)
    r = background.fitTo(data_SB0,Save(kTRUE),Minimizer("Minuit","simplex"));
  if(btag==1)
    r = background.fitTo(data_SB1,Save(kTRUE),Minimizer("Minuit","simplex"));
  if(btag==2)
    r = background.fitTo(data_SB2,Save(kTRUE),Minimizer("Minuit","simplex"));
  if(btag==-1)
  */

  // watch out!!!! this is fitting the siganl region (done as cross check)
  r = background.fitTo(data,Save(kTRUE),Minimizer("Minuit","simplex"));

  p1.setConstant(kTRUE);
  p2.setConstant(kTRUE);
  p3.setConstant(kTRUE);

  //generate toys...

  RooDataSet *data_toy0 = background.generate(mZZ,500);
  data_toy0->SetName("data_toy0");
  RooDataSet *data_toy1 = background.generate(mZZ,505);
  data_toy1->SetName("data_toy1");
  RooDataSet *data_toy2 = background.generate(mZZ,510);
  data_toy2->SetName("data_toy2");
  RooDataSet *data_toy3 = background.generate(mZZ,515);
  data_toy3->SetName("data_toy3");
  RooDataSet *data_toy4 = background.generate(mZZ,520);
  data_toy4->SetName("data_toy4");
  RooDataSet *data_toy5 = background.generate(mZZ,525);
  data_toy5->SetName("data_toy5");
  

  cout << "plotting data/fits..." << endl;

  //this was changed to plot data in signal region!!!!!
  RooPlot *plotmZZ=mZZ.frame(125,highMassCut,20);
  if(btag==0)
    data_0b.plotOn(plotmZZ);
  if(btag==1)
    data_1b.plotOn(plotmZZ);
  if(btag==2)
    data_2b.plotOn(plotmZZ);
  if(btag==-1)
    data.plotOn(plotmZZ);
  //background.plotOn(plotmZZ,VisualizeError(*r,10),FillColor(2));
  //background.plotOn(plotmZZ,VisualizeError(*r,5),FillColor(6));                      
  //background.plotOn(plotmZZ,VisualizeError(*r,3),FillColor(7));                      
  background.plotOn(plotmZZ,VisualizeError(*r,2),FillColor(5));                      
  background.plotOn(plotmZZ,VisualizeError(*r,1),FillColor(3));   
  background.plotOn(plotmZZ);


  //this was changed to plot data in signal region!!!!!
  if(btag==0){
    data_0b.plotOn(plotmZZ);
    //data_0btag.plotOn(plotmZZ,MarkerColor(2),MarkerStyle(kCircle));
  }
  if(btag==1){
    data_1b.plotOn(plotmZZ);
    //data_1btag.plotOn(plotmZZ,MarkerColor(2),MarkerStyle(kCircle));
  }
  if(btag==2){
    data_2b.plotOn(plotmZZ);
    //data_2btag.plotOn(plotmZZ,MarkerColor(2),MarkerStyle(kCircle));
  }
  if(btag==-1){
    data.plotOn(plotmZZ);
    //data.plotOn(plotmZZ,MarkerColor(2),MarkerStyle(kCircle));
  }

  cout << "drawing..." << endl;

  TCanvas*c = new TCanvas("c","c",500,500);
  plotmZZ->Draw();
  
  for (int i=0; i<plotmZZ->numItems(); i++) {
    TString obj_name=plotmZZ->nameOf(i); if (obj_name=="") continue;
    cout << Form("%d. '%s'\n",i,obj_name.Data());
  }

  TLegend *leg = new TLegend(.2,.8,.6,.6);
  char legendHeader[10];
  sprintf(legendHeader,"%i b-tag",btag);
  leg->SetHeader(legendHeader);
  leg->SetTextSize(0.036);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry("background_Norm[mZZ]","Background Fit","l");
  leg->AddEntry("h_data_SB","Inclusive Data","p");
  leg->Draw();

  cout << "saving canvas..." << endl;
  
  char imageName[50];
  sprintf(imageName,"mZZFitBackgroud_signalRegion_%ibtag_%i-%i.eps",btag,(int)lowMassCut,(int)highMassCut);
  c->SaveAs(imageName);
  //========================================
  //save everything to workspace ===========
  //========================================

  char inWorkspace[50], outWorkspace[50];
  if(leptType==0){
    sprintf(inWorkspace,"hzz2l2q_ee%i_%i.root",btag,mH);
    sprintf(outWorkspace,"hzz2l2q_ee%i_%i.input.root",btag,mH);
  }
  if(leptType==1){
    sprintf(inWorkspace,"hzz2l2q_mm%i_%i.root",btag,mH);
    sprintf(outWorkspace,"hzz2l2q_mm%i_%i.input.root",btag,mH);
  }

  TFile *workspaceFile = new TFile(inWorkspace);
  RooWorkspace* w = (RooWorkspace*) workspaceFile->Get("w");

  w->import(background);
  w->import(*data_toy0);
  w->import(*data_toy1);
  w->import(*data_toy2);  
  w->import(*data_toy3);
  w->import(*data_toy4);
  w->import(*data_toy5);

  if(btag==0)
    w->import(data_0b,Rename("dataset_obs"));
  if(btag==1)
    w->import(data_1b,Rename("dataset_obs"));
  if(btag==2)
    w->import(data_2b,Rename("dataset_obs"));

  TFile *newFile = new TFile(outWorkspace,"RECREATE");
  w->Write("w");

}
