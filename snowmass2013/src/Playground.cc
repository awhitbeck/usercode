#ifndef PLAYGROUND
#define PLAYGROUND

#include "RooSpinZero_7DComplex_withAccep.h"
#include "RooqqZZ_JHU_ZgammaZZ_fast.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
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
  bool background_;
  RooqqZZ_JHU_ZgammaZZ_fast* PDF_qqZZ;
  RooRealVar* upfrac;
  RooRealVar* bkg_frac;
  double SoverB_;
  RooAddPdf* model;

  RooDataSet* data;
  RooDataSet* data_bkg;
  RooDataSet* data_sigbkg;
  RooDataSet* toyData;
  int embedTracker;
  int embedTracker_bkg;

  Playground(double mH, bool debug_=false, int parameterization_=2, bool acceptance=false, bool background=false, double SoverB=0){
    
    debug=debug_;

    embedTracker=0;
    embedTracker_bkg=0;

    z1mass = new RooRealVar("Z1Mass","m_{Z1}",90.,12.,120.);
    z2mass = new RooRealVar("Z2Mass","m_{Z2}",25.,12.,120.);
    costhetastar = new RooRealVar("costhetastar","cos#theta*",0.,-1.,1.);
    costheta1 = new RooRealVar("helcosthetaZ1","cos#theta_{1}",0.,-1.,1.);
    costheta2 = new RooRealVar("helcosthetaZ2","cos#theta_{2}",0.,-1.,1.);
    phi = new RooRealVar("helphi","#Phi",0.,-TMath::Pi(),TMath::Pi());
    phi1 = new RooRealVar("phistarZ1","#Phi_{1}",0.,-TMath::Pi(),TMath::Pi());
      
    //mzz = new RooRealVar("ZZMass","m_{ZZ}",mH,106,141);
    mzz = new RooRealVar("ZZMass","m_{ZZ}",mH,120,130);

    varContainer.push_back(z1mass);
    varContainer.push_back(z2mass);
    varContainer.push_back(costhetastar);
    varContainer.push_back(costheta1);
    varContainer.push_back(costheta2);
    varContainer.push_back(phi);
    varContainer.push_back(phi1);
    varContainer.push_back(mzz);

    scalar = new ScalarPdfFactory(z1mass,z2mass,costhetastar,costheta1,costheta2,phi,phi1,mzz,parameterization_,acceptance);
    mzz->setConstant(kTRUE);
    scalar->makeParamsConst(true);

    //std::cout << "float these parameters by default" << std::endl;
    if ( parameterization_ == 2 ) {
        
      scalar->_modelParams.fa2->setConstant(kFALSE);
      scalar->_modelParams.phia2->setConstant(kFALSE);
      scalar->_modelParams.fa3->setConstant(kFALSE);
      scalar->_modelParams.phia3->setConstant(kFALSE);
    }
    
    if ( parameterization_ == 1 ) {
      scalar->_modelParams.g2Val->setConstant(kFALSE);
      scalar->_modelParams.g2ValIm->setConstant(kFALSE);
      scalar->_modelParams.g4Val->setConstant(kFALSE);
      scalar->_modelParams.g4ValIm->setConstant(kFALSE);
    }

    background_=background;
    if(background_){
      SoverB_ = SoverB;
      upfrac = new RooRealVar("upfrac","upfrac",0.5,0.,1.);
      upfrac->setConstant(kTRUE);
      //Bakcground fraction over total S+B
      bkg_frac = new RooRealVar("bkg_frac","bkg_frac",1/(SoverB_+1),0.,10.);
      bkg_frac->setConstant(kTRUE);
      PDF_qqZZ= new RooqqZZ_JHU_ZgammaZZ_fast("PDF_qqZZ","PDF_qqZZ",
					      *z1mass, *z2mass, *costheta1, *costheta2, *phi, *costhetastar, *phi1, *mzz,
					      *upfrac);
      model = new RooAddPdf("model","background+signal",*PDF_qqZZ,*(scalar->PDF),*bkg_frac);  
    }
   }
    
   ~Playground(){

     //std::cout << "~Playground" << std::endl;

    delete scalar;
    delete PDF_qqZZ;

    delete z1mass;
    delete z2mass;
    delete costhetastar;
    delete costheta1;
    delete costheta2;
    delete phi;
    delete phi1;
    delete mzz;

    if(data) delete data;
    if(data_bkg) delete data_bkg;
    if(data_sigbkg) delete data_sigbkg;
    //if(toyData) delete toyData;

  };

  void setEmbeddingCounter(int newStartingEvent){
    embedTracker = newStartingEvent;
    embedTracker_bkg = embedTracker/SoverB_;
  }

  int generate(int nEvents, bool pure=true){

    if(debug)
      cout << "Playground::generate()" << endl;

    if(pure)
      toyData = scalar->PDF->generate(RooArgSet(*z1mass,*z2mass,*costhetastar,*costheta1,*costheta2,*phi1,*phi),nEvents);
    else{

      RooArgSet *tempEvent;
      //if(toyData) delete toyData;
      toyData = new RooDataSet("toyData","toyData",RooArgSet(*z1mass,*z2mass,*costhetastar,*costheta1,*costheta2,*phi,*phi1));

      if(nEvents+embedTracker > data->sumEntries()){
	cout << "Playground::generate() - ERROR!!! Playground::data does not have enough events to fill toy!!!!  bye :) " << endl;
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

  //The user should be careful to pass here a number of events witch match with SoverB parameter of the constructor
  int generate_bkg(int nEvents, bool pure=true){ 

    if(!background_){
      cout<<"Error in Playground: you asked to generate bkg but you didn't setup Palyground to have background included!! Aborting..."<<endl;
      abort();
    }
    if(debug)
      cout << "Playground::generate_bkg()" << endl;

    if(pure)
      toyData = PDF_qqZZ->generate(RooArgSet(*z1mass,*z2mass,*costhetastar,*costheta1,*costheta2,*phi1,*phi),nEvents);
    else{

      RooArgSet *tempEvent;
      toyData = new RooDataSet("toyData","toyData",RooArgSet(*z1mass,*z2mass,*costhetastar,*costheta1,*costheta2,*phi,*phi1));

      if(nEvents+embedTracker_bkg > data->sumEntries()){
	cout << "Playground::generate_bkg() - ERROR!!! Playground::data does not have enough events to fill toy!!!!  bye :) " << endl;
	toyData = NULL;
	return kNotEnoughEvents;
      }

      for(int iEvent=0; iEvent<nEvents; iEvent++){

	if(debug) cout << "generating bkg event: " << iEvent << " embedTracker_bkg: " << embedTracker_bkg << endl;
	tempEvent = (RooArgSet*) data_bkg->get(embedTracker_bkg);
	toyData->add(*tempEvent);
	embedTracker_bkg++;

      }
	
    }

    return kNoError;

  };
 int generate_sigbkg(int nEvents, bool pure=true){

    if(debug)
      cout << "Playground::generate_sigbkg()" << endl;

    if(!background_){
      cout<<"Error in Playground: you asked to generate bkg but you didn't setup Palyground to have background included!! Aborting..."<<endl;
      abort();
    }

    if(pure){
      toyData = scalar->PDF->generate(RooArgSet(*z1mass,*z2mass,*costhetastar,*costheta1,*costheta2,*phi1,*phi),nEvents);
      toyData->append(*(PDF_qqZZ->generate(RooArgSet(*z1mass,*z2mass,*costhetastar,*costheta1,*costheta2,*phi1,*phi),TMath::Ceil(nEvents/SoverB_))));
    }
    else{

      RooArgSet *tempEvent;
      //if(toyData) delete toyData;
      toyData = new RooDataSet("toyData","toyData",RooArgSet(*z1mass,*z2mass,*costhetastar,*costheta1,*costheta2,*phi,*phi1));

      if(nEvents+embedTracker > data->sumEntries()){
	cout << "Playground::generate() - ERROR!!! Playground::data does not have enough events to fill toy!!!!  bye :) " << endl;
	toyData = NULL;
	return kNotEnoughEvents;
      }

      if(nEvents/SoverB_+embedTracker_bkg > data_bkg->sumEntries()){
	cout << "Playground::generate_bkg() - ERROR!!! Playground::data does not have enough events to fill toy!!!!  bye :) " << endl;
	toyData = NULL;
	return kNotEnoughEvents;
      }

      for(int iEvent=0; iEvent<nEvents; iEvent++){

	if(debug) cout << "generating event: " << iEvent << " embedTracker: " << embedTracker << endl;
	tempEvent = (RooArgSet*) data->get(embedTracker);
	toyData->add(*tempEvent);
	embedTracker++;
      }
  
      for(int iEvent=0; iEvent<nEvents/SoverB_; iEvent++){

	if(debug) cout << "generating bkg event: " << iEvent << " embedTracker_bkg: " << embedTracker_bkg << endl;
	tempEvent = (RooArgSet*) data_bkg->get(embedTracker_bkg);
	toyData->add(*tempEvent);
	embedTracker_bkg++;
      }
	
    }

    return kNoError;

  };

  int loadTree(RooDataSet* _data){

    data = _data;
    return kNoError;

  }

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


  int loadTree_bkg(RooDataSet* _data){

    if(!background_){
      cout<<"Error in Playground: you asked to load bkg but you didn't setup Palyground to have background included!! Aborting..."<<endl;
      abort();
    }

    data_bkg = _data;
    return kNoError;

  }

  int loadTree_bkg(TString fileName, TString treeName){

    if(!background_){
      cout<<"Error in Playground: you asked to load bkg but you didn't setup Palyground to have background included!! Aborting..."<<endl;
      abort();
    }

    TChain* myChain = new TChain(treeName);
    myChain->Add(fileName);
    
    if(!myChain || myChain->GetEntries()<=0) return kFileLoadFailure;

    if(data_bkg)
      data_bkg=0;

    data_bkg = new RooDataSet("data_bkg","data_bkg",myChain,RooArgSet(*z1mass,*z2mass,*costhetastar,*costheta1,*costheta2,*phi,*phi1),"");

    if(debug)
      cout << "Number of events in data_bkg: " << data_bkg->numEntries() << endl;

    return kNoError;
  
  };

  int loadTree_sigbkg(TString fileName_sig, TString treeName_sig, TString fileName_bkg, TString treeName_bkg){

    TChain* myChain_sig = new TChain(treeName_sig);
    myChain_sig->Add(fileName_sig);
    TChain* myChain_bkg = new TChain(treeName_bkg);
    myChain_bkg->Add(fileName_bkg);

    int maxSig = myChain_sig->GetEntries();
    int maxBkg = myChain_bkg->GetEntries();
    int sigEvts=0, bkgEvts=0;
    
    if(maxSig < maxBkg*SoverB_){
      bkgEvts=maxSig*(1/SoverB_);
      sigEvts=maxSig;
    }
    else{
      bkgEvts=maxBkg;
      sigEvts=maxBkg*SoverB_;
    }

    TTree* newtree = (TTree*)myChain_bkg->GetTree()->CloneTree(0);

    Int_t nbytes = 0, nb = 0, tnumber = -1;
    for (Int_t jentry=0; jentry<bkgEvts;jentry++) {
      nb = myChain_bkg->GetEntry(jentry);   nbytes += nb;

      if (tnumber != myChain_bkg->GetTreeNumber()) {
         tnumber = myChain_bkg->GetTreeNumber();
         myChain_bkg->CopyAddresses(newtree);
      }

      newtree->Fill();
   }

    for (Int_t jentry=0; jentry<sigEvts;jentry++) {
      nb = myChain_sig->GetEntry(jentry);   nbytes += nb;

      if (tnumber != myChain_sig->GetTreeNumber()) {
         tnumber = myChain_sig->GetTreeNumber();
         myChain_sig->CopyAddresses(newtree);
      }

      newtree->Fill();
   }

   data_sigbkg = new RooDataSet("data_sigbkg","data_sigbkg",newtree,RooArgSet(*z1mass,*z2mass,*costhetastar,*costheta1,*costheta2,*phi,*phi1),"");
 
   if(debug){
      cout << "Number of events in data_sigbkg: " << data_sigbkg->numEntries() << endl;
      cout<< "Number of signal: "<<sigEvts<<"   -   number of background "<<bkgEvts<<endl;
   }
    return kNoError;
   }

  int loadTree_sigbkg(RooDataSet* _data){

    if(!background_){
      cout<<"Error in Playground: you asked to load sig+bkg but you didn't setup Palyground to have background included!! Aborting..."<<endl;
      abort();
    }

    data_sigbkg = _data;
    return kNoError;

  }


  RooFitResult* fitData(bool istoy = false, int PrintLevel = 1){

    if ( istoy )  {
      return ((scalar->PDF)->fitTo(*toyData, RooFit::PrintLevel(PrintLevel), RooFit::Save(true)));
    }
    else  {
      return ((scalar->PDF)->fitTo(*data, RooFit::PrintLevel(PrintLevel), RooFit::Save(true))); 
    }
    
  };

  RooFitResult* fitData_bkg(bool istoy = false, int PrintLevel = 1){

    if(!background_){
      cout<<"Error in Playground: you asked to fit sig+bkg but you didn't setup Palyground to have background included!! Aborting..."<<endl;
      abort();
    }

    if ( istoy )  {
      return (PDF_qqZZ->fitTo(*toyData, RooFit::PrintLevel(PrintLevel), RooFit::Save(true)));
    }
    else  {
      return (PDF_qqZZ->fitTo(*data_bkg, RooFit::PrintLevel(PrintLevel), RooFit::Save(true))); 
    }
    
  };

  RooFitResult* fitData_sigbkg(bool istoy = false, int PrintLevel = 1){

    if(!background_){
      cout<<"Error in Playground: you asked to fit bkg but you didn't setup Palyground to have background included!! Aborting..."<<endl;
      abort();
    }

    if ( istoy )  {
      return (model->fitTo(*toyData, RooFit::PrintLevel(PrintLevel), RooFit::Save(true)));
    }
    else  {
      return (model->fitTo(*data_sigbkg, RooFit::PrintLevel(PrintLevel), RooFit::Save(true))); 
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

  int projectPDF_sigbkg(varList myVar, int bins=20, bool istoy=false){

    if(!background_){
      cout<<"Error in Playground: you asked to project sig+bkg but you didn't setup Palyground to have background included!! Aborting..."<<endl;
      abort();
    }

    if(!data_sigbkg && !istoy){
      cout<<"Error in Playground: you asked to project sig+bkg but you need first to call loadTree_sigbkg"<<endl;
      abort();
    }

    if(debug) cout << "Playground::projectionPDF()" << endl;
    RooPlot* plot = varContainer[myVar]->frame(bins);

    if(debug) cout << "RooPlot: " << plot << endl;

    if ( istoy ) {
      if( !toyData ) return kDataEmpty; 
      toyData->plotOn(plot);
    }else{
      if( !data_sigbkg ) return kDataEmpty;
      data_sigbkg->plotOn(plot);
    }

    model->plotOn(plot);
    
    plot->Draw();

    return kNoError;

  }

  int projectPDF_bkg(varList myVar, int bins=20, bool istoy=false){

    if(!background_){
      cout<<"Error in Playground: you asked to project bkg but you didn't setup Palyground to have background included!! Aborting..."<<endl;
      abort();
    }

    if(debug) cout << "Playground::projectionPDF()" << endl;
    RooPlot* plot = varContainer[myVar]->frame(bins);

    if(debug) cout << "RooPlot: " << plot << endl;

    if ( istoy ) {
      if( !toyData ) return kDataEmpty; 
      toyData->plotOn(plot);
    }else{
      if( !data_bkg ) return kDataEmpty;
      data_bkg->plotOn(plot);
    }

    PDF_qqZZ->plotOn(plot);
    
    plot->Draw();

    return kNoError;

  }
};

#endif



