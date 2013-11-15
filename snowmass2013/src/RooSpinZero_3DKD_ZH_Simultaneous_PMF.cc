#include "Riostream.h" 
#include "RooSpinZero_3DKD_ZH_Simultaneous_PMF.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <cmath>
#include "TMath.h"
#include "TH3F.h"

using namespace TMath;

ClassImp(RooSpinZero_3DKD_ZH_Simultaneous_PMF) 

RooSpinZero_3DKD_ZH_Simultaneous_PMF::RooSpinZero_3DKD_ZH_Simultaneous_PMF(const char *name, const char *title, 
				       RooAbsReal& _kd,
				       RooAbsReal& _kd2,
				       RooAbsReal& _kd3,
				       RooAbsReal& _fg2,
				       RooAbsReal& _fg4,
  				       vector<TH3F*>& _histos, 
				       bool _withAcc,
						bool _fg2_is_constant,
						bool _fg4_is_constant):
   RooAbsPdf(name,title), 
   kd("kd","kd",this,_kd),
   kd2("kd2","kd2",this,_kd2),
   kd3("kd2","kd2",this,_kd3),
   fg2("fg2","fg2",this,_fg2),
   fg4("fg4","fg4",this,_fg4),
   histos(_histos),
   withAcc(_withAcc),
   fg2_is_constant(_fg2_is_constant),
   fg4_is_constant(_fg4_is_constant)
 { 
  if (histos.size()!=6){
    coutE(InputArguments) << "RooSpinZero_3DKD_ZH_Simultaneous_PMF::RooSpinZero_3DKD_ZH_Simultaneous_PMF(" << GetName() 
			  << ") number of histograms must be 6" << endl ;
    assert(0);
  };
   int nbinsx = histos[0]->GetXaxis()->GetNbins();
   int nbinsy = histos[0]->GetYaxis()->GetNbins();
   int nbinsz = histos[0]->GetZaxis()->GetNbins();
   int binx_min=1,binx_max=nbinsx,biny_min=1,biny_max=nbinsy,binz_min=1,binz_max=nbinsz;
   for(int th=0;th<6;th++) IntegralT[th]=histos[th]->Integral(binx_min, binx_max, biny_min, biny_max, binz_min, binz_max);

//   cout << _kd.GetName() << '\t' << _kd2.GetName() << endl;
//   for(int th=0;th<6;th++) cout << IntegralT[th] << '\t';
   cout << endl;
 }


 RooSpinZero_3DKD_ZH_Simultaneous_PMF::RooSpinZero_3DKD_ZH_Simultaneous_PMF(const RooSpinZero_3DKD_ZH_Simultaneous_PMF& other, const char* name) :  
   RooAbsPdf(other,name), 
   kd("kd",this,other.kd),
   kd2("kd2",this,other.kd2),
   kd3("kd2",this,other.kd3),
   fg2("fg2",this,other.fg2),
   fg4("fg4",this,other.fg4),
   histos(other.histos),
   withAcc(other.withAcc),
   fg2_is_constant(other.fg2_is_constant),
   fg4_is_constant(other.fg4_is_constant)
 { 
   for(int th=0;th<6;th++) IntegralT[th]=other.IntegralT[th];

//   cout << kd.GetName() << '\t' << kd2.GetName() << endl;
//   for(int th=0;th<6;th++) cout << IntegralT[th] << '\t';
   cout << endl;
 } 



 Double_t RooSpinZero_3DKD_ZH_Simultaneous_PMF::evaluate() const 
 { 

   double intval = 0;
   double value = 0;

   int binx =  histos[0]->GetXaxis()->FindBin(kd);
   int biny =  histos[0]->GetYaxis()->FindBin(kd2);
   int binz =  histos[0]->GetZaxis()->FindBin(kd3);
   
   double T[6];
   for(int th=0;th<6;th++) T[th]=histos[th]->GetBinContent(binx, biny, binz);
   
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

Int_t RooSpinZero_3DKD_ZH_Simultaneous_PMF::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
{

  if (matchArgs(allVars,analVars,kd,kd2,kd3)) return 1 ;
  if (matchArgs(allVars,analVars,kd2,kd3)) return 2 ;
  if (matchArgs(allVars,analVars,kd,kd3)) return 3 ;
  if (matchArgs(allVars,analVars,kd,kd2)) return 4 ;
  if (matchArgs(allVars,analVars,kd3)) return 5 ;
  if (matchArgs(allVars,analVars,kd2)) return 6 ;
  if (matchArgs(allVars,analVars,kd)) return 7 ;

  return 0 ;
}

Double_t RooSpinZero_3DKD_ZH_Simultaneous_PMF::analyticalIntegral(Int_t code, const char* /*rangeName*/) const
{

  int nbinsx = histos[0]->GetXaxis()->GetNbins();
  int nbinsy = histos[0]->GetYaxis()->GetNbins();
  int nbinsz = histos[0]->GetZaxis()->GetNbins();

  int binx_min=1,binx_max=nbinsx,biny_min=1,biny_max=nbinsy,binz_min=1,binz_max=nbinsz;
  double T[6]={0};
  double value = 0;
 
	 if(code == 1){
		 for(int th=0;th<6;th++){T[th]=IntegralT[th];/* cout << T[th] << '\t' << IntegralT[th] << endl;*/};
	   }
     else if(code == 7){// integrate out kd and kd2, depend on kd3

	 int biny = histos[0]->GetYaxis()->FindBin(kd2);
	 biny_min=biny;
	 biny_max=biny;
	 int binz = histos[0]->GetZaxis()->FindBin(kd3);
	 binz_min=binz;
	 binz_max=binz;
     for(int th=0;th<6;th++) T[th]=histos[th]->Integral(binx_min, binx_max, biny_min, biny_max, binz_min, binz_max);
       }// integrate out kd and kd3, depend on kd2
     else if(code == 6){// integrate out kd and kd2, depend on kd3

	 int binx = histos[0]->GetXaxis()->FindBin(kd);
	 binx_min=binx;
	 binx_max=binx;
	 int binz = histos[0]->GetZaxis()->FindBin(kd3);
	 binz_min=binz;
	 binz_max=binz;
     for(int th=0;th<6;th++) T[th]=histos[th]->Integral(binx_min, binx_max, biny_min, biny_max, binz_min, binz_max);
       }// integrate out kd and kd3, depend on kd2
     else if(code == 5){// integrate out kd and kd2, depend on kd3

	 int binx = histos[0]->GetXaxis()->FindBin(kd);
	 binx_min=binx;
	 binx_max=binx;
	 int biny = histos[0]->GetYaxis()->FindBin(kd2);
	 biny_min=biny;
	 biny_max=biny;
     for(int th=0;th<6;th++) T[th]=histos[th]->Integral(binx_min, binx_max, biny_min, biny_max, binz_min, binz_max);
       }// integrate out kd and kd3, depend on kd2
     else if(code == 4){// integrate out kd and kd2, depend on kd3

	 int binz = histos[0]->GetZaxis()->FindBin(kd3);
	 binz_min=binz;
	 binz_max=binz;
     for(int th=0;th<6;th++) T[th]=histos[th]->Integral(binx_min, binx_max, biny_min, biny_max, binz_min, binz_max);
       }// integrate out kd and kd3, depend on kd2
     else if(code == 3){

	 int biny = histos[0]->GetYaxis()->FindBin(kd2);
	 biny_min=biny;
	 biny_max=biny;
     for(int th=0;th<6;th++) T[th]=histos[th]->Integral(binx_min, binx_max, biny_min, biny_max, binz_min, binz_max);
       } // integrate out  kd2 and kd3, depend on kd
     else if(code == 2){

	 int binx = histos[0]->GetXaxis()->FindBin(kd);
	 binx_min=binx;
	 binx_max=binx;
     for(int th=0;th<6;th++) T[th]=histos[th]->Integral(binx_min, binx_max, biny_min, biny_max, binz_min, binz_max);
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

