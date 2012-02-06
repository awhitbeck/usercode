#include "RooRealVar.h"
#include "../PDFs/RooNASAModelSig.cc"
#include "../PDFs/RooNASAModelBkg.cc"
#include "RooAddPdf.h"
#include <cmath>

class NASA2dPdfFactory{

public: 

  RooRealVar* mean;
  RooRealVar* sigma;
  RooRealVar* mean2;
  RooRealVar* sigma2;
  RooRealVar* frac;
  RooRealVar* a0;
  RooRealVar* a1;
  RooRealVar* a2;
  RooRealVar* a3;
  RooRealVar* a4;
  RooRealVar* a5;
  RooRealVar* a6;
  RooRealVar* a7;
  RooRealVar* a8;
  RooRealVar* a9;

  RooNASAModelSig* sigPDF;
  RooNASAModelBkg* bkgPDF;

  RooRealVar* nsig;
  RooRealVar* nbkg;

  RooAddPdf* PDF;

  NASA2dPdfFactory(RooRealVar* mzz, RooRealVar* D){

  frac=new RooRealVar("frac","frac",       6.55633e-01 ,0.0,1.0);  
  mean=new RooRealVar("mean","mean",       1.24779e+02 ,110.,180.);
  mean2=new RooRealVar("mean2","mean2",    1.21585e+02 ,110.,180.);
  sigma=new RooRealVar("sigma","sigma",    2.60320e+00 ,0.,100.);
  sigma2=new RooRealVar("sigma2","sigma2", 6.42654e+00 ,0.,100.);
	 
  a0=new RooRealVar("a0","a0",116.87,100,150);
  a1=new RooRealVar("a1","a1",11.65,5,15);
  a2=new RooRealVar("a2","a2",66.47,50,100);
  a3=new RooRealVar("a3","a3",0.152,0.05,0.5);
  a4=new RooRealVar("a4","a4",185,150,200);
  a5=new RooRealVar("a5","a5",11.7,5,15);
  a6=new RooRealVar("a6","a6",35.8,25,45);
  a7=new RooRealVar("a7","a7",0.13,0.05,0.2);
  a8=new RooRealVar("a8","a8",62.7,25,100);
  a9=new RooRealVar("a9","a9",0.062,0.01,0.1);

  bkgPDF = new RooNASAModelBkg("bkgPDF","bkgPDF",*mzz,*D,*a0,*a1,*a2,*a3,*a4,*a5,*a6,*a7,*a8,*a9);
  sigPDF = new RooNASAModelSig("sigPDF","sigPDF",*mzz,*D,*mean,*sigma,*mean2,*sigma2,*frac);

  nsig = new RooRealVar("nsig","nsig",0.,1000.);
  nbkg = new RooRealVar("nbkg","nbkg",0.,1000.);

  PDF = new RooAddPdf("PDF","PDF",RooArgList(*sigPDF,*bkgPDF),RooArgList(*nsig,*nbkg));

  };

  ~NASA2dPdfFactory(){

    delete frac  ;
    delete mean  ;
    delete mean2 ;
    delete sigma ;
    delete sigma2;
    
    delete a0;
    delete a1;
    delete a2;
    delete a3;
    delete a4;
    delete a5;
    delete a6;
    delete a7;
    delete a8;
    delete a9;

    delete sigPDF;
    delete bkgPDF;
    delete PDF;

  };


  void makeParamsConst(bool yesNo=true){

    if(yesNo){

      frac->setConstant(kTRUE);
      mean->setConstant(kTRUE);
      mean2->setConstant(kTRUE);
      sigma->setConstant(kTRUE);
      sigma2->setConstant(kTRUE);

      a0->setConstant(kTRUE);
      a1->setConstant(kTRUE);
      a2->setConstant(kTRUE);
      a3->setConstant(kTRUE);
      a4->setConstant(kTRUE);
      a5->setConstant(kTRUE);
      a6->setConstant(kTRUE);
      a7->setConstant(kTRUE);
      a8->setConstant(kTRUE);
      a9->setConstant(kTRUE);
      
    }else{

      frac->setConstant(kFALSE);
      mean->setConstant(kFALSE);
      mean2->setConstant(kFALSE);
      sigma->setConstant(kFALSE);
      sigma2->setConstant(kFALSE);
      
      a0->setConstant(kFALSE);
      a1->setConstant(kFALSE);
      a2->setConstant(kFALSE);
      a3->setConstant(kFALSE);
      a4->setConstant(kFALSE);
      a5->setConstant(kFALSE);
      a6->setConstant(kFALSE);
      a7->setConstant(kFALSE);
      a8->setConstant(kFALSE);
      a9->setConstant(kFALSE);

    }

  };


};
