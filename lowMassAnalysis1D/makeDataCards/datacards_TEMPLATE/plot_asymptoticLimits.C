#include <Riostream.h>
#include <string>
#include <vector>
#include <cmath>
//#include <cstdlib>
#include "TTree.h"
#include "TH1D.h"
#include "TFile.h"
#include "TROOT.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TLatex.h"
#include "TLegend.h"

const float intLumi=4.6;
const bool isXsBr=false;
const bool isSM4=false;
const bool showOBS=true;
void plot_asymptoticLimits(char* tag="6channels"){

  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();

  gROOT->SetStyle("tdrStyle");


  char fileName[50];
  sprintf(fileName,"higgsCombine%s.Asymptotic.TOTAL.root",tag);

  TFile *fFREQ=new TFile(fileName,"READ");

  TTree *t=(TTree*)fFREQ->Get("limit");
  double mh,limit;
  float quant;
  t->SetBranchAddress("mh",&mh);
  t->SetBranchAddress("limit",&limit);
  t->SetBranchAddress("quantileExpected",&quant);

  vector<double> v_mh, v_median,v_68l,v_68h,v_95l,v_95h, v_obs;

  for(int i=0;i<t->GetEntries();i++){
    t->GetEntry(i);
    //  cout<<"i "<<i<<flush<<"  m = "<<mh<<endl;
   
   
    // if(mh==600)cout<<"$$$$$$$$$ TREE 600 $$$$$$$$$$$$$$"<<endl;
    if(quant>-1.01&&quant<-0.99){
      v_obs.push_back(limit);
      v_mh.push_back(mh);
    }
    else if(quant>0.02&&quant<0.03)v_95l.push_back(limit);
    else if(quant>0.15&&quant<0.17)v_68l.push_back(limit);
    else if(quant>0.49&&quant<0.51)v_median.push_back(limit);
    else if(quant>0.83&&quant<0.85)v_68h.push_back(limit);
    else if(quant>0.965&&quant<0.98)v_95h.push_back(limit);
    else {cout<<"Error! Quantile =  "<<quant<<endl;}
  }
  cout<<"Out of the loop !"<<endl;

  const int nMass= v_mh.size(); // -13
  //cout << "nMass: " << nMass << endl;

  double mass[nMass],obs_lim_cls[nMass]; 
  double medianD[nMass];
  double up68err[nMass],down68err[nMass],up95err[nMass],down95err[nMass];
  int nMassEff=0;
  int nexcluded=0;

  for(int im=0;im<v_mh.size();im++){

    if(v_68h.at(im)>=v_95h.at(im) || v_68l.at(im)<=v_95l.at(im) ){
      cout<<"Point at M = "<<v_mh.at(im) <<" excluded"<<endl;
      nexcluded++;
      continue;
    }

    double xs=1.0;

    if(v_mh.at(im)>164) continue;

    mass[nMassEff]=v_mh.at(im);
    obs_lim_cls[nMassEff]=v_obs.at(im)*xs;
    medianD[nMassEff]=v_median.at(im)*xs;
    up68err[nMassEff]=(v_68h.at(im)-v_median.at(im))*xs;
    down68err[nMassEff]=(v_median.at(im)-v_68l.at(im))*xs;
    up95err[nMassEff]=(v_95h.at(im)-v_median.at(im))*xs;
    down95err[nMassEff]=(v_median.at(im)-v_95l.at(im))*xs;

    nMassEff++;
  }  cout<<"Excluded "<<nexcluded<<" sick mass points."<<endl;

  for(int m=0; m<v_mh.size(); m++){

    cout << "------------ mH:" << v_mh.at(m) << "GeV -----------" << endl;
    cout << "Observed Limit: " << v_obs.at(m) << endl;
    cout << "Expected Limit: " << v_median.at(m) << endl;
    cout << "1 Sigma Bands: " << v_68l.at(m) << " - " << v_68h.at(m) << endl;
    cout << "2 Sigma Bands: " << v_95l.at(m) << " - " << v_95h.at(m) << endl;

  }


  TGraph *grobslim_cls=new TGraphAsymmErrors(nMassEff,mass,obs_lim_cls);
  grobslim_cls->SetName("LimitObservedCLs");
  TGraph *grmedian_cls=new TGraphAsymmErrors(nMassEff,mass,medianD);
  grmedian_cls->SetName("LimitExpectedCLs");
  TGraphAsymmErrors *gr68_cls=new TGraphAsymmErrors(nMassEff,mass,medianD,0,0,down68err,up68err);
  gr68_cls->SetName("Limit68CLs");
  TGraphAsymmErrors *gr95_cls=new TGraphAsymmErrors(nMassEff,mass,medianD,0,0,down95err,up95err);
  gr95_cls->SetName("Limit95CLs");

  TCanvas *cMCMC=new TCanvas("c_lim_MCMC","canvas with limits for MCMC",630,600);
  cMCMC->cd();
  cMCMC->SetGridx(1);
  cMCMC->SetGridy(1);

  double yRangeUp, yRangeDown;
  if(isXsBr){
    yRangeUp=12;
    yRangeDown=.0001;
  }else{
    if(isSM4){
      yRangeUp=5;
      yRangeDown=.1;
    }else{
      yRangeUp=25;
      yRangeDown=.1;
    }
  }
  TH1F *hr = cMCMC->DrawFrame(125.0,yRangeDown,170.0,yRangeUp,"frame1");
  //hr->SetYTitle("#sigma_{95%}/#sigma_{SM}");
  //hr->SetYTitle("#sigma_{95%}/#sigma_{SM}");
  // cMCMC->GetFrame()->SetFillColor(21);
  //cMCMC->GetFrame()->SetBorderSize(12);
  
  gr95_cls->SetFillColor(kYellow);
  gr95_cls->SetFillStyle(1001);//solid
  gr95_cls->GetXaxis()->SetTitle("M_{H} [GeV]");

  if(isXsBr){
    hr->SetYTitle("#sigma_{95%}#timesBR(H#rightarrow ZZ) [pb]");
    gr95_cls->GetYaxis()->SetTitle("#sigma_{95%}#timesBR(H#rightarrow ZZ) [pb]");
    grmedian_cls->GetYaxis()->SetTitle("#sigma_{95%}#timesBR(H#rightarrow ZZ) [pb]");
  }
  else{
    if(isSM4){
      hr->SetYTitle("#sigma_{95%}/#sigma_{SM4}");
      grmedian_cls->GetYaxis()->SetTitle("#sigma_{95%}/#sigmaD_{SM4}");
      gr95_cls->GetYaxis()->SetTitle("#sigma_{95%}/#sigma_{SM4}");
    }else{
      hr->SetYTitle("#sigma_{95%}/#sigma_{SM}");
      grmedian_cls->GetYaxis()->SetTitle("#sigma_{95%}/#sigmaD_{SM}");
      gr95_cls->GetYaxis()->SetTitle("#sigma_{95%}/#sigma_{SM}");
    }
  }

  cout << "Initializing dummy... " << endl;

  gStyle->SetOptStat(0);

  TH1D* dummy = new TH1D("dummy","",1,125.,170.);
  //dummy->Draw("AXISG");
 
  cout << "Drawing limit graphs..." << endl;

  hr->SetXTitle("M_{H} [GeV]");
  gr95_cls->GetXaxis()->SetRangeUser(125.0,170.0);
  //gr95_cls->Draw("3");
  gr95_cls->Draw("L3");
  gr68_cls->SetFillColor(kGreen);
  gr68_cls->SetFillStyle(1001);//solid
  //gr68_cls->Draw("3same");
  gr68_cls->Draw("L3");
  grmedian_cls->GetXaxis()->SetTitle("M_{H} [GeV]");
  //grmedian_cls->SetMarkerStyle(21);
  grmedian_cls->SetLineStyle(kDashed);
  grmedian_cls->SetLineWidth(3);
  //grmedian_cls->SetMarkerColor(kBlack);
  grmedian_cls->SetMinimum(0.0);
  grmedian_cls->SetMaximum(8.0);
  //grmedian_cls->Draw("L");
  grmedian_cls->Draw("L3");
  grobslim_cls->SetMarkerColor(kBlack);
  grobslim_cls->SetMarkerStyle(21);
  grobslim_cls->SetMarkerSize(.85);
  grobslim_cls->SetLineWidth(3.);
  //grobslim_cls->SetLineStyle(2);
  if(showOBS)
    grobslim_cls->Draw("LP");
  
  cout << "Redrawing dummy..." << endl;

  dummy->Draw("AXIGSAME");


  TLine *l1=new TLine();
  l1->SetLineStyle(1);
  l1->SetLineWidth(2.0);
  l1->SetLineColor(kRed);
  //  l1->DrawLine(200.0,1.0,600.0,1.0);
  cMCMC->Update();
  // cMCMC->RedrawAxis("");
  gPad->RedrawAxis("");
  // hr->GetYaxis()->DrawClone();
  cMCMC->Update();
  


  //more graphics
   TLegend *leg = new TLegend(.35,.55,.65,.90);

   leg->SetFillColor(0);
   leg->SetShadowColor(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.05);
   leg->SetTextSize(0.030);
   if(showOBS)
     leg->AddEntry(grobslim_cls, "Asym. CL_{S} Observed", "LP");
   leg->AddEntry(grmedian_cls, "Asym. CL_{S} Expected", "L");
   leg->AddEntry(gr68_cls, "Asym. CL_{S} Expected #pm 1#sigma", "LF");
   leg->AddEntry(gr95_cls, "Asym. CL_{S} Expected #pm 2#sigma", "LF");

   leg->Draw();
   

   TLatex * latex = new TLatex();
   latex->SetNDC();
   latex->SetTextSize(0.04);
   latex->SetTextAlign(31);
   latex->SetTextAlign(11); // align left                                                                                                                   
   latex->DrawLatex(0.16, 0.96, "CMS preliminary 2011");
   latex->DrawLatex(0.63,0.96,Form("%.1f fb^{-1} at #sqrt{s} = 7 TeV",intLumi));
     
   TLine *l1b=new TLine();
   if(!isXsBr){
     l1b->SetLineStyle(1);
     l1b->SetLineWidth(2.0);
     l1b->SetLineColor(kRed);
     l1b->DrawLine(125.0,1.0,170.0,1.0);
   }

   cMCMC->Update();
   
   
   // cMCMC->RedrawAxis("");
   gPad->RedrawAxis("");
   // hr->GetYaxis()->DrawClone();
   cMCMC->Update();

   char saveFile[50];
   if(showOBS)
     sprintf(saveFile,"AsymptoticLimit_4600pb_wOBS_%s.eps",tag);
   else
     sprintf(saveFile,"AsymptoticLimit_4600pb_%s.eps",tag);

     cMCMC->SaveAs(saveFile);

}//end main
