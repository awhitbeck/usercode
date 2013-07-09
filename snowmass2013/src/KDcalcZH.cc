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

  KDcalcZH(){

    costheta1 = new RooRealVar("costheta1","costheta1",-1.,1.);
    costheta2 = new RooRealVar("costheta2","costheta2",-1.,1.);
    phi       = new RooRealVar("phi","phi",-TMath::Pi(),TMath::Pi());
    
    mH        = new RooRealVar("mH","mH",100.,1000.);

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
    float altSignalProb = altSignal->PDF->getVal();

    KD = SMHiggsProb/(SMHiggsProb+altSignalProb);

  };

  //template <class T>
  void addDtoTree(TString fileName,
		  TString treeName,
		  TString mHBranch="HMass",
		  TString costheta1Branch="costheta1",
		  TString costheta2Branch="costheta2",
		  TString phiBranch="phi"
		  ){
    
    TFile* f = new TFile(fileName,"UPDATE");
    TTree* t = (TTree*) f->Get(treeName);

    float mH_,costheta1_,costheta2_,phi_;
    float KD_;

    t->SetBranchAddress(mHBranch,&mH_);
    t->SetBranchAddress(costheta1Branch,&costheta1_);
    t->SetBranchAddress(costheta2Branch,&costheta2_);
    t->SetBranchAddress(phiBranch,&phi_);
    t->Branch("pseudoMELA",&KD_,"pseudoMELA/D");

    int maxEvents = t->GetEntries();

    for(int iEvt=0; iEvt<maxEvents; iEvt++){

      if(iEvt%100000==0) cout << iEvt << "/" << maxEvents << endl;

      t->GetEntry(iEvt);
      computeKD(mH_,costheta1_,costheta2_,phi_,KD_);
      t->Fill();

    }

    t->Write();

  };

};
