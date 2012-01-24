#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "TString.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "TFile.h"
#include "TLegend.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TString.h"
#include <sstream>
#include <string>
#include <vector>
#include "AngularPdfFactory.cc"
//hep.pha.jhu.edu:/scratch/hep/ntran/HZZ_materials
//to setup root: source /scratch/hep/ntran/ROOTdistributions/myROOT528_fromSource/root/bin/thisroot.csh

using namespace RooFit ;

void plotVariablesZZ(){
    
  //gROOT->ProcessLine(".L ~ntran/tdrstyle.C");
  // setTDRStyle();
  //gStyle->SetPadLeftMargin(0.16);
    
    RooRealVar* m1 = new RooRealVar("z1mass","m1",10,500);
    RooRealVar* m2 = new RooRealVar("z2mass","m2",10,500);
    RooRealVar* mZZ = new RooRealVar("zzmass","mZZ",0,3000);
    RooRealVar* costheta1 = new RooRealVar("costheta1","costheta1",-1,1);
    RooRealVar* costheta2 = new RooRealVar("costheta2","costheta2",-1,1);
    RooRealVar* costhetastar = new RooRealVar("costhetastar","costhetastar",-1,1);
    RooRealVar* phi = new RooRealVar("phi","phi",-3.14,3.14);
    RooRealVar* phi1 = new RooRealVar("phistar1","phi1",-3.14,3.14);
 
    TChain* chain = new TChain("angles");
    chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_1.root");
    chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_2.root");
    chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_3.root");
    chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_4.root");
    chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_5.root");
    chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_6.root");
    chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_7.root");
    chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_8.root");
    chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_9.root");
    chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_10.root");
    chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_11.root");
    chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_12.root");
    chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_13.root");
    chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_14.root");
    chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_15.root");
    chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_16.root");
    chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_17.root");
    chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_18.root");
    chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_19.root");
    
   
    RooDataSet *data = new RooDataSet("data","data",chain,RooArgSet(*m1,*m2,*mZZ,*costheta1,*costheta2,*costhetastar,*phi,*phi1));
    RooDataSet *data_cut1 = new RooDataSet("data_cut1","data_cut1",chain,RooArgSet(*m1,*m2,*mZZ,*costheta1,*costheta2,*costhetastar,*phi,*phi1),"zzmass>100 && zzmass<110");
    RooDataSet *data_cut2 = new RooDataSet("data_cut2","data_cut2",chain,RooArgSet(*m1,*m2,*mZZ,*costheta1,*costheta2,*costhetastar,*phi,*phi1),"zzmass>110 && zzmass<130");
    RooDataSet *data_cut3 = new RooDataSet("data_cut3","data_cut3",chain,RooArgSet(*m1,*m2,*mZZ,*costheta1,*costheta2,*costhetastar,*phi,*phi1),"zzmass>130 && zzmass<170");
    RooDataSet *data_cut4 = new RooDataSet("data_cut4","data_cut4",chain,RooArgSet(*m1,*m2,*mZZ,*costheta1,*costheta2,*costhetastar,*phi,*phi1),"zzmass>170 && zzmass<230");
    RooDataSet *data_cut5 = new RooDataSet("data_cut5","data_cut5",chain,RooArgSet(*m1,*m2,*mZZ,*costheta1,*costheta2,*costhetastar,*phi,*phi1),"zzmass>230 && zzmass<370");
    RooDataSet *data_cut6 = new RooDataSet("data_cut6","data_cut6",chain,RooArgSet(*m1,*m2,*mZZ,*costheta1,*costheta2,*costhetastar,*phi,*phi1),"zzmass>370 && zzmass<630");
    RooDataSet *data_cut7 = new RooDataSet("data_cut7","data_cut7",chain,RooArgSet(*m1,*m2,*mZZ,*costheta1,*costheta2,*costhetastar,*phi,*phi1),"zzmass>630 && zzmass<1370");
    
    RooPlot* z1frame =  m1->frame(55);
    data->plotOn(z1frame);
    data_cut1->plotOn(z1frame,MarkerColor(kRed-9),DrawOption("LP"),LineColor(kRed-9));
    data_cut2->plotOn(z1frame,MarkerColor(kRed-7),DrawOption("LP"),LineColor(kRed-7));
    data_cut3->plotOn(z1frame,MarkerColor(kRed-4),DrawOption("LP"),LineColor(kRed-4));
    data_cut4->plotOn(z1frame,MarkerColor(kRed),DrawOption("LP"),LineColor(kRed));
    data_cut5->plotOn(z1frame,MarkerColor(kRed+1),DrawOption("LP"),LineColor(kRed+1));
    data_cut6->plotOn(z1frame,MarkerColor(kRed+2),DrawOption("LP"),LineColor(kRed+2));
    data_cut7->plotOn(z1frame,MarkerColor(kRed+3),DrawOption("LP"),LineColor(kRed+3));
    //myPDF->plotOn(z1frame);
    //myPDFA->plotOn(z1frame, LineColor(2));

    for (int i=0; i<z1frame->numItems(); i++) {
    TString obj_name=z1frame->nameOf(i); 
    cout << Form("%d. '%s'\n",i,obj_name.Data());
    }    

    RooPlot* z2frame =  m2->frame(55);
    data->plotOn(z2frame);
    data_cut1->plotOn(z2frame,MarkerColor(kRed-9),DrawOption("LP"),LineColor(kRed-9));
    data_cut2->plotOn(z2frame,MarkerColor(kRed-7),DrawOption("LP"),LineColor(kRed-7));
    data_cut3->plotOn(z2frame,MarkerColor(kRed-4),DrawOption("LP"),LineColor(kRed-4));
    data_cut4->plotOn(z2frame,MarkerColor(kRed),DrawOption("LP"),LineColor(kRed));
    data_cut5->plotOn(z2frame,MarkerColor(kRed+1),DrawOption("LP"),LineColor(kRed+1));
    data_cut6->plotOn(z2frame,MarkerColor(kRed+2),DrawOption("LP"),LineColor(kRed+2));
    data_cut7->plotOn(z2frame,MarkerColor(kRed+3),DrawOption("LP"),LineColor(kRed+3));
    //myPDF->plotOn(z2frame);
    //myPDFA->plotOn(z2frame, LineColor(2));
    
    RooPlot* mzzframe =  mZZ->frame(55);
    data->plotOn(mzzframe);

    RooPlot* costhetastarframe =  costhetastar->frame(55);
    data->plotOn(costhetastarframe);
    data_cut1->plotOn(costhetastarframe,MarkerColor(kRed-9),DrawOption("LP"),LineColor(kRed-9));
    data_cut2->plotOn(costhetastarframe,MarkerColor(kRed-7),DrawOption("LP"),LineColor(kRed-7));
    data_cut3->plotOn(costhetastarframe,MarkerColor(kRed-4),DrawOption("LP"),LineColor(kRed-4));
    data_cut4->plotOn(costhetastarframe,MarkerColor(kRed),DrawOption("LP"),LineColor(kRed));
    data_cut5->plotOn(costhetastarframe,MarkerColor(kRed+1),DrawOption("LP"),LineColor(kRed+1));
    data_cut6->plotOn(costhetastarframe,MarkerColor(kRed+2),DrawOption("LP"),LineColor(kRed+2));
    data_cut7->plotOn(costhetastarframe,MarkerColor(kRed+3),DrawOption("LP"),LineColor(kRed+3));
    
    RooPlot* costheta1frame =  costheta1->frame(55);
    data->plotOn(costheta1frame);
    data_cut1->plotOn(costheta1frame,MarkerColor(kRed-9),DrawOption("LP"),LineColor(kRed-9));
    data_cut2->plotOn(costheta1frame,MarkerColor(kRed-7),DrawOption("LP"),LineColor(kRed-7));
    data_cut3->plotOn(costheta1frame,MarkerColor(kRed-4),DrawOption("LP"),LineColor(kRed-4));
    data_cut4->plotOn(costheta1frame,MarkerColor(kRed),DrawOption("LP"),LineColor(kRed));
    data_cut5->plotOn(costheta1frame,MarkerColor(kRed+1),DrawOption("LP"),LineColor(kRed+1));
    data_cut6->plotOn(costheta1frame,MarkerColor(kRed+2),DrawOption("LP"),LineColor(kRed+2));
    data_cut7->plotOn(costheta1frame,MarkerColor(kRed+3),DrawOption("LP"),LineColor(kRed+3));

    RooPlot* costheta2frame =  costheta2->frame(55);
    data->plotOn(costheta2frame);
    data_cut1->plotOn(costheta2frame,MarkerColor(kRed-9),DrawOption("LP"),LineColor(kRed-9));
    data_cut2->plotOn(costheta2frame,MarkerColor(kRed-7),DrawOption("LP"),LineColor(kRed-7));
    data_cut3->plotOn(costheta2frame,MarkerColor(kRed-4),DrawOption("LP"),LineColor(kRed-4));
    data_cut4->plotOn(costheta2frame,MarkerColor(kRed),DrawOption("LP"),LineColor(kRed));
    data_cut5->plotOn(costheta2frame,MarkerColor(kRed+1),DrawOption("LP"),LineColor(kRed+1));
    data_cut6->plotOn(costheta2frame,MarkerColor(kRed+2),DrawOption("LP"),LineColor(kRed+2));
    data_cut7->plotOn(costheta2frame,MarkerColor(kRed+3),DrawOption("LP"),LineColor(kRed+3));

    RooPlot* phiframe =  phi->frame(55);
    data->plotOn(phiframe);
    data_cut1->plotOn(phiframe,MarkerColor(kRed-9),DrawOption("LP"),LineColor(kRed-9));
    data_cut2->plotOn(phiframe,MarkerColor(kRed-7),DrawOption("LP"),LineColor(kRed-7));
    data_cut3->plotOn(phiframe,MarkerColor(kRed-4),DrawOption("LP"),LineColor(kRed-4));
    data_cut4->plotOn(phiframe,MarkerColor(kRed),DrawOption("LP"),LineColor(kRed));
    data_cut5->plotOn(phiframe,MarkerColor(kRed+1),DrawOption("LP"),LineColor(kRed+1));
    data_cut6->plotOn(phiframe,MarkerColor(kRed+2),DrawOption("LP"),LineColor(kRed+2));
    data_cut7->plotOn(phiframe,MarkerColor(kRed+3),DrawOption("LP"),LineColor(kRed+3));

    RooPlot* phi1frame =  phi1->frame(55);
    data->plotOn(phi1frame);
    data_cut1->plotOn(phi1frame,MarkerColor(kRed-9),DrawOption("LP"),LineColor(kRed-9));
    data_cut2->plotOn(phi1frame,MarkerColor(kRed-7),DrawOption("LP"),LineColor(kRed-7));
    data_cut3->plotOn(phi1frame,MarkerColor(kRed-4),DrawOption("LP"),LineColor(kRed-4));
    data_cut4->plotOn(phi1frame,MarkerColor(kRed),DrawOption("LP"),LineColor(kRed));
    data_cut5->plotOn(phi1frame,MarkerColor(kRed+1),DrawOption("LP"),LineColor(kRed+1));
    data_cut6->plotOn(phi1frame,MarkerColor(kRed+2),DrawOption("LP"),LineColor(kRed+2));
    data_cut7->plotOn(phi1frame,MarkerColor(kRed+3),DrawOption("LP"),LineColor(kRed+3));

    TCanvas* cz12 = new TCanvas( "cz12", "cz12", 1000, 600 );
    cz12->Divide(2,1);
    cz12->cd(1);
    z1frame->Draw();
    cz12->cd(2);
    z2frame->Draw();

    TCanvas* czz = new TCanvas("czz","czz",1000,600);
    mzzframe->Draw();

    TCanvas* cangles = new TCanvas( "cangles", "cangles", 1000, 600 );
    cangles->Divide(3,2);

    //decay angles
    cangles->cd(6);
    phiframe->Draw();
    cangles->cd(5);
    costheta1frame->Draw();
    cangles->cd(4);
    costheta2frame->Draw();

    //production angles
    cangles->cd(1);
    costhetastarframe->Draw();
    cangles->cd(2);
    phi1frame->Draw();

    TLegend *leg = new TLegend(0.1,0.1,0.7,0.8);
    leg->AddEntry("h_data","all mZZ","L");
    leg->AddEntry("h_data_cut1","mZZ 100-110","L");
    leg->AddEntry("h_data_cut2","mZZ 110-130","L");
    leg->AddEntry("h_data_cut3","mZZ 130-170","L");
    leg->AddEntry("h_data_cut4","mZZ 170-230","L");
    leg->AddEntry("h_data_cut5","mZZ 230-370","L");
    leg->AddEntry("h_data_cut6","mZZ 370-630","L");
    leg->AddEntry("h_data_cut7","mZZ 630-1370","L");

    cout<<"piipo3"<<endl;

    cangles->cd(3);
    leg->Draw();
    cz12->cd(1);
    leg->Draw();

}

//void build8DTemplate(){
//  TFile *f = new TFile("ZZ4l_8DTemplate.root","RECREATE");
//  build3DTemplate_m1m2mzz(f);
//}

//void build3DTemplate_m1m2mzz(TFile *f){
  //create TH3D and RooHistPdf
void build8DTemplate(){
    TChain* chain = new TChain("angles");
   chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_1.root");
   chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_2.root");
   chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_3.root");
   chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_4.root");
   chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_5.root");
   chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_6.root");
   chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_7.root");
   chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_8.root");
   chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_9.root");
   chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_10.root");
   chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_11.root");
   chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_12.root");
   chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_13.root");
   chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_14.root");
   chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_15.root");
   chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_16.root");
   chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_17.root");
   chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_18.root");
   chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_19.root");
     
   /*RooDataSet *data = new RooDataSet("data","data",chain,RooArgSet(*m1,*m2,*mZZ),"zzmass<180");
   RooDataHist *dataHist = new RooDataHist("dataHist","dataHist",RooArgSet(*m1,*m2,*mZZ),*data);
   RooHistPdf* m1m2mzz3DPdf = new RooHistPdf("m1m2mzz3DPdf","m1m2mzz3DPdf",RooArgSet(*m1,*m2,*mZZ),*dataHist);

   RooPlot* mzzframe =  mZZ->frame(55);
   data->plotOn(mzzframe);
   m1m2mzz3DPdf->plotOn(mzzframe);

   RooPlot* m1frame =  m1->frame(55);
   data->plotOn(m1frame);
   m1m2mzz3DPdf->plotOn(m1frame);

   RooPlot* m2frame =  m2->frame(55);
   data->plotOn(m2frame);
   m1m2mzz3DPdf->plotOn(m2frame);

   f->cd()
   m1m2mzz3DPdf->Write();

   TCanvas* cmasses = new TCanvas( "cmasses", "cmasses", 1000, 600 );
   cmasses->Divide(3,1);
   cmasses->cd(1);
   m1frame->Draw();
   cmasses->cd(2);
   m2frame->Draw();
   cmasses->cd(3);
   mzzframe->Draw();*/

   TH1F *h_mzz = new TH1F("h_mzz","h_mzz",35,110,180);
   TH3F *h_mzzm1m2 = new TH3F("h_mzzm1m2","h_mzzm1m2",35,110.,180.,84,12.,180.,54,12.,120.);
   TH2F *h_mzzcosthetastar = new TH2F("h_mzzcosthetastar","h_mzzcosthetastar",35,110,180,50,-1,1);
   TH2F *h_mzzcostheta1 = new TH2F("h_mzzcostheta1","h_mzzcostheta1",35,110,180,50,-1,1);
   TH2F *h_mzzcostheta2 = new TH2F("h_mzzcostheta2","h_mzzcostheta2",35,110,180,50,-1,1);
   TH2F *h_mzzphi = new TH2F("h_mzzphi","h_mzzphi",35,110,180,50,-3.14,3.14);
   TH2F *h_mzzphi1 = new TH2F("h_mzzphi1","h_mzzphi1",35,110,180,50,-3.14,3.14);
   vector<TH2F*> vh_m1m2;
   for (int i=1; i<36; i++){
     std::string s;
     std::stringstream out;
     out << i;
     s = out.str();
     TString name = "h_m1m2_"+s;
     vh_m1m2.push_back((new TH2F(name,name,54,12.,180.,54,12.,120.)));
   }

   double mZZ, m2, m1, costhetastar, costheta1, costheta2, phi, phi1;
   chain->SetBranchAddress("zzmass",&mZZ);
   chain->SetBranchAddress("z2mass",&m2);
   chain->SetBranchAddress("z1mass",&m1);
   chain->SetBranchAddress("costhetastar",&costhetastar);
   chain->SetBranchAddress("phi",&phi);
   chain->SetBranchAddress("costheta1",&costheta1);
   chain->SetBranchAddress("costheta2",&costheta2);
   chain->SetBranchAddress("phi1",&phi1);

   for (Int_t i=0; i<chain->GetEntries();i++) {
    chain->GetEvent(i); 
    if(mZZ>180 || mZZ<110)
      continue;
    h_mzz->Fill(mZZ);
    (vh_m1m2[h_mzz->FindBin(mZZ)-1])->Fill(m1,m2);
    h_mzzm1m2->Fill(mZZ,m1,m2);
    h_mzzcosthetastar->Fill(mZZ,costhetastar);
    h_mzzcostheta1->Fill(mZZ,costheta1);
    h_mzzcostheta2->Fill(mZZ,costheta2);
    h_mzzphi1->Fill(mZZ,phi1);
    h_mzzphi->Fill(mZZ,phi);
   }
   //save it in a file
   TFile *f = new TFile("my8DTemplateNotNorm.root","RECREATE");
   h_mzz->Write();
   h_mzzm1m2->Write();
   h_mzzcosthetastar->Write();
   h_mzzcostheta1->Write();
   h_mzzcostheta2->Write();
   h_mzzphi1->Write();
   h_mzzphi->Write();
   for (int i=0; i<35; i++){
      vh_m1m2[i]->Write();
   }
   f->Close();
}


void plotmy8DTemplate(){
 //read from a file the 3D and 2D template
  TFile *f = new TFile("my8DTemplateNotNorm.root","READ");
  TH1F *h_mzz= (TH1F*)(f->Get("h_mzz"));
  TH3F *h_mzzm1m2= (TH3F*)(f->Get("h_mzzm1m2"));
  TH2F *h_mzzcosthetastar= (TH2F*)(f->Get("h_mzzcosthetastar"));
  TH2F *h_mzzcostheta1= (TH2F*)(f->Get("h_mzzcostheta1"));
  TH2F *h_mzzcostheta2= (TH2F*)(f->Get("h_mzzcostheta2"));
  TH2F *h_mzzphi1= (TH2F*)(f->Get("h_mzzphi1"));
  TH2F *h_mzzphi= (TH2F*)(f->Get("h_mzzphi"));
  vector<TH2F*> vh_m1m2;
  vector<TCanvas*> v_m1m2;
  for (int i=1; i<36; i++){
    std::string s;
    std::stringstream out;
    out << i;
    s = out.str();
    TString name = "h_m1m2_"+s;
    vh_m1m2.push_back((TH2F*)(f->Get(name)));

    TString nameC = "m1m2_"+s;
    double min = h_mzz->GetBinLowEdge(i);
    double max = h_mzz->GetBinLowEdge(i)+h_mzz->GetBinWidth(i);
    std::string smax, smin;
    std::stringstream outmax, outmin;
    outmax << max;
    outmin << min;
    smax = outmax.str();
    smin = outmin.str();
    TString titleC = "m1m2: mZZ>"+smin+" mZZ<"+smax;
    v_m1m2.push_back(new TCanvas(nameC,titleC));
  }

  TCanvas *c_mzz = new TCanvas("mzz","mzz");
  h_mzz->Draw();
  c_mzz->Print("mzz.eps");

  TCanvas *c_mzzcosthetastar = new TCanvas("mzzcosthetastar");
  h_mzzcosthetastar->Draw("COLZ");
  c_mzzcosthetastar->Print("mzzcosthetastar.eps");

  TCanvas *c_mzzcostheta1 = new TCanvas("mzzcostheta1");
  h_mzzcostheta1->Draw("COLZ");
  c_mzzcostheta1->Print("mzzcostheta1.eps");

  TCanvas *c_mzzcostheta2 = new TCanvas("mzzcostheta2");
  h_mzzcostheta2->Draw("COLZ");
  c_mzzcostheta2->Print("mzzcostheta2.eps");  

  TCanvas *c_mzzphi1 = new TCanvas("mzzphi1");
  h_mzzphi1->Draw("COLZ");
  c_mzzphi1->Print("mzzphi1.eps");

  TCanvas *c_mzzphi = new TCanvas("mzzphi");
  h_mzzphi->Draw("COLZ");
  c_mzzphi->Print("mzzphi.eps");
  
  for (int i=1; i<36; i++){
    v_m1m2[i-1]->cd();
    vh_m1m2[i-1]->Draw("COLZ");
 
    std::string s;
    std::stringstream out;
    out << i;
    s = out.str();
    TString name = "m1m2_"+s+".eps";
    v_m1m2[i-1]->Print(name);
  }
  

}

 TFile *f = new TFile("my8DTemplateNotNorm.root","READ");

vector<double> my8DTemplate(bool normalized,double mZZ, double m1, double m2, double costhetastar, double costheta1, double costheta2, double phi, double phi1){
  //read from a file the 3D and 2D template
  TH1F *h_mzz= (TH1F*)(f->Get("h_mzz"));
  TH3F *h_mzzm1m2= (TH3F*)(f->Get("h_mzzm1m2"));
  TH2F *h_mzzcosthetastar= (TH2F*)(f->Get("h_mzzcosthetastar"));
  TH2F *h_mzzcostheta1= (TH2F*)(f->Get("h_mzzcostheta1"));
  TH2F *h_mzzcostheta2= (TH2F*)(f->Get("h_mzzcostheta2"));
  TH2F *h_mzzphi1= (TH2F*)(f->Get("h_mzzphi1"));
  TH2F *h_mzzphi= (TH2F*)(f->Get("h_mzzphi"));
  //multiply the value
  double n = h_mzz->GetBinContent(h_mzz->FindBin(mZZ));
  double Pmzzm1m2 = h_mzzm1m2->GetBinContent(h_mzzm1m2->FindBin(mZZ,m1,m2));
  //cout<<"Pmzzm1m2 "<<Pmzzm1m2<<endl;
  double Pmzzcosthetastar = h_mzzcosthetastar->GetBinContent(h_mzzcosthetastar->FindBin(mZZ,costhetastar));
  double Pmzzcostheta2 = h_mzzcostheta2->GetBinContent(h_mzzcostheta2->FindBin(mZZ,costheta2));
  double Pmzzcostheta1 = h_mzzcostheta1->GetBinContent(h_mzzcostheta1->FindBin(mZZ,costheta1));
  double Pmzzphi1 = h_mzzphi1->GetBinContent(h_mzzphi1->FindBin(mZZ,phi1));
  double Pmzzphi = h_mzzphi->GetBinContent(h_mzzphi->FindBin(mZZ,phi));

  double binwidth_mzzm1m2 = h_mzzm1m2->GetYaxis()->GetBinWidth(1) * h_mzzm1m2->GetZaxis()->GetBinWidth(1);
  //cout<<"binwidth_mzzm1m2 "<<binwidth_mzzm1m2<<endl;
  double binwidth_mzzcosthetastar = h_mzzcosthetastar->GetYaxis()->GetBinWidth(1);
  double binwidth_mzzcostheta1 = h_mzzcostheta1->GetYaxis()->GetBinWidth(1);
  double binwidth_mzzcostheta2 = h_mzzcostheta1->GetYaxis()->GetBinWidth(1);
  double binwidth_mzzphi1 = h_mzzphi1->GetYaxis()->GetBinWidth(1);
  double binwidth_mzzphi = h_mzzphi->GetYaxis()->GetBinWidth(1);

  double Pmzzm1m2_norm = Pmzzm1m2/(n*binwidth_mzzm1m2);
  //cout<<"n "<<n<<"  bin area "<<binwidth_mzzm1m2<<endl;
  //cout<<"Pmzzm1m2_norm "<<Pmzzm1m2_norm<<endl;
  double Pmzzcosthetastar_norm = Pmzzcosthetastar/(n*binwidth_mzzcosthetastar);
  double Pmzzcostheta1_norm = Pmzzcostheta1/(n*binwidth_mzzcostheta1);
  double Pmzzcostheta2_norm = Pmzzcostheta2/(n*binwidth_mzzcostheta2);
  double Pmzzphi1_norm = Pmzzphi1/(n*binwidth_mzzphi1);
  double Pmzzphi_norm = Pmzzphi/(n*binwidth_mzzphi);

  //debug normalization
  /*if(Pmzzcosthetastar_norm>1){
    cout<<"pmzzcosthetastar "<<Pmzzcosthetastar<<endl;
    int ncosthetastar=0;
    for(int i=0;  i<h_mzzcosthetastar->GetNbinsY(); i++){
      int binx = h_mzz->FindBin(mZZ);
      int biny = i+1;
      cout<<i<<" "<<binx<<" "<<biny<<" "<<h_mzzcosthetastar->GetBinContent(binx,biny)<<endl;
      ncosthetastar=ncosthetastar+h_mzzcosthetastar->GetBinContent(binx,biny);
    }
    cout<<"costhetastar nrom "<<ncosthetastar<<endl;
    cout<<"n "<<n<<endl;
    cout<<"bin "<<binwidth_mzzcosthetastar<<endl;
    cout<<"Pmzzcosthetastar_norm"<<Pmzzcosthetastar_norm<<endl;
    }*/

  vector <double> P;
  P.push_back(Pmzzm1m2);
  P.push_back(Pmzzcosthetastar);
  P.push_back(Pmzzcostheta1);
  P.push_back(Pmzzcostheta2);
  P.push_back(Pmzzphi);
  P.push_back(Pmzzphi1);

  vector <double> P_norm;
  P_norm.push_back(Pmzzm1m2_norm);
  P_norm.push_back(Pmzzcosthetastar_norm);
  P_norm.push_back(Pmzzcostheta1_norm);
  P_norm.push_back(Pmzzcostheta2_norm);
  P_norm.push_back(Pmzzphi_norm);
  P_norm.push_back(Pmzzphi1_norm);

  
  if(normalized)
    return P_norm;
  else
    return P;
}

void plotPBackground(){
 TChain* chain = new TChain("angles");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_1.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_2.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_3.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_4.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_5.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_6.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_7.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_8.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_9.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_10.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_11.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_12.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_13.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_14.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_15.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_16.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_17.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_18.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_19.root");
  
  double mZZ, m2, m1, costhetastar, costheta1, costheta2, phi, phi1;
  chain->SetBranchAddress("zzmass",&mZZ);
  chain->SetBranchAddress("z2mass",&m2);
  chain->SetBranchAddress("z1mass",&m1);
  chain->SetBranchAddress("costhetastar",&costhetastar);
  chain->SetBranchAddress("phi",&phi);
  chain->SetBranchAddress("costheta1",&costheta1);
  chain->SetBranchAddress("costheta2",&costheta2);
  chain->SetBranchAddress("phi1",&phi1);

  TH1F *h_P= new TH1F("h_P","h_P",1000,0,0.0002);
  TH1F *h_Pmzzm1m2= new TH1F("h_Pmzzm1m2","h_Pmzzm1m2",1000,0,0.1);
  TH1F *h_Pmzzcosthetastar= new TH1F("h_Pmzzcosthetastar","h_Pmzzcosthetastar",150,0,2);
  TH1F *h_Pmzzcostheta1= new TH1F("h_Pmzzcostheta1","h_Pmzzcostheta1",100,0,1);
  TH1F *h_Pmzzcostheta2= new TH1F("h_Pmzzcostheta2","h_Pmzzcostheta2",100,0,1);
  TH1F *h_Pmzzphi1= new TH1F("h_Pmzzphi1","h_Pmzzphi1",100,0,0.3);
  TH1F *h_Pmzzphi= new TH1F("h_Pmzzphi","h_Pmzzphi",100,0,0.3);
 
  for (Int_t i=0; i<chain->GetEntries();i++) {
    chain->GetEvent(i); 
    if(mZZ>180 || mZZ<110)
      continue;
    vector <double> P=my8DTemplate(1, mZZ,  m1,  m2,  costhetastar,  costheta1,  costheta2,  phi,  phi1);
    double Pbackg = P[0]*P[1]*P[2]*P[3]*P[4]*P[5];
    h_Pmzzm1m2->Fill(P[0]);
    h_Pmzzcosthetastar->Fill(P[1]);
    h_Pmzzcostheta1->Fill(P[2]);
    h_Pmzzcostheta2->Fill(P[3]);
    h_Pmzzphi->Fill(P[4]);
    h_Pmzzphi1->Fill(P[5]);
    h_P->Fill(Pbackg);
  }
  TFile *f1 = new TFile("background_Pdistributions.root","recreate");
  h_Pmzzm1m2->Write();
  h_Pmzzcosthetastar->Write();
  h_Pmzzcostheta1->Write();
  h_Pmzzcostheta2->Write();
  h_Pmzzphi->Write();
  h_Pmzzphi1->Write();
  h_P->Write();

  TCanvas *c = new TCanvas("background_Pdistributions","background_Pdistributions");
  c->Divide(3,2);
  c->cd(1)->SetLogy();
  h_Pmzzm1m2->Draw();
  c->cd(2);
  h_Pmzzcosthetastar->Draw();
  c->cd(3);
  h_Pmzzcostheta1->Draw();
  c->cd(4);
  h_Pmzzcostheta2->Draw();
  c->cd(5);
  h_Pmzzphi->Draw();
  c->cd(6);
  h_Pmzzphi1->Draw();
  c->Write();
  c->Print("background_Pdistributions.eps");
 
 TCanvas *c2 = new TCanvas("background_Ptotaldistribution","background_Ptotaldistribution");
 h_P->Draw();
 c2->Write();
 c2->Print("background_Ptotaldistribution.eps"); 
  
}

/*RooRealVar* z1mass_rrv = new RooRealVar("z1mass","m_{Z1}",0,180);
RooRealVar* z2mass_rrv = new RooRealVar("z2mass","m_{Z2}",0,120);
RooRealVar* costheta1_rrv = new RooRealVar("costheta1","cos#theta_{1}",-1,1);  
RooRealVar* costheta2_rrv = new RooRealVar("costheta2","cos#theta_{2}",-1,1);
RooRealVar* phi_rrv= new RooRealVar("phi","#Phi",-3.1415,3.1415);*/

//AngularPdfFactory SMHiggs(z1mass_rrv,z2mass_rrv,costheta1_rrv,costheta2_rrv,phi_rrv);

pair<double,double> likelihoodDiscriminant (double mZZ, double m1, double m2, double costhetastar, double costheta1, double costheta2, double phi, double phi1){

  //AngularPdfFactory PSHiggs(z1mass,z2mass,costheta1,costheta2,phi);
  //PSHiggs.makePSHiggs();
  //PSHiggs.makeParamsConst(true);
  RooRealVar* z1mass_rrv = new RooRealVar("z1mass","m_{Z1}",0,180);
  RooRealVar* z2mass_rrv = new RooRealVar("z2mass","m_{Z2}",0,120);
  RooRealVar* costheta1_rrv = new RooRealVar("costheta1","cos#theta_{1}",-1,1);  
  RooRealVar* costheta2_rrv = new RooRealVar("costheta2","cos#theta_{2}",-1,1);
  RooRealVar* phi_rrv= new RooRealVar("phi","#Phi",-3.1415,3.1415);
  RooRealVar* mzz_rrv= new RooRealVar("mzz","mZZ",110,180);
  AngularPdfFactory SMHiggs(z1mass_rrv,z2mass_rrv,costheta1_rrv,costheta2_rrv,phi_rrv,mzz_rrv);
  SMHiggs.makeSMHiggs();
  SMHiggs.makeParamsConst(true);

  z1mass_rrv->setVal(m1);  
  z2mass_rrv->setVal(m2);
  costheta1_rrv->setVal(costheta1);
  costheta2_rrv->setVal(costheta2);
  phi_rrv->setVal(phi);
  mzz_rrv->setVal(mZZ);

  //RooArgSet *ras = new RooArgSet(*z1mass_rrv,*z2mass_rrv,*costheta1_rrv,*costheta2_rrv,*phi_rrv);
  //double Psig = SMHiggs.PDF->getVal(ras);///SMHiggs.PDF->getNorm(ras);
  double Psig = SMHiggs.getVal(mZZ);

  /*if(fabs(SMHiggs.PDF->getVal(ras) - 4.68449e-11)>1/pow(10,10) ){
    cout<<"val "<<SMHiggs.PDF->getVal(ras)<<endl;
    cout<<"norm "<<SMHiggs.PDF->getNorm(ras)<<endl;
    cout<<"PDF from SM Higgs signal "<<Psig<<endl;
    
    cout<<m1<<" "<<m2<<" "<<costhetastar<<" "<<costheta1<<" "<<costheta2<<" "<<phi<<" "<<phi1<<endl;
  }
  else
  cout<<"no "<<endl;*/
  //double Psig=1;
  vector <double> P=my8DTemplate(1, mZZ,  m1,  m2,  costhetastar,  costheta1,  costheta2,  phi,  phi1);
  double Pbackg = P[0]*P[1]*P[2]*P[3]*P[4]*P[5];
  //cout<<"PDF from ZZ EWK background "<<Pbackg<<endl;

  //double LD =  (Psig/(Psig+Pbackg)) ;
  //cout<<"LD "<<LD<<endl;
  //return  LD;

  return make_pair(Psig,Pbackg);
}

TH1F *h_mzz= (TH1F*)(f->Get("h_mzz"));

vector<TH1F*> LDDistributionSignal(){
  TFile* file = new TFile("datafiles/SMHiggs_125_JHU.root");
  TTree* tree = (TTree*) file->Get("angles");
  
  double mZZ, m2, m1, costhetastar, costheta1, costheta2, phi, phi1;
  tree->SetBranchAddress("zzmass",&mZZ);
  tree->SetBranchAddress("z2mass",&m2);
  tree->SetBranchAddress("z1mass",&m1);
  tree->SetBranchAddress("costhetastar",&costhetastar);
  tree->SetBranchAddress("phi",&phi);
  tree->SetBranchAddress("costheta1",&costheta1);
  tree->SetBranchAddress("costheta2",&costheta2);
  tree->SetBranchAddress("phi1",&phi1);

  TH1F *h_LDsignal= new TH1F("LD_signal","LD_signal",100,0,1);
  TH1F *h_Psignal= new TH1F("P_signal","P_signal",100,0,0.0002);
  TH1F *h_Pbackground= new TH1F("P_background","P_background",100,0,0.0002);
  vector<TH1F*> vh_LDsignal;
  for (int i=1; i<36; i++){
    std::string s;
     std::stringstream out;
     out << i;
     s = out.str();
     TString name = "h_LDsignal_"+s;
     vh_LDsignal.push_back((new TH1F(name,name,100,0,1)));
  }

  for (Int_t i=0; i<tree->GetEntries();i++) {
    tree->GetEvent(i); 
    if(mZZ>180 || mZZ<110)
      continue;

    pair<double,double> P =  likelihoodDiscriminant(mZZ, m1, m2, costhetastar, costheta1, costheta2, phi, phi1);
    h_Psignal->Fill(P.first);
    h_Pbackground->Fill(P.second);
    h_LDsignal->Fill(P.first/(P.first+P.second));
    //running on gen: all events have same mass!!! LD_signal does not depend on mZZ!!!
    //(vh_LDsignal[h_mzz->FindBin(mZZ)-1])->Fill(P.first/(P.first+P.second));
    for (int j=1; j<36; j++){
      (vh_LDsignal[j-1])->Fill(P.first/(P.first+P.second)); 
    }
  }

  TCanvas *LD = new TCanvas("LD_signal","LD_signal");
  LD->Divide(3);
  LD->cd(1)->SetLogy();
  h_Psignal->Draw();
  LD->cd(2)->SetLogy();
  h_Pbackground->Draw();
  LD->cd(3);
  h_LDsignal->Draw();
  LD->Print("LD_signal.eps");

  vh_LDsignal.push_back(h_LDsignal);

  return vh_LDsignal;
}

vector<TH1F*> LDDistributionBackground(){
  TChain* chain = new TChain("angles");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_1.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_2.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_3.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_4.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_5.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_6.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_7.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_8.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_9.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_10.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_11.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_12.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_13.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_14.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_15.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_16.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_17.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_18.root");
  chain->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_19.root");
  
  double mZZ, m2, m1, costhetastar, costheta1, costheta2, phi, phi1;
  chain->SetBranchAddress("zzmass",&mZZ);
  chain->SetBranchAddress("z2mass",&m2);
  chain->SetBranchAddress("z1mass",&m1);
  chain->SetBranchAddress("costhetastar",&costhetastar);
  chain->SetBranchAddress("phi",&phi);
  chain->SetBranchAddress("costheta1",&costheta1);
  chain->SetBranchAddress("costheta2",&costheta2);
  chain->SetBranchAddress("phi1",&phi1);

  TH1F *h_Psignal= new TH1F("P_signal","P_signal",100,0,0.0002);
  TH1F *h_Pbackground= new TH1F("P_background","P_background",100,0,0.0002);
  TH1F *h_LDbackground= new TH1F("LD_background","LD_background",100,0,1);
  vector<TH1F*> vh_LDbackground;
  for (int i=1; i<36; i++){
    std::string s;
    std::stringstream out;
    out << i;
    s = out.str();
    TString name = "h_LDbackground_"+s;
    vh_LDbackground.push_back((new TH1F(name,name,100,0,1)));
  }

  for (Int_t i=0; i<chain->GetEntries();i++) {
    chain->GetEvent(i); 
    if(mZZ>180 || mZZ<110)
      continue;
    //if(m1+m2>125)
    //continue;
    if((i%10000)==0)
      cout<<"event "<<i<<endl;
    pair<double,double> P =  likelihoodDiscriminant(mZZ, m1, m2, costhetastar, costheta1, costheta2, phi, phi1);
    h_Psignal->Fill(P.first);
    h_Pbackground->Fill(P.second);
    h_LDbackground->Fill(P.first/(P.first+P.second));
    (vh_LDbackground[h_mzz->FindBin(mZZ)-1])->Fill(P.first/(P.first+P.second));
    //if(P.first/(P.first+P.second)<0.0001)
    //cout<<P.first/(P.first+P.second)<<" "<<P.first<<" "<<P.second<<endl<<endl;
  }
  TCanvas *LD = new TCanvas("LD_background","LD_background");
  LD->Divide(3);
  LD->cd(1)->SetLogy();
  h_Psignal->Draw();
  LD->cd(2)->SetLogy();
  h_Pbackground->Draw();
  LD->cd(3);
  h_LDbackground->Draw();
  LD->Print("LD_background.eps");

  vh_LDbackground.push_back(h_LDbackground);
 
  return vh_LDbackground;
}

void plotLDDistribution(){
  //SMHiggs.makeSMHiggs();
  //SMHiggs.makeParamsConst(true);

  vector<TH1F*> vh_LDsignal = LDDistributionSignal();
  vector<TH1F*> vh_LDbackground = LDDistributionBackground();
  cout<<"LD computed. Vector size "<<vh_LDsignal.size()<<"  "<<vh_LDbackground.size()<<endl;
  for (int i=1; i<(36+1); i++){ //the last one is integrated over mzz
    vh_LDsignal[i-1]->Scale(1./vh_LDsignal[i-1]->Integral());
    vh_LDbackground[i-1]->Scale(1./vh_LDbackground[i-1]->Integral());
  }

  cout<<"histo normalized"<<endl;
  TCanvas *LD = new TCanvas("LD","LD integrated over mZZ");
  vh_LDsignal[35]->SetLineColor(2);
  vh_LDsignal[35]->Draw();
  vh_LDbackground[35]->Draw("same");
  TLegend *leg = new TLegend(0.5,0.6,0.7,0.8);
  leg->AddEntry(vh_LDsignal[35],"signal: SM Higgs 125 GeV","L");
  leg->AddEntry(vh_LDbackground[35],"background: ZZ","L");
  leg->Draw("same");
  LD->Print("LD.eps");

  cout<<"mzz integrated canvas created"<<endl;
  vector<TCanvas*> vc_LD;
  for (int i=1; i<36; i++){
    std::string s;
    std::stringstream out;
    out << i;
    s = out.str();
    TString nameC = "LD_"+s;
    double min = h_mzz->GetBinLowEdge(i);
    double max = h_mzz->GetBinLowEdge(i)+h_mzz->GetBinWidth(i);
    std::string smax, smin;
    std::stringstream outmax, outmin;
    outmax << max;
    outmin << min;
    smax = outmax.str();
    smin = outmin.str();
    TString titleC = "LD: mZZ>"+smin+" mZZ<"+smax;
    vc_LD.push_back(new TCanvas(nameC,titleC));

    vc_LD[i-1]->cd();
    vh_LDsignal[i-1]->SetLineColor(2);
    vh_LDsignal[i-1]->Draw();
    vh_LDbackground[i-1]->Draw("same");
 
    leg->Draw("same");

    vc_LD[i-1]->Print(nameC+".eps");
  }
}

void storeLDDistribution(bool signal){
  //SMHiggs.makeSMHiggs();
  //SMHiggs.makeParamsConst(true);

  vector<TH1F*> vh_LD;
  TFile *file;
  if(signal){
    vh_LD = LDDistributionSignal();
    file= new TFile("Dsignal.root","recreate");
  }
  else {
    vh_LD = LDDistributionBackground();
    file= new TFile("Dbackground.root","recreate");
  }
  cout<<"LD computed. Vector size "<<vh_LD.size()<<endl;

  for (int i=1; i<(36+1); i++){ //the last one is integrated over mzz
    vh_LD[i-1]->Scale(1./vh_LD[i-1]->Integral());
  }

  TH2F* h_mzzD = new TH2F("h_mzzD","h_mzzD",35,110,180,vh_LD[0]->GetNbinsX(),vh_LD[0]->GetXaxis()->GetXmin(),vh_LD[0]->GetXaxis()->GetXmax());

  for (int i=1; i<36; i++){
    for(int j=1; j<vh_LD[0]->GetNbinsX(); j++)
      h_mzzD->SetBinContent(i,j,vh_LD[i-1]->GetBinContent(j));
  }
  file->cd();
  h_mzzD->Write();
}

void storeLDInfo(bool signal){
  TFile* file ;
  TChain* tree = new TChain("angles");
  if(signal){
    tree->Add("datafiles/SMHiggs_125_JHU.root");
  }

  if(!signal){
  tree->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_1.root");
  tree->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_2.root");
  tree->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_3.root");
  tree->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_4.root");
  tree->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_5.root");
  tree->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_6.root");
  tree->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_7.root");
  tree->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_8.root");
  tree->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_9.root");
  tree->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_10.root");
  tree->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_11.root");
  tree->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_12.root");
  tree->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_13.root");
  tree->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_14.root");
  tree->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_15.root");
  tree->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_16.root");
  tree->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_17.root");
  tree->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_18.root");
  tree->Add("datafiles/PowhegFiles/EWKZZ4l_Powheg_19.root");
 

  }
  double mZZ, m2, m1, costhetastar, costheta1, costheta2, phi, phi1;
  tree->SetBranchAddress("zzmass",&mZZ);
  tree->SetBranchAddress("z2mass",&m2);
  tree->SetBranchAddress("z1mass",&m1);
  tree->SetBranchAddress("costhetastar",&costhetastar);
  tree->SetBranchAddress("phi",&phi);
  tree->SetBranchAddress("costheta1",&costheta1);
  tree->SetBranchAddress("costheta2",&costheta2);
  tree->SetBranchAddress("phi1",&phi1);

  TFile* fileOut; 
  double mZZout, m2out, m1out, costhetastarout, costheta1out, costheta2out, phiout, phi1out, Psig, Pbkg, D;
  if(signal)
    fileOut = new TFile("datafiles/SMHiggs_125_JHU_withD.root","RECREATE");
  else if(!signal)
    fileOut = new TFile("datafiles/EWKZZ4l_Powheg_withD.root","RECREATE");
  TTree* treeOut = new TTree("eventInfo","angles, mzz, D");
  treeOut->Branch("zzmass",&mZZout,"zzmass/D");
  treeOut->Branch("z2mass",&m2out,"z2mass/D");
  treeOut->Branch("z1mass",&m1out,"z1mass/D");
  treeOut->Branch("costhetastar",&costhetastarout,"costhetastar/D");
  treeOut->Branch("phi",&phiout,"phi/D");
  treeOut->Branch("costheta1",&costheta1out,"costheta1/D");
  treeOut->Branch("costheta2",&costheta2out,"costheta2/D");
  treeOut->Branch("phi1",&phi1out,"phi1/D");
  treeOut->Branch("Psig",&Psig,"Psig/D");
  treeOut->Branch("Pbkg",&Pbkg,"Pbkg/D");
  treeOut->Branch("D",&D,"D/D");

  for (Int_t i=0; i<tree->GetEntries();i++) {
    tree->GetEvent(i); 
    if(mZZ>180 || mZZ<110)
      continue;
    //if(m1+m2>125)
    //continue;
    if((i%10000)==0)
      cout<<"event "<<i<<endl;
    pair<double,double> P =  likelihoodDiscriminant(mZZ, m1, m2, costhetastar, costheta1, costheta2, phi, phi1);
    Psig=P.first;
    Pbkg=P.second;
    D= P.first/(P.first+P.second);

    mZZout=mZZ;
    m1out= m1;
    m2out=m2;
    costhetastarout=costhetastar;
    costheta1out=costheta1;
    costheta2out=costheta2;
    phiout=phi;
    phi1out=phi1;
    treeOut->Fill();
 }

  fileOut->cd();
  treeOut->Write();
}

