#include "Riostream.h" 
#include "RooSpinZero_KDInt_ZH.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h>
#include "TMath.h"
#include "TH1F.h"

using namespace TMath;

ClassImp(RooSpinZero_KDInt_ZH) 

  RooSpinZero_KDInt_ZH::RooSpinZero_KDInt_ZH(const char *name, const char *title, 
				       RooAbsReal& _kd,
				       RooAbsReal& _fa3,
  				       vector<TH1F*>& _histos, 
				       bool      _withAcc):
   RooAbsPdf(name,title), 
   kd("kd","kd",this,_kd),
   fa3("fa3","fa3",this,_fa3),
   histos(_histos),
   withAcc(_withAcc)
 { 
  if (histos.size()!=3){
    coutE(InputArguments) << "RooSpinZero_KDInt_ZH::RooSpinZero_KDInt_ZH(" << GetName() 
			  << ") number of histograms must be 3" << endl ;
    assert(0);
  }
 } 


 RooSpinZero_KDInt_ZH::RooSpinZero_KDInt_ZH(const RooSpinZero_KDInt_ZH& other, const char* name) :  
   RooAbsPdf(other,name), 
   kd("kd",this,other.kd),
   fa3("fa3",this,other.fa3),
   histos(other.histos),
   withAcc(other.withAcc)
 { 
 } 



 Double_t RooSpinZero_KDInt_ZH::evaluate() const 
 { 

   double value = 0.;

   Double_t T1 = histos[0]->GetBinContent(histos[0]->FindBin(kd));
   Double_t T2 = histos[1]->GetBinContent(histos[1]->FindBin(kd));
   
   value = (1.-fa3) * T1 + fa3 * T2; 

   return value ; 
   
 } 

Int_t RooSpinZero_KDInt_ZH::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
{

  if (matchArgs(allVars,analVars,RooArgSet(*kd.absArg()))) return 1 ;
  return 0 ;
}

Double_t RooSpinZero_KDInt_ZH::analyticalIntegral(Int_t code, const char* rangeName) const
{
   
   switch(code)
     {
     case 1: 
       return 1;
     }
   
   assert(0) ;
   return 0 ;
}

