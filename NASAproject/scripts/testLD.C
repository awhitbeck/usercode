#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "AngularPdfFactory.cc"

using namespace RooFit;

void testLD(double scale=1.0){

  RooRealVar* z1mass = new RooRealVar("z1mass","m_{Z1}",0,120);
  RooRealVar* z2mass = new RooRealVar("z2mass","m_{Z2}",0,120);
  RooRealVar* costheta1 = new RooRealVar("costheta1","cos#theta_{1}",-1,1);  
  RooRealVar* costheta2 = new RooRealVar("costheta2","cos#theta_{2}",-1,1);
  RooRealVar* phi= new RooRealVar("phi","#Phi",-3.1415,3.1415);

  RooArgSet *argSet = new RooArgSet(*z1mass,*z2mass,*costheta1,*costheta2,*phi);

  vector<RooRealVar*> meas;
  meas.push_back(z1mass);
  meas.push_back(z2mass);
  meas.push_back(costheta1);
  meas.push_back(costheta2);
  meas.push_back(phi);

  cout << "Initializing PDFs" << endl;

  AngularPdfFactory SMHiggs(z1mass,z2mass,costheta1,costheta2,phi);
  SMHiggs.makeSMHiggs();
  SMHiggs.makeParamsConst(true);

  AngularPdfFactory PSHiggs(z1mass,z2mass,costheta1,costheta2,phi);
  PSHiggs.makePSHiggs();
  PSHiggs.makeParamsConst(true);

  TFile* file = new TFile("/scratch/hep/ntran/HZZ_materials/datafiles/SMHiggs_125_JHU.root");
  TTree* tree = (TTree*) file->Get("angles");
  RooDataSet *data = new RooDataSet("data","data",tree,RooArgSet(*z1mass,*z2mass,*costheta1,*costheta2,*phi));

  cout << "loading data for SM Higgs" << endl;

  TFile* fSMHiggs = new TFile("/scratch/hep/ntran/HZZ_materials/datafiles/SMHiggs_125_JHU.root");
  if(!fSMHiggs){
    cout << "ERROR: couldn't load file" << endl;
    return; 
  }
  TTree* tSMHiggs = (TTree*) fSMHiggs->Get("angles");
  if(!tSMHiggs){
    cout << "ERROR: couldn't load tree" << endl;
    return;
  }

  TH1F* SMhisto = new TH1F("SMhisto","SMhisto",50,0.,1.);

  double m1_,m2_,h1_,h2_,phi_;

  tSMHiggs->SetBranchAddress("z1mass",&m1_);
  tSMHiggs->SetBranchAddress("z2mass",&m2_);
  tSMHiggs->SetBranchAddress("costheta1",&h1_);
  tSMHiggs->SetBranchAddress("costheta2",&h2_);
  tSMHiggs->SetBranchAddress("phi",&phi_);

  for(int iEvt=0; iEvt<tSMHiggs->GetEntries(); iEvt++){
    tSMHiggs->GetEntry(iEvt);

    z1mass->setVal(m1_);
    z2mass->setVal(m2_);
    costheta1->setVal(h1_);
    costheta2->setVal(h2_);
    phi->setVal(phi_);

    SMhisto->Fill(1/(1+SMHiggs.PDF->getVal(argSet)/(PSHiggs.PDF->getVal(argSet)*scale)));
			
  }
  
  TFile* fPSHiggs = new TFile("/scratch/hep/ntran/HZZ_materials/datafiles/PSHiggs_125_JHU.root");
  if(!fPSHiggs){
    cout << "ERROR: couldn't load file" << endl;
    return;
  }
  TTree* tPSHiggs = (TTree*) fPSHiggs->Get("angles");
  if(!tPSHiggs){
    cout << "ERROR: couldn't load tree" << endl;
    return;
  }
  cout << "draw LD distribution" << endl;
  
  TH1F* PShisto = new TH1F("PShisto","PShisto",50,0.,1.); 
  
  tPSHiggs->SetBranchAddress("z1mass",&m1_);
  tPSHiggs->SetBranchAddress("z2mass",&m2_);
  tPSHiggs->SetBranchAddress("costheta1",&h1_);
  tPSHiggs->SetBranchAddress("costheta2",&h2_);
  tPSHiggs->SetBranchAddress("phi",&phi_);

  for(int iEvt=0; iEvt<tPSHiggs->GetEntries(); iEvt++){
    tPSHiggs->GetEntry(iEvt);

    z1mass->setVal(m1_);
    z2mass->setVal(m2_);
    costheta1->setVal(h1_);
    costheta2->setVal(h2_);
    phi->setVal(phi_);

    PShisto->Fill(1/(1+SMHiggs.PDF->getVal(argSet)/(PSHiggs.PDF->getVal(argSet)*scale)));
			
  }

  
  SMhisto->Scale(1/SMhisto->Integral());
  SMhisto->SetLineColor(2);
  PShisto->Scale(1/PShisto->Integral());
  PShisto->SetLineColor(4);
  
  TCanvas* can = new TCanvas("can","can",400,400);

  if(SMhisto->GetMaximum()>PShisto->GetMaximum()){
    SMhisto->Draw();  
    PShisto->Draw("SAME");
  }else{
    PShisto->Draw();  
    SMhisto->Draw("SAME");
  }    
  TLegend* leg = new TLegend(.7,.8,.95,.6);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);

  leg->AddEntry(SMhisto,"SM Higgs (0^{+}","l");
  leg->AddEntry(PShisto,"PS Higgs (0^{-}","l");

  leg->Draw();

  can->SaveAs("LDdistribution.eps"); 

}
