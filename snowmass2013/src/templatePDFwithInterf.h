#ifndef TEMPLATEPDFWITHINTERF
#define TEMPLATEPDFWITHINTERF

#include "TH1F.h"
#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"

class templatePDFwithInterf : public RooAbsPdf {
 public:
  templatePDFwithInterf() {} ; 
  templatePDFwithInterf(const char *name, const char *title,
			RooAbsReal& _x,
			vector<TH1F*>& _histos,
			RooAbsReal& _fa3);

  templatePDFwithInterf(const templatePDFwithInterf& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new templatePDFwithInterf(*this,newname); }
  inline virtual ~templatePDFwithInterf() { }

 protected:

  RooRealProxy x ;
  RooRealProxy coef ;
  vector<TH1F*> histos;
  
    
  Double_t evaluate() const ;


};

#endif
