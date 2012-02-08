#include "RooRealVar.h"
#include "../PDFs/RooMzzModelBkg.cc"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include <cmath>

class NASA1dPdfFactory{

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

  RooGaussian *gauss1;
  RooGaussian *gauss2;

  RooAddPdf* sigPDF;
  RooMzzModelBkg* bkgPDF;

  RooRealVar* nsig;
  RooRealVar* nbkg;

  RooAddPdf* PDF;

  NASA1dPdfFactory(RooRealVar* mzz){

    /*

    1  frac         5.88189e-01   3.56710e-02   4.57453e-04   1.77305e-01
    2  mean         1.24896e+02   7.75397e-02   3.56564e-05  -6.11887e-01
    3  mean2        1.22577e+02   2.26384e-01   8.57587e-05  -6.95339e-01
    4  sigma        2.46230e+00   9.25020e-02   5.46590e-05  -1.25566e+00
    5  sigma2       5.53265e+00   1.91025e-01   1.57196e-05  -1.09592e+00


    */


  frac=new RooRealVar("frac","frac",      5.88189e-01 ,0.0,1.0);  
  mean=new RooRealVar("mean","mean",      1.24896e+02 ,110.,180.);
  mean2=new RooRealVar("mean2","mean2",   1.22577e+02 ,110.,180.);
  sigma=new RooRealVar("sigma","sigma",   2.46230e+00 ,0.,100.);
  sigma2=new RooRealVar("sigma2","sigma2",5.53265e+00,0.,100.);

  /*

   1  a0           1.17517e+02   1.55389e-01   1.55272e-03  -3.03991e-01
   2  a1           1.41379e+01   3.13745e-01   2.66039e-02   9.74798e-01
   3  a2           5.00005e+01   9.44461e+00   2.28598e-01  -1.57726e+00
   4  a3           5.08424e-02   3.36876e-01  -1.47334e-03  -1.15736e+00
   5  a4           1.94791e+02   1.28534e+01   1.89374e-03   9.13475e-01
   6  a5           7.55886e+00   3.67183e+00  -1.20772e-03  -5.10057e-01
   7  a6           3.47929e+01   8.41290e+00   1.51704e-02  -2.07122e-02
   8  a7           5.00019e-02   3.44764e-02   1.94145e-04  -1.57800e+00
   9  a8           9.13964e+01   2.01118e+01   7.22309e-03   8.79735e-01
  10  a9           1.00041e-02   2.06845e-02  -1.66212e-04  -1.55736e+00

  */

	 
  a0=new RooRealVar("a0","a0",1.17517e+02,100,150);
  a1=new RooRealVar("a1","a1",1.41379e+01,5,15);
  a2=new RooRealVar("a2","a2",5.00005e+01,50,100);
  a3=new RooRealVar("a3","a3",5.08424e-02,0.05,0.5);
  a4=new RooRealVar("a4","a4",1.94791e+02,150,200);
  a5=new RooRealVar("a5","a5",7.55886e+00,5,15);
  a6=new RooRealVar("a6","a6",3.47929e+01,25,45);
  a7=new RooRealVar("a7","a7",5.00019e-02,0.05,0.2);
  a8=new RooRealVar("a8","a8",9.13964e+01,25,100);
  a9=new RooRealVar("a9","a9",1.00041e-02,0.01,0.1);

  gauss1 = new RooGaussian("gauss1","gauss2",*mzz,*mean,*sigma);
  gauss2 = new RooGaussian("gauss2","gauss2",*mzz,*mean2,*sigma2);

  bkgPDF = new RooMzzModelBkg("bkgPDF","bkgPDF",*mzz,*a0,*a1,*a2,*a3,*a4,*a5,*a6,*a7,*a8,*a9);
  sigPDF = new RooAddPdf("sigPDF","sigPDF",*gauss1,*gauss2,*frac);

  nsig = new RooRealVar("nsig","nsig",0.,1000.);
  nbkg = new RooRealVar("nbkg","nbkg",0.,1000.);
  
  PDF = new RooAddPdf("PDF","PDF",RooArgList(*sigPDF,*bkgPDF),RooArgList(*nsig,*nbkg));

  };

  ~NASA1dPdfFactory(){

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

    delete gauss1;
    delete gauss2;

    delete sigPDF;
    delete bkgPDF;

    delete nsig;
    delete nbkg;
    
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
