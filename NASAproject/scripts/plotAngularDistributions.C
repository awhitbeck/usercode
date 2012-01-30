#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
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
#include "TTree.h"
#include "../src/AngularPdfFactory.cc"

using namespace RooFit;
void plotVariablesZZ_background(){
    
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
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_1.root");
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_2.root");
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_3.root");
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_4.root");
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_5.root");
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_6.root");
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_7.root");
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_8.root");
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_9.root");
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_10.root");
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_11.root");
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_12.root");
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_13.root");
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_14.root");
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_15.root");
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_16.root");
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_17.root");
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_18.root");
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_19.root");
    
   
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

void plotAngularDistributions_signal(){

  RooRealVar* z1mass = new RooRealVar("z1mass","m_{Z1}",20,120);
  RooRealVar* z2mass = new RooRealVar("z2mass","m_{Z2}",20,120);
  RooRealVar* costheta1 = new RooRealVar("costheta1","cos#theta_{1}",-1,1);  
  RooRealVar* costheta2 = new RooRealVar("costheta2","cos#theta_{2}",-1,1);
  RooRealVar* phi= new RooRealVar("phi","#Phi",-3.1415,3.1415);
  
  AngularPdfFactory SMHiggs(z1mass,z2mass,costheta1,costheta2,phi);
  SMHiggs.makeSMHiggs();
  SMHiggs.makeParamsConst(true);

  AngularPdfFactory PSHiggs(z1mass,z2mass,costheta1,costheta2,phi);
  PSHiggs.makePSHiggs();
  PSHiggs.makeParamsConst(true);

  TFile* fin = new TFile("../datafiles/SMHiggs_125_JHU.root");
  TTree* tin = (TTree*) fin->Get("angles");
  RooDataSet data("data","data",tin,RooArgSet(*z1mass,*z2mass,*costheta1,*costheta2,*phi));

  TCanvas* c = new TCanvas("c","c",1200,800);
  c->Divide(3,2);

  RooPlot *plot_m1 = z1mass->frame(55);
  data.plotOn(plot_m1);
  PSHiggs.PDF->plotOn(plot_m1);
  SMHiggs.PDF->plotOn(plot_m1,LineColor(2));
  c->cd(1);
  plot_m1->Draw();

  RooPlot *plot_m2 = z2mass->frame(55);
  data.plotOn(plot_m2);
  PSHiggs.PDF->plotOn(plot_m2);
  SMHiggs.PDF->plotOn(plot_m2,LineColor(2));
  c->cd(2);
  plot_m2->Draw();

  RooPlot *plot_h1 = costheta1->frame(55);
  data.plotOn(plot_h1);
  PSHiggs.PDF->plotOn(plot_h1);
  SMHiggs.PDF->plotOn(plot_h1,LineColor(2));
  c->cd(3);
  plot_h1->Draw();
  RooPlot *plot_h2 = costheta2->frame(55);
  data.plotOn(plot_h2);
  PSHiggs.PDF->plotOn(plot_h2);
  SMHiggs.PDF->plotOn(plot_h2,LineColor(2));
  c->cd(4);
  plot_h2->Draw();
  RooPlot *plot_Phi = phi->frame(55);
  data.plotOn(plot_Phi);
  PSHiggs.PDF->plotOn(plot_Phi);
  SMHiggs.PDF->plotOn(plot_Phi,LineColor(2));
  c->cd(5);
  plot_Phi->Draw();

}
