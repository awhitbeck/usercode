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

#ifndef ROO_BPHIONE
#define ROO_BPHIONE

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;
class RooAbsReal;
 
class RooBphione : public RooAbsPdf {
public:
  RooBphione(const char *name, const char *title,
    	     RooAbsReal& _x,
             RooAbsReal& _para2,
             RooAbsReal& _para4,
             RooAbsReal& _acca2,
             RooAbsReal& _acca4,
             RooAbsReal& _acca6,
	     RooAbsReal& _acca8);
  RooBphione(const RooBphione& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooBphione(*this,newname); }
  inline virtual ~RooBphione() { }

protected:

  RooRealProxy x ;
  RooRealProxy para2 ;
  RooRealProxy para4 ;
  RooRealProxy acca2 ;
  RooRealProxy acca4 ;
  RooRealProxy acca6 ;
  RooRealProxy acca8 ;
  
  Double_t evaluate() const ;

private:

 ClassDef(RooBphione,1) // Your description goes here...
};
 
#endif
