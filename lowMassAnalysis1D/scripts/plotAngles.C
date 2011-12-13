#include "TFile.h"
#include "TTree.h"
#include "RooRealVar.h"
#include <vector>
#include <string>
#include "RooDataSet.h"
#include "TCanvas.h"
#include "RooPlot.h"

using namespace std;
using namespace RooFit;

void plotAngles(
			 char* sigFileName="../dataFiles/4fbData/summer11_data_lowmass.root",
			 char* bkgFileName="../dataFiles/4fbData/summer11_SMHiggs_150GeV_lowmass.root",
			 int mZZlowCut=120,
			 int mZZhighCut=180
			 ){


  double mZZ;
  double mJJ;
  double nBTags;
  double met;
  double weight;
  double cosThetaStar;
  double cosTheta1;
  double cosTheta2;
  double phi;
  double phiStar1;

  vector<string> saveFile;
  saveFile.push_back("costhetastar.eps");
  saveFile.push_back("costheta1.eps");
  saveFile.push_back("costheta2.eps");
  saveFile.push_back("phi.eps");
  saveFile.push_back("phistar1.eps");
  

  TFile* sigFile = new TFile(sigFileName);
  if(!sigFile){
    std::cout << "ERROR: no file - " << sigFileName << std::endl;
    return;
  }
  TFile* bkgFile = new TFile(bkgFileName);
  if(!bkgFile){
    std::cout << "ERROR: no file - " << sigFileName << std::endl;
    return;
  }

  //------------- Load data ---------------

  TTree* sigTree = (TTree*) sigFile->Get("AngularInfo");
  TTree* bkgTree = (TTree*) bkgFile->Get("AngularInfo");
  if(!bkgTree || !sigTree){
    cout << "ERROR: one or more trees could not be loaded" << endl;
    return;
  }

  sigTree->SetBranchAddress("mZZ",&mZZ);
  sigTree->SetBranchAddress("mJJ",&mJJ);
  sigTree->SetBranchAddress("nBTags",&nBTags);
  sigTree->SetBranchAddress("met",&met);
  sigTree->SetBranchAddress("weight",&weight);
  sigTree->SetBranchAddress("cosThetaStar",&cosThetaStar);
  sigTree->SetBranchAddress("cosTheta1",&cosTheta1);
  sigTree->SetBranchAddress("cosTheta2",&cosTheta2);
  sigTree->SetBranchAddress("phi",&phi);
  sigTree->SetBranchAddress("phiStar1",&phiStar1);

  bkgTree->SetBranchAddress("mZZ",&mZZ);
  bkgTree->SetBranchAddress("mJJ",&mJJ);
  bkgTree->SetBranchAddress("nBTags",&nBTags);
  bkgTree->SetBranchAddress("met",&met);
  bkgTree->SetBranchAddress("weight",&weight);
  bkgTree->SetBranchAddress("cosThetaStar",&cosThetaStar);
  bkgTree->SetBranchAddress("cosTheta1",&cosTheta1);
  bkgTree->SetBranchAddress("cosTheta2",&cosTheta2);
  bkgTree->SetBranchAddress("phi",&phi);
  bkgTree->SetBranchAddress("phiStar1",&phiStar1);

  TH1F* histoSig[5];
  TH1F* histoBkg[5];
  
  histoSig[0]= new TH1F("hsh_sig",";cos(#theta*);Num Events",20,-1,1);
  histoSig[0]->Sumw2();
  histoSig[1]= new TH1F("h1h_sig",";cos(#theta_{1});Num Events",20,-1,1);
  histoSig[1]->Sumw2();
  histoSig[2]= new TH1F("h2h_sig",";cos(#theta_{2});Num Events",10,0,1);
  histoSig[2]->Sumw2();
  histoSig[3]= new TH1F("ph_sig"," ;#Phi;Num Events",20,-3.1415,3.1415);
  histoSig[3]->Sumw2();
  histoSig[4]= new TH1F("p1h_sig",";#Phi*_{1};Num Events",20,-3.1415,3.1415);
  histoSig[4]->Sumw2();
  histoBkg[0]= new TH1F("hsh_bkg",";cos(#theta*);Num Events",20,-1,1);
  histoBkg[1]= new TH1F("h1h_bkg",";cos(#theta_{1});Num Events",20,-1,1);
  histoBkg[2]= new TH1F("h2h_bkg",";cos(#theta_{2});Num Events",10,0,1);
  histoBkg[3]= new TH1F("ph_bkg",";#Phi;Num Events",20,-3.1415,3.1415);
  histoBkg[4]= new TH1F("p1h_bkg",";#Phi*_{1};Num Events",20,-3.1415,3.1415);
  
  for(int iEvt=0; iEvt<sigTree->GetEntries(); iEvt++){
    sigTree->GetEntry(iEvt);

    if(mZZ>mZZlowCut && mZZ<mZZhighCut &&
       mJJ>75 && mJJ<105){
      histoSig[0]->Fill(cosThetaStar,weight);
      histoSig[1]->Fill(cosTheta1,weight);
      histoSig[2]->Fill(cosTheta2,weight);
      histoSig[3]->Fill(phi,weight);
      histoSig[4]->Fill(phiStar1,weight);
    }

  }

  for(int iEvt=0; iEvt<bkgTree->GetEntries(); iEvt++){
    bkgTree->GetEntry(iEvt);

    if(mZZ>mZZlowCut && mZZ<mZZhighCut &&
       ((mJJ>60 && mJJ<75) || (mJJ>105 && mJJ<150))){
      histoBkg[0]->Fill(cosThetaStar);
      histoBkg[1]->Fill(cosTheta1);
      histoBkg[2]->Fill(cosTheta2);
      histoBkg[3]->Fill(phi);
      histoBkg[4]->Fill(phiStar1);
    }

  }
    
  gStyle->SetOptStat(0);
  char cName[2];
  TCanvas* c[5];
  for(int i=0; i<5; i++){
    sprintf(cName,"c%i",i);
    c[i] = new TCanvas(cName,cName,400,400);
    histoBkg[i]->Scale(1/histoBkg[i]->Integral());
    histoBkg[i]->SetLineColor(2);
    histoBkg[i]->SetFillColor(2);
    histoBkg[i]->SetFillStyle(2);
    histoSig[i]->Scale(1/histoSig[i]->Integral());
    histoSig[i]->SetLineColor(4);
    histoSig[i]->SetFillColor(4);
    histoSig[i]->SetFillStyle(5);
    histoBkg[i]->Draw("H");
    histoSig[i]->Draw("SAMEH");
    c[i]->SaveAs(saveFile.at(i).c_str());
  }


  
}
