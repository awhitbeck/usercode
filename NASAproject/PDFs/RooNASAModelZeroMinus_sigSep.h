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

#ifndef ROO_NASAZEROMINUS_SIGSEP
#define ROO_NASAZEROMINUS_SIGSEP

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "TH2F.h"

class RooRealVar;
class RooAbsReal;
class TH2F;
class TFile;

 
class RooNASAModelZeroMinus_sigSep : public RooAbsPdf {
public:
  RooNASAModelZeroMinus_sigSep();
  RooNASAModelZeroMinus_sigSep(const char *name, const char *title,
		  RooAbsReal& _mZZ,
		  RooAbsReal& _D,
		  RooAbsReal& _mean,
		  RooAbsReal& _sigma,
		  RooAbsReal& _mean2,
		  RooAbsReal& _sigma2,
		  RooAbsReal& _frac
		  );
  RooNASAModelZeroMinus_sigSep(const RooNASAModelZeroMinus_sigSep& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooNASAModelZeroMinus_sigSep(*this,newname); }
  inline virtual ~RooNASAModelZeroMinus_sigSep() { }
  
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
  ClassDef(RooNASAModelZeroMinus_sigSep,1) 
};
 
#endif
