/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef ROOSPINZERO_3D_ZH
#define ROOSPINZERO_3D_ZH

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
 
class RooSpinZero_3D_ZH : public RooAbsPdf {
public:
  RooSpinZero_3D_ZH() {} ; 
  RooSpinZero_3D_ZH(const char *name, const char *title,
		    RooAbsReal& _h1,
		    RooAbsReal& _h2,
		    RooAbsReal& _Phi,
		    RooAbsReal& _sqrts,
		    RooAbsReal& _mX,
		    RooAbsReal& _mZ,
		    RooAbsReal& _R1val,
		    RooAbsReal& _R2Val,
		    int _parameterizatiion, 
		    RooAbsReal& _a1Val,
		    RooAbsReal& _phi1Val,
		    RooAbsReal& _a2Val,
		    RooAbsReal& _phi2Val,
		    RooAbsReal& _a3Val,
		    RooAbsReal& _phi3Val,
		    RooAbsReal& _g1Val,
		    RooAbsReal& _g2Val,
		    RooAbsReal& _g3Val,
		    RooAbsReal& _g4Val,
		    RooAbsReal& _g1ValIm,
		    RooAbsReal& _g2ValIm,
		    RooAbsReal& _g3ValIm,
		    RooAbsReal& _g4ValIm,			
		    RooAbsReal& _fa2,
		    RooAbsReal& _fa3,
		    RooAbsReal& _phia2,
		    RooAbsReal& _phia3);
		    
  RooSpinZero_3D_ZH(const RooSpinZero_3D_ZH& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooSpinZero_3D_ZH(*this,newname); }
  inline virtual ~RooSpinZero_3D_ZH() { }
  
  Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const ;
  Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const ;
  
	
protected:

  RooRealProxy h1 ;
  RooRealProxy h2 ;
  RooRealProxy Phi ;
  RooRealProxy sqrts ;
  RooRealProxy mX ;
  RooRealProxy mZ ;
  RooRealProxy R1Val ;
  RooRealProxy R2Val ;
  int parameterization ;
  RooRealProxy a1Val ;
  RooRealProxy phi1Val ;
  RooRealProxy a2Val ;
  RooRealProxy phi2Val ;
  RooRealProxy a3Val ;
  RooRealProxy phi3Val ;
  RooRealProxy g1Val ;
  RooRealProxy g2Val ;
  RooRealProxy g3Val ;
  RooRealProxy g4Val ;
  RooRealProxy g1ValIm ;
  RooRealProxy g2ValIm ;
  RooRealProxy g3ValIm ;
  RooRealProxy g4ValIm ;
  RooRealProxy fa2;
  RooRealProxy fa3;
  RooRealProxy phia2;
  RooRealProxy phia3;

    
  Double_t evaluate() const ;

private:

  ClassDef(RooSpinZero_3D_ZH,1) // Your description goes here...
};
 
#endif
