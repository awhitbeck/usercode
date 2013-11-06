#include "Riostream.h" 
#include "RooSpinZero_KDInt_ZH_Simultaneous_PMF.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <cmath>
#include "TMath.h"
#include "TH2F.h"

using namespace TMath;

ClassImp(RooSpinZero_KDInt_ZH_Simultaneous_PMF) 

  RooSpinZero_KDInt_ZH_Simultaneous_PMF::RooSpinZero_KDInt_ZH_Simultaneous_PMF(const char *name, const char *title, 
				       RooAbsReal& _kd,
				       RooAbsReal& _kdint,
				       RooAbsReal& _fg2,
				       RooAbsReal& _fg4,
  				       vector<TH2F*>& _histos, 
				       bool _withAcc,
						bool _fg2_is_constant,
						bool _fg4_is_constant):
   RooAbsPdf(name,title), 
   kd("kd","kd",this,_kd),
   kdint("kdint","kdint",this,_kdint),
   fg2("fg2","fg2",this,_fg2),
   fg4("fg4","fg4",this,_fg4),
   histos(_histos),
   withAcc(_withAcc),
   fg2_is_constant(_fg2_is_constant),
   fg4_is_constant(_fg4_is_constant)
 { 
  if (histos.size()!=6){
    coutE(InputArguments) << "RooSpinZero_KDInt_ZH_Simultaneous_PMF::RooSpinZero_KDInt_ZH_Simultaneous_PMF(" << GetName() 
			  << ") number of histograms must be 6" << endl ;
    assert(0);
  };
   int nbinsx = histos[0]->GetXaxis()->GetNbins();
   int nbinsy = histos[0]->GetYaxis()->GetNbins();
   int binx_min=1,binx_max=nbinsx,biny_min=1,biny_max=nbinsy;
   for(int th=0;th<6;th++) IntegralT[th]=histos[th]->Integral(binx_min, binx_max, biny_min, biny_max);

//   cout << _kd.GetName() << '\t' << _kdint.GetName() << endl;
//   for(int th=0;th<6;th++) cout << IntegralT[th] << '\t';
   cout << endl;
 }


 RooSpinZero_KDInt_ZH_Simultaneous_PMF::RooSpinZero_KDInt_ZH_Simultaneous_PMF(const RooSpinZero_KDInt_ZH_Simultaneous_PMF& other, const char* name) :  
   RooAbsPdf(other,name), 
   kd("kd",this,other.kd),
   kdint("kdint",this,other.kdint),
   fg2("fg2",this,other.fg2),
   fg4("fg4",this,other.fg4),
   histos(other.histos),
   withAcc(other.withAcc),
   fg2_is_constant(other.fg2_is_constant),
   fg4_is_constant(other.fg4_is_constant)
 { 
   for(int th=0;th<6;th++) IntegralT[th]=other.IntegralT[th];

//   cout << kd.GetName() << '\t' << kdint.GetName() << endl;
//   for(int th=0;th<6;th++) cout << IntegralT[th] << '\t';
   cout << endl;
 } 



 Double_t RooSpinZero_KDInt_ZH_Simultaneous_PMF::evaluate() const 
 { 

   double intval = 0;
   double value = 0;

   int binx =  histos[0]->GetXaxis()->FindBin(kd);
   int biny =  histos[0]->GetYaxis()->FindBin(kdint);
   
   double T[6];
   for(int th=0;th<6;th++) T[th]=histos[th]->GetBinContent(binx, biny);
   
/*   double fg1 = 1.0-fg2-fg4;
   if ( (fg1+fg2+fg4)>1.0) return 0.;
   value = fg1*T[0] + fg2*T[1] + fg4*T[2]
			+ sqrt(fg1*fg2) * T[3]
			+ sqrt(fg1*fg4) * T[4]
			+ sqrt(fg2*fg4) * T[5];
   
   if ( value < 0.0000) return 0.;
*/
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

   
   if (value<0) cout << "WARNING: VALUE<0" << endl;
//   if (value<0 || intval<=0) value=0;
   if (value<0 && intval<0) value=-value;
   if (intval==0) value=0;
   if (value<0 && intval>0) value=0;

   return value ; 
   
 } 

Int_t RooSpinZero_KDInt_ZH_Simultaneous_PMF::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
{

//  cout << "no 4" << endl;
  if (matchArgs(allVars,analVars,kd,kdint)) return 3 ;
//  cout << "no 3" << endl;
  if (matchArgs(allVars,analVars,kd)) return 1 ;
//  cout << "no 2" << endl;
  if (matchArgs(allVars,analVars,kdint)) return 2 ;
//  cout << "no 1" << endl;

  return 0 ;
}

Double_t RooSpinZero_KDInt_ZH_Simultaneous_PMF::analyticalIntegral(Int_t code, const char* /*rangeName*/) const
{

  int nbinsx = histos[0]->GetXaxis()->GetNbins();
  double xMin = histos[0]->GetXaxis()->GetBinLowEdge(1);
  double xMax = histos[0]->GetXaxis()->GetBinUpEdge(nbinsx);
  double dx = (xMax - xMin) / nbinsx; 

  
  int nbinsy = histos[0]->GetYaxis()->GetNbins();
  double yMin = histos[0]->GetYaxis()->GetBinLowEdge(1);
  double yMax = histos[0]->GetYaxis()->GetBinUpEdge(nbinsy);
  double dy = (yMax - yMin) / nbinsy;
  int binx_min=1,binx_max=nbinsx,biny_min=1,biny_max=nbinsy;
  double T[6]={0};
  double value = 0;
 
       // integrate out kd, depend on kdint
     if(code == 1){

	 int biny = histos[0]->GetYaxis()->FindBin(kdint);
	 biny_min=biny;
	 biny_max=biny;
     for(int th=0;th<6;th++) T[th]=histos[th]->Integral(binx_min, binx_max, biny_min, biny_max);
       } // integrate out  kdint, depend on kd
     else if(code == 2){

	 int binx = histos[0]->GetXaxis()->FindBin(kd);
	 binx_min=binx;
	 binx_max=binx;
     for(int th=0;th<6;th++) T[th]=histos[th]->Integral(binx_min, binx_max, biny_min, biny_max);
	   }
	 else if(code == 3){

		 for(int th=0;th<6;th++){T[th]=IntegralT[th];/* cout << T[th] << '\t' << IntegralT[th] << endl;*/};
	   }
	 else{
		   return 0;
	   };
   
/*   double fg1 = 1.0-fg2-fg4;
   if ( (fg1+fg2+fg4)>1.0) return 1.0e40;
   value = fg1*T[0] + fg2*T[1] + fg4*T[2]
				+ sqrt(fg1*fg2) * T[3]
				+ sqrt(fg1*fg4) * T[4]
				+ sqrt(fg2*fg4) * T[5];
*/
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
  if(value==0) cout << "WARNING: INTEGRAL 0" << endl;
  if(value<0) cout << "WARNING: INTEGRAL LT 0" << endl;
  if(value==0) value=-1;

  return value;
}

