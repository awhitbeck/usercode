#ifndef PLAYGROUND
#define PLAYGROUND

#include "RooSpinZero_7DComplex.h"
#include "AngularPdfFactory.cc"
#include "ScalarPdfFactory.cc"
#include "TMath.h"

class Playground{

public:
    
  RooRealVar* z1mass;  
  RooRealVar* z2mass;
  RooRealVar* costhetastar;  
  RooRealVar* costheta1;
  RooRealVar* costheta2;  
  RooRealVar* phi;
  RooRealVar* phi1;
  RooRealVar* mzz;

  ScalarPdfFactory* scalar;

  RooDataSet* data;

  Playground(){
    
    z1mass = new RooRealVar("Z1Mass","m_{Z1}",12.,120.);
    z2mass = new RooRealVar("Z2Mass","m_{Z2}",12.,120.);
    costhetastar = new RooRealVar("costhetastar","cos#theta*",-1.,1.);
    costheta1 = new RooRealVar("helcosthetaZ1","cos#theta_{1}",-1.,1.);
    costheta2 = new RooRealVar("helcosthetaZ2","cos#theta_{2}",-1.,1.);
    phi = new RooRealVar("helphi","#Phi",-TMath::Pi(),TMath::Pi());
    phi1 = new RooRealVar("phistarZ1","#Phi_{1}",-TMath::Pi(),TMath::Pi());
      
    mzz = new RooRealVar("ZZMass","m_{ZZ}",100,1000);

    scalar = new ScalarPdfFactory(z1mass,z2mass,costhetastar,costheta1,costheta2,phi,phi1,mzz);

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

  };

  void generate(){};
  void loadTree(TString fileName, TString treeName){};
  void fitData(){};

};

#endif



