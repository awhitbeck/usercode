#include <cstdlib>
#include <iostream>
#include "RooRealVar.h"
#include "TTree.h"
#include "TFile.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TCanvas.h"
#include "RooFitResult.h"
#include "../PDFs/RooDoubleCB.h"
#include "RooAddPdf.h"
#include "RooGaussian.h"
#include "RooWorkspace.h"

using namespace RooFit;

RooFitResult* fitmZZSignal(
		  int mH=150, 
		  int btag=0, 
		  int leptType=0,
		  bool returnMatched=true,
		  bool draw=false,
		  double low_M=120,
		  double high_M=180
		  ){

  //gSystem->Load("libRooFit");  
  //gROOT->ProcessLine(".L ../PDFs/RooDoubleCB.cc+");

  //================ Parameters for Signal PDF ===============
            //=====================================

 char* name="";

  //================== measurables

  cout << "initializing measurables..." <<endl;


  RooRealVar mLL("mLL","m_{LL}",20,80);
  RooRealVar mZZ("mZZ","m_{ZZ}",low_M,high_M);
  RooRealVar mJJ("mJJ","m_{jj}",75,105);
  RooRealVar PUweight("PUweight","PUweight",0,10);
  RooRealVar MCmatch("MCmatch","MCmatch",0,10);
  RooRealVar nBTags("nBTags","nBTags",-1,2);
  //============= Set Parameters to Extrapolated values 
  //===================================================

  double CBmean[3]={mH,mH,mH};
  double CBwidth[3]={-34.2747+.459182*mH-.00142578*mH*mH,
		    20.5495-.285063*mH+.00108348*mH*mH,
		    25.7944-.331101*mH+.00116889*mH*mH};
  double CBalpha1[3]={1.99625-.0057799*mH,
		      20.7158-.259139*mH+.000855349*mH*mH,
		      4.57891-.0173362*mH};
  double CBalpha2[3]={-23.6484+.317242*mH-.00102403*mH*mH,
		      -1.75895+.0170921*mH,
		      26.8051-.370267*mH+.0013091*mH*mH};
  double CBn1[3]={31.47-.160475*mH,
		  9,
		  12.2657-.0574034*mH};
  double CBn2[3]={18.4988-.100371*mH,
		  16.6304-0.0902441*mH,
		  -220+3.012*mH-.00996212*mH*mH};
  double mean[3]={288.8-1.8888*mH+.00496554*mH*mH+1.2149e-5*mH*mH*mH,//127.708+.0222537*mH,
		  125.536+.241826*mH,
		  100.889+.380056*mH};
  double width[3]={568.179-6.91623*mH+.0214025*mH*mH,
		   70.7661-.377219*mH,
		   67.889-.378584*mH};
  double fval[3]={.75-.00316*mH,
	       1.26634-.00625*mH,
	       .167667};
  
  //============== P(m'Z*,mZZ) ======================
	//============================

  RooRealVar CMS_zz2l2j_mean_sig("CMS_zz2l2j_mean_sig","CMS_zz2l2j_mean_sig",CBmean[btag],0,1000);
  CMS_zz2l2j_mean_sig.setConstant(kTRUE);
  RooRealVar CMS_zz2l2j_sigma_sig("CMS_zz2l2j_sigma_sig","CMS_zz2l2j_sigma_sig",CBwidth[btag],0.,100);
  //CMS_zz2l2j_sigma_sig.setConstant(kTRUE);
  RooRealVar CMS_zz2l2j_alpha1("CMS_zz2l2j_alpha1","CMS_zz2l2j_alpha1",CBalpha1[btag],0.,10.);
  //CMS_zz2l2j_alpha1.setConstant(kTRUE);
  RooRealVar CMS_zz2l2j_n1("CMS_zz2l2j_n1","CMS_zz2l2j_n1",CBn1[btag],0,10);
  //CMS_zz2l2j_n1.setConstant(kTRUE);
  RooRealVar CMS_zz2l2j_alpha2("CMS_zz2l2j_alpha2","CMS_zz2l2j_alpha2",CBalpha2[btag],0.,10.);
  //CMS_zz2l2j_alpha2.setConstant(kTRUE);
  RooRealVar CMS_zz2l2j_n2("CMS_zz2l2j_n2","CMS_zz2l2j_n2",CBn2[btag],0,10);
  //CMS_zz2l2j_n2.setConstant(kTRUE);

  RooRealVar unmatched_mean("unmatched_mean","unmatched_mean",mean[btag],0,300);
  //unmatched_mean.setConstant(kTRUE);
  RooRealVar unmatched_width("unmatched_width","unmatched_width",width[btag],0,1000);  
  //unmatched_width.setConstant(kTRUE);

  cout << "defining PDF..." << endl;
  
  RooDoubleCB matchedPDF("matchedPDF","matchedPDF",
				      mZZ,
				      CMS_zz2l2j_mean_sig,
				      CMS_zz2l2j_sigma_sig,
				      CMS_zz2l2j_alpha1,
				      CMS_zz2l2j_n1,
				      CMS_zz2l2j_alpha2,
				      CMS_zz2l2j_n2
				      );

  RooGaussian unMatchedPDF("unMatchedPDF","unMatchedPDF",
			       mZZ,
			       unmatched_mean,
			       unmatched_width
			       );

  RooRealVar f("f","f",.1,0,1);

  RooAddPdf signal("signal","signal",unMatchedPDF,matchedPDF,f);

  //============== load data ========================
	//============================
  
  cout << "loading data..." << endl;

  char fileName[100];
  if(mH==130 || mH==150)
    sprintf(fileName,"../dataFiles/4fbData/summer11_SMHiggs_%iGeV_lowmass.root",mH);
  else
    sprintf(fileName,"../dataFiles/4fbData/spring11_SMHiggs_%iGeV_SPRING11_lowmass.root",mH);
  TFile *file = new TFile(fileName);
  char cutString_matched[100];
  char cutString_unmatched[100];
  char cutString[100];

  sprintf(cutString_matched,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<80&&mJJ>75&&mJJ<105&&nBTags==%i&&MCmatch==1",high_M,low_M,btag);
  sprintf(cutString_unmatched,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<80&&mJJ>75&&mJJ<105&&nBTags==%i&&MCmatch==0",high_M,low_M,btag);
  sprintf(cutString,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<80&&mJJ>75&&mJJ<105.&&nBTags==%i",high_M,low_M,btag);

  cout << "check1" << endl;

  TTree* t;
  if(file)
    t = (TTree*) file->Get("AngularInfo");
  else{
    cout << "ERROR: could not load file..." << endl;
    return 0;
  }

  cout << "check2" << endl;

  if(!t){
    cout << "ERROR: could not load tree..." << endl;
    return 0;
  }
  
  RooDataSet data_matched("data_matched","data_matched",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,PUweight,MCmatch,nBTags),cutString_matched,"PUweight");
  double NumMatched = data_matched.sumEntries();
  RooDataSet data_unmatched("data_unmatched","data_unmatched",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,PUweight,MCmatch,nBTags),cutString_unmatched,"PUweight");
  double NumUnmatched = data_unmatched.sumEntries();
  RooDataSet data("data","data",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,PUweight,MCmatch,nBTags),cutString,"PUweight");
  double NumTotal = data.sumEntries();

  //f.setVal(NumUnmatched/NumTotal);
  f.setVal(fval[btag]);
  f.setConstant(kTRUE);

  cout << "fitting data..." << endl;

  RooFitResult *r1;
  RooFitResult *r2; 
  
  //r1 = matchedPDF.fitTo(data_matched,SumW2Error(kTRUE),Save(kTRUE));
  //r2 = unMatchedPDF.fitTo(data_unmatched,SumW2Error(kTRUE),Save(kTRUE));

  CMS_zz2l2j_mean_sig.setConstant(kTRUE);
  CMS_zz2l2j_sigma_sig.setConstant(kTRUE);
  CMS_zz2l2j_alpha1.setConstant(kTRUE);
  CMS_zz2l2j_n1.setConstant(kTRUE);
  CMS_zz2l2j_alpha2.setConstant(kTRUE);
  CMS_zz2l2j_n2.setConstant(kTRUE);
  unmatched_mean.setConstant(kTRUE);
  unmatched_width.setConstant(kTRUE);

  cout << "plotting data/fits..." << endl;

  RooPlot *plotmZZ=mZZ.frame(low_M,high_M,40);

  data.plotOn(plotmZZ,DataError(RooAbsData::SumW2));
  signal.plotOn(plotmZZ);
  unMatchedPDF.plotOn(plotmZZ,Normalization(NumUnmatched/NumTotal),LineStyle(2));

  char imageName[50];
  TCanvas*c;
  if(draw){
    cout << "drawing..." << endl;
    
    c = new TCanvas("c","c",600,600);
    plotmZZ->Draw();
    
    cout << "saving canvas..." << endl;
    
    sprintf(imageName,"mZZExtrapolation_%iGeV_%ibtag.eps",mH,btag);
    c->SaveAs(imageName);
  }
  RooWorkspace* w = new RooWorkspace("w","w");
  w->addClassDeclImportDir("/afs/cern.ch/cms/slc5_amd64_gcc434/lcg/roofit/5.28.00a-cms12/include/");
  w->addClassDeclImportDir("../PDFs/");

  w->importClassCode(RooDoubleCB::Class(),kTRUE);
  w->importClassCode("RooDoubleCB",kTRUE);
  w->import(signal);

  char outWorkspace[50];

  if(leptType==0)
    sprintf(outWorkspace,"hzz2l2q_ee%i_%i.root",btag,mH);
  if(leptType==1)
    sprintf(outWorkspace,"hzz2l2q_mm%i_%i.root",btag,mH);

  TFile *workspaceFile = new TFile(outWorkspace,"RECREATE");
  w->Write("w");

  workspaceFile->Close();

  cout << "f: " << f.getVal() << endl;

  if(returnMatched)
    return r1;
  else
    return r2;
}
