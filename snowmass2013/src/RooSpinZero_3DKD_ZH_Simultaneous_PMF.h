/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef ROOSPINZERO_3DKD_ZH_SIMULTANEOUS_PMF
#define ROOSPINZERO_3DKD_ZH_SIMULTANEOUS_PMF

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"

 
class RooSpinZero_3DKD_ZH_Simultaneous_PMF : public RooAbsPdf {
public:
  RooSpinZero_3DKD_ZH_Simultaneous_PMF() {} ; 
  RooSpinZero_3DKD_ZH_Simultaneous_PMF(const char *name, const char *title,
		    RooAbsReal& _kd,
		    RooAbsReal& _kd2,
		    RooAbsReal& _kd3,
		    RooAbsReal& _fg2,
		    RooAbsReal& _fg4,
 		    vector<TH3F*>& _histos,
		    bool  _withAcc,
		  bool _fg2_is_constant,
		  bool _fg4_is_constant);
		    
  RooSpinZero_3DKD_ZH_Simultaneous_PMF(const RooSpinZero_3DKD_ZH_Simultaneous_PMF& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooSpinZero_3DKD_ZH_Simultaneous_PMF(*this,newname); }
  inline virtual ~RooSpinZero_3DKD_ZH_Simultaneous_PMF() { }
  
  Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const ;
  Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const ;


  double IntegralT[6];
	
protected:

  RooRealProxy kd ;
  RooRealProxy kd2 ;
  RooRealProxy kd3 ;
  RooRealProxy fg2 ;
  RooRealProxy fg4 ;
  vector<TH3F*> histos;
  bool withAcc;
  bool fg2_is_constant;
  bool fg4_is_constant;
  
    
  Double_t evaluate() const ;

private:

  ClassDef(RooSpinZero_3DKD_ZH_Simultaneous_PMF,1) // Your description goes here...
};
 
#endif
