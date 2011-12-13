#include <iostream>
#include "RooRealVar.h"
#include "RooBernstein.h"

class backgroundModel{
  
 public:
  backgroundModel();
  backgroundModel(double p1Val_,double p2Val_,double p3Val_,double p4Val_);

  RooRealVar* mZZ;
	     		 
  RooRealVar* p1; 
  RooRealVar* p2; 
  RooRealVar* p3; 
  RooRealVar* p4; 

  RooBernstein* background;
};
