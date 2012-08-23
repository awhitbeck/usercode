#include <Riostream.h>
#include <string>
#include <vector>
#include <algorithm>
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
#include "DrawBase.C"
#include "fitTools.C"

using namespace std;

DrawBase* db = new DrawBase("dummy");
const float intLumi=4.6;
const bool isXsBr=true;
const bool isSM4=false;
const bool isLowMass=true;

void plot_golfcourse(char* tag ){

  char inputFile[30];
  sprintf(inputFile,"combineResults%s.MarkovChainMC.root",tag);
  TFile *fFREQ=new TFile(inputFile,"READ");

  TTree *t=(TTree*)fFREQ->Get("limit");

  double mh,limit;
  float quant;
  t->SetBranchAddress("mh",&mh);
  t->SetBranchAddress("limit",&limit);
  t->SetBranchAddress("quantileExpected",&quant);

  vector<double> v_mh,v_median,v_68l,v_68h,v_95l,v_95h, v_obs;

  for(int i=0;i<t->GetEntries();i++){
    t->GetEntry(i);
    cout<<"i "<<i<<flush<<"  m = "<<mh<<endl;
    cout << "quantile: " << quant << endl;
    cout << "limit: " << limit << endl;
    cout << "--------------------------" << endl;
   
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


  ifstream xsect_file("./xsect_higgs_173points_new.txt",ios::in);
  ifstream SM4xsect_file("./xsect_higgs_173points_4generation.txt",ios::in);
  if (! xsect_file.is_open()){ cout<<"Failed to open file with xsections"<<endl;}
  float mHxs, CSgg, CSgg_p, CSgg_m, CSpdfgg_p,CSpdfgg_m,CSvbf, CSvbf_p, CSvbf_m,CSpdfvbf_p,CSpdfvbf_m, 
    Gamma, BRHZZ, BRZZ2l2q;
  float mHxsSM4, CSggSM4, CSgg_pSM4, CSgg_mSM4, CSpdfgg_pSM4,CSpdfgg_mSM4,CSvbfSM4, CSvbf_pSM4, CSvbf_mSM4,CSpdfvbf_pSM4,CSpdfvbf_mSM4, 
    GammaSM4, BRHZZSM4, BRZZ2l2qSM4;

  vector<float> v_mhxs, v_ggxs,v_vbfxs, v_ggtoterrh,v_ggtoterrl,
                v_vbftoterrh,v_vbftoterrl, v_gamma, v_brhzz, v_brzz2l2q;
  vector<float> v_mhxsSM4, v_ggxsSM4,v_vbfxsSM4, v_ggtoterrhSM4,v_ggtoterrlSM4,
                v_vbftoterrhSM4,v_vbftoterrlSM4, v_gammaSM4, v_brhzzSM4, v_brzz2l2qSM4;
  while(xsect_file.good()&&SM4xsect_file.good()){
    xsect_file >> mHxs>> CSgg>> CSgg_p >> CSgg_m >>  CSpdfgg_p>> CSpdfgg_m>> CSvbf >> CSvbf_p >> CSvbf_m >>  CSpdfvbf_p>>CSpdfvbf_m>>Gamma >> BRHZZ >> BRZZ2l2q;
    SM4xsect_file >> mHxsSM4>> CSggSM4>> CSgg_pSM4 >> CSgg_mSM4 >>  CSpdfgg_pSM4>> CSpdfgg_mSM4>> CSvbfSM4 >> CSvbf_pSM4 >> CSvbf_mSM4 >>  CSpdfvbf_pSM4>>CSpdfvbf_mSM4>>GammaSM4 >> BRHZZSM4 >> BRZZ2l2qSM4;
    // if(mHxs==600)cout<<"~~~~~ 600 theor ~~~~~~~~~~~~~"<<endl;
    v_mhxs.push_back(mHxs);
    v_mhxsSM4.push_back(mHxsSM4);
    //v_ggxs.push_back(CSgg*BRHZZ*BRZZ2l2q);
    //v_vbfxs.push_back(CSvbf*BRHZZ*BRZZ2l2q);
    v_ggxs.push_back(CSgg*BRHZZ);
    v_vbfxs.push_back(CSvbf*BRHZZ);
    v_brhzz.push_back( BRHZZ);
    v_brzz2l2q.push_back(BRZZ2l2q);

    v_ggxsSM4.push_back(CSggSM4*BRHZZSM4);
    v_vbfxsSM4.push_back(CSvbfSM4*BRHZZSM4);
    v_brhzzSM4.push_back( BRHZZSM4);
    v_brzz2l2qSM4.push_back(BRZZ2l2qSM4);
    //sum up linearly theor errors    
    v_ggtoterrh.push_back(sqrt(
			        (CSgg_p+CSpdfgg_p)*(CSgg_p+CSpdfgg_p)*CSgg*CSgg
			       +(CSvbf_p+CSpdfvbf_p)*(CSvbf_p+CSpdfvbf_p)*CSvbf*CSvbf
				)*BRHZZ);//+CSgg*BRHZZ+CSvbf*BRHZZ);
    v_ggtoterrl.push_back(sqrt(
			        (CSgg_m+CSpdfgg_m)*(CSgg_m+CSpdfgg_m)*CSgg*CSgg
			       +(CSvbf_m+CSpdfvbf_m)*(CSvbf_m+CSpdfvbf_m)*CSvbf*CSvbf
				)*BRHZZ);//+CSgg*BRHZZ+CSvbf*BRHZZ);

    v_ggtoterrhSM4.push_back(sqrt(
			       (CSgg_pSM4+CSpdfgg_pSM4+.1)*(CSgg_pSM4+CSpdfgg_pSM4+.1)*CSggSM4*CSggSM4
			       +(CSvbf_p+CSpdfvbf_p)*(CSvbf_p+CSpdfvbf_p)*CSvbfSM4*CSvbfSM4
			       )*BRHZZSM4);//+CSggSM4*BRHZZSM4+CSvbfSM4*BRHZZSM4);
    //cout << "gg xs: " << CSggSM4 << " vbf xs: " << CSvbfSM4 << endl;
    //cout << "glue-glue errors: " << CSgg_pSM4 << " " << CSpdfgg_pSM4 << " .1" << endl;
    //cout << "                  " << CSgg_mSM4 << " " << CSpdfgg_mSM4 << " .1" << endl;
    v_ggtoterrlSM4.push_back(sqrt(
			       (CSgg_mSM4+CSpdfgg_mSM4-.1)*(CSgg_mSM4+CSpdfgg_mSM4-.1)*CSggSM4*CSggSM4
			       +(CSvbf_m+CSpdfvbf_m)*(CSvbf_m+CSpdfvbf_m)*CSvbfSM4*CSvbfSM4
			       )*BRHZZSM4);//+CSggSM4*BRHZZSM4+CSvbfSM4*BRHZZSM4);
  }
  //cout<<"Size of theor "<<v_mhxs.size()<<endl;


  const int nMass= v_mh.size(); // -13
  //cout << "nMass: " << nMass << endl;
  const int nMassSM4 = v_mh.size();
  if(nMassSM4!=nMass){
    cout << "nMassSM4!=nMass: there are bound to be errors... I am shutting this down." << endl;
    return;
  }
  double mass[nMass],obs_lim_cls[nMass]; 
  double medianD[nMass];
  double up68err[nMass],down68err[nMass],up95err[nMass],down95err[nMass];
  double ggxs[nMass], ggxs_uperr[nMass], ggxs_downerr[nMass];
  int nMassEff=0;
  int nexcluded=0;
  double ggxsSM4[nMass], ggxsSM4_uperr[nMass], ggxsSM4_downerr[nMass];

  //  for(int im=0;im<nMass;im++){
  for(int im=0;im<v_mh.size();im++){
    cout<<"Array "<<im<<flush<<"  m = "<<v_mh.at(im)<< endl;
    cout << "observed limit: " << v_obs.at(im) << endl;
    cout << "expected limit: " << v_median.at(im) << endl;

  //protection against messed up jobs
    if(v_68h.at(im)>=v_95h.at(im) || v_68l.at(im)<=v_95l.at(im) ){
      cout<<"Point at M = "<<v_mh.at(im) <<" excluded"<<endl;
      nexcluded++;
      continue;
    }

    //cout << "checkpoint" << endl;

    //search for right index in theor vectors
    bool found=false;
    int indtmp=0,ind=-1;
    while(!found){
      if(v_mhxs.at(indtmp)==v_mh.at(im)){found=true;ind=indtmp;}
      indtmp++;
      if(indtmp==v_mhxs.size()){
	cout<<"m="<<v_mh.at(im)<<" NOT found in theor matrix."<<endl;
	break;
      }
    }    
    if(!found){
      //  cout<<"(2) m="<<v_mh.at(im)<<" NOT found in theor matrix."<<endl;
      continue;
    }

    //cout<<"  Found mass, xs=  "<<v_ggxs.at(ind)<<endl;
    
    double xs;
    double xsSM4;
    if(isXsBr){
      xs=(double)(v_ggxs.at(ind)+v_vbfxs.at(ind));
      xsSM4=(double)(v_ggxsSM4.at(ind)+v_vbfxsSM4.at(ind));
    }
    else{
      xs=1;
      xsSM4=1;
    }

    if(v_mh.at(im)<226 && !isLowMass ) continue;
    if((v_mh.at(im)<130 || v_mh.at(im)>164) && isLowMass) continue;
    mass[nMassEff]=v_mh.at(im);
    obs_lim_cls[nMassEff]=v_obs.at(im)*xs;
    medianD[nMassEff]=v_median.at(im)*xs;
    up68err[nMassEff]=(v_68h.at(im)-v_median.at(im))*xs;
    down68err[nMassEff]=(v_median.at(im)-v_68l.at(im))*xs;
    up95err[nMassEff]=(v_95h.at(im)-v_median.at(im))*xs;
    down95err[nMassEff]=(v_median.at(im)-v_95l.at(im))*xs;
 
    ggxs[nMassEff]=xs;
    ggxs_uperr[nMassEff]=double( v_ggtoterrh.at(ind));
    ggxs_downerr[nMassEff]=double( v_ggtoterrl.at(ind));

    ggxsSM4[nMassEff]=xsSM4;
    ggxsSM4_uperr[nMassEff]=double( v_ggtoterrhSM4.at(ind));
    ggxsSM4_downerr[nMassEff]=double( v_ggtoterrlSM4.at(ind));
    //cout << "SM4 error bands: " << v_ggtoterrlSM4.at(ind) <<  " " << v_ggtoterrhSM4.at(ind) << endl;
    nMassEff++;
  }  

  cout<<"Excluded "<<nexcluded<<" sick mass points."<<endl;

  cout<<"Working on TGraph"<<endl;
  TGraph *grobslim_cls=new TGraphAsymmErrors(nMassEff,mass,obs_lim_cls);
  grobslim_cls->SetName("LimitObservedCLs");
  TGraph *grmedian_cls=new TGraphAsymmErrors(nMassEff,mass,medianD);
  grmedian_cls->SetName("LimitExpectedCLs");
  TGraphAsymmErrors *gr68_cls=new TGraphAsymmErrors(nMassEff,mass,medianD,0,0,down68err,up68err);
  gr68_cls->SetName("Limit68CLs");
  TGraphAsymmErrors *gr95_cls=new TGraphAsymmErrors(nMassEff,mass,medianD,0,0,down95err,up95err);
  gr95_cls->SetName("Limit95CLs");

  TGraphAsymmErrors *grthSM=new TGraphAsymmErrors(nMassEff,mass,ggxs,0,0,ggxs_downerr,ggxs_uperr);//ggxs_downerr,ggxs_uperr);
  grthSM->SetName("SMXSection");
  TGraphAsymmErrors *grthSM4=new TGraphAsymmErrors(nMassEff,mass,ggxsSM4,0,0,ggxsSM4_downerr,ggxsSM4_uperr);//ggxs_downerr,ggxs_uperr);
  grthSM4->SetName("SM4XSection");

  cout<<"Plotting"<<endl;
  TCanvas *cMCMC=new TCanvas("c_lim_MCMC","canvas with limits for MCMC",630,600);
  cMCMC->cd();
  cMCMC->SetGridx(1);
  cMCMC->SetGridy(1);
  // draw a frame to define the range
  double yRangeUp, yRangeDown;
  if(isXsBr){
    yRangeUp=12;
    yRangeDown=.0001;
 }else{
    if(isSM4){
      yRangeUp=5;
      yRangeDown=.1;
    }else{
      yRangeUp=16;
      yRangeDown=.1;
    }
  }
  if(isLowMass){
    if(isXsBr){
      yRangeUp=20;
      yRangeDown=.001;
    }else if(isSM4){
      yRangeUp=5;
      yRangeDown=.0001;
    }else{
      yRangeUp=35;
      yRangeDown=.0001;
    }
  }

  cout << "Setting plotting style" << endl;

  TH1F *hr = cMCMC->DrawFrame(isLowMass?125.:200.0,yRangeDown,isLowMass?170.:610.0,yRangeUp,"frame1");
  //hr->SetYTitle("#sigma_{95%}/#sigma_{SM}");
  //hr->SetYTitle("#sigma_{95%}/#sigma_{SM}");
  // cMCMC->GetFrame()->SetFillColor(21);
  //cMCMC->GetFrame()->SetBorderSize(12);
  
  gr95_cls->SetFillColor(kYellow);
  gr95_cls->SetFillStyle(1001);//solid
  gr95_cls->GetXaxis()->SetTitle("m_{H} [GeV]");

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
  hr->SetXTitle("m_{H} [GeV]");
  gr95_cls->GetXaxis()->SetRangeUser(isLowMass?125.:200.0,isLowMass?170.:600.0);
  gr95_cls->Draw("3");
  gr68_cls->SetFillColor(kGreen);
  gr68_cls->SetFillStyle(1001);//solid
  gr68_cls->Draw("3same");
  grmedian_cls->GetXaxis()->SetTitle("m_{H} [GeV]");
  //  grmedian_cls->SetMarkerStyle(21);
  grmedian_cls->SetLineStyle(kDashed);
  grmedian_cls->SetLineWidth(3);
  //  grmedian_cls->SetMarkerColor(kBlack);
  grmedian_cls->SetMinimum(0.0);
  grmedian_cls->SetMaximum(8.0);
  grmedian_cls->Draw("L");
  grobslim_cls->SetMarkerColor(kBlack);
  grobslim_cls->SetMarkerStyle(21);
  grobslim_cls->SetMarkerSize(.85);
  grobslim_cls->SetLineWidth(3.);
  //  grobslim_cls->SetLineStyle(2);
  grobslim_cls->Draw("LP");
  
  grthSM->SetLineColor(kRed);
  grthSM->SetLineWidth(2);
  grthSM->SetFillColor(kRed);
  grthSM->SetFillStyle(3244);

  grthSM4->SetLineColor(kRed);
  grthSM4->SetLineWidth(2);
  grthSM4->SetFillColor(kRed);
  grthSM4->SetFillStyle(3353);

  if(isXsBr){
    grthSM->Draw("L3same");
    grthSM4->Draw("L3same");
  }

  //draw grid on top of limits
  gStyle->SetOptStat(0);
  TH1D* postGrid = new TH1D("postGrid","",1,125.,170.);
  postGrid->GetYaxis()->SetRangeUser(yRangeUp,yRangeDown);
  postGrid->Draw("AXIGSAME");
  //==========================

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
  
  cout << "legend" << endl;

  //more graphics
   TLegend *leg = new TLegend(.35,.60,.85,.90);

   leg->SetFillColor(0);
   leg->SetShadowColor(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.034);

   leg->AddEntry(grobslim_cls, "Bayesian Observed", "LP");
   leg->AddEntry(grmedian_cls, "Bayesian Expected", "L");
   leg->AddEntry(gr68_cls, "Bayesian Expected #pm 1#sigma", "LF");
   leg->AddEntry(gr95_cls, "Bayesian Expected #pm 2#sigma", "LF");
   if(isXsBr){
     leg->AddEntry(grthSM, "#sigma(pp#rightarrow H)#timesBR(H#rightarrowZZ), SM", "LF");
     leg->AddEntry(grthSM4, "#sigma(pp#rightarrow H)#timesBR(H#rightarrowZZ), SM4", "LF");
   }else{
     if(isSM4)
       leg->AddEntry(l1,"SM4","l");
     else
       leg->AddEntry(l1,"SM","l");
   }

   leg->Draw();
   
   cout << "labels for top" << endl;

   db->set_lumiNormalization(4600.);

   cout << "add file" << endl;
   db->add_dataFile(fFREQ,"limits");                            
   cout << "done" << endl;
   db->set_isCMSArticle(false);
   TPaveText* cmsLabel = db->get_labelCMS();
   cmsLabel->SetBorderSize(0);
   cmsLabel->Draw();
   TPaveText* sqrtLabel = db->get_labelSqrt();
   sqrtLabel->SetBorderSize(0);
   sqrtLabel->Draw();

   TLine *l1b=new TLine();
   if(!isXsBr){
     l1b->SetLineStyle(1);
     l1b->SetLineWidth(2.0);
     l1b->SetLineColor(kRed);
     l1b->DrawLine(isLowMass?125.:200.0,1.0,isLowMass?170.:600.0,1.0);
   }

   cMCMC->Update();
   
   // cMCMC->RedrawAxis("");
   //gPad->RedrawAxis("");
   // hr->GetYaxis()->DrawClone();
   cMCMC->Update();
   char saveName[20];
   sprintf(saveName,"MCMCLimit_4600pb_%s.eps",tag);
   cMCMC->SaveAs(saveName);
   
}


