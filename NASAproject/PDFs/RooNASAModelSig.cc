#include <iostream>
#include <cmath>
#include "RooNASAModelSig.h"
#include "RooRealVar.h"
#include "RooRealConstant.h"
#include "TFile.h"
#include "TH2F.h"
#include "TMath.h"

using namespace RooFit;

 ClassImp(RooNASAModelSig) 

 RooNASAModelSig::RooNASAModelSig(){}

 RooNASAModelSig::RooNASAModelSig(const char *name, const char *title, 
				  RooAbsReal& _mZZ,
				  RooAbsReal& _D,				  
				  RooAbsReal& _mean,
				  RooAbsReal& _sigma,
				  RooAbsReal& _mean2,
				  RooAbsReal& _sigma2,
				  RooAbsReal& _frac
				  ) :
   RooAbsPdf(name,title), 
   mZZ("mZZ","mZZ",this,_mZZ),
   D("D","D",this,_D),
   mean("mean","mean",this,_mean),
   sigma("sigma","sigma",this,_sigma),
   mean2("mean2","mean2",this,_mean2),
   sigma2("sigma2","sigma2",this,_sigma2),
   frac("frac","frac",this,_frac)
 { 

 } 

RooNASAModelSig::RooNASAModelSig(const RooNASAModelSig& other, const char* name) :  
   RooAbsPdf(other,name), 
   mZZ("mZZ",this,other.mZZ),
   D("D",this,other.D),
   mean("mean",this,other.mean),
   sigma("sigma",this,other.sigma),
   mean2("mean2",this,other.mean2),
   sigma2("sigma2",this,other.sigma2),
   frac("frac",this,other.frac)
 { 
   
 }

void RooNASAModelSig::setD(TFile* f){
  h_mzzD = (TH2F*)(f->Get("h_mzzD"));
}


double RooNASAModelSig::evaluate() const 
 { 

   
   double PmzzD = h_mzzD->GetBinContent(h_mzzD->Find(mZZ,D));

   double gauss1 = frac*exp(-(mZZ-mean)*(mZZ-mean)/(2*sigma*sigma))/sqrt(2*3.1415*sigma*sigma);
   double gauss2 = exp(-pow(mZZ-mean2,2)/(2*sigma2*sigma2))/sqrt(2*3.1415*sigma2*sigma2);

   return (gauss1+gauss2)*PmzzD;

 } 

