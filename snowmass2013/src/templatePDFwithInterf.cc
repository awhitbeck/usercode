#include "templatePDFwithInterf.h"



templatePDFwithInterf::templatePDFwithInterf(const char *name, const char *title, 
					     RooAbsReal &_x,
					     vector<TH1F*>& _histos, 
					     RooAbsReal& _coef):
  RooAbsPdf(name,title),
  x("x","Independent variable",this,_x),
  coef("coef","Coefficient",this,_coef),
  histos(_histos)
{ 

  if (histos.size()!=3){
    coutE(InputArguments) << "templatePDFwithInterf::templatePDFwithInterf(" << GetName() 
			  << ") number of histograms must be 3" << endl ;
    assert(0);
  }

}

templatePDFwithInterf::templatePDFwithInterf(const templatePDFwithInterf& other, const char* name) :  
  RooAbsPdf(other,name), 
  x("x",this,other.x), 
  coef("coef",this,other.coef),
  histos(other.histos)
{ 
} 


Double_t templatePDFwithInterf::evaluate() const{

  Int_t xbin = histos[0]->GetXaxis()->FindBin(x);
  //Int_t ybin = histos[0]->GetYaxis()->FindBin(y);

  Double_t T1 = histos[0]->GetBinContent(xbin);
  Double_t T2 = histos[1]->GetBinContent(xbin);
  Double_t T3 = histos[2]->GetBinContent(xbin);

  Double_t total = coef*T1 + (1.-coef)*T2 + sqrt((1-coef)*coef)*T3;

  if(total < 1e-30)
    {
      total = 1e-30;
    }

  return total;

}
