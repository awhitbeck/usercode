#include <cstdlib>
#include <iostream>
#include "RooRealVar.h"
#include "TTree.h"
#include "TFile.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TCanvas.h"
#include "RooWorkspace.h"
#include "TLegend.h"
#include "TH1F.h"
#include "THStack.h"
#include "RooAddPdf.h"
#include "TStyle.h"
#include "TPaveText.h"
#include <fstream>
#include "DrawBase.C"
#include "fitTools.C"

DrawBase* db = new DrawBase("dummy");

using namespace RooFit;

TH1F* generateHisto_mZZ(char* fileName, char* treeName,float EleLumi, float MuLumi, int btag){

  cout << "Generating MC expected background distributions from:" << endl;
  cout << fileName << endl;

  TFile *f = new TFile(fileName);
  if(!f){
    return 0;
    cout << "ERROR: could not load file" << endl;
  }
  TTree *t = (TTree*) f->Get(treeName);
  if(!t){
    return 0;
    cout << "ERROR: could not load tree" << endl;
  }

  TH1F* histo=new TH1F("histo0",";m_{ZZ} [GeV];Events / 2.5 GeV",20,125,170);
  histo->Sumw2();

  double mZZ,mLL,mJJ,met,weight,lep;
  double intWeight=0.;
  double nBTags;

  t->SetBranchAddress("mZZ",&mZZ);
  t->SetBranchAddress("mLL",&mLL);
  t->SetBranchAddress("mJJ",&mJJ);
  t->SetBranchAddress("nBTags",&nBTags);
  t->SetBranchAddress("met",&met);
  t->SetBranchAddress("weight",&weight);  
  t->SetBranchAddress("lep",&lep);

  for(int iEvt=0; iEvt<t->GetEntries(); iEvt++){
    t->GetEntry(iEvt);

    if(mZZ<170&&mZZ>125&&mJJ<105&&mJJ>75&&mLL>20){//&&mLL<90){
      if(nBTags==btag && btag<2){
	if(lep==0){
	  histo->Fill(mZZ,weight*EleLumi);
	  intWeight+=weight*EleLumi;
	}else{
	  histo->Fill(mZZ,weight*MuLumi);
	  intWeight+=weight*MuLumi;
	}
      }
      else if(nBTags==btag && btag==2 && met<50){
	if(lep==0){
	  histo->Fill(mZZ,weight*EleLumi);
	  intWeight+=weight*EleLumi;
	}else{
	  histo->Fill(mZZ,weight*MuLumi);
	  intWeight+=weight*MuLumi;
	}
      }
    }
  }// end loop over events

  cout << "Expected Normalization: " << intWeight << endl;

  return histo;

}
TH1F* generateHisto_mLL(char* fileName, char* treeName,float EleLumi,float MuLumi, int btag){

  TFile *f = new TFile(fileName);
  if(!f){
    return 0;
    cout << "ERROR: could not load file" << endl;
  }
  TTree *t = (TTree*) f->Get(treeName);
  if(!t){
    return 0;
    cout << "ERROR: could not load tree" << endl;
  }

  TH1F* histo= new TH1F("histo1",";m_{ll} [GeV];Events / 3 GeV",20,20,80);
  histo->Sumw2();

  double mZZ,mLL,mJJ,met,weight,lep;
  double nBTags;

  t->SetBranchAddress("mZZ",&mZZ);
  t->SetBranchAddress("mLL",&mLL);
  t->SetBranchAddress("mJJ",&mJJ);
  t->SetBranchAddress("nBTags",&nBTags);
  t->SetBranchAddress("met",&met);
  t->SetBranchAddress("weight",&weight);  
  t->SetBranchAddress("lep",&lep);  

  for(int iEvt=0; iEvt<t->GetEntries(); iEvt++){
    t->GetEntry(iEvt);

    if(mZZ<170&&mZZ>125&&mJJ<105&&mJJ>75){//&&mLL>20&&mLL<80){
      if(nBTags==btag && btag<2)
	if(lep==0)
	  histo->Fill(mLL,weight*EleLumi);
	else
	  histo->Fill(mLL,weight*MuLumi);
      else if(nBTags==btag && btag==2 && met<50)
	if(lep==0)
	  histo->Fill(mLL,weight*EleLumi);
	else
	  histo->Fill(mLL,weight*MuLumi);
    }
  }// end loop over events

  return histo;

}


void masterPlot(int btag=0, double MultFactor=10,int mH=150,float EleLumi=4.572, float MuLumi=4.590){

  char* fileName="../dataFiles/4fbData/summer11_data_4600pb_lowmass_forMaster.root";
  double lowMassCut=125, highMassCut=170;
  const int three=3;
  double signalNorm[three]={2.13873,
			    0.890778,
			    0.316242};

  //double backgroundNorm[three]={633.319+705.23,190.819+223.531,16.9002+12.2057};
  double backgroundNorm[three]={498.2280+625.0730,    //504.4610+633.5400,
				134.8290+165.5620,     //120.4700+147.9300,
				8.1466+14.2565       //8.0195+12.4747
  };
  cout << "backgroundNorm[0]: " << backgroundNorm[btag] << endl;
  //================== load workspace
  
  char workspaceFileName[50];
  //sprintf(workspaceFileName,"Final2DWorkspace_%i.root",mH);
  sprintf(workspaceFileName,"~/2l2jHelicity/CMSSW_4_2_2/src/HiggsAnalysis/CombinedLimit/test/lowMassAnalysis/makeDataCards/datacards_20111115_newPUapprox_125/150/hzz2l2q_ee%ib.input.root",btag);
  TFile *workspaceFile_ee = new TFile(workspaceFileName);
  RooWorkspace* w = (RooWorkspace*) workspaceFile_ee->Get("w");

  cout << "backgroundNorm[0]: " << backgroundNorm[btag] << endl;
  //================== measurables

  cout << "initializing measurables..." <<endl;

  RooRealVar mJJ("mJJ","m_{jj}",50,150);
  RooRealVar nBTags("nBTags","Number of b-tags",-1,3);
  RooRealVar met("met","ME_T",0,1000);

  cout << "plotting data/fits..." << endl;

  RooRealVar SignalFrac("SignalFrac","SignalFrac",signalNorm[btag]*MultFactor/(backgroundNorm[btag]+signalNorm[btag]*MultFactor),0,1);
  RooAddPdf model("model","model",*w->pdf("signal"),*w->pdf("background"),SignalFrac);

  cout << "check point" << endl;
  cout << "backgroundNorm[0]: " << backgroundNorm[btag] << endl;
  //============= load data =================

  TFile *dataFile = new TFile("../dataFiles/4fbData/summer11_data_4600pb_lowmass_forMaster.root");

  char cutString[100];
  if(btag<2)
    sprintf(cutString,"CMS_hzz2l2q_mZZ>%f&&CMS_hzz2l2q_mZZ<%f&&mJJ>75&&mJJ<105&&nBTags==%i",125.,highMassCut,btag);
  else if(btag==2)
    sprintf(cutString,"CMS_hzz2l2q_mZZ>%f&&CMS_hzz2l2q_mZZ<%f&&mJJ>75&&mJJ<105&&nBTags==2&&met<50",125.,highMassCut);

  RooDataSet data("data","data",(TTree*)dataFile->Get("AngularInfo"),RooArgSet(*w->var("CMS_hzz2l2q_mZZ"),mJJ,nBTags,met),cutString);

  cout << "dataset Norm: " << data.sumEntries() << endl;
  cout << "btag: " << btag << endl;
  cout << "expected Norm: " << backgroundNorm[btag] << endl;
  //=========================================
  int Nbins=20;
  RooPlot *plot=w->var("CMS_hzz2l2q_mZZ")->frame(lowMassCut,highMassCut,Nbins);
  data.plotOn(plot);
  //model.plotOn(plot,LineStyle(2),Normalization((backgroundNorm[btag]+signalNorm[btag]*MultFactor)/w->data("dataset_obs")->sumEntries()));
  //w->pdf("signal")->plotOn(plot,LineColor(2),Normalization(MultFactor*signalNorm[btag]/data->sumEntries()));
  w->pdf("background")->plotOn(plot);//,Normalization(backgroundNorm[btag]/data.sumEntries()));
  
  cout << "drawing..." << endl;

  //================ getting MC histograms

  TH1F *Zjets_mZZ=generateHisto_mZZ("../dataFiles/4fbData/summer11_ZJets_madgraph_lowmass_newPUapprox.root","AngularInfo",EleLumi,MuLumi,btag);
  Zjets_mZZ->SetFillColor(30);
  Zjets_mZZ->SetName("Zjets_mZZ");
  TH1F *DY_mZZ=generateHisto_mZZ("../dataFiles/4fbData/spring11_DYJets_madgraph_lowmass.root","AngularInfo",EleLumi*3,MuLumi*3,btag);
  Zjets_mZZ->Add(DY_mZZ);
  TH1F *TT_mZZ=generateHisto_mZZ("../dataFiles/4fbData/summer11_TTbarIncl_lowmass_newPUapprox.root","AngularInfo",EleLumi,MuLumi,btag);
  TT_mZZ->SetFillColor(39);
  TT_mZZ->SetName("TT_mZZ");
  TH1F *ZZ_mZZ=generateHisto_mZZ("../dataFiles/4fbData/summer11_ZZ_lowmass_newPUapprox.root","AngularInfo",EleLumi,MuLumi,btag);
  ZZ_mZZ->SetFillColor(38);
  ZZ_mZZ->SetName("ZZ_mZZ");
  gStyle->SetOptStat(0);
  TH1F *mH150_mZZ=generateHisto_mZZ("/home/whitbeck/2l2jHelicity/dataFiles/4fbData/summer11_SMHiggs_150GeV_lowmass_newPUapprox.root","AngularInfo",EleLumi,MuLumi,btag);
  mH150_mZZ->SetFillColor(kYellow);
  mH150_mZZ->Scale(MultFactor);
  mH150_mZZ->SetName("mH150_mZZ");
  char yAxisLabel[20];
  int LeftOfDecimal=(highMassCut-lowMassCut)/Nbins;
  int RightOfDecimal=((highMassCut-lowMassCut)/Nbins-LeftOfDecimal)/.01;
  sprintf(yAxisLabel,"Events / (%i.%i GeV)",LeftOfDecimal,RightOfDecimal);
  mH150_mZZ->GetYaxis()->SetTitle(yAxisLabel);

  cout << "Setting y-axis range" << endl;
  
  if(btag==0)
    mH150_mZZ->GetYaxis()->SetRangeUser(0,250);
  if(btag==1)
    mH150_mZZ->GetYaxis()->SetRangeUser(0,120);
  if(btag==2)
    mH150_mZZ->GetYaxis()->SetRangeUser(0,16);


  cout << "Stacked histo... " << endl;

  THStack *allBkg_mZZ = new THStack("allBkg",";m_{ZZ};Num Events");
  cout << "changing text size" << endl;
  //allBkg_mZZ->GetXaxis()->SetTitleSize(.035);
  //allBkg_mZZ->GetYaxis()->SetTitleSize(.035);
  cout << "stacking ZZ" << endl;
  allBkg_mZZ->Add(ZZ_mZZ);
  cout << "stacking TT" << endl;
  allBkg_mZZ->Add(TT_mZZ);
  cout << "stacking Zjets" << endl;
  //allBkg_mZZ->Add(DY_mZZ);
  allBkg_mZZ->Add(Zjets_mZZ);
  allBkg_mZZ->Add(mH150_mZZ);

  cout << "Initializing Canvas" << endl;

  TCanvas*c = new TCanvas("c","c",600,600);
  mH150_mZZ->Draw("HISTO");
  //allBkg_mZZ->Draw("SAMEHISTO");
  plot->Draw("SAME");  


  db->set_lumiNormalization(4600.);
  cout << "add file" << endl;
  db->add_dataFile(dataFile,"data");
  cout << "done" << endl;
  db->set_isCMSArticle(false);
  TPaveText* cmsLabel = db->get_labelCMS();
  cmsLabel->SetBorderSize(0);
  TPaveText* sqrtLabel = db->get_labelSqrt();
  sqrtLabel->SetBorderSize(0);
  
  //============== Legend =============

  cout << "plot objects:\n";

  for (int i=0; i<plot->numItems(); i++) {

    TString obj_name=plot->nameOf(i); if (obj_name=="") continue;

    cout << Form("%d. '%s'\n",i,obj_name.Data());

  }

  TLegend *leg = new TLegend(.2,.92,.6,.55);
  leg->SetTextSize(0.036);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);

  char dataLabel[25];
  sprintf(dataLabel,"%i b-tag category",btag);
  leg->SetHeader(dataLabel);
  leg->AddEntry("h_data","Data","p");
  leg->AddEntry("background_Norm[CMS_hzz2l2q_mZZ]","Expected background","l");
  char Higgs[25];
  sprintf(Higgs,"H (%i GeV) #times %i",mH,(int)MultFactor);
  leg->AddEntry("mH150_mZZ",Higgs,"f");
  //leg->AddEntry("Zjets_mZZ","Z + Jets","f");
  //leg->AddEntry("DY_mZZ","Z + Jets (20<m_{ll}<50)","f");
  //leg->AddEntry("ZZ_mZZ","ZZ","f");
  //leg->AddEntry("TT_mZZ","t#bar{t}/tW","f");
  cmsLabel->Draw();
  sqrtLabel->Draw();
  leg->Draw();
  //===================================
  
  cout << "saving canvas..." << endl;
 
  char imageName[50];
  sprintf(imageName,"masterPlot_%ibtag_1D_normFixed.eps",btag);
  c->SaveAs(imageName);

}

