/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/

#ifndef ROO_BTHETA
#define ROO_BTHETA

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;
class RooAbsReal;
 
class RooBtheta : public RooAbsPdf {
public:
  RooBtheta(const char *name, const char *title,
 	    RooAbsReal& _x,
            RooAbsReal& _para2,
            RooAbsReal& _para4,
	    RooAbsReal& _acca2,
	    RooAbsReal& _acca4);
  RooBtheta(const RooBtheta& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooBtheta(*this,newname); }
  inline virtual ~RooBtheta() { }

protected:

  RooRealProxy x ;
  RooRealProxy para2 ;
  RooRealProxy para4 ;
  RooRealProxy acca2 ;
  RooRealProxy acca4 ;
  
  Double_t evaluate() const ;

private:

  ClassDef(RooBtheta,1) // Your description goes here...
};
 
#endif
