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
  if (matchArgs(allVars,analVars,RooArgSet(*kd.absArg(), *kdint.absArg()))) return 3 ;
  if (matchArgs(allVars,analVars,kd)) return 1 ;
  if (matchArgs(allVars,analVars,kdint)) return 2 ;

  return 0 ;

}

Double_t RooSpinZero_KDInt_ZH::analyticalIntegral(Int_t code, const char* rangeName) const
{

  int nbinsx = histos[0]->GetXaxis()->GetNbins();
  double binwidthx = histos[0]->GetXaxis()->GetBinWidth(1);
  double xMin = histos[0]->GetXaxis()->GetBinLowEdge(1);
  double xMax = histos[0]->GetXaxis()->GetBinUpEdge(nbinsx);
  double dx = (xMax - xMin) / nbinsx; 

  
  int nbinsy = histos[0]->GetYaxis()->GetNbins();
  double binwidthy = histos[0]->GetYaxis()->GetBinWidth(1);
  double yMin = histos[0]->GetYaxis()->GetBinLowEdge(1);
  double yMax = histos[0]->GetYaxis()->GetBinUpEdge(nbinsy);
  double dy = (yMax - yMin) / nbinsy; 

  /*
  std::cout << "nbinsx = " << nbinsx << ", binwidthx = " << binwidthx << ", xMin = " << xMin << ", xMax = " << xMax << "\n";
  std::cout << "nbinsy = " << nbinsy << ", binwidthy = " << binwidthy << ", yMin = " << yMin << ", yMax = " << yMax << "\n";
  */
  
   switch(code)
     {

       // integrate out kd, depend on kdint
     case 1: 
       {

	 int biny = histos[0]->GetYaxis()->FindBin(kdint);
	 
	 double Int_T1 = histos[0]->Integral(1, nbinsx, biny, biny);
	 double Int_T2 = histos[1]->Integral(1, nbinsx, biny, biny);
	 double Int_T4 = histos[2]->Integral(1, nbinsx, biny, biny);
	 // something related to phase factor, this is by guess
	 double integral = (1.-fa3) * Int_T1 + fa3 * Int_T2; // + sqrt((1.-fa3)*fa3) * Int_T4; ;

	 integral = integral * dx * 2.; 
	  
	 return integral; 
	 
       }
       
       // integrate out  kdint, depend on kd
     case 2: 
       {
	 
	 int binx = histos[0]->GetXaxis()->FindBin(kd);

	 double Int_T1 = histos[0]->Integral(binx, binx, 1, nbinsy);
	 double Int_T2 = histos[1]->Integral(binx, binx, 1, nbinsy);
	 double Int_T4 = histos[2]->Integral(binx, binx, 1, nbinsy);

	 double integral = (1.-fa3) * Int_T1 + fa3 * Int_T2; // + sqrt((1.-fa3)*fa3) * Int_T4; 
	 
	 // something related to phase factor, this is by guess
	 integral = integral * dy * 2.;

	 return integral;
       }
       
     case 3: 
       {
	 double Int_T1 = histos[0]->Integral();
	 double Int_T2 = histos[1]->Integral();
	 double Int_T4 = histos[2]->Integral();

	 double integral = (1.-fa3) * Int_T1 + fa3 * Int_T2 ; //+ sqrt((1.-fa3)*fa3) * Int_T4; ;
	 int nbinsx = histos[0]->GetXaxis()->GetNbins();
	 int nbinsy = histos[0]->GetYaxis()->GetNbins();


	 integral = integral * dx * dy * 4.;

	 // std::cout << __LINE__ << " "<< integral << "\n";

	 return integral;
       }
       
     }
   
   assert(0) ;
   return 0 ;
}

