#include <iostream>
#include <vector>
#include "TLorentzVector.h"
#include "TChain.h"
#include "TLorentzRotation.h"
#include "TH1F.h"
#include "TLegend.h"

vector<TLorentzVector> Calculate4Momentum(float Mx,float M1,float M2,float theta,float theta1,float theta2,float Phi1,float Phi);

void calculatePtEta(){

  //gROOT->ProcessLine(".L ~/tdrstyle.C");
  //setTDRStyle();
  //gStyle->SetOptStat(1);

  TChain* t = new TChain("SelectedTree");
  t->Add("/export/d1/scratch/snowmass2013/ilcsamples/signal/spin0/Events_20130618/unweighted_unpol_g1_1M_false.root");

  float costhetastar=0,costheta1,costheta2,phi,phi1=0;
  float l1pt,l2pt;
  float l1eta,l2eta;
  float ptlplus,ptlminus;
  float etalplus,etalminus;
  float ZMass,HMass;

  float s = 250.;
  float mH = 125.;
  float mZ = 91.18;

  //t->SetBranchAddress("costhetastar",&costhetastar);
  t->SetBranchAddress("costheta1",&costheta1);
  t->SetBranchAddress("costheta2",&costheta2);
  //t->SetBranchAddress("phistar1",&phi1);
  t->SetBranchAddress("phi",&phi);
  t->SetBranchAddress("ptlplus",&ptlplus);
  t->SetBranchAddress("ptlminus",&ptlminus);
  t->SetBranchAddress("etalplus",&etalplus);
  t->SetBranchAddress("etalminus",&etalminus);
  //t->SetBranchAddress("ZMass",&mZ);
  //t->SetBranchAddress("HMass",&mH);
 
  TH1F* difflep1 = new TH1F("difflep1",";#Delta p_{T}/p_{T};",100,-1,1);
  //TH1F* difflep1 = new TH1F("difflep1",";#Delta #eta/#eta;",100,-1,1);
  TH1F* difflep2 = new TH1F("difflep2",";#Delta p_{T}/p_{T};",100,-1,1);
  //TH1F* difflep2 = new TH1F("difflep2",";#Delta #eta/#eta;",100,-1,1);
  difflep1->SetLineWidth(2);
  difflep1->SetLineColor(2);

  for(int i = 0 ; i<t->GetEntries(); i++){
   
    t->GetEntry(i);

    vector<TLorentzVector> p = Calculate4Momentum(s, mZ, mH, acos(costheta1), acos(costheta2), acos(costhetastar), phi, phi1);

    difflep1->Fill((p[0].Pt()-ptlminus)/ptlminus);
    //difflep1->Fill((lepM4vec.Eta()-etalminus)/etalminus);
    difflep2->Fill((p[1].Pt()-ptlplus)/ptlminus);
    //difflep2->Fill((lepP4vec.Eta()-etalplus)/etalminus);

  }

  difflep1->Draw();
  difflep2->Draw("SAME");

  TLegend* leg = new TLegend(.3,.7,.5,.9);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  
  leg->AddEntry(difflep1,"l^{-}","l");
  leg->AddEntry(difflep2,"l^{+}","l");

  leg->Draw();
}


vector<TLorentzVector> Calculate4Momentum(float Mx,float M1,float M2,float theta,float theta1,float theta2,float Phi1,float Phi)
{

  float phi1,phi2;
  phi1=TMath::Pi()-Phi1;
  phi2=Phi1+Phi;
    
    
  float gamma1,gamma2,beta1,beta2;
    
  gamma1=(Mx*Mx+M1*M1-M2*M2)/(2*Mx*M1);
  gamma2=(Mx*Mx-M1*M1+M2*M2)/(2*Mx*M2);
  beta1=sqrt(1-1/(gamma1*gamma1));
  beta2=sqrt(1-1/(gamma2*gamma2));
    
  //gluon 4 vectors
  TLorentzVector p1CM(0,0,Mx/2,Mx/2);
  TLorentzVector p2CM(0,0,-Mx/2,Mx/2);
    
  //vector boson 4 vectors
  TLorentzVector kZ1(gamma1*M1*sin(theta)*beta1,0, gamma1*M1*cos(theta)*beta1,gamma1*M1*1);   
  TLorentzVector kZ2(-gamma2*M2*sin(theta)*beta2,0, -gamma2*M2*cos(theta)*beta2,gamma2*M2*1);
    
  //Rotation and Boost matrices. Note gamma1*beta1*M1=gamma2*beta2*M2.
    
  TLorentzRotation Z1ToZ,Z2ToZ;
    
  Z1ToZ.Boost(0,0,beta1);
  Z2ToZ.Boost(0,0,beta2);
  Z1ToZ.RotateY(theta);
  Z2ToZ.RotateY(TMath::Pi()+theta);

  //fermons 4 vectors in vector boson rest frame
    
  TLorentzVector p3Z1((M1/2)*sin(theta1)*cos(phi1),(M1/2)*sin(theta1)*sin(phi1),(M1/2)*cos(theta1),(M1/2)*1);
       
  TLorentzVector p4Z1(-(M1/2)*sin(theta1)*cos(phi1),-(M1/2)*sin(theta1)*sin(phi1),-(M1/2)*cos(theta1),(M1/2)*1);
      
  TLorentzVector p5Z2((M2/2)*sin(theta2)*cos(phi2),(M2/2)*sin(theta2)*sin(phi2),(M2/2)*cos(theta2),(M2/2)*1);
    
  TLorentzVector p6Z2(-(M2/2)*sin(theta2)*cos(phi2),-(M2/2)*sin(theta2)*sin(phi2),-(M2/2)*cos(theta2),(M2/2)*1);
      

  // fermions 4 vectors in CM frame
    
  TLorentzVector p3CM,p4CM,p5CM,p6CM;
    
  p3CM=Z1ToZ*p3Z1;
  p4CM=Z1ToZ*p4Z1;
  p5CM=Z2ToZ*p5Z2;
  p6CM=Z2ToZ*p6Z2;

  vector<TLorentzVector> p;
    
  p.push_back(p3CM);
  p.push_back(p4CM);
  p.push_back(p5CM);
  p.push_back(p6CM);

  return p;
}
