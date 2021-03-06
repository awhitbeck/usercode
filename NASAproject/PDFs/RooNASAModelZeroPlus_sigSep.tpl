#include <iostream>
#include <cmath>
#include "RooNASAModelZeroPlus_sigSep.h"
#include "RooRealVar.h"
#include "RooRealConstant.h"
#include "TFile.h"
#include "TH2F.h"
#include "TMath.h"

using namespace RooFit;

 ClassImp(RooNASAModelZeroPlus_sigSep) 

 RooNASAModelZeroPlus_sigSep::RooNASAModelZeroPlus_sigSep(){}

 RooNASAModelZeroPlus_sigSep::RooNASAModelZeroPlus_sigSep(const char *name, const char *title, 
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

RooNASAModelZeroPlus_sigSep::RooNASAModelZeroPlus_sigSep(const RooNASAModelZeroPlus_sigSep& other, const char* name) :  
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

double RooNASAModelZeroPlus_sigSep::evaluate() const 
 { 

   int binX =(int) floor((mZZ-110)/2);  //check!!!!!!!
   int binY =(int) floor(D/0.01);  //check!!!!!!!!!

   double PmzzD[35][100];

   <INSERT ARRAY>

   double gauss1 = exp(-(mZZ-mean)*(mZZ-mean)/(2*sigma*sigma))/sqrt(2*3.1415*sigma*sigma);
   double gauss2 = frac*exp(-(mZZ-mean2)*(mZZ-mean2)/(2*sigma2*sigma2))/sqrt(2*3.1415*sigma2*sigma2);

   if(PmzzD[binX][binY]==0)
     return(gauss1+gauss2)*.0000001;
   else
     return (gauss1+gauss2)*PmzzD[binX][binY];

 } 

