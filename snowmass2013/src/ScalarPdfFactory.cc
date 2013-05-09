#ifndef SCALAR_PDF_FACTORY
#define SCALAR_PDF_FACTORY

#include "RooSpinZero_7DComplex.h"
#include "AngularPdfFactory.cc"
#include "TF1.h"
#include "TMath.h"

class ScalarPdfFactory: public AngularPdfFactory{

public:
    
  RooRealVar* a1Val;  
  RooRealVar* phi1Val;
  RooRealVar* a2Val;  
  RooRealVar* phi2Val;
  RooRealVar* a3Val;  
  RooRealVar* phi3Val;

  int parameterization;
  RooRealVar* g1Val;
  RooRealVar* g2Val;
  RooRealVar* g3Val;
  RooRealVar* g4Val;
  RooRealVar* g1ValIm;
  RooRealVar* g2ValIm;
  RooRealVar* g3ValIm;
  RooRealVar* g4ValIm;
  RooRealVar* fa2;
  RooRealVar* fa3;
  RooRealVar* phia2;
  RooRealVar* phia3;

  RooSpinZero_7DComplex* PDF;

  ScalarPdfFactory(){};
    
  ScalarPdfFactory(RooRealVar* m1,RooRealVar* m2,RooRealVar* hs,RooRealVar* h1,RooRealVar* h2,RooRealVar* Phi,RooRealVar* Phi1,RooRealVar* mZZ, int para) {

    // Parameters
    mZ     = new RooRealVar("mZ","mZ",91.188);
    gamZ   = new RooRealVar("gamZ","gamZ",2.5);
           
    a1Val  = new RooRealVar("a1Val","a1Val",0.);
    phi1Val= new RooRealVar("phi1Val","phi1Val",0.);
    a2Val  = new RooRealVar("a2Val","a2Val",0.);
    phi2Val= new RooRealVar("phi2Val","phi2Val",0.);
    a3Val  = new RooRealVar("a3Val","a3Val",0.);
    phi3Val= new RooRealVar("phi3Val","phi3Val",0.);
           
    parameterization = para;

    g1Val  = new RooRealVar("g1Val","g1Val",1.);
    g2Val  = new RooRealVar("g2Val","g2Val",0.);
    g3Val  = new RooRealVar("g3Val","g3Val",0.);
    g4Val  = new RooRealVar("g4Val","g4Val",0.);

    g1ValIm  = new RooRealVar("g1ValIm","g1ValIm",0.);
    g2ValIm  = new RooRealVar("g2ValIm","g2ValIm",0.);
    g3ValIm  = new RooRealVar("g3ValIm","g3ValIm",0.);
    g4ValIm  = new RooRealVar("g4ValIm","g4ValIm",0.);

    fa2  = new RooRealVar("fa2","f_{g2}",0.,0.,1.0);
    fa3  = new RooRealVar("fa3","f_{g4}",0.,0.,1.0);
    phia2  = new RooRealVar("phia2","#phi_{g2}",0.,-2.*TMath::Pi(),2*TMath::Pi());
    phia3  = new RooRealVar("phia3","#phi_{g4}",0.,-2.*TMath::Pi(),2*TMath::Pi());

    R1Val  = new RooRealVar("R1Val","R1Val",0.15);
    R2Val  = new RooRealVar("R2Val","R2Val",0.15);

    PDF = new RooSpinZero_7DComplex("PDF","PDF",*m1,*m2,*h1,*h2,*hs,*Phi,*Phi1,*a1Val,*phi1Val,*a2Val,*phi2Val,*a3Val,*phi3Val,parameterization,*g1Val,*g2Val,*g3Val,*g4Val,*g1ValIm,*g2ValIm,*g3ValIm,*g4ValIm,*fa2,*fa3,*phia2,*phia3,*mZ,*gamZ,*mZZ,*R1Val,*R2Val);

  };

  ~ScalarPdfFactory(){

    if(a1Val){
      delete a1Val;
      delete phi1Val;
      delete a2Val;
      delete phi2Val;
      delete a3Val;
      delete phi3Val;
      
      delete g1Val;
      delete g2Val;
      delete g3Val;
      delete g4Val;
      
      delete g1ValIm;
      delete g2ValIm;
      delete g3ValIm;
      delete g4ValIm;
      
      delete fa2;
      delete fa3;
      delete phia2;
      delete phia3;
    }

  };

  void makeSMHiggs(){

    parameterization=1;

    g1Val->setVal(1.0);
    g2Val->setVal(0.0);
    g3Val->setVal(0.0);
    g4Val->setVal(0.0);

    g1ValIm->setVal(0.0);
    g2ValIm->setVal(0.0);
    g3ValIm->setVal(0.0);
    g4ValIm->setVal(0.0);

  };

  void makeLGHiggs(){          

    parameterization=1;

    g1Val->setVal(0.0);        // need to calculate the proper normalizations
    g2Val->setVal(1.0);
    g3Val->setVal(0.0);
    g4Val->setVal(0.0);

    g1ValIm->setVal(0.0);        // need to calculate the proper normalizations
    g2ValIm->setVal(0.0);
    g3ValIm->setVal(0.0);
    g4ValIm->setVal(0.0);

    };


  void makePSHiggs(){

    parameterization=1;

    g1Val->setVal(0.0);
    g2Val->setVal(0.0);
    g3Val->setVal(0.0);
    g4Val->setVal(1.0);

    g1ValIm->setVal(0.0);
    g2ValIm->setVal(0.0);
    g3ValIm->setVal(0.0);
    g4ValIm->setVal(0.0);
    
    
  };

  void makeCustom(double a1, double a2, double a3,
		  double phi1, double phi2, double phi3){

    parameterization=0;

    a1Val->setVal(a1);
    phi1Val->setVal(phi1);
    a2Val->setVal(a2);
    phi2Val->setVal(phi2);
    a3Val->setVal(a3);
    phi3Val->setVal(phi3);

  };

  void makeParamsConst(bool yesNo=true){
    if(yesNo){
      a1Val->setConstant(kTRUE);
      phi1Val->setConstant(kTRUE);
      a2Val->setConstant(kTRUE);
      phi2Val->setConstant(kTRUE);
      a3Val->setConstant(kTRUE);
      phi3Val->setConstant(kTRUE);
      g1Val->setConstant(kTRUE);
      g2Val->setConstant(kTRUE);
      g3Val->setConstant(kTRUE);
      g4Val->setConstant(kTRUE);
      g1ValIm->setConstant(kTRUE);
      g2ValIm->setConstant(kTRUE);
      g3ValIm->setConstant(kTRUE);
      g4ValIm->setConstant(kTRUE);
      fa2->setConstant(kTRUE);
      fa3->setConstant(kTRUE);
      phia2->setConstant(kTRUE);
      phia3->setConstant(kTRUE);
      gamZ->setConstant(kTRUE);
      mZ->setConstant(kTRUE);
      R1Val->setConstant(kTRUE);
      R2Val->setConstant(kTRUE);
    }else{
      a1Val->setConstant(kFALSE);
      phi1Val->setConstant(kFALSE);
      a2Val->setConstant(kFALSE);
      phi2Val->setConstant(kFALSE);
      a3Val->setConstant(kFALSE);
      phi3Val->setConstant(kFALSE);
      g1Val->setConstant(kFALSE);
      g2Val->setConstant(kFALSE);
      g3Val->setConstant(kFALSE);
      g4Val->setConstant(kFALSE);
      g1ValIm->setConstant(kFALSE);
      g2ValIm->setConstant(kFALSE);
      g3ValIm->setConstant(kFALSE);
      g4ValIm->setConstant(kFALSE);
      fa2->setConstant(kFALSE);
      fa3->setConstant(kFALSE);
      phia2->setConstant(kFALSE);
      phia3->setConstant(kFALSE);
      gamZ->setConstant(kFALSE);
      mZ->setConstant(kFALSE);
      R1Val->setConstant(kFALSE);
      R2Val->setConstant(kFALSE);
    }
  };

};




#endif



