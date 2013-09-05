#include "Riostream.h" 
#include "RooSpinZero_KD_ZH.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h>
#include "TMath.h"
#include "TH1F.h"

using namespace TMath;

ClassImp(RooSpinZero_KD_ZH) 

  RooSpinZero_KD_ZH::RooSpinZero_KD_ZH(const char *name, const char *title, 
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
    coutE(InputArguments) << "RooSpinZero_KD_ZH::RooSpinZero_KD_ZH(" << GetName() 
			  << ") number of histograms must be 3" << endl ;
    assert(0);
  }
 
 }
  


 RooSpinZero_KD_ZH::RooSpinZero_KD_ZH(const RooSpinZero_KD_ZH& other, const char* name) :  
   RooAbsPdf(other,name), 
   kd("kd",this,other.kd),
   fa3("fa3",this,other.fa3),
   histos(other.histos),
   withAcc(other.withAcc)
 { 
 } 



 Double_t RooSpinZero_KD_ZH::evaluate() const 
 { 

   double value = 0.;

   int binx =  histos[0]->GetXaxis()->FindBin(kd);
   
   Double_t T1 = histos[0]->GetBinContent(binx);
   Double_t T2 = histos[1]->GetBinContent(binx);
   Double_t T4 = histos[2]->GetBinContent(binx);
   
   value = (1.-fa3) * T1 + fa3 * T2 + sqrt((1.-fa3)*fa3) * T4; 
   
   if ( value < 0.0000) return 0.;
   
   return value ; 
   
 } 

Int_t RooSpinZero_KD_ZH::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
{

  if (matchArgs(allVars,analVars,RooArgSet(*kd.absArg()))) return 1;

  return 0 ;

}


Double_t RooSpinZero_KD_ZH::analyticalIntegral(Int_t code, const char* rangeName) const
{
  
  // from the constants: 
  // " We know we are not efficient
  //   Andrew please help making us efficient" 
  // 
  double Int_T1 = histos[0]->Integral();
  double Int_T2 = histos[1]->Integral();
  double Int_T4 = histos[2]->Integral();
  int nbinsx = histos[0]->GetXaxis()->GetNbins();
  double xMin = histos[0]->GetXaxis()->GetBinLowEdge(1);
  double xMax = histos[0]->GetXaxis()->GetBinUpEdge(nbinsx);
  double dx = (xMax - xMin) / double(nbinsx); 
  
  /*
  std::cout << "Int_T1 = " << Int_T1 << "\n";
  std::cout << "Int_T2 = " << Int_T2 << "\n";
  std::cout << "Int_T4 = " << Int_T4 << "\n";
  std::cout << "dx = " << dx << "\n";
  */
  switch(code)
     {

       // integrate out kd, depend on kdint
     case 1: 
       {
         double integral =  (1.-fa3) * Int_T1 + fa3 * Int_T2 + sqrt((1.-fa3)*fa3) * Int_T4; 
	 integral = integral * dx; 
	 return integral;
       }
     }
}
