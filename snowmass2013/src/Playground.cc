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

  Playground(double mH, bool debug_=false){
    
    debug=debug_;

    z1mass = new RooRealVar("z1mass","m_{Z1}",12.,120.);
    z2mass = new RooRealVar("z2mass","m_{Z2}",12.,120.);
    costhetastar = new RooRealVar("costhetastar","cos#theta*",-1.,1.);
    costheta1 = new RooRealVar("costheta1","cos#theta_{1}",-1.,1.);
    costheta2 = new RooRealVar("costheta2","cos#theta_{2}",-1.,1.);
    phi = new RooRealVar("phi","#Phi",-TMath::Pi(),TMath::Pi());
    phi1 = new RooRealVar("phistar1","#Phi_{1}",-TMath::Pi(),TMath::Pi());
      
    mzz = new RooRealVar("ZZMass","m_{ZZ}",mH,100,1000);

    varContainer.push_back(z1mass);
    varContainer.push_back(z2mass);
    varContainer.push_back(costhetastar);
    varContainer.push_back(costheta1);
    varContainer.push_back(costheta2);
    varContainer.push_back(phi);
    varContainer.push_back(phi1);
    varContainer.push_back(mzz);

    int parameterization = 2;
    scalar = new ScalarPdfFactory(z1mass,z2mass,costhetastar,costheta1,costheta2,phi,phi1,mzz,parameterization);
    mzz->setConstant(kTRUE);
    scalar->makeParamsConst(true);
    
    if ( parameterization = 2 ) {
      
      scalar->fg2->setConstant(kFALSE);
      scalar->fg2->setRange(0.,1.);
      scalar->fg4->setConstant(kFALSE);
      scalar->fg4->setVal(0.5);
      scalar->fg4->setRange(0.,1.);
      
      scalar->phig2->setConstant(kFALSE);
      scalar->phig2->setRange(0, 2*TMath::Pi());
      scalar->phig4->setConstant(kFALSE);
      scalar->phig4->setVal(0.5*TMath::Pi());
      scalar->phig4->setRange(0, 2*TMath::Pi());
    }
    
    if ( parameterization == 1 ) {
      scalar->g2Val->setConstant(kFALSE);
      scalar->g2ValIm->setConstant(kFALSE);
      scalar->g4Val->setConstant(kFALSE); 
      scalar->g4Val->setVal(0.);
      scalar->g4ValIm->setConstant(kFALSE);
      scalar->g4ValIm->setVal(2.5);
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


