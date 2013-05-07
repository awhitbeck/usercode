#ifndef PLAYGROUND
#define PLAYGROUND

#include "RooSpinZero_7DComplex.h"
#include "RooPlot.h"
#include "AngularPdfFactory.cc"
#include "ScalarPdfFactory.cc"
#include "RooDataSet.h"
#include "TMath.h"
#include "TChain.h"
#include "vector"

class Playground{

public:
    
  enum ERRORcode {kNoError,kFileLoadFailure};
  enum varList {kz1mass,kz2mass,kcosthetastar,kcostheta1,kcostheta2,kphi,kphi1,kmzz};

  bool debug;

  RooRealVar* z1mass;  
  RooRealVar* z2mass;
  RooRealVar* costhetastar;  
  RooRealVar* costheta1;
  RooRealVar* costheta2;  
  RooRealVar* phi;
  RooRealVar* phi1;
  RooRealVar* mzz;
  vector<RooRealVar*> varContainer;

  ScalarPdfFactory* scalar;

  RooDataSet* data;

  Playground(){};

  Playground(double mH, bool debug_=false, int parameterization_=2){
    
    debug=debug_;

    z1mass = new RooRealVar("Z1Mass","m_{Z1}",12.,120.);
    z2mass = new RooRealVar("Z2Mass","m_{Z2}",12.,120.);
    costhetastar = new RooRealVar("costhetastar","cos#theta*",-1.,1.);
    costheta1 = new RooRealVar("helcosthetaZ1","cos#theta_{1}",-1.,1.);
    costheta2 = new RooRealVar("helcosthetaZ2","cos#theta_{2}",-1.,1.);
    phi = new RooRealVar("helphi","#Phi",-TMath::Pi(),TMath::Pi());
    phi1 = new RooRealVar("phistarZ1","#Phi_{1}",-TMath::Pi(),TMath::Pi());
      
    mzz = new RooRealVar("ZZMass","m_{ZZ}",mH,100,1000);

    varContainer.push_back(z1mass);
    varContainer.push_back(z2mass);
    varContainer.push_back(costhetastar);
    varContainer.push_back(costheta1);
    varContainer.push_back(costheta2);
    varContainer.push_back(phi);
    varContainer.push_back(phi1);
    varContainer.push_back(mzz);

    scalar = new ScalarPdfFactory(z1mass,z2mass,costhetastar,costheta1,costheta2,phi,phi1,mzz,parameterization_);
    mzz->setConstant(kTRUE);
    scalar->makeParamsConst(true);

    // float these parameters by default
    if ( parameterization_ == 2 ) {
        
      scalar->fa2->setConstant(kFALSE);
      scalar->phia2->setConstant(kFALSE);
      scalar->fa3->setConstant(kFALSE);
      scalar->phia3->setConstant(kFALSE);
    }
    
    if ( parameterization_ == 1 ) {
      scalar->g2Val->setConstant(kFALSE);
      scalar->g2ValIm->setConstant(kFALSE);
      scalar->g4Val->setConstant(kFALSE);
      scalar->g4ValIm->setConstant(kFALSE);
    }

  }
    
   ~Playground(){

    delete z1mass;
    delete z2mass;
    delete costhetastar;
    delete costheta1;
    delete costheta2;
    delete phi;
    delete phi1;
    delete mzz;
    
    delete scalar;
    if(data) delete data;

  };

  void generate(){};
  int loadTree(TString fileName, TString treeName){

    TChain* myChain = new TChain(treeName);
    myChain->Add(fileName);
    
    if(!myChain || myChain->GetEntries()<=0) return kFileLoadFailure;

    data = new RooDataSet("data","data",myChain,RooArgSet(*z1mass,*z2mass,*costhetastar,*costheta1,*costheta2,*phi,*phi1),"");

    if(debug)
      cout << "Number of events in data: " << data->numEntries() << endl;

    return kNoError;
  
  };

  void fitData(){
    scalar->PDF->fitTo(*data);
  };

  void projectPDF(varList myVar, int bins=20){
    RooPlot* plot = varContainer[myVar]->frame(bins);
    data->plotOn(plot);
    scalar->PDF->plotOn(plot);
    
    plot->Draw();

  }
};

#endif



