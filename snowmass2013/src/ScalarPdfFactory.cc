#ifndef SCALAR_PDF_FACTORY
#define SCALAR_PDF_FACTORY

#include "RooSpinZero_7DComplex.h"
#include "AngularPdfFactory.cc"
#include "TF1.h"

class ScalarPdfFactory: public AngularPdfFactory{

public:
    
  RooRealVar* a1Val;  
  RooRealVar* phi1Val;
  RooRealVar* a2Val;  
  RooRealVar* phi2Val;
  RooRealVar* a3Val;  
  RooRealVar* phi3Val;

  RooRealVar* useGTerm;
  RooRealVar* g1Val;
  RooRealVar* g2Val;
  RooRealVar* g3Val;
  RooRealVar* g4Val;
  RooRealVar* g1ValIm;
  RooRealVar* g2ValIm;
  RooRealVar* g3ValIm;
  RooRealVar* g4ValIm;

  TF1* fmZZNorm;
    
  ScalarPdfFactory(RooRealVar* m1,RooRealVar* m2,RooRealVar* hs,RooRealVar* h1,RooRealVar* h2,RooRealVar* Phi,RooRealVar* Phi1,RooRealVar* mZZ){

    // Parameters
    mZ     = new RooRealVar("mZ","mZ",91.188);
    gamZ   = new RooRealVar("gamZ","gamZ",2.5);
           
    a1Val  = new RooRealVar("a1Val","a1Val",0.);
    phi1Val= new RooRealVar("phi1Val","phi1Val",0.);
    a2Val  = new RooRealVar("a2Val","a2Val",0.);
    phi2Val= new RooRealVar("phi2Val","phi2Val",0.);
    a3Val  = new RooRealVar("a3Val","a3Val",0.);
    phi3Val= new RooRealVar("phi3Val","phi3Val",0.);
           
    useGTerm  = new RooRealVar("useGTerm","useGTerm",1.);

    g1Val  = new RooRealVar("g1Val","g1Val",1.);
    g2Val  = new RooRealVar("g2Val","g2Val",0.);
    g3Val  = new RooRealVar("g3Val","g3Val",0.);
    g4Val  = new RooRealVar("g4Val","g4Val",0.);

    g1ValIm  = new RooRealVar("g1ValIm","g1ValIm",0.);
    g2ValIm  = new RooRealVar("g2ValIm","g2ValIm",0.);
    g3ValIm  = new RooRealVar("g3ValIm","g3ValIm",0.);
    g4ValIm  = new RooRealVar("g4ValIm","g4ValIm",0.);

    R1Val  = new RooRealVar("R1Val","R1Val",0.15);
    R2Val  = new RooRealVar("R2Val","R2Val",0.15);

    PDF = new RooSpinZero_7DComplex("PDF","PDF",*m1,*m2,*h1,*h2,*hs,*Phi,*Phi1,*a1Val,*phi1Val,*a2Val,*phi2Val,*a3Val,*phi3Val,*useGTerm,*g1Val,*g2Val,*g3Val,*g4Val,*g1ValIm,*g2ValIm,*g3ValIm,*g4ValIm,*mZ,*gamZ,*mZZ,*R1Val,*R2Val);

  };

  ~ScalarPdfFactory(){

    delete a1Val;
    delete phi1Val;
    delete a2Val;
    delete phi2Val;
    delete a3Val;
    delete phi3Val;
    
    delete useGTerm;

    delete g1Val;
    delete g2Val;
    delete g3Val;
    delete g4Val;

    delete g1ValIm;
    delete g2ValIm;
    delete g3ValIm;
    delete g4ValIm;

    delete fmZZNorm;
  };

  void makeSMHiggs(){
    g1Val->setVal(1.0);
    g2Val->setVal(0.0);
    g3Val->setVal(0.0);
    g4Val->setVal(0.0);

    g1ValIm->setVal(0.0);
    g2ValIm->setVal(0.0);
    g3ValIm->setVal(0.0);
    g4ValIm->setVal(0.0);
    modelIndex=0;

    fmZZNorm=new TF1("fmZZNorm","exp([0]+[1]*x+[2]*x*x+[3]*pow(x,3)+[4]*pow(x,4)+[5]*pow(x,5) +[6]*pow(x,6))",100,180);
    fmZZNorm->FixParameter(0,-3.70118);
    fmZZNorm->FixParameter(1,0.158572);
    fmZZNorm->FixParameter(2,0.000805714);
    fmZZNorm->FixParameter(3,-3.564e-07);
    fmZZNorm->FixParameter(4,-2.74836e-08);
    fmZZNorm->FixParameter(5,-1.31034e-10);
    fmZZNorm->FixParameter(6,9.34302e-13);
  };

  void makeLGHiggs(){          
    g1Val->setVal(0.0);        // need to calculate the proper normalizations
    g2Val->setVal(1.0);
    g3Val->setVal(0.0);
    g4Val->setVal(0.0);

    g1ValIm->setVal(0.0);        // need to calculate the proper normalizations
    g2ValIm->setVal(0.0);
    g3ValIm->setVal(0.0);
    g4ValIm->setVal(0.0);
    modelIndex=2;

    fmZZNorm=new TF1("fmZZNorm","exp([0]+[1]*x+[2]*x*x+[3]*pow(x,3)+[4]*pow(x,4)+[5]*pow(x,5) +[6]*pow(x,6))",100,180);
    fmZZNorm->FixParameter(0,-5.57938);
    fmZZNorm->FixParameter(1,0.146732);
    fmZZNorm->FixParameter(2,0.000835817);
    fmZZNorm->FixParameter(3,3.31867e-07);
    fmZZNorm->FixParameter(4,-2.34111e-08);
    fmZZNorm->FixParameter(5,-1.25844e-10);
    fmZZNorm->FixParameter(6,7.81798e-13);
    };


  void makePSHiggs(){
    g1Val->setVal(0.0);
    g2Val->setVal(0.0);
    g3Val->setVal(0.0);
    g4Val->setVal(1.0);

    g1ValIm->setVal(0.0);
    g2ValIm->setVal(0.0);
    g3ValIm->setVal(0.0);
    g4ValIm->setVal(0.0);
    modelIndex=1;

    fmZZNorm=new TF1("fmZZNorm","exp([0]+[1]*x+[2]*x*x+[3]*pow(x,3)+[4]*pow(x,4)+[5]*pow(x,5)+[6]*pow(x,6))",100,180);
    fmZZNorm->FixParameter(0,-3.66798);
    fmZZNorm->FixParameter(1,0.123215);
    fmZZNorm->FixParameter(2,0.000722998);
    fmZZNorm->FixParameter(3,6.37042e-07);
    fmZZNorm->FixParameter(4,-1.71937e-08);
    fmZZNorm->FixParameter(5,-1.0142e-10);
    fmZZNorm->FixParameter(6,5.38823e-13);
  };

  void makeCustom(double a1, double a2, double a3,
		  double phi1, double phi2, double phi3){

    useGTerm->setVal(-1.0);

    a1Val->setVal(a1);
    phi1Val->setVal(phi1);
    a2Val->setVal(a2);
    phi2Val->setVal(phi2);
    a3Val->setVal(a3);
    phi3Val->setVal(phi3);
    modelIndex=-1;
  };

  void makeParamsConst(bool yesNo=true){
    if(yesNo){
      a1Val->setConstant(kTRUE);
      phi1Val->setConstant(kTRUE);
      a2Val->setConstant(kTRUE);
      phi2Val->setConstant(kTRUE);
      a3Val->setConstant(kTRUE);
      phi3Val->setConstant(kTRUE);
      useGTerm->setConstant(kTRUE);
      g1Val->setConstant(kTRUE);
      g2Val->setConstant(kTRUE);
      g3Val->setConstant(kTRUE);
      g4Val->setConstant(kTRUE);
      g1ValIm->setConstant(kTRUE);
      g2ValIm->setConstant(kTRUE);
      g3ValIm->setConstant(kTRUE);
      g4ValIm->setConstant(kTRUE);
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
      useGTerm->setConstant(kFALSE);
      g1Val->setConstant(kFALSE);
      g2Val->setConstant(kFALSE);
      g3Val->setConstant(kFALSE);
      g4Val->setConstant(kFALSE);
      g1ValIm->setConstant(kFALSE);
      g2ValIm->setConstant(kFALSE);
      g3ValIm->setConstant(kFALSE);
      g4ValIm->setConstant(kFALSE);
      gamZ->setConstant(kFALSE);
      mZ->setConstant(kFALSE);
      R1Val->setConstant(kFALSE);
      R2Val->setConstant(kFALSE);
    }
  };

  double getVal(double mZZ){

    double norm=-999;
    if(mZZ>180 || mZZ<100){
      std::cout << "Normalization is not available for this value of mZZ: I'm extrapolating ..." << mZZ << std::endl;
    }
    norm = fmZZNorm->Eval(mZZ);
    return PDF->getVal()/(4.*3.1415*norm);
  };

  double getValIntegrOutAngles(RooRealVar* m1,RooRealVar* m2,RooRealVar* h1,RooRealVar* h2,RooRealVar* Phi,RooRealVar* mZZ){
    RooAbsPdf* PDFIntegratedOut =PDF->createProjection(RooArgSet(*h1,*h2,*Phi));
    double norm = PDFIntegratedOut->getNorm(RooArgSet(*m1, *m2, *mZZ));
    std::cout<<"norm "<<norm<<std::endl;
    double val = PDFIntegratedOut->getVal()/norm;
    std::cout<<"val "<<val<<std::endl;
   return val;
  }

};

#endif



