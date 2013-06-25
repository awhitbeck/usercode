 // Your description goes here... 

#include "Riostream.h" 

#include "RooZZ_3D.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 

#include "TLorentzRotation.h"
#include "TLorentzVector.h"

#include <math.h>
#include "TMath.h"

using namespace TMath;

enum parameterizationList {kMagPhase_As=0,kRealImag_Gs=1,kFracPhase_Gs=2,kNUMparameterizations=3};


 ClassImp(RooZZ_3D) 

 RooZZ_3D::RooZZ_3D(const char *name, const char *title, 
				      RooAbsReal& _h1,
				      RooAbsReal& _h2,
	                              RooAbsReal& _Phi,
	                              RooAbsReal& _h1pol2,
 	                              RooAbsReal& _h1pol4,
 	                              RooAbsReal& _h1pol6,
 	                              RooAbsReal& _h2pol2,
  		                      RooAbsReal& _phiconst,
  		                      RooAbsReal& _twophiconst,
	                              bool _withAcc):
   
   RooAbsPdf(name,title), 
   h1("h1","h1",this,_h1),
   h2("h2","h2",this,_h2),
   Phi("Phi","Phi",this,_Phi),
   h1pol2("h1pol2","h1pol2",this,_h1pol2),
   h1pol4("h1pol4","h1pol4",this,_h1pol4),
   h1pol6("h1pol6","h1pol6",this,_h1pol6),
   h2pol2("h2pol2","h2pol2",this,_h2pol2),
   phiconst("phiconst","phiconst",this,_phiconst),
   twophiconst("twophiconst","twophiconst",this,_twophiconst),
   withAcc(_withAcc)
 { 
 } 


 RooZZ_3D::RooZZ_3D(const RooZZ_3D& other, const char* name) :  
   RooAbsPdf(other,name), 
   h1("h1",this,other.h1),
   h2("h2",this,other.h2),
   Phi("Phi",this,other.Phi),
   h1pol2("h1pol2",this,other.h1pol2),
   h1pol4("h1pol4",this,other.h1pol4),
   h1pol6("h1pol6",this,other.h1pol6),
   h2pol2("h2pol2",this,other.h2pol2),
   phiconst("phiconst",this,other.phiconst),
   twophiconst("twophiconst",this,other.twophiconst),
   withAcc(other.withAcc)
 { 
 } 



 Double_t RooZZ_3D::evaluate() const 
 { 

   double value;
   value += ( 1. + Power(h1,2) * h1pol2 + Power(h1,4)*h1pol4 + Power(h1,6)*h1pol6) * (1 + Power(h2,2) * h2pol2) 
     *(1+cos(Phi)*phiconst+cos(2*Phi)*twophiconst);
   return value ; 
 } 

Int_t RooZZ_3D::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
{

  if (matchArgs(allVars,analVars,RooArgSet(*h1.absArg(),*h2.absArg(),*Phi.absArg()))) return 4 ;
  if (matchArgs(allVars,analVars,h1,h2)) return 1 ;
  if (matchArgs(allVars,analVars,h1,Phi)) return 2 ;
  if (matchArgs(allVars,analVars,h2,Phi)) return 3 ;
  
  return 0 ;
}

Double_t RooZZ_3D::analyticalIntegral(Int_t code, const char* rangeName) const
{
   
 switch(code)
     {

       // projections to phi
     case 1:
       {
	 double value = 0.;
	 
	 value += (2. + 2./3.*h1pol2 + 2./5.*h1pol4 + 2./7.*h1pol6) * (2. + 2./3.*h2pol2) *(1+cos(Phi)*phiconst+cos(2*Phi)*twophiconst);
	 
	 return value;
       }

       // projections to h2
     case 2: 
       {

	 double value = 0.;

	 value += (2. + 2./3.*h1pol2 + 2./5.*h1pol4 + 2./7.*h1pol6) * (1 + Power(h2,2)*h2pol2) * 2*Pi();

	 return value;

       }

       // projections to h1
     case 3: 
       {

	 double value = 0.;

	 value += (1 + Power(h1,2)*h1pol2 + Power(h1,4)*h1pol4 + Power(h1,6)*h1pol6) * (2. + 2./3.*h2pol2) * 2*Pi();

	 return value; 

       }

       // projections to all
     case 4:
       {
	 double value = 0.;
	 
	 value += (2. + 2./3.*h1pol2 + 2./5.*h1pol4 + 2./7.*h1pol6) * (2. + 2./3.*h2pol2) * 2.*Pi();
	 
	 return value;
       }
     }
}



