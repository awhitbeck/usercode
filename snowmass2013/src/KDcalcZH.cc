#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "RooRealVar.h"
#include <math.h>
#include "TMath.h"
#include "ScalarPdfFactoryZH.cc"

using namespace std;
using namespace TMath;

class KDcalcZH {

public:

  RooRealVar* costheta1;
  RooRealVar* costheta2;
  RooRealVar* phi;
  
  RooRealVar* mH;
  
  ScalarPdfFactoryZH* SMHiggs;
  ScalarPdfFactoryZH* altSignal;
  ScalarPdfFactoryZH* mixHiggs;
  

  KDcalcZH(double sqrtsVal, double mX){

    costheta1 = new RooRealVar("costheta1","costheta1",-1.,1.);
    costheta2 = new RooRealVar("costheta2","costheta2",-1.,1.);
    phi       = new RooRealVar("phi","phi",-TMath::Pi(),TMath::Pi());
    
    mH        = new RooRealVar("mH","mH",100.,1000.);
    mH->setVal(mX);

    SMHiggs   = new ScalarPdfFactoryZH(costheta1,
				       costheta2,
				       phi,
				       mH,
				       1,
				       false);

    SMHiggs->fa2->setVal(0.0);
    SMHiggs->fa3->setVal(0.0);
    SMHiggs->phia2->setVal(0.0);
    SMHiggs->phia3->setVal(0.0);
    SMHiggs->sqrts->setVal(sqrtsVal);

    altSignal = new ScalarPdfFactoryZH(costheta1,
					  costheta2,
					  phi,
					  mH,
					  1,
					  false);

    altSignal->fa2->setVal(0.0);
    altSignal->fa3->setVal(0.999999999);
    altSignal->g1Val->setVal(0.0);
    altSignal->g4Val->setVal(1.0);
    altSignal->phia2->setVal(0.0);
    altSignal->phia3->setVal(0.0);
    altSignal->sqrts->setVal(sqrtsVal);


    mixHiggs   = new ScalarPdfFactoryZH(costheta1,
				       costheta2,
				       phi,
				       mH,
				       1,
				       false);

    mixHiggs->fa2->setVal(0.0);
    mixHiggs->fa3->setVal(0.5);
    mixHiggs->phia2->setVal(0.0);
    mixHiggs->phia3->setVal(0.0);
    mixHiggs->g1Val->setVal(1.0);
    mixHiggs->g4Val->setVal(0.351949);
    mixHiggs->sqrts->setVal(sqrtsVal);

    SMHiggs->makeParamsConst(true);
    altSignal->makeParamsConst(true);
    mixHiggs->makeParamsConst(true);

  };


  //template <class T> 
  void computeKD(float mH_,
		 float costheta1_,
		 float costheta2_,
		 float phi_,
		 float& KD,
		 float& KDInt){
    
    mH->setVal(mH_);
    costheta1->setVal(costheta1_);
    costheta2->setVal(costheta2_);
    phi->setVal(phi_);

    // calculate 0+ vs 0- KD
    float SMHiggsProb = SMHiggs->PDF->getVal();
    if(SMHiggsProb<0.0) cout << "KDcalcZH::computeKD - ERROR: SMHiggs prob is negative!!!" << endl;
    float altSignalProb = altSignal->PDF->getVal();
    if(altSignalProb<0.0) cout << "KDcalcZH::computeKD - ERROR: altSig prob is negative!!!" << endl;
    
    float c = 0.84;

   if ( SMHiggs->sqrts->getVal() == 350. ) 
      c = 2;

    if ( SMHiggs->sqrts->getVal() == 500. ) 
      c = 0.5;

    if ( SMHiggs->sqrts->getVal() == 1000. ) 
      c = 2;

    KD = SMHiggsProb/(SMHiggsProb+c*altSignalProb);


    // calculate the interference KD
    float mixHiggsProb = mixHiggs->PDF->getVal();
    if(mixHiggsProb<0.0) cout << "KDcalcZH::computeKD - ERROR: mixHiggs prob is negative!!!" << endl;
    
    double intProb = 2 * mixHiggsProb - SMHiggsProb - altSignalProb; 
    KDInt = intProb / (SMHiggsProb + altSignalProb); 
    /*
    std::cout <<"-------From PDF--------------------\n";
    std::cout << "SMHiggsProb = " << SMHiggsProb << "\n";
    std::cout << "altSignalProb = " << altSignalProb << "\n";
    std::cout << "mixHiggsProb = " << mixHiggsProb << "\n";
    std::cout << "intProb = " << intProb << "\n";
    std::cout << "KD = " << KD << "\n";
    std::cout << "KDInt = " << KDInt << "\n";
    */
    double g4mix = 0.351949;

    double SMHiggsProb_func = zeropdfvalue(1., 0., 0, costheta1_, costheta2_, phi_, 0.15, 0.15, mH_); 
    double altSignalProb_func = zeropdfvalue(0., 1., 0, costheta1_, costheta2_, phi_, 0.15, 0.15, mH_); 
    double mixHiggsProb_func = zeropdfvalue(1., g4mix, 0, costheta1_, costheta2_, phi_, 0.15, 0.15, mH_); 
    double intProb_func = mixHiggsProb_func - SMHiggsProb_func - altSignalProb_func * g4mix*g4mix ; 
    double KDInt_func = intProb_func / (SMHiggsProb_func + altSignalProb_func * g4mix*g4mix); 
    /*
    std::cout <<"-------From Function--------------------\n";
    std::cout << "SMHiggsProb_func = " << SMHiggsProb_func << "\n";
    std::cout << "altSignalProb_func = " << altSignalProb_func << "\n";
    std::cout << "mixHiggsProb_func = " << mixHiggsProb_func << "\n";
    std::cout << "intProb_func = " << intProb_func << "\n";
    std::cout << "KDInt_func = " << KDInt_func << "\n";
    */
    KDInt = KDInt_func;

  };




  //template <class T>
  void addDtoTree(TString fileName,
		  TString treeName,
		  TString mHBranch="HMass",
		  TString costheta1Branch="costheta1",
		  TString costheta2Branch="costheta2",
		  TString phiBranch="phi"
		  ){
    
    TFile* f = new TFile(fileName,"READ");
    TTree* t = (TTree*) f->Get(treeName);

    fileName.ReplaceAll(".root","_withKD.root");
    TFile* ff = new TFile(fileName,"RECREATE");
    TTree* tt = (TTree*) t->CloneTree(0,"fast");

    float mH_,costheta1_,costheta2_,phi_;
    float KD_;
    float KDInt_;

    t->SetBranchAddress(mHBranch,&mH_);
    t->SetBranchAddress(costheta1Branch,&costheta1_);
    t->SetBranchAddress(costheta2Branch,&costheta2_);
    t->SetBranchAddress(phiBranch,&phi_);
    
  if ( t->GetBranchStatus("pseudoMELA") )
    tt->SetBranchAddress("pseudoMELA",&KD_);
  else 
    tt->Branch("pseudoMELA",&KD_,"pseudoMELA/F");

  if ( t->GetBranchStatus("pseudoMELAInt") )
    tt->SetBranchAddress("pseudoMELAInt",&KDInt_);
  else 
    tt->Branch("pseudoMELAInt",&KDInt_,"pseudoMELAInt/F");
  
    cout << "t entries: "  << t->GetEntries() << endl;

    for(int iEvt=0; iEvt<t->GetEntries(); iEvt++){

      // if ( iEvt > 10000 ) continue;

      if(iEvt%100000==0)  cout << iEvt << "/" << t->GetEntries() << endl;

      t->GetEntry(iEvt);

      computeKD(mH_,costheta1_,costheta2_,phi_,KD_, KDInt_);

      /*
      cout << "mH: " << mH_ << endl;
      cout << "costheta1: " << costheta1_ << endl;
      cout << "costheta2: " << costheta2_ << endl;
      cout << "phi: " << phi_ << endl;
      cout << "KD: " << KD_ << endl;
      */

      tt->Fill();

    }

    ff->cd();
    tt->Write("SelectedTree");
    f->Close();
    ff->Close();
  };


  double zeropdfvalue(double g1Val, double g4Val, double g4ValIm, double h1, double h2, double Phi, double R1Val, double R2Val, double mH) 
  {
    double mX = mH;
    double sqrts = 250.;
    double mZ =91.188;
    
    double s=-(mX*mX-sqrts*sqrts-mZ*mZ)/2.;
    double kappa=s/(1000*1000);

    double g1ValIm = 1.;
    double a1=0,a2=0,a3=0,a1Im=0,a2Im=0,a3Im=0;
    double g1(1.0), g1Im(0.), g2(0.), g2Im(0.), g3(0.), g3Im(0.), g4(0.), g4Im(0.);
    g1   =  g1Val;
    g1Im =  0.;
    g2   =  0.;
    g2Im =  0.;
    g3   =  0.;
    g3Im =  0.;
    g4   = - g4Val;
    g4Im = - g4ValIm;
    
    a1 = g1*mZ*mZ/(mX*mX) + g2*2.*s/(mX*mX) + g3*kappa*s/(mX*mX);
    a1Im = g1Im*mZ*mZ/(mX*mX) + g2Im*2.*s/(mX*mX) + g3Im*kappa*s/(mX*mX);
    a2 = -2.*g2 - g3*kappa;
    a2Im = -2.*g2Im - g3Im*kappa;
    a3 = -2.*g4;
    a3Im = -2.*g4Im;


   Double_t x = pow((mX*mX-sqrts*sqrts-mZ*mZ)/(2.*sqrts*mZ),2)-1;
   
   Double_t A00Real = - (a1*sqrt(1+x) + a2*(mZ*sqrts)/(mX*mX)*x);
   Double_t A00Imag = - (a1Im*sqrt(1+x) + a2Im*(mZ*sqrts)/(mX*mX)*x);

   Double_t Ap0Real = a1 - a3Im*(mZ*sqrts)/(mX*mX)*sqrt(x);
   Double_t Ap0Imag = a1Im + a3*(mZ*sqrts)/(mX*mX)*sqrt(x);

   Double_t Am0Real = a1 + a3Im*(mZ*sqrts)/(mX*mX)*sqrt(x);
   Double_t Am0Imag = a1Im - a3*(mZ*sqrts)/(mX*mX)*sqrt(x);

   Double_t f00 = A00Real*A00Real + A00Imag*A00Imag;
   Double_t fp0 = Ap0Real*Ap0Real + Ap0Imag*Ap0Imag;
   Double_t fm0 = Am0Real*Am0Real + Am0Imag*Am0Imag;

   /*   
   Double_t ftotal = f00 + fp0 + fm0;
   // normalize to the total
   f00 = f00 / ftotal;
   fp0 = fp0 / ftotal;
   fm0 = fm0 / ftotal;
   */
   Double_t phi00=atan2(A00Imag,A00Real);
   Double_t phip0=atan2(Ap0Imag,Ap0Real)-phi00;
   Double_t phim0=atan2(Am0Imag,Am0Real)-phi00;
   
   Double_t value = 0;
   

   value += (f00*(-1 + Power(h1,2))*(-1 + Power(h2,2)))/4.;

   value += (fp0*(1 + Power(h1,2) - 2*h1*R1Val)*(1 + Power(h2,2) + 2*h2*R2Val))/16.;

   value += (fm0*(1 + Power(h1,2) + 2*h1*R1Val)*(1 + Power(h2,2) - 2*h2*R2Val))/16.;

   value += -(Sqrt(f00)*Sqrt(fp0)*Sqrt(1 - Power(h1,2))*Sqrt(1 - Power(h2,2))*(h1 - R1Val)*(h2 + R2Val)*Cos(Phi + phip0))/4.;

   value += -(Sqrt(f00)*Sqrt(fm0)*Sqrt(1 - Power(h1,2))*Sqrt(1 - Power(h2,2))*(h1 + R1Val)*(h2 - R2Val)*Cos(Phi - phim0))/4.;

   value += (Sqrt(fm0)*Sqrt(fp0)*(-1 + Power(h1,2))*(-1 + Power(h2,2))*Cos(2*Phi - phim0 + phip0))/8.;


   return value ; 

  }; 

};
