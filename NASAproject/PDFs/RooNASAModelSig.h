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

#ifndef ROO_NASASIG
#define ROO_NASASIG

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;
class RooAbsReal;
class TH2F;
class TFile;

 
class RooNASAModelSig : public RooAbsPdf {
public:
  RooNASAModelSig();
  RooNASAModelSig(const char *name, const char *title,
		  RooAbsReal& _mZZ,
		  RooAbsReal& _D,
		  RooAbsReal& _mean,
		  RooAbsReal& _sigma
		  );
  RooNASAModelSig(const RooNASAModelSig& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooNASAModelSig(*this,newname); }
  inline virtual ~RooNASAModelSig() { }

  void setD(TFile* f);

protected:

  RooRealProxy mZZ ;
  RooRealProxy D ;
  RooRealProxy mean ;
  RooRealProxy sigma ;
  
  TH2F* h_mzzD;

  Double_t evaluate() const ;

private:

  ClassDef(RooNASAModelSig,1) 
};
 
#endif
