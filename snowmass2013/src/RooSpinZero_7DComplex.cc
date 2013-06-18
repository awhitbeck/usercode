/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
 * This code was autogenerated by RooClassFactory                            *
 *****************************************************************************/

// Your description goes here...

#include "Riostream.h"

#include "RooSpinZero_7DComplex.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include <math.h>
#include "TMath.h"

using namespace TMath;

enum parameterizationList {kMagPhase_As=0,kRealImag_Gs=1,kFracPhase_Gs=2,kNUMparameterizations=3};

RooSpinZero_7DComplex::RooSpinZero_7DComplex(const char *name, const char *title,
					     RooAbsReal& _m1,
					     RooAbsReal& _m2,
					     RooAbsReal& _h1,
					     RooAbsReal& _h2,
					     RooAbsReal& _hs,
					     RooAbsReal& _Phi,
					     RooAbsReal& _Phi1,
					     RooAbsReal& _a1Val,
					     RooAbsReal& _phi1Val,
					     RooAbsReal& _a2Val,
					     RooAbsReal& _phi2Val,
					     RooAbsReal& _a3Val,
					     RooAbsReal& _phi3Val,
					     int _parameterization,
					     RooAbsReal& _g1Val,
					     RooAbsReal& _g2Val,
					     RooAbsReal& _g3Val,
					     RooAbsReal& _g4Val,
					     RooAbsReal& _g1ValIm,
					     RooAbsReal& _g2ValIm,
					     RooAbsReal& _g3ValIm,
					     RooAbsReal& _g4ValIm,
					     RooAbsReal& _fa2,
					     RooAbsReal& _fa3,
					     RooAbsReal& _phia2,
					     RooAbsReal& _phia3,
					     RooAbsReal& _mZ,
					     RooAbsReal& _gamZ,
					     RooAbsReal& _mX,
					     RooAbsReal& _R1Val,
					     RooAbsReal& _R2Val) :
  RooAbsPdf(name,title),
  m1("m1","m1",this,_m1),
  m2("m2","m2",this,_m2),
  h1("h1","h1",this,_h1),
  h2("h2","h2",this,_h2),
  hs("hs","hs",this,_hs),
  Phi("Phi","Phi",this,_Phi),
  Phi1("Phi1","Phi1",this,_Phi1),
  a1Val("a1Val","a1Val",this,_a1Val),
  phi1Val("phi1Val","phi1Val",this,_phi1Val),
  a2Val("a2Val","a2Val",this,_a2Val),
  phi2Val("phi2Val","phi2Val",this,_phi2Val),
  a3Val("a3Val","a3Val",this,_a3Val),
  phi3Val("phi3Val","phi3Val",this,_phi3Val),
  parameterization(_parameterization),
  g1Val("g1Val","g1Val",this,_g1Val),
  g2Val("g2Val","g2Val",this,_g2Val),
  g3Val("g3Val","g3Val",this,_g3Val),
  g4Val("g4Val","g4Val",this,_g4Val),
  g1ValIm("g1ValIm","g1ValIm",this,_g1ValIm),
  g2ValIm("g2ValIm","g2ValIm",this,_g2ValIm),
  g3ValIm("g3ValIm","g3ValIm",this,_g3ValIm),
  g4ValIm("g4ValIm","g4ValIm",this,_g4ValIm),

  fa2("fa2","fa2",this,_fa2),
  fa3("fa3","fa3",this,_fa3),
  phia2("phia2","phia2",this,_phia2),
  phia3("phia3","phia3",this,_phia3),

  mZ("mZ","mZ",this,_mZ),
  gamZ("gamZ","gamZ",this,_gamZ),
  mX("mX","mX",this,_mX),
  R1Val("R1Val","R1Val",this,_R1Val),
  R2Val("R2Val","R2Val",this,_R2Val)
{
}
  

RooSpinZero_7DComplex::RooSpinZero_7DComplex(const RooSpinZero_7DComplex& other, const char* name) :
  RooAbsPdf(other,name),
  m1("m1",this,other.m1),
  m2("m2",this,other.m2),
  h1("h1",this,other.h1),
  h2("h2",this,other.h2),
  hs("hs",this,other.hs),
  Phi("Phi",this,other.Phi),
  Phi1("Phi1",this,other.Phi1),
  a1Val("a1Val",this,other.a1Val),
  phi1Val("phi1Val",this,other.phi1Val),
  a2Val("a2Val",this,other.a2Val),
  phi2Val("phi2Val",this,other.phi2Val),
  a3Val("a3Val",this,other.a3Val),
  phi3Val("phi3Val",this,other.phi3Val),
  parameterization(other.parameterization),
  g1Val("g1Val",this,other.g1Val),
  g2Val("a2Val",this,other.g2Val),
  g3Val("g3Val",this,other.g3Val),
  g4Val("g4Val",this,other.g4Val),
  g1ValIm("g1ValIm",this,other.g1ValIm),
  g2ValIm("a2ValIm",this,other.g2ValIm),
  g3ValIm("g3ValIm",this,other.g3ValIm),
  g4ValIm("g4ValIm",this,other.g4ValIm),

  fa2("fa2",this,other.fa2),
  fa3("fa3",this,other.fa3),
  phia2("phia2",this,other.phia2),
  phia3("phia3",this,other.phia3),

  mZ("mZ",this,other.mZ),
  gamZ("gamZ",this,other.gamZ),
  mX("mX",this,other.mX),
  R1Val("R1Val",this,other.R1Val),
  R2Val("R2Val",this,other.R2Val)
{
}



Double_t RooSpinZero_7DComplex::evaluate() const
{

  // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE
  //   const Double_t VEV = 246.;
  Double_t sh1 = sqrt(1-h1*h1);
  Double_t sh2 = sqrt(1-h2*h2);
    
  bool isZZ = true;
  if ( mZ < 90.) isZZ = false;
  if ( isZZ ) {
    if( (m1+m2) > mX || m2>m1 ) return 1e-9;
  } else {
    if( (m1+m2) > mX ) return 1e-9;
  }
  double nanval = sqrt((1 - TMath::Power(m1 - m2,2)/TMath::Power(mX,2))*(1 - TMath::Power(m1 + m2,2)/TMath::Power(mX,2)));
  if (nanval != nanval) return 1e-9;
    
  double s=(mX*mX-m1*m1-m2*m2)/2.;
  double kappa=s/(1000*1000);
    
  double a1=0,a2=0,a3=0,a1Im=0,a2Im=0,a3Im=0;
  double g1(1.0), g1Im(0.), g2(0.), g2Im(0.), g3(0.), g3Im(0.), g4(0.), g4Im(0.);

  if(parameterization==kMagPhase_As){
    a1=a1Val;
    a1Im=phi1Val;
    a2=a2Val;
    a2Im=phi2Val;
    a3=a3Val;
    a3Im=phi3Val;
  } else {

    // 
    // Important, the PDF has an emphirical fix
    // Forcing the couplings to be its conjugate 
    // to agree with the generator shapes
    // 
    if(parameterization==kFracPhase_Gs){
      nanval = sqrt(1 - fa2 - fa3);
      if (nanval != nanval) return 1e-9;

      // convert fraction and phase to g1,g2...etc
      double sigma_1=1.860351; // was 2.03971 at 126 GeV
      double sigma_2=0.672859; // was 0.77517 at 126 GeV
      double sigma_4=0.284353; // was 0.32689 at 126 GeV
      
      double g1Mag = 1.;
      double g2Mag = sqrt(fa2/(1.-fa2-fa3))*sqrt(sigma_1/sigma_2); 
      double g4Mag = sqrt(fa3/(1.-fa2-fa3))*sqrt(sigma_1/sigma_4); 
      
      g1   = g1Mag;
      g1Im = 0.0;
      g2   = g2Mag*cos(phia2);
      g2Im = - g2Mag*sin(phia2);
      g3   = 0.0;
      g3Im = 0.0;
      g4   = g4Mag*cos(phia3);
      g4Im = - g4Mag*sin(phia3);
      
    }else if(parameterization==kRealImag_Gs){
      
      g1   = g1Val;
      g1Im = - g1ValIm;
      g2   = g2Val;
      g2Im = - g2ValIm;
      g3   = g3Val;
      g3Im = - g3ValIm;
      g4   = g4Val;
      g4Im = - g4ValIm;
    }
    
    a1 = g1*mZ*mZ/(mX*mX) + g2*2.*s/(mX*mX) + g3*kappa*s/(mX*mX);
    a1Im = g1Im*mZ*mZ/(mX*mX) + g2Im*2.*s/(mX*mX) + g3Im*kappa*s/(mX*mX);
    a2 = -2.*g2 - g3*kappa;
    a2Im = -2.*g2Im - g3Im*kappa;
    a3 = -2.*g4;
    a3Im = -2.*g4Im;

  }
  
  // From the form output in terms of the g couplings
  // https://dl.dropbox.com/u/86680464/result_spin0.txt
  Double_t x = (mX*mX-m1*m1-m2*m2)/(2.0*m1*m2);
  Double_t f00Real =
    a2 * (  - (x*x-1)*m1*m2 )
    + a1 * ( 1./4.*TMath::Power(m1, -1)*TMath::Power(m2, 3) - 1./2.*m1*m2 + 1./4.*TMath::Power(m1,3)*TMath::Power(m2,-1) - (x*x-1)*m1*m2 )
    + TMath::Power(mX,4)*a1 * (  - 1./4.*TMath::Power(m1,-1)*TMath::Power(m2,-1) );
  //    Double_t f00Imag = 0.;
  Double_t f00Imag =     
    a2Im * (  - (x*x-1)*m1*m2 )
    + a1Im * ( 1./4.*TMath::Power(m1, -1)*TMath::Power(m2, 3) - 1./2.*m1*m2 + 1./4.*TMath::Power(m1,3)*TMath::Power(m2,-1) - (x*x-1)*m1*m2 )
    + TMath::Power(mX,4)*a1Im * (  - 1./4.*TMath::Power(m1,-1)*TMath::Power(m2,-1) );
    
  //    Double_t fppReal = TMath::Power(mX, 2)*a1;
  //    Double_t fppImag = a3*sqrt(x*x-1)*m1*m2 ;
  Double_t fppReal = TMath::Power(mX, 2)*a1 - a3Im*sqrt(x*x-1)*m1*m2;
  Double_t fppImag = a3*sqrt(x*x-1)*m1*m2 + TMath::Power(mX, 2)*a1Im;
    
  Double_t fmmReal = TMath::Power(mX, 2)*a1 + a3Im*sqrt(x*x-1)*m1*m2;
  Double_t fmmImag = -a3*sqrt(x*x-1)*m1*m2 + TMath::Power(mX, 2)*a1Im;
    
  Double_t f00 = f00Imag*f00Imag + f00Real*f00Real;
  Double_t fpp = fppImag*fppImag + fppReal*fppReal;
  Double_t fmm = fmmImag*fmmImag + fmmReal*fmmReal;
    
  Double_t phi00=atan2(f00Imag,f00Real);
  Double_t phipp=atan2(fppImag,fppReal)-phi00;
  Double_t phimm=atan2(fmmImag,fmmReal)-phi00;
    
    
  Double_t betaValSquared = (1.-(pow(m1-m2,2)/pow(mX,2)))*(1.-(pow(m1+m2,2)/pow(mX,2)));
  Double_t betaVal = sqrt(betaValSquared);
    
  Double_t term1Coeff = (pow(m1,3))/( (pow(m1,2)-pow(mZ,2))*(pow(m1,2)-pow(mZ,2))+pow(mZ,2)*pow(gamZ,2) );
  Double_t term2Coeff = (pow(m2,3))/( (pow(m2,2)-pow(mZ,2))*(pow(m2,2)-pow(mZ,2))+pow(mZ,2)*pow(gamZ,2) );
    
  double value = 0.;
  value += (f00*(-1 + Power(h1,2))*(-1 + Power(h2,2)))/4.;
    
  value += (fmm*(-1 - Power(h1,2) + 2*h1*R1Val)*(-1 - Power(h2,2) + 2*h2*R2Val))/16.;
    
  value += (fpp*(1 + Power(h1,2) + 2*h1*R1Val)*(1 + Power(h2,2) + 2*h2*R2Val))/16.;
    
  value += (sqrt(f00)*sqrt(fmm)*sqrt(1 - Power(h1,2))*sqrt(1 - Power(h2,2))*(h1 - R1Val)*(h2 - R2Val)*Cos(Phi - phimm))/4.;
    
  value += (sqrt(f00)*sqrt(fpp)*sqrt(1 - Power(h1,2))*sqrt(1 - Power(h2,2))*(h1 + R1Val)*(h2 + R2Val)*Cos(Phi + phipp))/4.;
    
  value += (sqrt(fmm)*sqrt(fpp)*(-1 + Power(h1,2))*(-1 + Power(h2,2))*Cos(2*Phi - phimm + phipp))/8.;
    
    
  Double_t term1 = betaVal*term1Coeff*term2Coeff*value;
    
  Double_t accp = 1.;
  return term1*accp;
    
    
}


Int_t RooSpinZero_7DComplex::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
{

  if (matchArgs(allVars,analVars,RooArgSet(*m1.absArg(),*m2.absArg(),*hs.absArg(),*h1.absArg(),*h2.absArg(),*Phi.absArg(),*Phi1.absArg()))) return 7 ; // all integrated
  if (matchArgs(allVars,analVars,RooArgSet(*hs.absArg(),*h1.absArg(),*h2.absArg(),*Phi.absArg(),*Phi1.absArg()))) return 6 ; // No m1,m2
  if (matchArgs(allVars,analVars,hs,h1,h2,Phi)) return 2 ; // No Phi1
  if (matchArgs(allVars,analVars,hs,h1,h2,Phi1)) return 5 ; // No Phi
  if (matchArgs(allVars,analVars,hs,h1,Phi,Phi1)) return 3 ; // No h2
  if (matchArgs(allVars,analVars,hs,h2,Phi,Phi1)) return 4 ; // No h1
  if (matchArgs(allVars,analVars,h1,h2,Phi,Phi1)) return 1 ; // No hs
  
  return 0 ;
}
Double_t RooSpinZero_7DComplex::analyticalIntegral(Int_t code, const char* rangeName) const
{
    
  bool isZZ = true;
  if ( mZ < 90.) isZZ = false;
  if ( isZZ ) {
    if( (m1+m2) > mX || m2>m1 ) return 1e-9;
  } else {
    if( (m1+m2) > mX ) return 1e-9;
  }
  double nanval = sqrt((1 - TMath::Power(m1 - m2,2)/TMath::Power(mX,2))*(1 - TMath::Power(m1 + m2,2)/TMath::Power(mX,2)));
  if (nanval != nanval) return 1e-9;
  
  //
  //  common variables to use for all cases
  //
  Double_t sh1 = sqrt(1-h1*h1);
  Double_t sh2 = sqrt(1-h2*h2);
  double s=(mX*mX-m1*m1-m2*m2)/2.;
  double kappa=s/(1000*1000);
    
  double a1=0,a2=0,a3=0,a1Im=0,a2Im=0,a3Im=0;

  double g1(1.0), g1Im(0.), g2(0.), g2Im(0.), g3(0.), g3Im(0.), g4(0.), g4Im(0.);

  if(parameterization==kMagPhase_As){
    a1=a1Val;
    a1Im=phi1Val;
    a2=a2Val;
    a2Im=phi2Val;
    a3=a3Val;
    a3Im=phi3Val;
  } else {

    // 
    // Important, the PDF has an emphirical fix
    // Forcing the couplings to be its conjugate 
    // to agree with the generator shapes
    // 
    if(parameterization==kFracPhase_Gs){
      nanval = sqrt(1 - fa2 - fa3);
      if (nanval != nanval) return 1e-9;
      
      // convert fraction and phase to g1,g2...etc
      double sigma_1=1.860351; // was 2.03971 at 126 GeV
      double sigma_2=0.672859; // was 0.77517 at 126 GeV
      double sigma_4=0.284353; // was 0.32689 at 126 GeV
      
      double g1Mag = 1.;
      double g2Mag = sqrt(fa2/(1.-fa2-fa3))*sqrt(sigma_1/sigma_2); 
      double g4Mag = sqrt(fa3/(1.-fa2-fa3))*sqrt(sigma_1/sigma_4); 
      
      g1   = g1Mag;
      g1Im = 0.0;
      g2   = g2Mag*cos(phia2);
      g2Im = - g2Mag*sin(phia2);
      g3   = 0.0;
      g3Im = 0.0;
      g4   = g4Mag*cos(phia3);
      g4Im = - g4Mag*sin(phia3);
      
    }else if(parameterization==kRealImag_Gs){
      
      g1   = g1Val;
      g1Im = - g1ValIm;
      g2   = g2Val;
      g2Im = - g2ValIm;
      g3   = g3Val;
      g3Im = - g3ValIm;
      g4   = g4Val;
      g4Im = - g4ValIm;
    }
    
    // std::cout << "g4 = " << g4 << "\t " << "i " << g4Im << "\n";
    a1 = g1*mZ*mZ/(mX*mX) + g2*2.*s/(mX*mX) + g3*kappa*s/(mX*mX);
    a1Im = g1Im*mZ*mZ/(mX*mX) + g2Im*2.*s/(mX*mX) + g3Im*kappa*s/(mX*mX);
    a2 = -2.*g2 - g3*kappa;
    a2Im = -2.*g2Im - g3Im*kappa;
    a3 = -2.*g4;
    a3Im = -2.*g4Im;

  }
  
  // From the form output in terms of the g couplings
  // https://dl.dropbox.com/u/86680464/result_spin0.txt
  Double_t x = (mX*mX-m1*m1-m2*m2)/(2.0*m1*m2);
  Double_t f00Real =
    a2 * (  - (x*x-1)*m1*m2 )
    + a1 * ( 1./4.*TMath::Power(m1, -1)*TMath::Power(m2, 3) - 1./2.*m1*m2 + 1./4.*TMath::Power(m1,3)*TMath::Power(m2,-1) - (x*x-1)*m1*m2 )
    + TMath::Power(mX,4)*a1 * (  - 1./4.*TMath::Power(m1,-1)*TMath::Power(m2,-1) );
  //    Double_t f00Imag = 0.;
  Double_t f00Imag =     
    a2Im * (  - (x*x-1)*m1*m2 )
    + a1Im * ( 1./4.*TMath::Power(m1, -1)*TMath::Power(m2, 3) - 1./2.*m1*m2 + 1./4.*TMath::Power(m1,3)*TMath::Power(m2,-1) - (x*x-1)*m1*m2 )
    + TMath::Power(mX,4)*a1Im * (  - 1./4.*TMath::Power(m1,-1)*TMath::Power(m2,-1) );
    
  //    Double_t fppReal = TMath::Power(mX, 2)*a1;
  //    Double_t fppImag = a3*sqrt(x*x-1)*m1*m2 ;
  Double_t fppReal = TMath::Power(mX, 2)*a1 - a3Im*sqrt(x*x-1)*m1*m2;
  Double_t fppImag = a3*sqrt(x*x-1)*m1*m2 + TMath::Power(mX, 2)*a1Im;
    
  Double_t fmmReal = TMath::Power(mX, 2)*a1 + a3Im*sqrt(x*x-1)*m1*m2;
  Double_t fmmImag = -a3*sqrt(x*x-1)*m1*m2 + TMath::Power(mX, 2)*a1Im;
    
  Double_t f00 = f00Imag*f00Imag + f00Real*f00Real;
  Double_t fpp = fppImag*fppImag + fppReal*fppReal;
  Double_t fmm = fmmImag*fmmImag + fmmReal*fmmReal;
    
  Double_t phi00=atan2(f00Imag,f00Real);
  Double_t phipp=atan2(fppImag,fppReal)-phi00;
  Double_t phimm=atan2(fmmImag,fmmReal)-phi00;
    
  Double_t betaValSquared = (1.-(pow(m1-m2,2)/pow(mX,2)))*(1.-(pow(m1+m2,2)/pow(mX,2)));
  Double_t betaVal = sqrt(betaValSquared);
    
  Double_t term1Coeff = (pow(m1,3))/( (pow(m1,2)-pow(mZ,2))*(pow(m1,2)-pow(mZ,2))+pow(mZ,2)*pow(gamZ,2) );
  Double_t term2Coeff = (pow(m2,3))/( (pow(m2,2)-pow(mZ,2))*(pow(m2,2)-pow(mZ,2))+pow(mZ,2)*pow(gamZ,2) );
    
  switch(code)
    {
      // projections to hs
    case 1:
      {
        double value = 0.;
        value += (16*f00*Power(Pi(),2))/9.;
        
        value += (16*fmm*Power(Pi(),2))/9.;
        
        value += (16*fpp*Power(Pi(),2))/9.;
        
        
        Double_t term1 = betaVal*term1Coeff*term2Coeff*value;
        
        Double_t accp = 1.;
        return term1*accp;
      }
      // projections to Phi1
    case 2:
      {
        double value = 0.;
        value += (16*f00*Pi())/9.;
        
        value += (16*fmm*Pi())/9.;
        
        value += (16*fpp*Pi())/9.;
        
        
        Double_t term1 = betaVal*term1Coeff*term2Coeff*value;
        
        Double_t accp = 1.;
        return term1*accp;
      }
      // projections to h2
    case 3:
      {
        double value = 0.;
        value += (-8*f00*(-1 + Power(h2,2))*Power(Pi(),2))/3.;
        
        value += (4*fmm*Power(Pi(),2)*(1 + Power(h2,2) - 2*h2*R2Val))/3.;
        
        value += (4*fpp*Power(Pi(),2)*(1 + Power(h2,2) + 2*h2*R2Val))/3.;
        
        
        Double_t term1 = betaVal*term1Coeff*term2Coeff*value;
        
        Double_t accp = 1.;
        return term1*accp;
      }
      // projections to h1
    case 4:
      {
        double value = 0.;
        value += (-8*f00*(-1 + Power(h1,2))*Power(Pi(),2))/3.;
        
        value += (4*fmm*Power(Pi(),2)*(1 + Power(h1,2) - 2*h1*R1Val))/3.;
        
        value += (4*fpp*Power(Pi(),2)*(1 + Power(h1,2) + 2*h1*R1Val))/3.;
        
        
        Double_t term1 = betaVal*term1Coeff*term2Coeff*value;
        
        Double_t accp = 1.;
        return term1*accp;
      }
      // projections to Phi
    case 5:
      {
        double value = 0.;
        value += (16*f00*Pi())/9.;
        
        value += (16*fmm*Pi())/9.;
        
        value += (16*fpp*Pi())/9.;
        
        value += (sqrt(f00)*sqrt(fmm)*Power(Pi(),3)*R1Val*R2Val*Cos(Phi - phimm))/4.;
        
        value += (sqrt(f00)*sqrt(fpp)*Power(Pi(),3)*R1Val*R2Val*Cos(Phi + phipp))/4.;
        
        value += (8*sqrt(fmm)*sqrt(fpp)*Pi()*Cos(2*Phi - phimm + phipp))/9.;
        
        
        Double_t term1 = betaVal*term1Coeff*term2Coeff*value;
        Double_t accp = 1.;
        return term1*accp;
      }
      // projections to mz1/mz2
    case 6:
      {
        double value = 0.;
        value += (32*f00*Power(Pi(),2))/9.;
        
        value += (32*fmm*Power(Pi(),2))/9.;
        
        value += (32*fpp*Power(Pi(),2))/9.;
        
        
        Double_t term1 = betaVal*term1Coeff*term2Coeff*value;
        
        Double_t accp = 1.;
        return term1*accp;
      }
      // integral (projection to mX)
    case 7:
      {
        double value = 0.;

        value += (g1*g1+g1Im*g1Im)*2.07146e9;     // for 126. use -> 2.37727e9;
	value += (g2*g2+g2Im*g2Im)*8.21207e8;     //              -> 9.86079e8;
	value += (g4*g4+g4Im*g4Im)*3.39973e8;     //              -> 4.08027e8;
        value += sqrt(g1*g1+g1Im*g1Im)*g2*2.25963e9; //           -> 2.65195e9;

	// fudge factor of /2 to make the normalization of plots ready 
	// this does not affect the fitting
	return value/2.; 

      }
    }
  assert(0) ;
  return 0 ;
}


