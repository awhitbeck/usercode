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

#ifndef ROO_NASAZEROPLUS_SIGSEP
#define ROO_NASAZEROPLUS_SIGSEP

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "TH2F.h"

class RooRealVar;
class RooAbsReal;
class TH2F;
class TFile;

 
class RooNASAModelZeroPlus_sigSep : public RooAbsPdf {
public:
  RooNASAModelZeroPlus_sigSep();
  RooNASAModelZeroPlus_sigSep(const char *name, const char *title,
		  RooAbsReal& _mZZ,
		  RooAbsReal& _D,
		  RooAbsReal& _mean,
		  RooAbsReal& _sigma,
		  RooAbsReal& _mean2,
		  RooAbsReal& _sigma2,
		  RooAbsReal& _frac
		  );
  RooNASAModelZeroPlus_sigSep(const RooNASAModelZeroPlus_sigSep& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooNASAModelZeroPlus_sigSep(*this,newname); }
  inline virtual ~RooNASAModelZeroPlus_sigSep() { }
  
protected:

  RooRealProxy mZZ ;
  RooRealProxy D ;
  RooRealProxy mean ;
  RooRealProxy sigma ;
  RooRealProxy mean2 ;
  RooRealProxy sigma2 ;
  RooRealProxy frac ;
  
  Double_t evaluate() const ;

private:
  ClassDef(RooNASAModelZeroPlus_sigSep,1) 
};
 
#endif
