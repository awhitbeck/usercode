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

#ifndef ROO_BKG2L2JHL
#define ROO_BKG2L2JHL

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooBkg2l2jHL : public RooAbsPdf {
public:
  RooBkg2l2jHL(const char *name, const char *title,
 	        RooAbsReal& _x,
                RooAbsReal& _acca0,
	        RooAbsReal& _acca2,
	        RooAbsReal& _acca4,
		RooAbsReal& _g,
		RooAbsReal& _cutOff);

  RooBkg2l2jHL(const RooBkg2l2jHL& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooBkg2l2jHL(*this,newname); }
  inline virtual ~RooBkg2l2jHL() { }

protected:

  RooRealProxy x ;
  RooRealProxy acca0 ;
  RooRealProxy acca2 ;
  RooRealProxy acca4 ;
  RooRealProxy g ;
  RooRealProxy cutOff ;
  double evaluate() const ;

private:

   ClassDef(RooBkg2l2jHL,1) // Your description goes here...
};
 
#endif
