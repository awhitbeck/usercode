
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

using namespace RooFit;

TH1F* generateHisto_mZZ(char* fileName, char* treeName,double lumi,int btag){

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

  TH1F* histo=new TH1F("histo0",";m_{ZZ} [GeV];Events / 2.5 GeV",20,120,170);
  histo->Sumw2();

  double mZZ,mLL,mJJ,met,weight;
  double nBTags;

  t->SetBranchAddress("mZZ",&mZZ);
  t->SetBranchAddress("mLL",&mLL);
  t->SetBranchAddress("mJJ",&mJJ);
  t->SetBranchAddress("nBTags",&nBTags);
  t->SetBranchAddress("met",&met);
  t->SetBranchAddress("weight",&weight);  

  for(int iEvt=0; iEvt<t->GetEntries(); iEvt++){
    t->GetEntry(iEvt);

    if(mZZ<170&&mZZ>120&&mJJ<105&&mJJ>75){//&&mLL>20&&mLL<90){
      if(nBTags==btag && btag<2)
	histo->Fill(mZZ,weight*lumi);
      else if(nBTags==btag && btag==2 && met<50)
	histo->Fill(mZZ,weight*lumi);
    }
  }// end loop over events

  return histo;

}
TH1F* generateHisto_mLL(char* fileName, char* treeName,double lumi,int btag){

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

  double mZZ,mLL,mJJ,met,weight;
  double nBTags;

  t->SetBranchAddress("mZZ",&mZZ);
  t->SetBranchAddress("mLL",&mLL);
  t->SetBranchAddress("mJJ",&mJJ);
  t->SetBranchAddress("nBTags",&nBTags);
  t->SetBranchAddress("met",&met);
  t->SetBranchAddress("weight",&weight);  

  for(int iEvt=0; iEvt<t->GetEntries(); iEvt++){
    t->GetEntry(iEvt);

    if(mZZ<170&&mZZ>120&&mJJ<105&&mJJ>75){//&&mLL>20&&mLL<80){
      if(nBTags==btag && btag<2)
	histo->Fill(mLL,weight*lumi);
      else if(nBTags==btag && btag==2 && met<50)
	  histo->Fill(mLL,weight*lumi);
      
    }
  }// end loop over events

  return histo;

}


void masterPlot_2D(int btag=0, double MultFactor=10,int mH=150,int intLumi=3.8){

  char* fileName="../dataFiles/4fbData/summer11_data_lowmass.root";
  double lowMassCut=120, highMassCut=170;
  const int three=3;
  double signalNorm[three]={8.61,3.66,1.23};
  double backgroundNorm[three]={425.157+483.19,131.938+153.746,8.45008+13.1446};
  cout << "backgroundNorm[0]: " << backgroundNorm[btag] << endl;
  //================== load workspace
  
  char workspaceFileName[50];
  //sprintf(workspaceFileName,"Final2DWorkspace_%i.root",mH);
  sprintf(workspaceFileName,"~/2l2jHelicity/CMSSW_4_2_2/src/HiggsAnalysis/CombinedLimit/test/lowMassAnalysis/makeDataCards/datacards_20111104/150/hzz2l2q_ee%ib.input.root",btag);
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

  TFile *dataFile = new TFile("../dataFiles/4fbData/summer11_data_lowmass_forMaster.root");

  char cutString[100];
  if(btag<2)
    sprintf(cutString,"CMS_hzz2l2q_mZZ>%f&&CMS_hzz2l2q_mZZ<%f&&mJJ>75&&mJJ<105&&nBTags==%i",lowMassCut,highMassCut,btag);
  else if(btag==2)
    sprintf(cutString,"CMS_hzz2l2q_mZZ>%f&&CMS_hzz2l2q_mZZ<%f&&mJJ>75&&mJJ<105&&nBTags==2&&met<50",lowMassCut,highMassCut);

  RooDataSet data("data","data",(TTree*)dataFile->Get("AngularInfo"),RooArgSet(*w->var("CMS_hzz2l2q_mZZ"),mJJ,nBTags,met),cutString);

  cout << "dataset Norm: " << data.sumEntries() << endl;
  cout << "btag: " << btag << endl;
  cout << "expected Norm: " << backgroundNorm[btag] << endl;
  //=========================================
  
  RooPlot *plot=w->var("CMS_hzz2l2q_mZZ")->frame(lowMassCut,highMassCut,20);
  data.plotOn(plot);
  //model.plotOn(plot,LineStyle(2),Normalization((backgroundNorm[btag]+signalNorm[btag]*MultFactor)/w->data("dataset_obs")->sumEntries()));
  //w->pdf("signal")->plotOn(plot,LineColor(2),Normalization(MultFactor*signalNorm[btag]/data->sumEntries()));
  w->pdf("background")->plotOn(plot,Normalization(backgroundNorm[btag]/data.sumEntries()));
  data.plotOn(plot);
  
  cout << "drawing..." << endl;

  //================ getting MC histograms

  TH1F *Zjets_mZZ=generateHisto_mZZ("../dataFiles/withHLTweights/summer11_ZJets_madgraph_lowmass.root","AngularInfo",intLumi,btag);
  Zjets_mZZ->SetFillColor(30);
  Zjets_mZZ->SetName("Zjets_mZZ");
  TH1F *Zjets_mLL=generateHisto_mLL("../dataFiles/withHLTweights/summer11_ZJets_madgraph_lowmass.root","AngularInfo",intLumi,btag);
  Zjets_mLL->SetFillColor(30);
  Zjets_mLL->SetName("Zjets_mLL");
  TH1F *DY_mZZ=generateHisto_mZZ("../dataFiles/withHLTweights/spring11_DYJets_madgraph_lowmass.root","AngularInfo",intLumi*3,btag);
  DY_mZZ->SetFillColor(40);
  DY_mZZ->SetName("DY_mZZ");
  TH1F *DY_mLL=generateHisto_mLL("../dataFiles/withHLTweights/spring11_DYJets_madgraph_lowmass.root","AngularInfo",intLumi*3,btag);
  DY_mLL->SetFillColor(40);
  DY_mLL->SetName("DY_mLL");
  TH1F *TT_mZZ=generateHisto_mZZ("../dataFiles/withHLTweights/summer11_TTbarIncl_lowmass.root","AngularInfo",intLumi,btag);
  TT_mZZ->SetFillColor(39);
  TT_mZZ->SetName("TT_mZZ");
  TH1F *TT_mLL=generateHisto_mLL("../dataFiles/withHLTweights/summer11_TTbarIncl_lowmass.root","AngularInfo",intLumi,btag);
  TT_mLL->SetFillColor(39);
  TT_mLL->SetName("TT_mLL");
  TH1F *ZZ_mZZ=generateHisto_mZZ("../dataFiles/withHLTweights/summer11_ZZ_lowmass.root","AngularInfo",intLumi,btag);
  ZZ_mZZ->SetFillColor(38);
  ZZ_mZZ->SetName("ZZ_mZZ");
  gStyle->SetOptStat(0);
  TH1F *mH150_mZZ=generateHisto_mZZ("/home/whitbeck/2l2jHelicity/dataFiles/withHLTweights/summer11_SMHiggs_150GeV_lowmass.root","AngularInfo",intLumi,btag);
  mH150_mZZ->SetFillColor(kYellow);
  mH150_mZZ->Scale(MultFactor);
  mH150_mZZ->SetName("mH150_mZZ");
  TH1F *mH150_mLL=generateHisto_mLL("/home/whitbeck/2l2jHelicity/dataFiles/withHLTweights/summer11_SMHiggs_150GeV_lowmass.root","AngularInfo",intLumi,btag);
  mH150_mLL->SetFillColor(kYellow);
  mH150_mLL->Scale(MultFactor);
  mH150_mLL->SetName("mH150_mLL");
  
  if(btag==0)
    ZZ_mZZ->GetYaxis()->SetRangeUser(0,250);
  if(btag==1)
    ZZ_mZZ->GetYaxis()->SetRangeUser(0,120);
  if(btag==2)
    ZZ_mZZ->GetYaxis()->SetRangeUser(0,16);
  TH1F *ZZ_mLL=generateHisto_mLL("../dataFiles/withHLTweights/summer11_ZZ_lowmass.root","AngularInfo",intLumi,btag);
  ZZ_mLL->SetFillColor(38);
  ZZ_mLL->SetName("ZZ_mLL");
  if(btag==0)
    ZZ_mLL->GetYaxis()->SetRangeUser(0,120);
  if(btag==1)
    ZZ_mLL->GetYaxis()->SetRangeUser(0,70);
  if(btag==2)
    ZZ_mLL->GetYaxis()->SetRangeUser(0,8);

  THStack *allBkg_mZZ = new THStack("allBkg",";m_{ZZ};Num Events");
  allBkg_mZZ->Add(ZZ_mZZ);
  allBkg_mZZ->Add(TT_mZZ);
  allBkg_mZZ->Add(DY_mZZ);
  allBkg_mZZ->Add(Zjets_mZZ);
  allBkg_mZZ->Add(mH150_mZZ);

  THStack *allBkg_mLL = new THStack("allBkg",";m_{ll};Num Events");
  allBkg_mLL->Add(ZZ_mLL);
  allBkg_mLL->Add(TT_mLL);
  allBkg_mLL->Add(DY_mLL);
  allBkg_mLL->Add(Zjets_mLL);
  allBkg_mLL->Add(mH150_mLL);

  //======================================
  // ------------ for 2D ----------------
  TCanvas*c = new TCanvas("c","c",800,400);
  c->Divide(2,1);
  c->cd(1);
  ZZ_mLL->Draw();
  allBkg_mLL->Draw("SAMEHISTO");
  plot->Draw("SAME");
  c->cd(2);  
  ZZ_mZZ->Draw();
  allBkg_mZZ->Draw("SAMEHISTO");
  plot2->Draw("SAME");  
  //======================================


  //============== Label ==============

  TPaveText* cmslabel = new TPaveText( 0.145, 0.953, 0.64, 0.975, "brNDC" );
  cmslabel->SetFillColor(kWhite);
  cmslabel->SetTextSize(0.038);
  cmslabel->SetTextAlign(11);
  cmslabel->SetTextFont(62);
  cmslabel->SetBorderSize(0);

  cmslabel->AddText("CMS Preliminary #sqrt{s}=7 TeV 3.8 fb^{-1}");
 
  TPaveText* label_sqrt = new TPaveText(0.7,0.953,.96,0.975, "brNDC");
  label_sqrt->SetFillColor(kWhite);
  label_sqrt->SetTextSize(0.038);
  label_sqrt->SetTextFont(42);
  label_sqrt->SetTextAlign(31); // align right                                                                                                                                                       
  label_sqrt->SetBorderSize(0);
  label_sqrt->AddText("#sqrt{s} = 7 TeV");

  label_sqrt->Draw();
  //============== Legend =============

  cout << "plot objects:\n";

  for (int i=0; i<plot->numItems(); i++) {

    TString obj_name=plot->nameOf(i); if (obj_name=="") continue;

    cout << Form("%d. '%s'\n",i,obj_name.Data());

  }

  TLegend *leg = new TLegend(.2,.35,.4,.9);
  leg->SetTextSize(0.036);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);

  char dataLabel[25];
  sprintf(dataLabel,"%i b-tag 2l2q data",btag);
  leg->AddEntry("h_data",dataLabel,"p");
  //char shapeLabel[25];
  //sprintf(shapeLabel,"%i#timesSignal+Background",(int)MultFactor);
  //leg->AddEntry("background_Int[mLL]_Norm[mLL,mZZ]",shapeLabel,"l");
  leg->AddEntry("background_Norm[CMS_hzz2l2q_mZZ]","Background","l");
  char Higgs[25];
  sprintf(Higgs,"SM Higgs#times%i(%i GeV)",(int)MultFactor,mH);
  leg->AddEntry("mH150_mZZ",Higgs,"f");
  leg->AddEntry("Zjets_mZZ","Z + jets (50<m_{ll})","f");
  leg->AddEntry("DY_mZZ","Z + jet (20<m_{ll}<50)","f");
  leg->AddEntry("TT_mZZ","t#bar{t}","f");
  leg->AddEntry("ZZ_mZZ","ZZ","f");
  cmslabel->Draw();
  leg->Draw();
  //===================================
  
  cout << "saving canvas..." << endl;
 
  char imageName[50];
  sprintf(imageName,"masterPlot_%ibtag_1D.eps",btag);
  c->SaveAs(imageName);

}

