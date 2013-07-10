#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "RooRealVar.h"

#include "ScalarPdfFactoryZH.cc"

using namespace std;

class KDcalcZH {

public:

  RooRealVar* costheta1;
  RooRealVar* costheta2;
  RooRealVar* phi;
  
  RooRealVar* mH;
  
  ScalarPdfFactoryZH* SMHiggs;
  ScalarPdfFactoryZH* altSignal;

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
				       2,
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
					  2,
					  false);

    altSignal->fa2->setVal(0.0);
    altSignal->fa3->setVal(0.999999);
    altSignal->phia2->setVal(0.0);
    altSignal->phia3->setVal(0.0);
    altSignal->sqrts->setVal(sqrtsVal);

    SMHiggs->makeParamsConst(true);
    altSignal->makeParamsConst(true);

  };


  //template <class T> 
  void computeKD(float mH_,
		 float costheta1_,
		 float costheta2_,
		 float phi_,
		 float& KD){
    
    mH->setVal(mH_);
    costheta1->setVal(costheta1_);
    costheta2->setVal(costheta2_);
    phi->setVal(phi_);

    float SMHiggsProb = SMHiggs->PDF->getVal();
    if(SMHiggsProb<0.0) cout << "KDcalcZH::computeKD - ERROR: SMHiggs prob is negative!!!" << endl;
    float altSignalProb = altSignal->PDF->getVal();
    if(altSignalProb<0.0) cout << "KDcalcZH::computeKD - ERROR: altSig prob is negative!!!" << endl;
    
    float c = 0.84;

    KD = SMHiggsProb/(SMHiggsProb+c*altSignalProb);

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

    t->SetBranchAddress(mHBranch,&mH_);
    t->SetBranchAddress(costheta1Branch,&costheta1_);
    t->SetBranchAddress(costheta2Branch,&costheta2_);
    t->SetBranchAddress(phiBranch,&phi_);
    tt->Branch("pseudoMELA",&KD_,"pseudoMELA/F");
    
    cout << "t entries: "  << t->GetEntries() << endl;

    for(int iEvt=0; iEvt<t->GetEntries(); iEvt++){

      // if ( iEvt > 10000 ) continue;

      if(iEvt%100000==0)  cout << iEvt << "/" << t->GetEntries() << endl;

      t->GetEntry(iEvt);

      computeKD(mH_,costheta1_,costheta2_,phi_,KD_);

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

};
