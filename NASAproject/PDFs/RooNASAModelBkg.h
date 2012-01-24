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

#ifndef ROO_NASABKG
#define ROO_NASABKG

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;
class RooAbsReal;
class TH2F;
class TFile;

 
class RooNASAModelBkg : public RooAbsPdf {
public:
  RooNASAModelBkg();
  RooNASAModelBkg(const char *name, const char *title,
		  RooAbsReal& _mZZ,
		  RooAbsReal& _D,
		  RooAbsReal& _a1,
		  RooAbsReal& _a2,
		  RooAbsReal& _a3,
		  RooAbsReal& _b1,
		  RooAbsReal& _b2,
		  RooAbsReal& _b3,
		  RooAbsReal& _frac
		  );
  RooNASAModelBkg(const RooNASAModelBkg& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooNASAModelBkg(*this,newname); }
  inline virtual ~RooNASAModelBkg() { }

  void setD(TFile* f);

protected:

  RooRealProxy mZZ ;
  RooRealProxy D ;
  RooRealProxy a1 ;
  RooRealProxy a2 ;
  RooRealProxy a3 ;
  RooRealProxy b1 ;
  RooRealProxy b2 ;
  RooRealProxy b3 ;
  RooRealProxy frac ;
  
  TH2F* h_mzzD;

  Double_t evaluate() const ;

private:

  ClassDef(RooNASAModelBkg,1) 
};
 
#endif
