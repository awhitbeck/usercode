#ifndef PLAYGROUNDZH
#define PLAYGROUNDZH

#include "RooPlot.h"
#include "ScalarPdfFactoryZH.cc"
#include "RooDataSet.h"
#include "TMath.h"
#include "TChain.h"
#include "vector"

namespace PlaygroundZHhelpers{

  enum ERRORcode {kNoError,kFileLoadFailure,kNotEnoughEvents,kDataEmpty};
  enum varList {kcosthetastar,kcostheta1,kcostheta2,kphi,kphi1};
}

using namespace PlaygroundZHhelpers;

class PlaygroundZH{

public:
    
  bool debug;

  RooRealVar* costhetastar;  
  RooRealVar* costheta1;
  RooRealVar* costheta2;  
  RooRealVar* phi;
  RooRealVar* phi1;

  RooRealVar* mX;

  vector<RooRealVar*> varContainer;

  ScalarPdfFactoryZH* scalar;

  RooDataSet* data;
  RooDataSet* toyData;
  int embedTracker;

  PlaygroundZH(double mH, bool debug_=false, int parameterization_=2){
    
    debug=debug_;

    embedTracker=0;

    costhetastar = new RooRealVar("costhetastar","cos#theta*",0.,-1.,1.);
    costheta1 = new RooRealVar("costheta1","cos#theta_{1}",0.,-1.,1.);
    costheta2 = new RooRealVar("costheta2","cos#theta_{2}",0.,-1.,1.);
    phi = new RooRealVar("phi","#Phi",0.,-TMath::Pi(),TMath::Pi());
    phi1 = new RooRealVar("phistar1","#Phi_{1}",0.,-TMath::Pi(),TMath::Pi());

    mX = new RooRealVar("mX","mX",mH);
      
    varContainer.push_back(costhetastar);
    varContainer.push_back(costheta1);
    varContainer.push_back(costheta2);
    varContainer.push_back(phi);
    varContainer.push_back(phi1);

    scalar = new ScalarPdfFactoryZH(costhetastar,costheta1,costheta2,phi,phi1,mX,parameterization_);

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
    
   ~PlaygroundZH(){

    delete scalar;

    delete costhetastar;
    delete costheta1;
    delete costheta2;
    delete phi;
    delete phi1;
    
    delete mX;

    if(data) delete data;
    if(toyData) delete toyData;

  };

  int generate(int nEvents, bool pure=true){

    if(debug)
      cout << "PlaygroundZH::generate()" << endl;

    if(pure)
      toyData = scalar->PDF->generate(RooArgSet(*costhetastar,*costheta1,*costheta2,*phi,*phi1),nEvents);
    else{

      RooArgSet *tempEvent;
      //if(toyData) delete toyData;
      toyData = new RooDataSet("toyData","toyData",RooArgSet(*costhetastar,*costheta1,*costheta2,*phi,*phi1));

      if(nEvents+embedTracker > data->sumEntries()){
	cout << "PlaygroundZH::generate() - ERROR!!! PlaygroundZH::data does not have enough events to fill toy!!!!  bye :) " << endl;
	toyData = NULL;
	return kNotEnoughEvents;
      }

      for(int iEvent=0; iEvent<nEvents; iEvent++){

	if(debug) cout << "generating event: " << iEvent << " embedTracker: " << embedTracker << endl;
	tempEvent = (RooArgSet*) data->get(embedTracker);
	toyData->add(*tempEvent);
	embedTracker++;

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

    data = new RooDataSet("data","data",myChain,RooArgSet(*costheta1,*costheta2,*costhetastar,*phi1,*phi),"");

    if(debug)
      cout << "Number of events in data: " << data->numEntries() << endl;

    return kNoError;
  
  };

  RooFitResult* fitData(bool istoy = false, int PrintLevel = 1){

    if ( istoy )  {
      return ((scalar->PDF)->fitTo(*toyData, RooFit::PrintLevel(PrintLevel), RooFit::Save(true)));
    }
    else  {
      return ((scalar->PDF)->fitTo(*data, RooFit::PrintLevel(PrintLevel), RooFit::Save(true))); 
    }
    
  };

  int projectPDF(varList myVar, int bins=20, bool istoy=false){

    if(debug) cout << "PlaygroundZH::projectionPDF()" << endl;
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


void calcfractionphase(double g1Re,  double g1Im,  double g2Re,   double g2Im,  double g4Re,  double g4Im, 
		       double & fa2, double & fa3, double & phia2, double & phia3) 
  {
    // ILC numbers at 250 GeV at mH= 125 GeV (narrow Z width approximation)
    Double_t sigma1_e = 0.981396; // was 0.94696 at 126 GeV
    Double_t sigma2_e = 33.4674;  // was 32.1981 at 126 GeV
    Double_t sigma4_e = 7.9229;   // was 7.45502 at 126 GeV
    
    Double_t g1 = sqrt(g1Re*g1Re + g1Im*g1Im);
    Double_t g2 = sqrt(g2Re*g2Re + g2Im*g2Im);
    Double_t g4 = sqrt(g4Re*g4Re + g4Im*g4Im);
    
    fa2 = sigma2_e*g2*g2 / (  sigma1_e*g1*g1 + sigma2_e*g2*g2 + sigma4_e*g4*g4 );
    phia2 = atan2(g2Im, g2Re);
    fa3 = sigma4_e*g4*g4 / (  sigma1_e*g1*g1 + sigma2_e*g2*g2 + sigma4_e*g4*g4 );
    phia3 = atan2(g4Im, g4Re);
    
    std::cout << "fa2 = " << fa2 << "\t with phase " << phia2 << "\n"; 
    std::cout << "fa3 = " << fa3 << "\t with phase " << phia3 << "\n"; 
  }
  
};

#endif



