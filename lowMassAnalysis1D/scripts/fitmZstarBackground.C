#include <cstdlib>
#include <iostream>
#include "RooRealVar.h"
#include "TTree.h"
#include "TFile.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "../PDFs/Roo2l2jMasses2D_Bkg.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TLegend.h"
#include "TString.h"
#include "RooWorkspace.h"
#include "RooFitResult.h"

using namespace RooFit;

RooFitResult* fitmZstarBackground(double lowMassCut=120, double highMassCut=170, double normOverride=1.0, bool fix=false, char* fileName="../dataFiles/4fbData/summer11_data_4600pb_lowmass.root"){

  //================== measurables

  cout << "initializing measurables..." <<endl;

  RooRealVar mLL("mLL","mZ*",20,90);
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

  //================= G(m'Z*) ========================
 	//=============================
  /* 
  5  p1           7.71624e+02   8.85781e+02   7.45648e-03   5.74301e-01
		  			     
  6  p2           3.46748e-08   2.41010e+00   6.48880e-03  -1.57078e+00
		  			     
  7  p3           4.76286e-11   5.96397e-02   1.02176e-03  -1.57080e+00
		  			     
  8  p4           2.49963e-02   2.89589e-02   2.01776e-05  -1.56080e+00
		  			     
  1  a1           5.61787e-03   4.12978e-02   7.10655e-04  -1.57554e+00
		  			     
  2  a2           4.29136e-01   7.63264e-01   4.33096e-04  -1.52936e+00
		  			     
  3  a3           3.30070e+00   5.36060e+00   2.08141e-04  -1.45583e+00
		  			     
  4  a4           2.94961e+00   4.83434e+00   3.85837e-04  -1.46212e+00

  */


  RooRealVar p1("p1","p1" ,7.71624e+02,0.,1000.);
  //p1.setConstant(kTRUE); 		     
  RooRealVar p2("p2","p2" ,3.46748e-08,0.,1000.);
  //p2.setConstant(kTRUE); 			     
  RooRealVar p3("p3","p3" ,4.76286e-11,0.,1000.);
  //p3.setConstant(kTRUE); 			     
  RooRealVar p4("p4","p4" ,2.49963e-02,0.,1000.);
  //p4.setConstant(kTRUE); 			     
  RooRealVar a1("a1","a1" ,5.61787e-03,0.,1000.);
  a1.setConstant(kTRUE); 			     
  RooRealVar a2("a2","a2" ,4.29136e-01,0.,1000.);
  a2.setConstant(kTRUE); 			     
  RooRealVar a3("a3","a3" ,3.30070e+00,0.,1000.);
  a3.setConstant(kTRUE); 			     
  RooRealVar a4("a4","a4" ,2.94961e+00,0.,1000.);
  a4.setConstant(kTRUE);

  Roo2l2jMasses2D_Bkg totalPDF_bkg("totalPDF_bkg","totalPDF_bkg",mLL,mZ,mZZ,Gamma,p1,p2,p3,p4,a1,a2,a3,a4);

  //============== load data ========================
	//============================
  
  cout << "loading data..." << endl;

  TFile *file = new TFile(fileName);
  char cutString_SB[100], cutString_0btag[100],cutString_1btag[100],cutString_2btag[100];
  char cutString_SB_0btag[100],cutString_SB_1btag[100],cutString_SB_2btag[100],cutString[100];

  sprintf(cutString_SB_0btag,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<90&&(mJJ<75||mJJ>105)&&nBTags==0",highMassCut,lowMassCut);
  sprintf(cutString_SB_1btag,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<90&&(mJJ<75||mJJ>105)&&nBTags==1",highMassCut,lowMassCut);
  sprintf(cutString_SB_2btag,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<90&&(mJJ<75||mJJ>105)&&nBTags==2&&met<50",highMassCut,lowMassCut);
  sprintf(cutString_SB,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<90&&((mJJ>60&&mJJ<75)||(mJJ<130&&mJJ>105))",highMassCut,lowMassCut);  
  sprintf(cutString,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<90&&mJJ>75&&mJJ<105",highMassCut,lowMassCut);  
  sprintf(cutString_0btag,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<90&&mJJ>75&&mJJ<105&&nBTags==0",highMassCut,lowMassCut);
  sprintf(cutString_1btag,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<90&&mJJ>75&&mJJ<105&&nBTags==1",highMassCut,lowMassCut);
  sprintf(cutString_2btag,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<90&&mJJ>75&&mJJ<105&&nBTags==2&&met<50",highMassCut,lowMassCut);

  cout << "check1" << endl;

  /*
  RooDataSet data_SB_0btag("data_SB_0btag","data_SB_0btag",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags),cutString_SB_0btag);
  RooDataSet data_SB_1btag("data_SB_1btag","data_SB_1btag",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags),cutString_SB_1btag);
  RooDataSet data_SB_2btag("data_SB_2btag","data_SB_2btag",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags,met),cutString_2B_2btag);
  */

  RooDataSet data_SB("data_SB","data_SB",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags),cutString_SB);

  /*
  RooDataSet data_0btag("data_0btag","data_0btag",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags),cutString_0btag);
  RooDataSet data_1btag("data_1btag","data_1btag",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags),cutString_1btag);
  RooDataSet data_2btag("data_2btag","data_2btag",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags,met),cutString_2btag);
  */

  RooFitResult* r;

  if(!fix){
    cout << "fitting data..." << endl;
    r=totalPDF_bkg.fitTo(data_SB,Save(kTRUE));
  }
 
  a1.setConstant(kTRUE);
  a2.setConstant(kTRUE);
  a3.setConstant(kTRUE);
  a4.setConstant(kTRUE);
  p1.setConstant(kTRUE);
  p2.setConstant(kTRUE);
  p3.setConstant(kTRUE);
  p4.setConstant(kTRUE);

  cout << "plotting data/fits..." << endl;

  RooPlot *plot=mLL.frame(20,90,20);
  RooPlot *plot2=mZZ.frame(lowMassCut,highMassCut,20);

  data_SB.plotOn(plot2);
  totalPDF_bkg.plotOn(plot2,Normalization(normOverride));//DrawOption("F"),FillColor(3));
  data_SB.plotOn(plot);
  totalPDF_bkg.plotOn(plot,Normalization(normOverride));//,DrawOption("F"),FillColor(3));
  
  cout << "drawing..." << endl;

  TCanvas*c = new TCanvas("c","c",800,400);
  c->Divide(2,1);
  c->cd(1);
  plot->Draw();
  char imageName[50];
  sprintf(imageName,"mZBackgroundFix_%i-%i_mZZ.eps",(int)lowMassCut,(int)highMassCut);
  c->SaveAs(imageName);
  c->cd(2);  
  plot2->Draw();	
  sprintf(imageName,"mZBackgroundFix_%i-%i_mLL.eps",(int)lowMassCut,(int)highMassCut);
  c->SaveAs(imageName);

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


  cout << "saving canvas..." << endl;

  /* 
  char imageName[50];
  if(fix)
    sprintf(imageName,"mZBackgroundFix_%i-%i.eps",(int)lowMassCut,(int)highMassCut);
  else
    sprintf(imageName,"mZBackgroundFit_%i-%i.eps",(int)lowMassCut,(int)highMassCut);
  c->SaveAs(imageName);
  */

  return r;

}
