#include "Riostream.h" 
#include "RooSpinZero_KD_ZH_Simultaneous_PMF.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <cmath>
#include "TMath.h"
#include "TH1F.h"

using namespace TMath;

ClassImp(RooSpinZero_KD_ZH_Simultaneous_PMF) 

  RooSpinZero_KD_ZH_Simultaneous_PMF::RooSpinZero_KD_ZH_Simultaneous_PMF(const char *name, const char *title, 
				       RooAbsReal& _kd,
				       RooAbsReal& _fg2,
				       RooAbsReal& _fg4,
  				       vector<TH1F*>& _histos, 
				       bool _withAcc,
						bool _fg2_is_constant,
						bool _fg4_is_constant):
   RooAbsPdf(name,title), 
   kd("kd","kd",this,_kd),
   fg2("fg2","fg2",this,_fg2),
   fg4("fg4","fg4",this,_fg4),
   histos(_histos),
   withAcc(_withAcc),
   fg2_is_constant(_fg2_is_constant),
   fg4_is_constant(_fg4_is_constant)
 { 
  if (histos.size()!=6){
    coutE(InputArguments) << "RooSpinZero_KD_ZH_Simultaneous_PMF::RooSpinZero_KD_ZH_Simultaneous_PMF(" << GetName() 
			  << ") number of histograms must be 6" << endl ;
    assert(0);
  };
   int nbinsx = histos[0]->GetXaxis()->GetNbins();
   int binx_min=1,binx_max=nbinsx;
   for(int th=0;th<6;th++) IntegralT[th]=histos[th]->Integral(binx_min, binx_max);
 }
  


 RooSpinZero_KD_ZH_Simultaneous_PMF::RooSpinZero_KD_ZH_Simultaneous_PMF(const RooSpinZero_KD_ZH_Simultaneous_PMF& other, const char* name) :  
   RooAbsPdf(other,name), 
   kd("kd",this,other.kd),
   fg2("fg2",this,other.fg2),
   fg4("fg4",this,other.fg4),
   histos(other.histos),
   withAcc(other.withAcc),
   fg2_is_constant(other.fg2_is_constant),
   fg4_is_constant(other.fg4_is_constant)
 { 
   for(int th=0;th<6;th++) IntegralT[th]=other.IntegralT[th];
 } 



 Double_t RooSpinZero_KD_ZH_Simultaneous_PMF::evaluate() const 
 { 

   double intval = 0;
   double value = 0;

   int binx =  histos[0]->GetXaxis()->FindBin(kd);
   
   double T[6];
   for(int th=0;th<6;th++) T[th]=histos[th]->GetBinContent(binx);
   
   double f2 = abs(fg2)*(1.0 - abs(fg4));
   double f4 = abs(fg2)*abs(fg4);
   if(fg2_is_constant || fg4_is_constant){
	   f2 = abs(fg2);
	   f4 = abs(fg4);
   };
   double f1 = 1.0 - f2 - f4;

    value = f1*T[0] + f2*T[1] + f4*T[2];
	if( fg2!=0 ) value += sqrt(f1*f2) * T[3] * (fg2/abs(fg2));
	if( fg4!=0 ) value += sqrt(f1*f4) * T[4] * (fg4/abs(fg4));
	if( fg2!=0 && fg4!=0 ) value += sqrt(f2*f4) * T[5] * (fg4/abs(fg4)) * (fg2/abs(fg2));

	intval = f1*IntegralT[0] + f2*IntegralT[1] + f4*IntegralT[2];
	if( fg2!=0 ) intval += sqrt(f1*f2) * IntegralT[3] * (fg2/abs(fg2));
	if( fg4!=0 ) intval += sqrt(f1*f4) * IntegralT[4] * (fg4/abs(fg4));
	if( fg2!=0 && fg4!=0 ) intval += sqrt(f2*f4) * IntegralT[5] * (fg4/abs(fg4)) * (fg2/abs(fg2));
   
//   if ( value < 0.0000) return 1.0e30;
   if (value<0 && intval<0) value=-value;
   if (intval==0) value=0;
   if (value<0 && intval>0) value=0;
   
   return value ; 
   
 } 

Int_t RooSpinZero_KD_ZH_Simultaneous_PMF::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
{

  if (matchArgs(allVars,analVars,RooArgSet(*kd.absArg()))) return 1;

  return 0 ;
}


Double_t RooSpinZero_KD_ZH_Simultaneous_PMF::analyticalIntegral(Int_t code, const char* rangeName) const
{
  double value = 0.;
  int nbinsx = histos[0]->GetNbinsX();
  double xMin = histos[0]->GetXaxis()->GetBinLowEdge(1);
  double xMax = histos[0]->GetXaxis()->GetBinUpEdge(nbinsx);
  double dx = (xMax - xMin) / ((double) nbinsx); 

   double T[6];
   for(int th=0;th<6;th++){T[th]=IntegralT[th];/* cout << T[th] << '\t' << IntegralT[th] << endl;*/};
   
   double f2 = abs(fg2)*(1.0 - abs(fg4));
   double f4 = abs(fg2)*abs(fg4);
   if(fg2_is_constant || fg4_is_constant){
	   f2 = abs(fg2);
	   f4 = abs(fg4);
   };
   double f1 = 1.0 - f2 - f4;

    value = f1*T[0] + f2*T[1] + f4*T[2];
	if( fg2!=0 ) value += sqrt(f1*f2) * T[3] * (fg2/abs(fg2));
	if( fg4!=0 ) value += sqrt(f1*f4) * T[4] * (fg4/abs(fg4));
	if( fg2!=0 && fg4!=0 ) value += sqrt(f2*f4) * T[5] * (fg4/abs(fg4)) * (fg2/abs(fg2));

//  for(int o=0;o<6;o++) cout << T[o] << '\t';
//  cout << value << endl;
  if(value<0) value=-value;
  if(value==0) value=-1;
  switch(code)
     {
     case 1: 
       {
	 return value;
       }
	 default:
       {
		   assert(0);
		   return 0;
	   }       
     }
}
