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

using namespace RooFit ;

void build8DTemplate(){
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
    cout<<mZZ<<" "<<m1<<" "<<m2<<" "<<costhetastar<<" "<<costheta1<<" "<<costheta2<<" "<<phi<<" "<<phi1<<endl;
    cout << "my8DTemplate says ... " << endl;
    cout<<"Pmzzm1m2: "<<Pmzzm1m2<<endl;
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

  return make_pair(Psig,Pbackg);
}

TH1F *h_mzz= (TH1F*)(f->Get("h_mzz"));

vector<TH1F*> LDDistributionSignal(){
  TFile* file = new TFile("../datafiles/JHUGenFiles/SMHiggs_125_JHU_withCuts.root");
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
//============================================================
vector<TH1F*> LDDistributionBackground(){
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
  AngularPdfFactory SMHiggs(z1mass_rrv,z2mass_rrv,costheta1_rrv,costheta2_rrv,phi_rrv,mzz_rrv);
  SMHiggs.makeSMHiggs();
  SMHiggs.makeParamsConst(true);
  AngularPdfFactory PSHiggs(z1mass_rrv,z2mass_rrv,costheta1_rrv,costheta2_rrv,phi_rrv,mzz_rrv);
  PSHiggs.makePSHiggs();
  PSHiggs.makeParamsConst(true);

  return 1/(1+SMHiggs.getVal(mzz_rrv->getVal())/PSHiggs.getVal(mzz_rrv->getVal()));

}


//=======================================================================
void addDtoTree(char* inputFile){

  char inputFileName[50];
  char outputFileName[50];
  sprintf(inputFileName,"%s.root",inputFile);
  sprintf(outputFileName,"%s_withDiscriminants.root",inputFile);

  TFile* sigFile = new TFile(inputFileName);
  TTree* sigTree = (TTree*) sigFile->Get("angles");

  TFile* newFile = new TFile(outputFileName,"RECREATE");
  TTree* newTree = new TTree("newTree","angles"); ///??????

  double m1,m2,mzz,h1,h2,hs,phi,phi1,D,sigP,bkgP,sepLD;
  sigTree->SetBranchAddress("z1mass",&m1);
  sigTree->SetBranchAddress("z2mass",&m2);
  sigTree->SetBranchAddress("zzmass",&mzz);
  sigTree->SetBranchAddress("costheta1",&h1); 
  sigTree->SetBranchAddress("costheta2",&h2);
  sigTree->SetBranchAddress("costhetastar",&hs);
  sigTree->SetBranchAddress("phi",&phi);  
  sigTree->SetBranchAddress("phistar1",&phi1);
  sigTree->Branch("nasaLD",&D,"nasaLD/D");    //???????

  newTree->Branch("z1mass",&m1,"z1mass/D");
  newTree->Branch("z2mass",&m2,"z2mass/D");
  newTree->Branch("zzmass",&mzz,"zzmass/D");
  newTree->Branch("costheta1",&h1,"costheta1/D"); 
  newTree->Branch("costheta2",&h2,"costheta2/D");
  newTree->Branch("costhetastar",&hs,"costhetastar/D");
  newTree->Branch("phi",&phi,"phi/D");  
  newTree->Branch("phistar1",&phi1,"phistar1/D");
  newTree->Branch("nasaLD",&D,"nasaLD/D");  
  newTree->Branch("nasaSigProb",&sigP,"nasaSigProb/D"); 
  newTree->Branch("nasaBkgProb",&bkgP,"nasaBkgProb/D");  
  newTree->Branch("sigSepLD",&sepLD,"sigSepLD/D");    

  for(int iEvt=0; iEvt<sigTree->GetEntries(); iEvt++){

    if(iEvt%5000==0) cout << "event: " << iEvt << endl;

    sigTree->GetEntry(iEvt);

    if(mzz>110 && mzz<180 && m2>20){  //???????????????

      pair<double,double> P = likelihoodDiscriminant(mzz, m1, m2, hs, h1, h2, phi, phi1);

      sigP=P.first;
      bkgP=P.second;
      D=P.first/(P.first+P.second);
      sepLD=separationLikelihoodDiscriminant(mzz,m1,m2,h1,h2,phi);
      newTree->Fill();
    }

  }

  newFile->cd();
  newTree->Write("angles"); ///???????
  newFile->Close();

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
    tree->Add("../datafiles/JHUGenFiles/SMHiggs_125_JHU_withCuts.root");
  }

  if(!signal){
  tree->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_1.root");
  tree->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_2.root");
  tree->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_3.root");
  tree->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_4.root");
  tree->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_5.root");
  tree->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_6.root");
  tree->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_7.root");
  tree->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_8.root");
  tree->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_9.root");
  tree->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_10.root");
  tree->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_11.root");
  tree->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_12.root");
  tree->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_13.root");
  tree->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_14.root");
  tree->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_15.root");
  tree->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_16.root");
  tree->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_17.root");
  tree->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_18.root");
  tree->Add("../datafiles/PowhegFiles/EWKZZ4l_Powheg_19.root");
 

  }
  double mZZ, m2, m1, costhetastar, costheta1, costheta2, phi, phi1,sepLD;
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
    fileOut = new TFile("../datafiles/JHUGenFiles/SMHiggs_125_JHU_withCuts_withD.root","RECREATE");
  else if(!signal)
    fileOut = new TFile("../datafiles/PowhegFiles/EWKZZ4l_Powheg_withCuts_withD.root","RECREATE");
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
  treeOut->Branch("sigSepLD",&sepLD,"sigSepLD/D");    

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
    //sepLD=separationLikelihoodDiscriminant(mZZ,m1,m2,costheta1,costheta2,phi);

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

//=======================================================================

void plotROCcurve(){ //only in mZZ 124-126

  cout << "plotting ROC curve " << endl;

  TFile* file_sig = new TFile("../datafiles/JHUGenFiles/SMHiggs_125_JHU_withCuts_withD.root","read");
  TTree* tree_sig = (TTree*) file_sig->Get("eventInfo");
  TH1F* h_LD_sig = new TH1F("h_LD_sig","h_LD_sig",101,0,1.01);
  TH1F *h_costhetastar_sig = new TH1F("h_costhetastar_sig","h_costhetastar_sig",50,-1,1);
  TH1F *h_costheta1_sig = new TH1F("h_costheta1_sig","h_costheta1_sig",50,-1,1);
  TH1F *h_costheta2_sig = new TH1F("h_costheta2_sig","h_costheta2_sig",50,-1,1);
  TH1F *h_phi_sig = new TH1F("h_phi_sig","h_phi_sig",50,-3.14,3.14);
  TH1F *h_phi1_sig = new TH1F("h_phi1_sig","h_phi1_sig",50,-3.14,3.14);
  TH1F* h_m2_sig = new TH1F("h_m2_sig","h_m2_sig",44,20,64);
 
  TFile* file_bkg = new TFile("../datafiles/PowhegFiles/EWKZZ4l_Powheg_withCuts_withD.root","read");
  TTree* tree_bkg = (TTree*) file_bkg->Get("eventInfo");
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
  //tree1->SetBranchAddress("nasaLD",&LD);  
  //tree2->SetBranchAddress("nasaLD",&LD);
  tree_sig->SetBranchAddress("D",&LD);  
  tree_sig->SetBranchAddress("costhetastar",&costhetastar);
  tree_sig->SetBranchAddress("phi",&phi);
  tree_sig->SetBranchAddress("costheta1",&costheta1);
  tree_sig->SetBranchAddress("costheta2",&costheta2);
  tree_sig->SetBranchAddress("phi1",&phi1);
  tree_sig->SetBranchAddress("zzmass",&mZZ);
  tree_sig->SetBranchAddress("z2mass",&m2);

  tree_bkg->SetBranchAddress("D",&LD);
  tree_bkg->SetBranchAddress("costhetastar",&costhetastar);
  tree_bkg->SetBranchAddress("phi",&phi);
  tree_bkg->SetBranchAddress("costheta1",&costheta1);
  tree_bkg->SetBranchAddress("costheta2",&costheta2);
  tree_bkg->SetBranchAddress("phi1",&phi1);
  tree_bkg->SetBranchAddress("zzmass",&mZZ);
  tree_bkg->SetBranchAddress("z2mass",&m2);

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

  TCanvas* ROCcanvas = new TCanvas("ROCcanvas","ROC curve for LD",600,600);
  /*double lineX[100],lineY[100];
  for(int i=0; i<100; i++){
    lineX[i]=(double)i/100;
    lineY[i]=1.-(double)i/100;
  }
  TGraph* line = new TGraph(100,lineX,lineY);
  line->SetLineColor(2);*/
  TF1* f= new TF1("f","1-x",0,1.2);
  f->SetLineColor(2);
  TGraph* ROC_LD = new TGraph(100,sigEff_LD,bkgEff_LD);
  ROC_LD->GetXaxis()->SetTitle("#epsilon_{signal}");
  ROC_LD->GetYaxis()->SetTitle("#epsilon_{background}");
  ROC_LD->SetMarkerStyle(8);
  ROC_LD->Draw("APL");
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
  leg->AddEntry(ROC_LD,"NASA LD","LP");
  leg->AddEntry(ROC_costhetastar,"|costhetastar|<cut(1-0)","LP");
  leg->AddEntry(ROC_costheta1,"|costheta1|<cut(1-0)","LP");
  leg->AddEntry(ROC_costheta2,"|costheta2|<cut(1-0)","LP");
  leg->AddEntry(ROC_phi,"|phi|>cut(0-3.14)","LP");
  leg->AddEntry(ROC_m2,"m2>cut(20-64)","LP");

  leg->Draw("same");
  ROCcanvas->Print("ROCcanvas.eps");


}

