#include "Riostream.h" 
#include "RooSpinZero_KDInt_ZH.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h>
#include "TMath.h"
#include "TH2F.h"

using namespace TMath;

ClassImp(RooSpinZero_KDInt_ZH) 

  RooSpinZero_KDInt_ZH::RooSpinZero_KDInt_ZH(const char *name, const char *title, 
				       RooAbsReal& _kd,
				       RooAbsReal& _kdint,
				       RooAbsReal& _fa3,
  				       vector<TH2F*>& _histos, 
				       bool      _withAcc):
   RooAbsPdf(name,title), 
   kd("kd","kd",this,_kd),
   kdint("kdint","kdint",this,_kdint),
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
   kdint("kdint",this,other.kdint),
   fa3("fa3",this,other.fa3),
   histos(other.histos),
   withAcc(other.withAcc)
 { 
 } 



 Double_t RooSpinZero_KDInt_ZH::evaluate() const 
 { 

   double value = 0.;

   int binx =  histos[0]->GetXaxis()->FindBin(kd);
   int biny =  histos[0]->GetYaxis()->FindBin(kdint);
   
   Double_t T1 = histos[0]->GetBinContent(binx, biny);
   Double_t T2 = histos[1]->GetBinContent(binx, biny);
   Double_t T4 = histos[2]->GetBinContent(binx, biny);
   
   value = (1.-fa3) * T1 + fa3 * T2; //  + sqrt((1.-fa3)*fa3) * T4; 
   
   return value ; 
   
 } 

Int_t RooSpinZero_KDInt_ZH::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
{
  /*
  if (matchArgs(allVars,analVars,RooArgSet(*kd.absArg(), *kdint.absArg()))) return 3 ;
  if (matchArgs(allVars,analVars,kd)) return 1 ;
  if (matchArgs(allVars,analVars,kdint)) return 2 ;
  */
  return 0 ;

}

Double_t RooSpinZero_KDInt_ZH::analyticalIntegral(Int_t code, const char* rangeName) const
{
   
   switch(code)
     {

       // integrate out the kdint
     case 1: 
       {

	 int binx = histos[0]->GetXaxis()->FindBin(kd);
	 int nbinsy = histos[0]->GetYaxis()->GetNbins();

	 double Int_T1 = histos[0]->Integral(binx, binx, 0, nbinsy);
	 double Int_T2 = histos[1]->Integral(binx, binx, 0, nbinsy);
	 double Int_T4 = histos[2]->Integral(binx, binx, 0, nbinsy);

	 return (1.-fa3) * Int_T1 + fa3 * Int_T2; // + sqrt((1.-fa3)*fa3) * Int_T4; ;
	 
       }
       
       // integrate out the kd
     case 2: 
       {

	 int biny = histos[0]->GetYaxis()->FindBin(kdint);
	 int nbinsx = histos[0]->GetXaxis()->GetNbins();

	 double Int_T1 = histos[0]->Integral(0, nbinsx, biny, biny);
	 double Int_T2 = histos[1]->Integral(0, nbinsx, biny, biny);
	 double Int_T4 = histos[2]->Integral(0, nbinsx, biny, biny);

	 return (1.-fa3) * Int_T1 + fa3 * Int_T2; // + sqrt((1.-fa3)*fa3) * Int_T4; ;
       }
       
     case 3: 
       {
	 double Int_T1 = histos[0]->Integral();
	 double Int_T2 = histos[1]->Integral();
	 double Int_T4 = histos[2]->Integral();
	 return (1.-fa3) * Int_T1 + fa3 * Int_T2 ; //+ sqrt((1.-fa3)*fa3) * Int_T4; ;
       }
       
     }
   
   assert(0) ;
   return 0 ;
}

