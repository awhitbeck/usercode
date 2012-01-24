#include <iostream>
#include <math.h>

#include "RooNASAModelBkg.h"
#include "RooRealVar.h"
#include "RooRealConstant.h"
#include "TFile.h"
#include "TH2F.h"
#include "TMath.h"

using namespace RooFit;

 ClassImp(RooNASAModelBkg) 

 RooNASAModelBkg::RooNASAModelBkg(){}

 RooNASAModelBkg::RooNASAModelBkg(const char *name, const char *title, 
				  RooAbsReal& _mZZ,
				  RooAbsReal& _D,
				  RooAbsReal& _a1,
				  RooAbsReal& _a2,
				  RooAbsReal& _a3,
				  RooAbsReal& _b1,
				  RooAbsReal& _b2,
				  RooAbsReal& _b3,
				  RooAbsReal& _frac) :
   RooAbsPdf(name,title), 
   mZZ("mZZ","mZZ",this,_mZZ),
   D("D","D",this,_D),
   a1("a1","a1",this,_a1),
   a2("a2","a2",this,_a2),
   a3("a3","a3",this,_a3),
   b1("b1","b1",this,_b1),
   b2("b2","b2",this,_b2),
   b3("b3","b3",this,_b3),
   frac("frac","frac",this,_frac)
 { 

 } 

RooNASAModelBkg::RooNASAModelBkg(const RooNASAModelBkg& other, const char* name) :  
   RooAbsPdf(other,name), 
   mZZ("mZZ",this,other.mZZ),
   D("D",this,other.D),
   a1("a1",this,other.a1),
   a2("a2",this,other.a2),
   a3("a3",this,other.a3),
   b1("b1",this,other.b1),
   b2("b2",this,other.b2),
   b3("b3",this,other.b3),
   frac("frac","frac",this,other.frac)
 { 
   
 }

void RooNASAModelBkg::setD(TFile* f){
  h_mzzD= (TH2F*)(f->Get("h_mzzD"));
}

double RooNASAModelBkg::evaluate() const 
 { 
   double PmzzD = h_mzzD->GetBinContent(h_mzzD->FindBin(mZZ,D));

   double signa = 0.;
   if ((mZZ-a1) > 0) { signa = 1.; }
   else if ((mZZ-a1) < 0) { signa = -1.; }
   else { signa = 0.; }
   
   double signb = 0.;
   if ((mZZ-b1) > 0) { signb = 1.; }
   else if ((mZZ-b1) < 0) { signb = -1.; }
   else { signb = 0.; }
   
   double bkglo = (0.5 + 0.5*signa * TMath::Erf(TMath::Abs(mZZ-a1)/a2)) * exp(-1.*mZZ/a3);
   double bkghi = (0.5 + 0.5*signb * TMath::Erf(TMath::Abs(mZZ-b1)/b2)) * exp(-1.*mZZ/b3);
   double total = bkglo*frac + (1-frac)*bkghi;
   
   double dynamicKqq = 1. + mZZ*0.001074 + mZZ*mZZ*-7.851e-07;
   
   double totalNLO = total*dynamicKqq;
  
   return totalNLO*PmzzD;
 } 

