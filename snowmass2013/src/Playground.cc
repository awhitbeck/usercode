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

namespace PlaygroundHelpers{

  enum ERRORcode {kNoError,kFileLoadFailure,kNotEnoughEvents,kDataEmpty};
  enum varList {kz1mass,kz2mass,kcosthetastar,kcostheta1,kcostheta2,kphi,kphi1,kmzz};
}

using namespace PlaygroundHelpers;

class Playground{

public:
    
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
  RooDataSet* toyData;

  Playground(double mH, bool debug_=false, int parameterization_=2){
    
    debug=debug_;

    z1mass = new RooRealVar("z1mass","m_{Z1}",90.,12.,120.);
    z2mass = new RooRealVar("z2mass","m_{Z2}",25.,12.,120.);
    costhetastar = new RooRealVar("costhetastar","cos#theta*",0.,-1.,1.);
    costheta1 = new RooRealVar("costheta1","cos#theta_{1}",0.,-1.,1.);
    costheta2 = new RooRealVar("costheta2","cos#theta_{2}",0.,-1.,1.);
    phi = new RooRealVar("phi","#Phi",0.,-TMath::Pi(),TMath::Pi());
    phi1 = new RooRealVar("phistar1","#Phi_{1}",0.,-TMath::Pi(),TMath::Pi());
      
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

    delete scalar;

    delete z1mass;
    delete z2mass;
    delete costhetastar;
    delete costheta1;
    delete costheta2;
    delete phi;
    delete phi1;
    delete mzz;
    
    if(data) delete data;
    if(toyData) delete toyData;

  };

  int generate(int nEvents, bool pure=true){

    if(debug)
      cout << "Playground::generate()" << endl;

    if(pure)
      toyData = scalar->PDF->generate(RooArgSet(*z1mass,*z2mass,*costhetastar,*costheta1,*costheta2,*phi1,*phi),nEvents);
    else{

      if(debug) cout << "Playground::generate() - drawing a random number of events from Playground::data ... note events will be removed from this dataset!" << endl;
      
      RooArgSet *tempEvent;
      if(toyData) delete toyData;
      toyData = new RooDataSet("toyData","toyData",RooArgSet(*z1mass,*z2mass,*costhetastar,*costheta1,*costheta2,*phi,*phi1));

      if(nEvents>data->sumEntries()){
	cout << "Playground::generate() - ERROR!!! Playground::data does not have enough events to fill toy!!!!  bye :) " << endl;
	toyData = NULL;
	return kNotEnoughEvents;
      }

      for(int iEvent=0; iEvent<nEvents; iEvent++){
	if(debug) cout << "generating event: " << iEvent << endl;
	tempEvent = (RooArgSet*) data->get(0);
	toyData->add(*tempEvent);
	data->reduce(*tempEvent);
      }
	
    }

    return kNoError;

  };

  int loadTree(TString fileName, TString treeName){

    TChain* myChain = new TChain(treeName);
    myChain->Add(fileName);
    
    if(!myChain || myChain->GetEntries()<=0) return kFileLoadFailure;

    if(data)
      data=0;

    data = new RooDataSet("data","data",myChain,RooArgSet(*z1mass,*z2mass,*costhetastar,*costheta1,*costheta2,*phi,*phi1),"");

    if(debug)
      cout << "Number of events in data: " << data->numEntries() << endl;

    return kNoError;
  
  };

  RooFitResult* fitData(bool istoy = false){

    if ( istoy )  {
      return (scalar->PDF->fitTo(*toyData, RooFit::PrintLevel(-1), RooFit::Save(true)));
    }
    else  {
      return ((scalar->PDF)->fitTo(*data, RooFit::Save(true))); 
    }
    
  };

  int projectPDF(varList myVar, int bins=20, bool istoy=false){

    if(debug) cout << "Playground::projectionPDF()" << endl;
    RooPlot* plot = varContainer[myVar]->frame(bins);

    if(debug) cout << "RooPlot: " << plot << endl;

    if ( istoy ) {
      if( !toyData ) return kDataEmpty; 
      toyData->plotOn(plot);
    }else{
      if( !data ) return kDataEmpty;
      data->plotOn(plot);
    }

    scalar->PDF->plotOn(plot);
    
    plot->Draw();

    return kNoError;

  }
};

#endif



