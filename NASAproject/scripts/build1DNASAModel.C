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
#include "TF1.h"
#include "TString.h"
#include <sstream>
#include <string>
#include <vector>
#include "../src/AngularPdfFactory.cc"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"

/*
//hep.pha.jhu.edu:/scratch/hep/ntran/HZZ_materials
//to setup root: source /scratch/hep/ntran/ROOTdistributions/myROOT528_fromSource/root/bin/thisroot.csh
//be sure to compile/load everything:
 gSystem->AddIncludePath("-I/$ROOFITSYS/include/");
.L ../PDFs/RooXZsZs_5D.cxx+
.L ../src/AngularPdfFactory.cc+
.L build1DNASAModel.C+
*/

using namespace RooFit ;

void build8DTemplate(){
    TChain* chain = new TChain("angles");
   chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_total_v5.root");
   chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_total_v6.root");
   chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_total_v7.root");

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
   chain->SetBranchAddress("phistar1",&phi1);

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
   TFile *f = new TFile("../datafiles/my8DTemplateNotNorm.root","RECREATE");
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
  TFile *f = new TFile("../datafiles/my8DTemplateNotNorm.root","READ");
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

 TFile *f = new TFile("../datafiles/my8DTemplateNotNorm.root","READ");

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

  // - - - - - - - - - - - - - - - whitbeck
  // if bin has no events: add 1
  // safety feature to prevent LD = 1 as a
  // result of low statistics

  if(Pmzzm1m2==0){
    //cout<<mZZ<<" "<<m1<<" "<<m2<<" "<<costhetastar<<" "<<costheta1<<" "<<costheta2<<" "<<phi<<" "<<phi1<<endl;
    //cout << "my8DTemplate says ... " << endl;
    //cout<<"Pmzzm1m2: "<<Pmzzm1m2<<endl;
    Pmzzm1m2++;
    }
  // - - - - - - - - - - - - - - - 

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

   chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_total_v5.root");
   chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_total_v6.root");
   chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_total_v7.root");
  
  double mZZ, m2, m1, costhetastar, costheta1, costheta2, phi, phi1;
  chain->SetBranchAddress("zzmass",&mZZ);
  chain->SetBranchAddress("z2mass",&m2);
  chain->SetBranchAddress("z1mass",&m1);
  chain->SetBranchAddress("costhetastar",&costhetastar);
  chain->SetBranchAddress("phi",&phi);
  chain->SetBranchAddress("costheta1",&costheta1);
  chain->SetBranchAddress("costheta2",&costheta2);
  chain->SetBranchAddress("phistar1",&phi1);

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


pair<double,double> likelihoodDiscriminant (double mZZ, double m1, double m2, double costhetastar, double costheta1, double costheta2, double phi, double phi1,double scaleFactor=5.0){

  //AngularPdfFactory PSHiggs(z1mass,z2mass,costheta1,costheta2,phi);
  //PSHiggs.makePSHiggs();
  //PSHiggs.makeParamsConst(true);
  RooRealVar* z1mass_rrv = new RooRealVar("z1mass","m_{Z1}",0,180);
  RooRealVar* z2mass_rrv = new RooRealVar("z2mass","m_{Z2}",0,120); 
  RooRealVar* costheta1_rrv = new RooRealVar("costheta1","cos#theta_{1}",-1,1);  
  RooRealVar* costheta2_rrv = new RooRealVar("costheta2","cos#theta_{2}",-1,1);
  RooRealVar* phi_rrv= new RooRealVar("phi","#Phi",-3.1415,3.1415);
  RooRealVar* mzz_rrv= new RooRealVar("mzz","mZZ",110,180);
  AngularPdfFactory *SMHiggs = new AngularPdfFactory(z1mass_rrv,z2mass_rrv,costheta1_rrv,costheta2_rrv,phi_rrv,mzz_rrv);
  SMHiggs->makeSMHiggs();
  SMHiggs->makeParamsConst(true);

  z1mass_rrv->setVal(m1);  
  z2mass_rrv->setVal(m2);
  costheta1_rrv->setVal(costheta1);
  costheta2_rrv->setVal(costheta2);
  phi_rrv->setVal(phi);
  mzz_rrv->setVal(mZZ);

  //RooArgSet *ras = new RooArgSet(*z1mass_rrv,*z2mass_rrv,*costheta1_rrv,*costheta2_rrv,*phi_rrv);
  //double Psig = SMHiggs.PDF->getVal(ras);///SMHiggs.PDF->getNorm(ras);
  double Psig = SMHiggs->getVal(mZZ);

  vector <double> P=my8DTemplate(1, mZZ,  m1,  m2,  costhetastar,  costheta1,  costheta2,  phi,  phi1);
  double Pbackg = P[0]*P[1]*P[2]*P[3]*P[4]*P[5]*scaleFactor;

  // - - - - - - - - - - - - - - - - - - - - - Whitbeck 
  // check whether P[i] is zero and print warning
  // message if so

  char* varName[6]={"m1/m2","costhetastar","costheta1","coshteta2","phi","phi1"};
  for(int iVar=0; iVar<6; iVar++){

    if(P[iVar]==0 && (m1+m2)<mZZ && m2>20 && mZZ>110 && mZZ<180)
	cout << " uh oh... Probability of " << varName[iVar] << " is zero." << endl;
  }
  // - - - - - - - - - - - - - - - - - - - - - 

  //cout<<"PDF from ZZ EWK background "<<Pbackg<<endl;

  //double LD =  (Psig/(Psig+Pbackg)) ;
  //cout<<"LD "<<LD<<endl;
  //return  LD;

  delete z1mass_rrv; 
  delete z2mass_rrv; 
  delete costheta1_rrv;
  delete costheta2_rrv;
  delete phi_rrv;
  delete mzz_rrv; 

  delete SMHiggs;

  return make_pair(Psig,Pbackg);
}

TH1F *h_mzz= (TH1F*)(f->Get("h_mzz"));

vector<TH1F*> LDDistributionSignal(){

  TFile* file = new TFile("../datafiles/JHUGenFiles/SMHiggs_125_JHU_wResolution_withDiscriminants_withCuts.root");
  //TFile* file = new TFile("../datafiles/JHUGenFiles/SMHiggs_125_JHU_wResolution.root");
  TTree* tree = (TTree*) file->Get("angles");
  double mZZ, m2, m1, costhetastar, costheta1, costheta2, phi, phi1;
  tree->SetBranchAddress("zzmass",&mZZ);
  tree->SetBranchAddress("z2mass",&m2);
  tree->SetBranchAddress("z1mass",&m1);
  tree->SetBranchAddress("costhetastar",&costhetastar);
  tree->SetBranchAddress("phi",&phi);
  tree->SetBranchAddress("costheta1",&costheta1);
  tree->SetBranchAddress("costheta2",&costheta2);
  tree->SetBranchAddress("phistar1",&phi1);

  TH1F *h_LDsignal= new TH1F("LD_signal","LD_signal",101,0,1.01);
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
    if(mZZ>180 || mZZ<110 || m2<20)
      continue;

    pair<double,double> P =  likelihoodDiscriminant(mZZ, m1, m2, costhetastar, costheta1, costheta2, phi, phi1);
    h_Psignal->Fill(P.first);
    h_Pbackground->Fill(P.second);
    h_LDsignal->Fill(P.first/(P.first+P.second));
    (vh_LDsignal[h_mzz->FindBin(mZZ)-1])->Fill(P.first/(P.first+P.second));
    
    //running on gen: all events have same mass!!! LD_signal does not depend on mZZ!!!
    //therefore I store the same LD for all the mass bins!!!
    //for (int j=1; j<36; j++){
    //  (vh_LDsignal[j-1])->Fill(P.first/(P.first+P.second)); 
    //}
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
//============================================================
vector<TH1F*> LDDistributionBackground(){

  TChain* chain = new TChain("angles");

  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_total_v8_wResolution_withDiscriminants_withCuts.root");
  chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_total_v9_wResolution_withDiscriminants_withCuts.root");
  /*chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_total_v8_wResolution.root");
    chain->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_total_v9_wResolution.root");*/

  double mZZ, m2, m1, costhetastar, costheta1, costheta2, phi, phi1;
  chain->SetBranchAddress("zzmass",&mZZ);
  chain->SetBranchAddress("z2mass",&m2);
  chain->SetBranchAddress("z1mass",&m1);
  chain->SetBranchAddress("costhetastar",&costhetastar);
  chain->SetBranchAddress("phi",&phi);
  chain->SetBranchAddress("costheta1",&costheta1);
  chain->SetBranchAddress("costheta2",&costheta2);
  chain->SetBranchAddress("phistar1",&phi1);

  TH1F *h_Psignal= new TH1F("P_signal","P_signal",100,0,0.0002);
  TH1F *h_Pbackground= new TH1F("P_background","P_background",100,0,0.0002);
  TH1F *h_LDbackground= new TH1F("LD_background","LD_background",101,0,1.01);
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
    if(mZZ>180 || mZZ<110 || m2<20)
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

//=======================================================================

double separationLikelihoodDiscriminant(double mzz,double m1,double m2,double h1,double h2,double phi){

  // initialize measurables
  RooRealVar* z1mass_rrv = new RooRealVar("z1mass","m_{Z1}",m1,0,180);
  RooRealVar* z2mass_rrv = new RooRealVar("z2mass","m_{Z2}",m2,0,120); 
  RooRealVar* costheta1_rrv = new RooRealVar("costheta1","cos#theta_{1}",h1,-1,1);  
  RooRealVar* costheta2_rrv = new RooRealVar("costheta2","cos#theta_{2}",h2,-1,1);
  RooRealVar* phi_rrv= new RooRealVar("phi","#Phi",phi,-3.1415,3.1415);
  RooRealVar* mzz_rrv= new RooRealVar("mzz","mZZ",mzz,110,180);

  // build PDFs
  AngularPdfFactory *SMHiggs = new AngularPdfFactory(z1mass_rrv,z2mass_rrv,costheta1_rrv,costheta2_rrv,phi_rrv,mzz_rrv);
  SMHiggs->makeSMHiggs();
  SMHiggs->makeParamsConst(true);
  AngularPdfFactory *PSHiggs = new AngularPdfFactory(z1mass_rrv,z2mass_rrv,costheta1_rrv,costheta2_rrv,phi_rrv,mzz_rrv);
  PSHiggs->makePSHiggs();
  PSHiggs->makeParamsConst(true);

  double LD=1/(1+PSHiggs->getVal(mzz_rrv->getVal())/SMHiggs->getVal(mzz_rrv->getVal()));

  delete z1mass_rrv;
  delete z2mass_rrv;
  delete costheta1_rrv;
  delete costheta2_rrv;
  delete phi_rrv;
  delete mzz_rrv;

  delete SMHiggs;
  delete PSHiggs;

  return LD;

}
//=======================================================================
vector<TH1F*> sepLDDistributionSignal0minus(){
  TChain* tree = new TChain("angles");
  tree->Add("../datafiles/PSHiggs_125_JHU_v3_wResolution_withDiscriminants.root");
  tree->Add("../datafiles/PSHiggs_120_JHU_wResolution_withDiscriminants.root");
  tree->Add("../datafiles/PSHiggs_130_JHU_wResolution_withDiscriminants.root");
  tree->Add("../datafiles/PSHiggs_140_JHU_wResolution_withDiscriminants.root");

  vector<TH1F*> LDhist;
  for (int i=1; i<36; i++){
    std::string s;
    std::stringstream out;
    out << i;
    s = out.str();
    TString name = "h_sepLDsignal0minus_"+s;
    LDhist.push_back((new TH1F(name,name,100,0,1)));
  }

  double mzz, m1, m2, h1, h2, phi;
  tree->SetBranchAddress("zzmass",&mzz);
  tree->SetBranchAddress("z2mass",&m2);
  tree->SetBranchAddress("z1mass",&m1);
  tree->SetBranchAddress("phi",&phi);
  tree->SetBranchAddress("costheta1",&h1);
  tree->SetBranchAddress("costheta2",&h2);

  for(int iEvt=0; iEvt<tree->GetEntries(); iEvt++){
    if(iEvt%10000==0) cout << iEvt << endl;
    tree->GetEntry(iEvt);
    LDhist[(int)floor((mzz-110.0)/2.0)]->Fill( separationLikelihoodDiscriminant(mzz,m1,m2,h1,h2,phi) );
  }
  for(int iHist=0; iHist<35; iHist++){
    if(LDhist[iHist]->Integral()>0)
      LDhist[iHist]->Scale(1/LDhist[iHist]->Integral());
  }

  return LDhist;

}
//=======================================================================
vector<TH1F*> sepLDDistributionSignal0plus(){
  TChain* tree = new TChain("angles");
  tree->Add("../datafiles/SMHiggs_125_JHU_v3_wResolution_withDiscriminants.root");
  tree->Add("../datafiles/SMHiggs_120_JHU_wResolution_withDiscriminants.root");
  tree->Add("../datafiles/SMHiggs_130_JHU_wResolution_withDiscriminants.root");
  tree->Add("../datafiles/SMHiggs_140_JHU_wResolution_withDiscriminants.root");


  vector<TH1F*> LDhist;
  for (int i=1; i<36; i++){
    std::string s;
    std::stringstream out;
    out << i;
    s = out.str();
    TString name = "h_sepLDsignal0plus_"+s;
    LDhist.push_back((new TH1F(name,name,100,0,1)));
  }

  double mzz, m1, m2, h1, h2, phi;
  tree->SetBranchAddress("zzmass",&mzz);
  tree->SetBranchAddress("z2mass",&m2);
  tree->SetBranchAddress("z1mass",&m1);
  tree->SetBranchAddress("phi",&phi);
  tree->SetBranchAddress("costheta1",&h1);
  tree->SetBranchAddress("costheta2",&h2);

  for(int iEvt=0; iEvt<tree->GetEntries(); iEvt++){
    if(iEvt%10000==0) cout << iEvt << endl;
    tree->GetEntry(iEvt);
    LDhist[(int)floor((mzz-110.0)/2.0)]->Fill( separationLikelihoodDiscriminant(mzz,m1,m2,h1,h2,phi) );
  }
  for(int iHist=0; iHist<35; iHist++){
    if(LDhist[iHist]->Integral()>0)
      LDhist[iHist]->Scale(1/LDhist[iHist]->Integral());
  }

  return LDhist;

}

//=======================================================================
vector<TH1F*> sepLDDistributionBackground(){

  TChain* tree = new TChain("angles");
  tree->Add("../datafiles/EWKZZ4l_Powheg_total_v5_wResolution_withDiscriminants.root");
  tree->Add("../datafiles/EWKZZ4l_Powheg_total_v6_wResolution_withDiscriminants.root");
  tree->Add("../datafiles/EWKZZ4l_Powheg_total_v7_wResolution_withDiscriminants.root");
  tree->Add("../datafiles/EWKZZ4l_Powheg_total_v8_wResolution_withDiscriminants.root");

  vector<TH1F*> LDhist;
  for (int i=1; i<36; i++){
    std::string s;
    std::stringstream out;
    out << i;
    s = out.str();
    TString name = "h_sepLDbackground_"+s;
    LDhist.push_back((new TH1F(name,name,100,0,1)));
  }

  double mzz, m1, m2, h1, h2, phi;
  tree->SetBranchAddress("zzmass",&mzz);
  tree->SetBranchAddress("z2mass",&m2);
  tree->SetBranchAddress("z1mass",&m1);
  tree->SetBranchAddress("phi",&phi);
  tree->SetBranchAddress("costheta1",&h1);
  tree->SetBranchAddress("costheta2",&h2);

  for(int iEvt=0; iEvt<tree->GetEntries(); iEvt++){
    if(iEvt%10000==0) cout << iEvt << endl;
    tree->GetEntry(iEvt);
    LDhist[(int)floor((mzz-110.0)/2.0)]->Fill( separationLikelihoodDiscriminant(mzz,m1,m2,h1,h2,phi) );
  }
  for(int iHist=0; iHist<35; iHist++){
    if(LDhist[iHist]->Integral()>0)
      LDhist[iHist]->Scale(1/LDhist[iHist]->Integral());
  }

  return LDhist;

}

//=======================================================================
void addDtoTree(char* inputFile){

  char inputFileName[100];
  char outputFileName[100];
  sprintf(inputFileName,"%s.root",inputFile);
  sprintf(outputFileName,"%s_withDiscriminants_withCuts.root",inputFile);

  TFile* sigFile = new TFile(inputFileName);
  TTree* sigTree;
    if(sigFile)
        sigTree = (TTree*) sigFile->Get("angles");
    if(!sigTree)
        return;

  TFile* newFile = new TFile(outputFileName,"RECREATE");
  TTree* newTree = new TTree("newTree","angles"); ///??????

  double m1,m2,mzz,h1,h2,hs,phi,phi1,D,sigP,bkgP,sepLD;
  double l1p_pt,l1m_pt,l2p_pt,l2m_pt,l1p_eta,l1m_eta,l2p_eta,l2m_eta;
  double tmva_likelihood, tmva_bdt,tmva_likelihood_sigSep, tmva_bdt_sigSep;
  sigTree->SetBranchAddress("z1mass",&m1);
  sigTree->SetBranchAddress("z2mass",&m2);
  sigTree->SetBranchAddress("zzmass",&mzz);
  sigTree->SetBranchAddress("costheta1",&h1); 
  sigTree->SetBranchAddress("costheta2",&h2);
  sigTree->SetBranchAddress("costhetastar",&hs);
  sigTree->SetBranchAddress("phi",&phi);  
  sigTree->SetBranchAddress("phistar1",&phi1);
  sigTree->SetBranchAddress("l1p_pT",&l1p_pt);  
  sigTree->SetBranchAddress("l1m_pT",&l1m_pt);  
  sigTree->SetBranchAddress("l2p_pT",&l2p_pt);  
  sigTree->SetBranchAddress("l2m_pT",&l2m_pt);  
  sigTree->SetBranchAddress("l1p_eta",&l1p_eta);  
  sigTree->SetBranchAddress("l1m_eta",&l1m_eta);  
  sigTree->SetBranchAddress("l2p_eta",&l2p_eta);  
  sigTree->SetBranchAddress("l2m_eta",&l2m_eta);  

  newTree->Branch("z1mass",&m1,"z1mass/D");
  newTree->Branch("z2mass",&m2,"z2mass/D");
  newTree->Branch("zzmass",&mzz,"zzmass/D");
  newTree->Branch("costheta1",&h1,"costheta1/D"); 
  newTree->Branch("costheta2",&h2,"costheta2/D");
  newTree->Branch("costhetastar",&hs,"costhetastar/D");
  newTree->Branch("phi",&phi,"phi/D");  
  newTree->Branch("phistar1",&phi1,"phistar1/D");
  newTree->Branch("l1p_pT",&l1p_pt,"l1p_pT/D");  
  newTree->Branch("l1m_pT",&l1m_pt,"l1m_pT/D");
  newTree->Branch("l2p_pT",&l2p_pt,"l2p_pT/D");  
  newTree->Branch("l2m_pT",&l2m_pt,"l2m_pT/D");  
  newTree->Branch("l1p_eta",&l1p_eta,"l1p_eta/D");  
  newTree->Branch("l1m_eta",&l1m_eta,"l1m_eta/D");
  newTree->Branch("l2p_eta",&l2p_eta,"l2p_eta/D");  
  newTree->Branch("l2m_eta",&l2m_eta,"l2m_eta/D");  
  newTree->Branch("nasaLD",&D,"nasaLD/D");  
  newTree->Branch("nasaSigProb",&sigP,"nasaSigProb/D"); 
  newTree->Branch("nasaBkgProb",&bkgP,"nasaBkgProb/D");  
  newTree->Branch("tmva_likelihood",&tmva_likelihood,"tmva_likelihood/D");  
  newTree->Branch("tmva_bdt",&tmva_bdt,"tmva_bdt/D");  
  newTree->Branch("tmva_bdt_sigSep",&tmva_bdt_sigSep,"tmva_bdt_sigSep/D");
  newTree->Branch("tmva_likelihood_sigSep",&tmva_likelihood_sigSep,"tmva_likelihood_sigSep/D");  newTree->Branch("sigSepLD",&sepLD,"sigSepLD/D");    

  ///// Setting up TMVA
  //gSystem->Load("/home/ntran/ROOTdistributions/myROOT528_fromSource/root/lib/libTMVA.so");
  TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );    
  TMVA::Reader *reader_sigSep = new TMVA::Reader( "!Color:!Silent" );    
  float r_z1mass, r_z2mass, r_costheta1, r_costheta2, r_costhetastar, r_phi, r_phistar1, r_zzmass;
  reader->AddVariable( "z1mass", &r_z1mass );
  reader->AddVariable( "z2mass", &r_z2mass );
  reader->AddVariable( "costheta1", &r_costheta1 );
  reader->AddVariable( "costheta2", &r_costheta2 );
  reader->AddVariable( "phi", &r_phi );
  reader->AddVariable( "costhetastar", &r_costhetastar );
  reader->AddVariable( "phistar1", &r_phistar1 );
  reader->AddSpectator( "zzmass",   &r_zzmass );
  reader->BookMVA( "Likelihood method", "../datafiles/TMVA_zz4l_m125_Likelihood.weights_SigBkg.xml" ); 
  reader->BookMVA( "BDT method", "../datafiles/TMVA_zz4l_m125_BDT.weights_SigBkg.xml" ); 

  reader_sigSep->AddVariable( "z1mass", &r_z1mass );
  reader_sigSep->AddVariable( "z2mass", &r_z2mass );
  reader_sigSep->AddVariable( "costheta1", &r_costheta1 );
  reader_sigSep->AddVariable( "costheta2", &r_costheta2 );
  reader_sigSep->AddVariable( "phi", &r_phi );
  reader_sigSep->AddVariable( "costhetastar", &r_costhetastar );
  reader_sigSep->AddVariable( "phistar1", &r_phistar1 );
  reader_sigSep->AddSpectator( "zzmass",   &r_zzmass );
  reader_sigSep->BookMVA( "Likelihood method", "../datafiles/TMVA_zz4l_m125_Likelihood.weights_SMvsPS.xml" ); 
  reader_sigSep->BookMVA( "BDT method", "../datafiles/TMVA_zz4l_m125_BDT.weights_SMvsPS.xml" ); 

  vector<double> pT;
  
  for(int iEvt=0; iEvt<sigTree->GetEntries(); iEvt++){

    if(iEvt%5000==0) cout << "event: " << iEvt << endl;

    sigTree->GetEntry(iEvt);

    pT.push_back(l1p_pt);
    pT.push_back(l1m_pt);
    pT.push_back(l2p_pt);
    pT.push_back(l2m_pt);

    sort(pT.begin(),pT.end());

    if(mzz>110. && mzz<180. && m2>20. 
       && m1>50 && m2<120 && m1<120 &&
       pT[3]>20. && pT[2]>10. && pT[1]>7. && pT[0]>7. &&
       fabs(l1m_eta)<2.4 && fabs(l2m_eta)<2.4 && 
       fabs(l1p_eta)<2.4 && fabs(l2p_eta)<2.4 
       ){

      //NASA LD
      pair<double,double> P = likelihoodDiscriminant(mzz, m1, m2, hs, h1, h2, phi, phi1);

      sigP=P.first;
      bkgP=P.second;
      D=P.first/(P.first+P.second);
      sepLD=separationLikelihoodDiscriminant(mzz,m1,m2,h1,h2,phi);

      //TMVA LD
      r_z1mass = (float) m1;
      r_z2mass = (float) m2;
      r_costheta1 = (float) h1;
      r_costheta2 = (float) h2;
      r_phi = (float) phi;
      r_costhetastar = (float) hs;
      r_phistar1 = (float) phi1;
      tmva_likelihood = reader->EvaluateMVA( "Likelihood method" );
      tmva_bdt = reader->EvaluateMVA( "BDT method" );
      tmva_likelihood_sigSep = reader_sigSep->EvaluateMVA( "Likelihood method" );
      tmva_bdt_sigSep = reader_sigSep->EvaluateMVA( "BDT method" );

      newTree->Fill();

    }

      pT.clear();

  }

  newFile->cd();
  newTree->Write("angles"); ///???????
  newFile->Close();

}

//========================================================================

TGraph* plotSingleROCcurve(TString sigFileName,TString bkgFileName,char* var){

  cout << "plotting ROC curve " << endl;

  TFile* file1 = new TFile(sigFileName);
  TTree* tree1 = (TTree*) file1->Get("angles");
  TH1F* h_LD1 = new TH1F("h_LD1","h_LD1",200,-1.2,1.2);
  
  TFile* file2 = new TFile(bkgFileName);
  TTree* tree2 = (TTree*) file2->Get("angles");
  TH1F* h_LD2 = new TH1F("h_LD2","h_LD2",200,-1.2,1.2);

  double sigEff[200],bkgEff[200];

  double LD,mzz,m2;
  tree1->SetBranchAddress(var,&LD);  
  tree1->SetBranchAddress("zzmass",&mzz);  
  tree1->SetBranchAddress("z2mass",&m2);  
  tree2->SetBranchAddress(var,&LD);
  tree2->SetBranchAddress("zzmass",&mzz);  
  tree2->SetBranchAddress("z2mass",&m2);  

  //Get Histos for LD
  for(int iEvt=0; iEvt<tree1->GetEntries(); iEvt++){
    tree1->GetEntry(iEvt);
    if(mzz>124 && mzz<126 && m2>20)
      h_LD1->Fill(LD);
  }
  for(int iEvt=0; iEvt<tree2->GetEntries(); iEvt++){
    tree2->GetEntry(iEvt);
    if(mzz>124 && mzz<126 && m2>20)
      h_LD2->Fill(LD);
  }

  h_LD1->Scale(1/h_LD1->Integral());
  h_LD2->Scale(1/h_LD2->Integral());

  //loop over cut values
  for(int iCut=0; iCut<200; iCut++){
    sigEff[iCut]=h_LD1->Integral(iCut+1,200);
    bkgEff[iCut]=1-h_LD2->Integral(iCut+1,200);
  }

  TCanvas* ROCcanvas = new TCanvas("ROCcanvas","ROC curve for LD",600,600);
  double lineX[200],lineY[200];
  for(int i=0; i<200; i++){
    lineX[i]=(double)i/200;
    lineY[i]=1.-(double)i/200;
  }
  TGraph* line = new TGraph(200,lineX,lineY);
  line->SetLineColor(2);
  TGraph* ROC = new TGraph(200,sigEff,bkgEff);
  ROC->GetXaxis()->SetTitle("#epsilon_{signal}");
  ROC->GetYaxis()->SetTitle("1-#epsilon_{background}");
  ROC->Draw("AL");
  line->Draw("SAME");
  
  
  //delete file1; 
  delete tree1; 
  delete h_LD1;
  
  //delete file2; 
  delete tree2; 
  delete h_LD2; 


  return ROC;

}

//========================================================================

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
    vh_LDbackground[i-1]->Draw();
    vh_LDsignal[i-1]->Draw("same");
 
    leg->Draw("same");

    vc_LD[i-1]->Print(nameC+".eps");
  }
}

void storeSepLDDistribution(int dataType){
  //dataType: 0 background
  //dataType: 1 0+
  //dataType:-1 0-

  vector<TH1F*> vh_LD;
  TFile *file;
  if(dataType==0){
    vh_LD = sepLDDistributionBackground();
    file = new TFile("sepDbackground.root","recreate");
  }else if(dataType==1){
    vh_LD = sepLDDistributionSignal0plus();
    file = new TFile("sepDsignal0plus.root","recreate");
  }else if(dataType==-1){
    vh_LD = sepLDDistributionSignal0minus();
    file = new TFile("sepDsignal0minus.root","recreate");
  }else{
    cout << "wrong identifier (dataType: 0 background dataType: 1 J^P=0+ dataType:-1 J^P=)0- " << endl;
    return;
  }

  TH2F* h_mzzD = new TH2F("h_mzzD","h_mzzD",35,110,180,vh_LD[0]->GetNbinsX(),vh_LD[0]->GetXaxis()->GetXmin(),vh_LD[0]->GetXaxis()->GetXmax());

  for (int i=1; i<36; i++){
    for(int j=1; j<=vh_LD[0]->GetNbinsX(); j++){
      //cout << vh_LD[i-1]->GetBinContent(j) << endl;
      h_mzzD->SetBinContent(i,j,vh_LD[i-1]->GetBinContent(j));
    }
  }

  file->cd();
  h_mzzD->Write();
  file->Close();

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
    if(vh_LD[i-1]->Integral()>0)
      vh_LD[i-1]->Scale(1./vh_LD[i-1]->Integral());
  }

  TH2F* h_mzzD = new TH2F("h_mzzD","h_mzzD",35,110,180,vh_LD[0]->GetNbinsX(),vh_LD[0]->GetXaxis()->GetXmin(),vh_LD[0]->GetXaxis()->GetXmax());

  for (int i=1; i<36; i++){
    for(int j=1; j<=vh_LD[0]->GetNbinsX(); j++){
      cout << vh_LD[i-1]->GetBinContent(j) << endl;
      h_mzzD->SetBinContent(i,j,vh_LD[i-1]->GetBinContent(j));
    }
  }
  file->cd();
  h_mzzD->Write();
}


//=======================================================================

void plotROCcurve(){ //only in mZZ 124-126

  cout << "plotting ROC curve " << endl;
  TString nomeSig="../datafiles/JHUGenFiles/SMHiggs_125_JHU_wResolution_withDiscriminants_withCuts.root";
  TString nomeBkg="../datafiles/PowhegFiles/EWKZZ4l_Powheg_total_v2_wResolution_withDiscriminants_withCuts.root";
  TFile* file_sig = new TFile(nomeSig,"read");
  TTree* tree_sig = (TTree*) file_sig->Get("angles");
  TH1F* h_LD_sig = new TH1F("h_LD_sig","h_LD_sig",101,0,1.01);
  TH1F *h_costhetastar_sig = new TH1F("h_costhetastar_sig","h_costhetastar_sig",50,-1,1);
  TH1F *h_costheta1_sig = new TH1F("h_costheta1_sig","h_costheta1_sig",50,-1,1);
  TH1F *h_costheta2_sig = new TH1F("h_costheta2_sig","h_costheta2_sig",50,-1,1);
  TH1F *h_phi_sig = new TH1F("h_phi_sig","h_phi_sig",50,-3.14,3.14);
  TH1F *h_phi1_sig = new TH1F("h_phi1_sig","h_phi1_sig",50,-3.14,3.14);
  TH1F* h_m2_sig = new TH1F("h_m2_sig","h_m2_sig",44,20,64);
 
  TFile* file_bkg = new TFile(nomeBkg,"read");
  TTree* tree_bkg = (TTree*) file_bkg->Get("angles");
  TH1F* h_LD_bkg = new TH1F("h_LD_bkg","h_LD_bkg",101,0,1.01);
  TH1F *h_costhetastar_bkg = new TH1F("h_costhetastar_bkg","h_costhetastar_bkg",50,-1,1);
  TH1F *h_costheta1_bkg = new TH1F("h_costheta1_bkg","h_costheta1_bkg",50,-1,1);
  TH1F *h_costheta2_bkg = new TH1F("h_costheta2_bkg","h_costheta2_bkg",50,-1,1);
  TH1F *h_phi_bkg = new TH1F("h_phi_bkg","h_phi_bkg",50,-3.14,3.14);
  TH1F *h_phi1_bkg = new TH1F("h_phi1_bkg","h_phi1_bkg",50,-3.14,3.14);
  TH1F* h_m2_bkg = new TH1F("h_m2_bkg","h_m2_bkg",44,20,64);

  double sigEff_LD[100],bkgEff_LD[100];
  double sigEff_costhetastar[25],bkgEff_costhetastar[25];
  double sigEff_costheta1[25],bkgEff_costheta1[25];
  double sigEff_costheta2[25],bkgEff_costheta2[25];
  double sigEff_phi[25],bkgEff_phi[25];
  double sigEff_m2[44],bkgEff_m2[44];

  double LD, costhetastar, costheta1, costheta2, phi, phi1,mZZ,m2;
  tree_sig->SetBranchAddress("nasaLD",&LD);  
  tree_sig->SetBranchAddress("costhetastar",&costhetastar);
  tree_sig->SetBranchAddress("phi",&phi);
  tree_sig->SetBranchAddress("costheta1",&costheta1);
  tree_sig->SetBranchAddress("costheta2",&costheta2);
  tree_sig->SetBranchAddress("phistar1",&phi1);
  tree_sig->SetBranchAddress("zzmass",&mZZ);
  tree_sig->SetBranchAddress("z2mass",&m2);

  tree_bkg->SetBranchAddress("nasaLD",&LD);
  tree_bkg->SetBranchAddress("costhetastar",&costhetastar);
  tree_bkg->SetBranchAddress("phi",&phi);
  tree_bkg->SetBranchAddress("costheta1",&costheta1);
  tree_bkg->SetBranchAddress("costheta2",&costheta2);
  tree_bkg->SetBranchAddress("phistar1",&phi1);
  tree_bkg->SetBranchAddress("zzmass",&mZZ);
  tree_bkg->SetBranchAddress("z2mass",&m2);
  cout<<"prova 0"<<endl;

  //Get Histos for LD
  for(int iEvt=0; iEvt<tree_sig->GetEntries(); iEvt++){
    tree_sig->GetEntry(iEvt);
    if(mZZ>126 || mZZ<124)
      continue;
    h_LD_sig->Fill(LD);
    h_costhetastar_sig->Fill(costhetastar);
    h_costheta1_sig->Fill(costheta1);
    h_costheta2_sig->Fill(costheta2);
    h_phi1_sig->Fill(phi1);
    h_phi_sig->Fill(phi);
    h_m2_sig->Fill(m2);
   }
  for(int iEvt=0; iEvt<tree_bkg->GetEntries(); iEvt++){
    tree_bkg->GetEntry(iEvt);
     if(mZZ>126 || mZZ<124)
      continue;
     h_LD_bkg->Fill(LD);
    h_costhetastar_bkg->Fill(costhetastar);
    h_costheta1_bkg->Fill(costheta1);
    h_costheta2_bkg->Fill(costheta2);
    h_phi1_bkg->Fill(phi1);
    h_phi_bkg->Fill(phi);
    h_m2_bkg->Fill(m2);
  }
  h_LD_sig->Scale(1/h_LD_sig->Integral());
  h_costhetastar_sig->Scale(1/h_costhetastar_sig->Integral());
  h_costheta1_sig->Scale(1/h_costheta1_sig->Integral());
  h_costheta2_sig->Scale(1/h_costheta2_sig->Integral());
  h_phi_sig->Scale(1/h_phi_sig->Integral());
  h_phi1_sig->Scale(1/h_phi1_sig->Integral());
  h_m2_sig->Scale(1/h_m2_sig->Integral());
  h_LD_bkg->Scale(1/h_LD_bkg->Integral());
  h_costhetastar_bkg->Scale(1/h_costhetastar_bkg->Integral());
  h_costheta1_bkg->Scale(1/h_costheta1_bkg->Integral());
  h_costheta2_bkg->Scale(1/h_costheta2_bkg->Integral());
  h_phi_bkg->Scale(1/h_phi_bkg->Integral());
  h_phi1_bkg->Scale(1/h_phi1_bkg->Integral());
  h_m2_bkg->Scale(1/h_m2_bkg->Integral());

  //loop over cut values
  for(int iCut=0; iCut<100; iCut++){
    sigEff_LD[iCut]=h_LD_sig->Integral(iCut,100);
    bkgEff_LD[iCut]=1-h_LD_bkg->Integral(iCut,100);
   cout<<sigEff_LD[iCut]<<" "<<bkgEff_LD[iCut]<<endl;
   }
  for(int iCut=0; iCut<25; iCut++){
    sigEff_costhetastar[iCut]=h_costhetastar_sig->Integral(0+iCut,50-iCut);
    bkgEff_costhetastar[iCut]=1-h_costhetastar_bkg->Integral(0+iCut,50-iCut);
  }
  for(int iCut=0; iCut<25; iCut++){
    sigEff_costheta1[iCut]=h_costheta1_sig->Integral(0+iCut,50-iCut);
    bkgEff_costheta1[iCut]=1-h_costheta1_bkg->Integral(0+iCut,50-iCut);
  }
  for(int iCut=0; iCut<25; iCut++){
    sigEff_costheta2[iCut]=h_costheta2_sig->Integral(0+iCut,50-iCut);
    bkgEff_costheta2[iCut]=1-h_costheta2_bkg->Integral(0+iCut,50-iCut);
  }
  for(int iCut=0; iCut<25; iCut++){
    sigEff_phi[iCut]=h_phi_sig->Integral(25-iCut,25+iCut);
    bkgEff_phi[iCut]=1-h_phi_bkg->Integral(25-iCut,25+iCut);
  }
  for(int iCut=0; iCut<44; iCut++){
    sigEff_m2[iCut]=h_m2_sig->Integral(iCut,44);
    bkgEff_m2[iCut]=1-h_m2_bkg->Integral(iCut,44);
  }

  cout<<"prova 1"<<endl;
  /*double lineX[100],lineY[100];
  for(int i=0; i<100; i++){
    lineX[i]=(double)i/100;
    lineY[i]=1.-(double)i/100;
  }
  TGraph* line = new TGraph(100,lineX,lineY);
  line->SetLineColor(2);*/
  TF1* f= new TF1("f","1-x",0,1.2);
  f->SetLineColor(2);
  /*TGraph* ROC_LD = new TGraph(100,sigEff_LD,bkgEff_LD);
  ROC_LD->GetXaxis()->SetTitle("#epsilon_{signal}");
  ROC_LD->GetYaxis()->SetTitle("#epsilon_{background}");
  ROC_LD->SetMarkerStyle(8);
  ROC_LD->Draw("APL");*/
  TGraph* ROC_Andrew = plotSingleROCcurve(nomeSig,nomeBkg,"nasaLD");
  ROC_Andrew->SetMarkerStyle(2);
  ROC_Andrew->SetMarkerColor(kGreen);
  TCanvas* ROCcanvas = new TCanvas("ROC_c","ROC curve for LD",600,600);
  ROC_Andrew->Draw("APL");
  cout<<"prova 2"<<endl;
  TGraph* ROC_costhetastar = new TGraph(25,sigEff_costhetastar,bkgEff_costhetastar);
  ROC_costhetastar->SetMarkerStyle(24);
  ROC_costhetastar->SetMarkerColor(2);
  ROC_costhetastar->Draw("PLsame");
  TGraph* ROC_costheta1 = new TGraph(25,sigEff_costheta1,bkgEff_costheta1);
  ROC_costheta1->SetMarkerStyle(24);
  ROC_costheta1->SetMarkerColor(4);
  ROC_costheta1->Draw("PLsame");
  TGraph* ROC_costheta2 = new TGraph(25,sigEff_costheta2,bkgEff_costheta2);
  ROC_costheta2->SetMarkerStyle(24);
  ROC_costheta2->SetMarkerColor(kGreen+2);
  ROC_costheta2->Draw("PLsame");
  TGraph* ROC_phi = new TGraph(25,sigEff_phi,bkgEff_phi);
  ROC_phi->SetMarkerStyle(29);
  ROC_phi->SetMarkerColor(kMagenta);
  ROC_phi->Draw("PLsame");
  TGraph* ROC_m2 = new TGraph(44,sigEff_m2,bkgEff_m2);
  ROC_m2->SetMarkerStyle(34);
  ROC_m2->SetMarkerColor(kOrange);
  ROC_m2->Draw("PLsame");


  f->Draw("same");
  
  TLegend *leg = new TLegend(0.5,0.6,0.7,0.8);
  //leg->AddEntry(ROC_LD,"NASA LD","LP");
  leg->AddEntry(ROC_Andrew,"MELA LD","LP");
  leg->AddEntry(ROC_costhetastar,"|costhetastar|<cut(1-0)","LP");
  leg->AddEntry(ROC_costheta1,"|costheta1|<cut(1-0)","LP");
  leg->AddEntry(ROC_costheta2,"|costheta2|<cut(1-0)","LP");
  leg->AddEntry(ROC_phi,"|phi|>cut(0-3.14)","LP");
  leg->AddEntry(ROC_m2,"m2>cut(20-64)","LP");

  leg->Draw("same");
  
  ROCcanvas->Print("allROCcanvas.eps");

}

