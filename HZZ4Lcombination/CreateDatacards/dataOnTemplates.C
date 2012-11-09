#include "TPaveText.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TString.h"
#include "TAxis.h"
#include "TFile.h"
#include "TLegend.h"
#include "TChain.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TString.h"
#include <iostream>
#include <sstream>
#include <string>
#include "TTree.h"
#include "TText.h"
#include "TROOT.h"
#include "TStyle.h"
#include<iostream>
#include<fstream>


using namespace std;
void setTDRStyle();
TGraphErrors* createGraph(TString pathTree, int channel, int sqrt);

void dataOnTemplates_looping(bool isLowMass, bool signal, bool background){

  setTDRStyle();
  char fileName[150];
 
  TChain* MC = new TChain("SelectedTree");
  // MC->Add("/tmp/whitbeck/7plus8TeV_FSR/HZZ*Tree_ZZTo*.root");
  MC->Add("/tmp/sbologne/261012/PRODFSR_8TeV/4e/HZZ*Tree_ZZTo*.root");
  MC->Add("/tmp/sbologne/261012/PRODFSR_8TeV/4mu/HZZ*Tree_ZZTo*.root");
  MC->Add("/tmp/sbologne/261012/PRODFSR_8TeV/2mu2e/HZZ*Tree_ZZTo*.root");
  MC->Add("/tmp/sbologne/261012/PRODFSR/4e/HZZ*Tree_ZZTo*.root");
  MC->Add("/tmp/sbologne/261012/PRODFSR/4mu/HZZ*Tree_ZZTo*.root");
  MC->Add("/tmp/sbologne/261012/PRODFSR/2mu2e/HZZ*Tree_ZZTo*.root");

  if(isLowMass)
    MC->Draw("ZZLD:ZZMass>>sum(40,100,180,30,0,1)","MC_weight*(ZZMass>0)");
  else
    MC->Draw("ZZLD:ZZMass>>sum(310,145,800,30,0,1)","MC_weight*(ZZMass>0)");

  TChain* MCReweight = new TChain("SelectedTree");
  MCReweight->Add("/tmp/sbologne/261012/PRODFSR_8TeV/4e/HZZ*Tree_H126.root");
  MCReweight->Add("/tmp/sbologne/261012/PRODFSR_8TeV/4mu/HZZ*Tree_H126.root");
  MCReweight->Add("/tmp/sbologne/261012/PRODFSR_8TeV/2mu2e/HZZ*Tree_H126.root");
  MCReweight->Add("/tmp/sbologne/261012/PRODFSR/4e/HZZ*Tree_H126.root");
  MCReweight->Add("/tmp/sbologne/261012/PRODFSR/4mu/HZZ*Tree_H126.root");
  MCReweight->Add("/tmp/sbologne/261012/PRODFSR/2mu2e/HZZ*Tree_H126.root");
  if(isLowMass && !background)
    //MCReweight->Draw("ZZLD:ZZMass>>sum2(40,100,180,30,0,1)","MC_weight*(ZZMass>100 && ZZMass<150)");
    MCReweight->Draw("ZZLD:ZZMass>>sum2(40,100,180,30,0,1)","MC_weight");
  else if(isLowMass)
    MCReweight->Draw("ZZLD:ZZMass>>sum2(40,100,180,30,0,1)","MC_weight*(ZZMass>0)*(ZZMass>100 && ZZMass<180)");
  else
    MCReweight->Draw("ZZLD:ZZMass>>sum2(310,145,800,30,0,1)","MC_weight*(ZZMass>0)");

  //ONLY ICHEP STATISTICS !!!!!!!!!
  TGraphErrors * graph7_or=createGraph("/tmp/sbologne/261012/PRODFSR/data/HZZ4lTree_DoubleOr.root",3,7);
  TGraphErrors * graph7_ele=createGraph("/tmp/sbologne/261012/PRODFSR/data/HZZ4lTree_DoubleEle.root",2,7);
  TGraphErrors * graph7_mu=createGraph("/tmp/sbologne/261012/PRODFSR/data/HZZ4lTree_DoubleMu.root",1,7);
  TGraphErrors * graph8_or=createGraph("/tmp/sbologne/261012/PRODFSR_8TeV/data/HZZ4lTree_DoubleOr_5300.root",3,8);
  TGraphErrors * graph8_ele=createGraph("/tmp/sbologne/261012/PRODFSR_8TeV/data/HZZ4lTree_DoubleEle_5300.root",2,8);
  TGraphErrors * graph8_mu=createGraph("/tmp/sbologne/261012/PRODFSR_8TeV/data/HZZ4lTree_DoubleMu_5300.root",1,8);
 
  if(!isLowMass){
    graph7_or->SetMarkerSize(1);
    graph8_or->SetMarkerSize(1);
    graph7_ele->SetMarkerSize(1);
    graph8_ele->SetMarkerSize(1);
    graph7_mu->SetMarkerSize(1);
    graph8_mu->SetMarkerSize(1);
  }

  TH2F* sum = (TH2F*) gDirectory->Get("sum");
  TH2F* sum2 = (TH2F*) gDirectory->Get("sum2");
  TH2F* sum3 = (TH2F*) gDirectory->Get("sum3");
  if(signal && background){
    sum->Add(sum2);
    sum->Add(sum3);
  }
  //if(signal && !background)
  //sum2->Add(sum3);

  sum->GetYaxis()->SetTitle("#it{K_{D}}");
  sum->GetXaxis()->SetTitle("m_{4l} (GeV)");
  if(isLowMass)
    sum->GetXaxis()->SetRangeUser(100,179.9);
  else
    sum->GetXaxis()->SetRangeUser(145,799.9);

  sum2->GetYaxis()->SetTitle("#it{K_{D}}");
  sum2->GetXaxis()->SetTitle("m_{4l} (GeV)");
  if(isLowMass)
    sum2->GetXaxis()->SetRangeUser(100,179.9);
  else
    sum2->GetXaxis()->SetRangeUser(145,799.9);

  for (int i = 100; i< 145; i=i+2){
    for (int j = 0; j< 30; j++){
      //cout<<i<<" "<<j/30.<<endl;
      sum2->Fill(i,j/30.,0.00001);
    }
  }
  sum->Smooth(1);
  sum->Smooth(1);
  sum2->Smooth(1);
  sum2->Smooth(1);
  if(!isLowMass && background){
    //sum->Scale(285./sum->Integral());
    //sum2->Scale(285./sum2->Integral());
    sum->Scale(295./sum->Integral());
    sum2->Scale(295./sum2->Integral());
  }
  if(isLowMass && signal){
    //sum->Scale(77./sum->Integral());
    //sum2->Scale(77./sum2->Integral());
    sum->Scale(81./sum->Integral());
    sum2->Scale(81./sum2->Integral());
  }
  if(isLowMass && !signal){
    sum->Scale(260./sum->Integral());
    sum2->Scale(260./sum2->Integral());
  }
  char canName[100];
  if(!isLowMass && !signal && background)
    sprintf(canName,"mela2D_highMassPaperNoErr_background_highmass");
  else if (isLowMass && !signal && background)
    sprintf(canName,"mela2D_highMassPaperNoErr_background_lowmass");
  else if (isLowMass && signal && !background)
    sprintf(canName,"mela2D_highMassPaperNoErr_signal126_lowmass");
  cout<<canName<<endl;
  TCanvas* cmass = new TCanvas(canName, canName);
  cmass->SetFillColor(kWhite);
  /*  delete sum->FindObject("palette");
  delete sum2->FindObject("palette");
  delete dataHisto7->FindObject("palette");
  delete dataHisto8->FindObject("palette");*/
  //sum->FindObject("palette")->SetObjectStat(0000000);
  //sum2->FindObject("palette")->SetObjectStat(0000000);
  //dataHisto7->FindObject("palette")->SetObjectStat(0000000);
  //dataHisto8->FindObject("palette")->SetObjectStat(0000000);
  if(background)
    sum->Draw("COLZ");
  else if(signal)
    sum2->Draw("COLZ");
  sum->GetYaxis()->SetTitleOffset(0.9);
  sum2->GetYaxis()->SetTitleOffset(0.9);
  //dataHisto7->Draw("SAME");
  //dataHisto8->Draw("SAME");
  graph7_or->Draw("Psame");
  graph8_or->Draw("Psame");
  graph7_ele->Draw("Psame");
  graph8_ele->Draw("Psame");
  graph7_mu->Draw("Psame");
  graph8_mu->Draw("Psame");
  TLegend* leg2 = new TLegend(.7,.7,.9,.9);
  leg2->SetFillColor(0);
  leg2->SetTextFont(42);
  //leg2->AddEntry(bkg_K_{D},"qqZZ","l");
  leg2->AddEntry(graph7_ele,"4e","LP");
  leg2->AddEntry(graph7_mu,"4#mu","LP");
  leg2->AddEntry(graph7_or,"2e2#mu","LP");
  leg2->Draw("same");
  leg2->SetFillColor(0);
  TPaveText *t=new TPaveText(0.16,0.95,0.45,0.99,"NDC");
  t->SetFillColor(0);
  t->SetTextFont(42);
  t->AddText("CMS");
  t->SetTextAlign(12);
  t->Draw("same");
  TPaveText *t2=new TPaveText(0.16,0.95,0.45,0.99,"NDC");
  //t2->AddText("#sqrt{s}=7 TeV, L=5.05 fb^{-1};  #sqrt{s}=8 TeV, L=12.21 fb^{-1}");
  t2->AddText("#sqrt{s}=7 TeV, L=5.05 fb^{-1};  #sqrt{s}=8 TeV, L=5.26 fb^{-1}");
  t2->SetFillColor(0);
  t2->SetTextFont(42);
  t2->Draw("same");
  TPaveText *t3=new TPaveText(0.16,0.95,0.45,0.99,"NDC");
  t3->AddText("#sqrt{s}=8 TeV, L=12.21 fb^{-1}");
  t3->SetFillColor(0);
  t3->SetTextFont(42);
  //t3->Draw("same");
  }

//vector<pair<float,float> > readErrors(int sqrt){
void readErrors(){
 TFile *f = new TFile("eventlist7TeV.root","RECREATE"); 
 TTree *T = new TTree("ntuple","data from ascii file"); 
 Long64_t nlines = T->ReadFile("eventlist7TeV.txt","run:ls:event:m4l:mZ1:mZ2:massErr:massErrCorr:KD"); 
 T->Write(); 
 f->Close();
 TFile *f2 = new TFile("eventlist8TeV.root","RECREATE"); 
 TTree *T2 = new TTree("ntuple","data from ascii file"); 
 Long64_t nlines2 = T2->ReadFile("eventlist8TeV.txt","run:ls:event:m4l:mZ1:mZ2:massErr:massErrCorr:KD"); 
 T2->Write(); 
 f2->Close();
 /*  char* infile ;
  if(sqrt==7)
    infile = "eventlist7TeV.txt";
  else if(sqrt==8)
    infile = "eventlist8TeV.txt";
  ifstream inFile(infile, ifstream::in);
  
  for(int l=0;l<90;l++){
    char a;
    float dummy, evNumb, massErr;
    for(int i=0; i<8; i++){
    inFile>>dummy;
    cout<<dummy<<endl;
    inFile>>a;
    cout<<a<<endl;
    }
    inFile>>dummy;
    cout<<dummy<<endl;
    cout<<endl;
  }
  cout<<"closing"<<endl;
  inFile.close();*/
  
}

TGraphErrors* createGraph(TString pathTree, int channel, int sqrt){
  /*  double scale=0;
  if(channel==1)
    scale=1.15;
  else if(channel==2)
    scale=1.30;
  else if(channel==3)
  scale=1.45;*/
  
  TChain* treeErr ;
  treeErr= new TChain("ntuple");
  if(sqrt==7)
    treeErr->Add("eventlist7TeV.root");
  if(sqrt==8)
    treeErr->Add("eventlist8TeV.root");
  float ZZMassError, EventErr, ZZMassForErr;
  vector<pair<float,float> > eventAndErr;
  treeErr->SetBranchAddress("massErrCorr",&ZZMassError);
  treeErr->SetBranchAddress("event",&EventErr); 
  treeErr->SetBranchAddress("m4l",&ZZMassForErr); 
  for (int j=0; j<treeErr->GetEntries(); j++){
    treeErr->GetEntry(j);
    //    if(EventErr==72158025)
    eventAndErr.push_back(make_pair(EventErr,ZZMassError));
    if(ZZMassForErr>100 && ZZMassForErr<180){
      cout<<EventErr<<" "<<ZZMassForErr<<" "<<ZZMassError<<endl;
      cout<<eventAndErr[j].first<<" "<<eventAndErr[j].second<<endl;
    }
  }
  cout<<endl;
  TChain* tree ;
  tree= new TChain("SelectedTree");
  tree->Add(pathTree);
  float ZZMass, ZZLD;
  long EventNumber;
  tree->SetBranchAddress("ZZMass",&ZZMass);
  tree->SetBranchAddress("ZZLD",&ZZLD); 
  tree->SetBranchAddress("EventNumber",&EventNumber); 
  vector<float> ZZMassV7;
  vector<float> ZZMassErrorV7;
  vector<float> ZZLDV7;
  vector<int> eventList;
  for (int i=0; i<tree->GetEntries(); i++){
    //cout<<"looping 7"<<i<<endl;
    tree->GetEntry(i);
    ZZMassV7.push_back(ZZMass);
    ZZLDV7.push_back(ZZLD);
    eventList.push_back(EventNumber);
    //if(EventNumber == 72158025)
    cout<<EventNumber<<" "<<ZZMass<<" "<<ZZLD<<"--------"<<endl;
    bool found=false;
    for(int j=0; j<treeErr->GetEntries(); j++){
      //if(EventNumber == 72158025)
      //cout<<eventAndErr[j].first<<" "<<eventAndErr[j].second<<endl;
      if(fabs((eventAndErr[j]).first - EventNumber)<0.000001){
	found=true;
	//if(EventNumber == 72158025)	cout<<"FOUND !!!!!!!!!!"<<(eventAndErr[j]).second<<endl;
	ZZMassErrorV7.push_back((eventAndErr[j]).second);
      }
    }
    if(!found){
      cout<<EventNumber<<"NOT FOUND !!!!!!!!!"<<endl;
      abort();
    }
  }
  const int n7= ZZMassV7.size();
  float x7[n7];
  float y7[n7];
  float ex7[n7];
  float ey7[n7];
  for (int i=0;i<n7;i++){
    if(ZZMassV7[i]<180 && ZZMassV7[i]>100){
      //cout<<"looping 7"<<i<<endl;
      cout<<eventList[i]<<" ";
      cout<<ZZMassV7[i]<<" ";
      cout<<ZZMassErrorV7[i]<<" ";
      cout<<ZZLDV7[i]<<endl;
    }
    x7[i] = ZZMassV7[i];
    y7[i] = ZZLDV7[i];
    ex7[i] = 0;//ZZMassErrorV7[i];
    ey7[i] = 0;
  }
  cout<<endl<<endl;
  TGraphErrors * graph7=new TGraphErrors(n7,x7,y7,ex7,ey7);
  if(channel==1 && sqrt==7)
    graph7->SetMarkerStyle(8);
  else if(channel==2 && sqrt==7)
    graph7->SetMarkerStyle(22);
  else if(channel==3 && sqrt==7)
    graph7->SetMarkerStyle(21);
  else if(channel==1 && sqrt==8)
    //graph7->SetMarkerStyle(4);
    graph7->SetMarkerStyle(8);
  else if(channel==2 && sqrt==8)
    //graph7->SetMarkerStyle(26);
    graph7->SetMarkerStyle(22);
  else if(channel==3 && sqrt==8)
    //graph7->SetMarkerStyle(25);
    graph7->SetMarkerStyle(21);
  graph7->SetMarkerSize(1.7);
  //graph7->SetMarkerSize(1.2);
  graph7->SetLineWidth(2);
  return graph7;

}

void setTDRStyle() {
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

// For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(750); //Height of canvas
  tdrStyle->SetCanvasDefW(1050); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

// For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

// For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

// For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

  tdrStyle->SetEndErrorSize(2);
  //  tdrStyle->SetErrorMarker(20);
  tdrStyle->SetErrorX(0.);
  
  tdrStyle->SetMarkerStyle(20);

//For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

//For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

// For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.010);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.25);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

// Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.14);
  tdrStyle->SetPadRightMargin(0.04);

// For the Global title:

  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.005);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

// For the axis titles:

  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.25);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

// For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

// For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  //tdrStyle->SetNdivisions(505, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

// Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

// Postscript options:
  tdrStyle->SetPaperSize(20.,20.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  Int_t n=35;
  Int_t *colors = new Int_t[n];
  for (Int_t i =0; i < n; i++) {
    //colors[i] = i+61;
    //colors[i] = 70-i;
    colors[i] = i+63;
    }
  /*colors[0]=61
  colors[1]=62;
  colors[2]=63;
  colors[3]=;
  colors[4]=;
  colors[5]=;
  colors[6]=;
  colors[7]=;
  colors[8]=;
  colors[9]=;
  colors[10]=;*/
  gStyle->SetPalette(n, colors);
  
  //gStyle->SetPalette(1, 0);
  tdrStyle->cd();
}
/*
void damnedPlot(){
TCanvas *c1 =  new TCanvas("paddy","paddy",700,700)
  c1->Divide(1,2,0,0);
 c1->cd(1).SetBottomMargin(0.001);
 c1->cd(1).SetTopMargin(0.01);
 c1->cd(1).SetRightMargin(0.01);
 c1.cd(2);
 c1.cd(2).SetTopMargin(0.001);
 c1.cd(2).SetRightMargin(0.01);

}
*/
