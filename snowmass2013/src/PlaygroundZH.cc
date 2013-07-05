#ifndef PLAYGROUNDZH
#define PLAYGROUNDZH

#include "RooPlot.h"
#include "ScalarPdfFactoryZH.cc"
#include "RooDataSet.h"
#include "TMath.h"
#include "TChain.h"
#include "vector"
#include "RooAddPdf.h"
#include "TRandom3.h"

// PDFs
#include "RooZZ_3D.h"


namespace PlaygroundZHhelpers{

  enum ERRORcode {kNoError,kFileLoadFailure,kNotEnoughEvents,kDataEmpty,kNoPdf};
  enum varList {kcostheta1,kcostheta2,kphi};
}

using namespace PlaygroundZHhelpers;

class PlaygroundZH{

public:
    
  bool debug;

  RooRealVar* costheta1;
  RooRealVar* costheta2;  
  RooRealVar* phi;
  RooRealVar* mX;

  vector<RooRealVar*> varContainer;

  ScalarPdfFactoryZH* scalar;

  RooDataSet* data;
  RooDataSet* sigData;
  RooDataSet* bkgData; 
  RooDataSet* toyData;
  int embedTrackerSig;
  int embedTrackerBkg;
  
  RooRealVar* nsig;
  RooRealVar* nbkg;
  RooAbsPdf* bkgPdf; 

  int seed_;

  PlaygroundZH(double mH, float nsignal, float nbackground, const unsigned int seed, bool debug_=false, int parameterization_=2, bool withAcc_=false){
    
    debug=debug_;
    embedTrackerSig=0;
    embedTrackerBkg=0;
    seed_= seed;

    costheta1 = new RooRealVar("costheta1","cos#theta_{1}",-1.,1.);
    costheta2 = new RooRealVar("costheta2","cos#theta_{2}",-1.,1.);
    phi = new RooRealVar("phi","#Phi",-TMath::Pi(),TMath::Pi());
    mX = new RooRealVar("mX","mX",mH);
    // signal and background
    nsig = new RooRealVar("nsig","number of signal events",  0, 10000000.);
    nbkg = new RooRealVar("nbkg","number of background events", 0, 10000000.);

    varContainer.push_back(costheta1);
    varContainer.push_back(costheta2);
    varContainer.push_back(phi);

    scalar = new ScalarPdfFactoryZH(costheta1,costheta2,phi,mX,parameterization_,withAcc_);

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

    nsig->setVal(nsignal);
    nbkg->setVal(nbackground);
    
  }
    
   ~PlaygroundZH(){

    delete scalar;
    delete costheta1;
    delete costheta2;
    delete phi;
    delete nsig;
    delete nbkg;
    delete mX;
    
    if(sigData) delete sigData;
    if(toyData) delete toyData;
    if(bkgData) delete bkgData;

  };

  int generate(RooAbsPdf* sigPdf, RooAbsPdf* bkgPdf, int toy_index, bool pure=true){
  
    if(debug) 
      cout << "PlaygroundZH::generate() with seed " << seed_ + toy_index  << endl;
    TRandom3 rng_;
    rng_.SetSeed(seed_+toy_index); 


    int nsigEvents = nsig->getVal();
    int nbkgEvents = nbkg->getVal();
    
    if ( pure ) {
      nsigEvents = rng_.Poisson( nsig->getVal());
      nbkgEvents = rng_.Poisson( nbkg->getVal());
    }
    
    int nEvents = nsigEvents + nbkgEvents; 
    RooAddPdf* totalPdf = new RooAddPdf("totalPdf","totalPdf",RooArgList(*sigPdf,*bkgPdf),RooArgList(*nsig,*nbkg));

    if ( debug ) {
      std::cout << "nsignal = " << nsigEvents << ",\t nbackground = " << nbkgEvents << ", total is " << nEvents << "\n";
    }
    

    if(pure) {
      if ( debug ) 
	std::cout << "Generating pure toy\n";  
      toyData = totalPdf->generate(RooArgSet(*costheta1,*costheta2,*phi), (int) nEvents);
    }  else{
      
      // reset the events starting point
      embedTrackerSig = toy_index * nsigEvents;
      embedTrackerBkg = toy_index * nbkgEvents;
      
      int isigEvent = toy_index * nsigEvents;
      int ibkgEvent = toy_index * nbkgEvents;
      
      RooArgSet *tempEvent;
      toyData = new RooDataSet("toyData","toyData",RooArgSet(*costheta1,*costheta2,*phi));

      if(  (nsigEvents+embedTrackerSig > sigData->sumEntries()) 
	  || (nbkgEvents+embedTrackerBkg > bkgData->sumEntries()) ) {
	cout << "PlaygroundZH::generate() - ERROR!!! PlaygroundZH::data does not have enough events to fill toy!!!!  bye :) " << endl;
	toyData = NULL;
	return kNotEnoughEvents;
      }

      for(int iEvent=isigEvent; iEvent<isigEvent+nsigEvents; iEvent++){
	if(debug) cout << "generating signal event: " << iEvent << " embedTrackerSig: " << embedTrackerSig << endl;
	tempEvent = (RooArgSet*) sigData->get(embedTrackerSig);
	toyData->add(*tempEvent);
	embedTrackerSig++;
      }

      for(int iEvent=ibkgEvent; iEvent<ibkgEvent+nbkgEvents; iEvent++){
	if(debug) cout << "generating background event: " << iEvent << " embedTrackerBkg: " << embedTrackerBkg << endl;
	tempEvent = (RooArgSet*) bkgData->get(embedTrackerBkg);
	toyData->add(*tempEvent);
	embedTrackerBkg++;
      }
      
      /*
	// example of adding pure toy data
      RooDataSet* bkgToyData = bkgPdf->generate(RooArgSet(*costheta1,*costheta2,*phi), (int) nbkgEvents);

      for (int i = 0 ; i < nbkgEvents; i++ ) {
	RooArgSet *tempbkgEvent = (RooArgSet*) bkgToyData->get(i);
	toyData->add(*tempbkgEvent);
      }
      */
    }

    return kNoError;

  };

  int loadTree(RooDataSet* _data){

    data = _data;
    return kNoError;

  }

  int loadSigTree(TString fileName, TString treeName){

    TChain* myChain = new TChain(treeName);
    myChain->Add(fileName);
    
    if(!myChain || myChain->GetEntries()<=0) return kFileLoadFailure;

    if(sigData)
      sigData=0;

    sigData = new RooDataSet("sigData","sigData",myChain,RooArgSet(*costheta1,*costheta2,*phi),"");

    if(debug)
      cout << "Number of signal events: " << sigData->numEntries() << endl;

    return kNoError;
  
  };


  int loadBkgTree(TString fileName, TString treeName){

    TChain* myChain = new TChain(treeName);
    myChain->Add(fileName);
    
    if(!myChain || myChain->GetEntries()<=0) return kFileLoadFailure;

    if(bkgData)
      bkgData=0;

    bkgData = new RooDataSet("bkgData","bkgData",myChain,RooArgSet(*costheta1,*costheta2,*phi),"");
    
    if(debug)
      cout << "Number of background events in data: " << bkgData->numEntries() << endl;
    
    return kNoError;
  
  };


  RooFitResult* fitData(RooAbsPdf* sigPdf, RooAbsPdf* bkgPdf, bool istoy = false, int PrintLevel = 1){

    RooAddPdf* totalPdf = new RooAddPdf("totalPdf","totalPdf",RooArgList(*sigPdf,*bkgPdf),RooArgList(*nsig,*nbkg));
    
    if ( istoy )  {
      return ( totalPdf->fitTo(*toyData, RooFit::PrintLevel(PrintLevel), RooFit::Save(true), RooFit::Extended(kTRUE)) );
    }
    else  {
      return ( totalPdf->fitTo(*sigData, RooFit::PrintLevel(PrintLevel), RooFit::Save(true), RooFit::Extended(kTRUE)) ); 
    }
    
  };

  int projectPDF(varList myVar, RooAbsPdf* sigPdf, RooAbsPdf* bkgPdf, int bins=20, bool istoy=false, bool isbkg=false){

    if(debug) cout << "PlaygroundZH::projectionPDF()" << endl;
    RooPlot* plot = varContainer[myVar]->frame(bins);
    
    if(debug) cout << "RooPlot: " << plot << endl;

    if ( istoy ) {
      if( !toyData ) return kDataEmpty; 
      if ( debug ) std::cout << "Drawing toy dataset\n";  
      toyData->plotOn(plot);
      RooAddPdf* totalPdf = new RooAddPdf("totalPdf","totalPdf",RooArgList(*sigPdf,*bkgPdf),RooArgList(*nsig,*nbkg));
      totalPdf->plotOn(plot);
    } else {
      if ( isbkg ) {
	if( ! bkgData ) return kDataEmpty;
	bkgData->plotOn(plot);
	bkgPdf->plotOn(plot);
      } else {
	if(  !sigData ) return kDataEmpty;
	sigData->plotOn(plot);
	scalar->PDF->plotOn(plot);
      }
    }
    
    plot->Draw();
    
    return kNoError;

  }


void calcfractionphase(double sqrts, double g1Re,  double g1Im,  double g2Re,   double g2Im,  double g4Re,  double g4Im, 
		       double & fa2, double & fa3, double & phia2, double & phia3) 
  {
    // ILC numbers at 250 GeV at mH= 125 GeV (narrow Z width approximation)
    Double_t sigma1_e = 0.981396; // was 0.94696 at 126 GeV
    Double_t sigma2_e = 33.4674;  // was 32.1981 at 126 GeV
    Double_t sigma4_e = 7.9229;   // was 7.45502 at 126 GeV
    
    // ILC numbers at 500 GeV with mH = 125 GeV ( narrow Z width approximation) 
    if ( sqrts == 500. ) {
      sigma1_e = 2.57246; 
      sigma2_e = 516.556;  
      sigma4_e = 414.378;
    }
    
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



