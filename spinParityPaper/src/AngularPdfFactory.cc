#ifndef ANGULAR_PDF_FACTORY
#define ANGULAR_PDF_FACTORY

#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include <cmath>

class AngularPdfFactory{

public:

  RooRealVar* mZ;     
  RooRealVar* gamZ;   
    
  RooRealVar* R1Val;  
  RooRealVar* R2Val;  
  
  RooAbsPdf *PDF;

  int modelIndex;  

  AngularPdfFactory(){};

  ~AngularPdfFactory(){

    delete mZ;
    delete gamZ;

    delete R1Val;
    delete R2Val;
    
    delete PDF;

  };

  double getVal(double mZZ){ return -99.0*mZZ/mZZ; };

};

#endif

