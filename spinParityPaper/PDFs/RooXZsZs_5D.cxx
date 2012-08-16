/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "RooXZsZs_5D.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h> 
#include "TMath.h" 

using namespace TMath;


ClassImp(RooXZsZs_5D) 

RooXZsZs_5D::RooXZsZs_5D(const char *name, const char *title, 
                         RooAbsReal& _m1,
                         RooAbsReal& _m2,
                         RooAbsReal& _h1,
                         RooAbsReal& _h2,
                         RooAbsReal& _Phi,
                         RooAbsReal& _a1Val,
                         RooAbsReal& _phi1Val,
                         RooAbsReal& _a2Val,
                         RooAbsReal& _phi2Val,
                         RooAbsReal& _a3Val,
                         RooAbsReal& _phi3Val,
			 RooAbsReal& _useGTerm,
			 RooAbsReal& _g1Val,
			 RooAbsReal& _g2Val,
			 RooAbsReal& _g3Val,
			 RooAbsReal& _g4Val,
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
Phi("Phi","Phi",this,_Phi),
a1Val("a1Val","a1Val",this,_a1Val),
phi1Val("phi1Val","phi1Val",this,_phi1Val),
a2Val("a2Val","a2Val",this,_a2Val),
phi2Val("phi2Val","phi2Val",this,_phi2Val),
a3Val("a3Val","a3Val",this,_a3Val),
phi3Val("phi3Val","phi3Val",this,_phi3Val),
useGTerm("useGTerm","useGTerm",this,_useGTerm),
g1Val("g1Val","g1Val",this,_g1Val),
g2Val("g2Val","g2Val",this,_g2Val),
g3Val("g3Val","g3Val",this,_g3Val),
g4Val("g4Val","g4Val",this,_g4Val),
mZ("mZ","mZ",this,_mZ),
gamZ("gamZ","gamZ",this,_gamZ),
mX("mX","mX",this,_mX),
R1Val("R1Val","R1Val",this,_R1Val),
R2Val("R2Val","R2Val",this,_R2Val)
{ 
} 


RooXZsZs_5D::RooXZsZs_5D(const RooXZsZs_5D& other, const char* name) :  
RooAbsPdf(other,name), 
m1("m1",this,other.m1),
m2("m2",this,other.m2),
h1("h1",this,other.h1),
h2("h2",this,other.h2),
Phi("Phi",this,other.Phi),
a1Val("a1Val",this,other.a1Val),
phi1Val("phi1Val",this,other.phi1Val),
a2Val("a2Val",this,other.a2Val),
phi2Val("phi2Val",this,other.phi2Val),
a3Val("a3Val",this,other.a3Val),
phi3Val("phi3Val",this,other.phi3Val),
useGTerm("useGTerm",this,other.useGTerm),
g1Val("g1Val",this,other.g1Val),
g2Val("a2Val",this,other.g2Val),
g3Val("g3Val",this,other.g3Val),
g4Val("g4Val",this,other.g4Val),
mZ("mZ",this,other.mZ),
gamZ("gamZ",this,other.gamZ),
mX("mX",this,other.mX),
R1Val("R1Val",this,other.R1Val),
R2Val("R2Val",this,other.R2Val)
{ 
} 



Double_t RooXZsZs_5D::evaluate() const 
{ 
    // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
    //   const Double_t VEV = 246.;
    Double_t sh1 = sqrt(1-h1*h1);
    Double_t sh2 = sqrt(1-h2*h2);        
      
    bool isZZ = true;
    if ( mZ < 90. ) isZZ = false;

    if ( isZZ ) {
      if( (m1+m2) > mX || m2>m1 ) return 1e-9; 
    } else {
      if( (m1+m2) > mX ) return 1e-9; 
    }
    double nanval = sqrt((1 - TMath::Power(m1 - m2,2)/TMath::Power(mX,2))*(1 - TMath::Power(m1 + m2,2)/TMath::Power(mX,2)));
    if (nanval != nanval) return 1e-9;

    double s=(mX*mX-m1*m1-m2*m2)/2.;
    double kappa=s/(1000*1000);
    
    double a1=0,a2=0,a3=0,phi1=0,phi2=0,phi3=0;

    if(useGTerm>0.0){
      a1 = g1Val*mZ*mZ/(mX*mX) + g2Val*2.*s/(mX*mX) + g3Val*kappa*s/(mX*mX);
      phi1 = atan2(0., a1);
      a2 = -2.*g2Val - g3Val*kappa;
      phi2 = atan2(0.,a2);
      a3 = -2.*g4Val;
      phi3 = atan2(0.,a3);
      
    }else{
      a1=a1Val;
      phi1=phi1Val;
      a2=a2Val;
      phi2=phi2Val;
      a3=a3Val;
      phi3=phi3Val;
    }

    /*
  
    // these were derived from the B-paper, page 197, Eqs.(3.3)-(3.6): http://prd.aps.org/pdf/PRD/v45/i1/p193_1
    Double_t chi = (mX*mX-m1*m1-m2*m2)/(2.*m2*m1);
    Double_t eta = (m1*m2)/pow(mX,2);
    if (chi<1) chi = 1;
    Double_t f00Val = (a1*a1*chi*chi+pow(a2,2)*pow(eta,2)*(chi*chi-1.)*(chi*chi-1.)+2.*a1*a2*chi*(chi*chi-1)*eta*cos(phi1-phi2));
    Double_t fppVal = (a1*a1+pow(a3,2)*pow(eta,2)*(chi*chi-1)+2.*a1*a3*chi*sqrt(chi*chi-1)*eta*cos(phi1-phi3));
    Double_t fmmVal = (a1*a1+pow(a3,2)*pow(eta,2)*(chi*chi-1)+2.*a1*a3*chi*sqrt(chi*chi-1)*eta*cos(phi1-phi3));
    	     
    Double_t phi00Val = atan2(a1Val*sin(phi1Val)+a2Val*eta*(chi*chi-1)*sin(phi2Val),a1Val*cos(phi1Val)+a2Val*eta*(chi*chi-1)*cos(phi2Val))+TMath::Pi();
    Double_t phippVal = atan2(a1*sin(phi1)+a3*eta*sqrt(chi*chi-1)*sin(phi3),a1*cos(phi3)-a3*eta*sqrt(chi*chi-1)*cos(phi3));
    Double_t phimmVal = atan2(a1*sin(phi1)-a3*eta*sqrt(chi*chi-1)*sin(phi3),a1*cos(phi3)+a3*eta*sqrt(chi*chi-1)*cos(phi3));

    //cout << " ============= B-Physics =============== " << endl;
    
    //cout << "fmm: " << fmmVal << " fpp: " << fppVal << " f00: " << f00Val << endl;
    //cout << "phimm: " << phimmVal << " phipp: " << phippVal << " phi00: " << phi00Val << endl;

    */
    
    // From the form output in terms of the g couplings 
    // https://dl.dropbox.com/u/86680464/result_spin0.txt
    Double_t x = (mX*mX-m1*m1-m2*m2)/(2.0*m1*m2);
    Double_t f00Real = 
      a2 * (  - (x*x-1)*m1*m2 )
      + a1 * ( 1./4.*TMath::Power(m1, -1)*TMath::Power(m2, 3) - 1./2.*m1*m2 + 1./4.*TMath::Power(m1,3)*TMath::Power(m2,-1) - (x*x-1)*m1*m2 )
      + TMath::Power(mX,4)*a1 * (  - 1./4.*TMath::Power(m1,-1)*TMath::Power(m2,-1) );
    Double_t f00Imag = 0.;
    
    Double_t fppReal = TMath::Power(mX, 2)*a1;
    Double_t fppImag = a3*sqrt(x*x-1)*m1*m2 ;
    
    Double_t fmmReal = fppReal;
    Double_t fmmImag =  - fppImag;
    
    Double_t f00Val = f00Imag*f00Imag + f00Real*f00Real;
    Double_t fppVal = fppImag*fppImag + fppReal*fppReal;
    Double_t fmmVal = fmmImag*fmmImag + fmmReal*fmmReal;
    	     
    Double_t phi00Val=atan2(f00Imag,f00Real);
    //if( isZZ ) phi00Val += TMath::Pi();
    Double_t phippVal=atan2(fppImag,fppReal);
    Double_t phimmVal=atan2(fmmImag,fmmReal);

    //cout << " ============= FORM OUTPUT =============== " << endl;
    
    //cout << "fmm: " << fmmVal << " fpp: " << fppVal << " f00: " << f00Val << endl;
    //cout << "phimm: " << phimmVal << " phipp: " << phippVal << " phi00: " << phi00Val << endl;
  
   
    Double_t betaValSquared = (1.-(pow(m1-m2,2)/pow(mX,2)))*(1.-(pow(m1+m2,2)/pow(mX,2)));
    Double_t betaVal = sqrt(betaValSquared);

    Double_t term1Coeff = (pow(m1,3))/( (pow(m1,2)-pow(mZ,2))*(pow(m1,2)-pow(mZ,2))+pow(mZ,2)*pow(gamZ,2) );
    Double_t term2Coeff = (pow(m2,3))/( (pow(m2,2)-pow(mZ,2))*(pow(m2,2)-pow(mZ,2))+pow(mZ,2)*pow(gamZ,2) );
    
    Double_t term1A = 4.*f00Val*pow(sh1,2)*pow(sh2,2);
    Double_t term1B = (fppVal+fmmVal)*((1.+h1*h1)*(1.+h2*h2)+4.*R1Val*R2Val*h1*h2);
    Double_t term1C = -2.*(fppVal-fmmVal)*(R1Val*h1*(1.+h2*h2)+R2Val*h2*(1.+h1*h1));
    Double_t term1D = 4.*sqrt(fppVal*f00Val)*(R1Val-h1)*(R2Val-h2)*sh1*sh2*cos(Phi+phippVal-phi00Val);
    Double_t term1E = 4.*sqrt(fmmVal*f00Val)*(R1Val+h1)*(R2Val+h2)*sh1*sh2*cos(Phi-phimmVal+phi00Val);
    Double_t term1F = 2.*sqrt(fppVal*fmmVal)*pow(sh1,2)*pow(sh2,2)*cos(2.*Phi+phippVal-phimmVal);
    Double_t term1 = betaVal*term1Coeff*term2Coeff*(term1A+term1B+term1C+term1D+term1E+term1F);

    Double_t accp = 1.;
    return term1*accp;
    
    
} 


Int_t RooXZsZs_5D::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
{
    if (matchArgs(allVars,analVars,h1,h2,Phi)) return 6 ;
    if (matchArgs(allVars,analVars,Phi,h1)) return 3 ;
    if (matchArgs(allVars,analVars,Phi,h2)) return 4 ;
    if (matchArgs(allVars,analVars,h1,h2)) return 5 ;
    return 0 ;
}
Double_t RooXZsZs_5D::analyticalIntegral(Int_t code, const char* rangeName) const
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
  
  double a1=0,a2=0,a3=0,phi1=0,phi2=0,phi3=0;
  
  if(useGTerm>0.0){
    a1 = g1Val*mZ*mZ/(mX*mX) + g2Val*2.*s/(mX*mX) + g3Val*kappa*s/(mX*mX);
    phi1 = atan2(0., a1);
    a2 = -2.*g2Val - g3Val*kappa;
    phi2 = atan2(0.,a2);
    a3 = -2.*g4Val;
    // phi3 = atan2(0.,a3);    
    phi3 = 0.;
  }else{
    a1=a1Val;
    phi1=phi1Val;
    a2=a2Val;
    phi2=phi2Val;
    a3=a3Val;
    phi3=phi3Val;
  }

  /*
  // these were derived from the B-paper, page 197, Eqs.(3.3)-(3.6): http://prd.aps.org/pdf/PRD/v45/i1/p193_1
  Double_t chi = (mX*mX-m1*m1-m2*m2)/(2.*m2*m1);
  Double_t eta = (m1*m2)/pow(mX,2);
  if (chi<1) chi = 1;
  Double_t f00Val = (a1*a1*chi*chi+pow(a2,2)*pow(eta,2)*(chi*chi-1.)*(chi*chi-1.)+2.*a1*a2*chi*(chi*chi-1)*eta*cos(phi1-phi2));
  Double_t fppVal = (a1*a1+pow(a3,2)*pow(eta,2)*(chi*chi-1)+2.*a1*a3*chi*sqrt(chi*chi-1)*eta*cos(phi1-phi3));
  Double_t fmmVal = (a1*a1+pow(a3,2)*pow(eta,2)*(chi*chi-1)+2.*a1*a3*chi*sqrt(chi*chi-1)*eta*cos(phi1-phi3));
  
  Double_t phi00Val = atan2(a1Val*sin(phi1Val)+a2Val*eta*(chi*chi-1)*sin(phi2Val),a1Val*cos(phi1Val)+a2Val*eta*(chi*chi-1)*cos(phi2Val))+TMath::Pi();
  Double_t phippVal = atan2(a1*sin(phi1)+a3*eta*sqrt(chi*chi-1)*sin(phi3),a1*cos(phi3)-a3*eta*sqrt(chi*chi-1)*cos(phi3));
  Double_t phimmVal = atan2(a1*sin(phi1)-a3*eta*sqrt(chi*chi-1)*sin(phi3),a1*cos(phi3)+a3*eta*sqrt(chi*chi-1)*cos(phi3));
  */

  // From the form output in terms of the g couplings 
  // https://dl.dropbox.com/u/86680464/result_spin0.txt
  Double_t x = (mX*mX-m1*m1-m2*m2)/(2.0*m1*m2);
  Double_t f00Real = 
    a2 * (  - (x*x-1)*m1*m2 )
    + a1 * ( 1./4.*TMath::Power(m1, -1)*TMath::Power(m2, 3) - 1./2.*m1*m2 + 1./4.*TMath::Power(m1,3)*TMath::Power(m2,-1) - (x*x-1)*m1*m2 )
    + TMath::Power(mX,4)*a1 * (  - 1./4.*TMath::Power(m1,-1)*TMath::Power(m2,-1) );
  Double_t f00Imag = 0.;
  
  Double_t fppReal = TMath::Power(mX, 2)*a1;
  Double_t fppImag = a3*sqrt(x*x-1)*m1*m2 ;
  
  Double_t fmmReal = fppReal;
  Double_t fmmImag =  - fppImag;
  
  Double_t f00Val = f00Imag*f00Imag + f00Real*f00Real;
  Double_t fppVal = fppImag*fppImag + fppReal*fppReal;
  Double_t fmmVal = fmmImag*fmmImag + fmmReal*fmmReal;
  
  Double_t phi00Val=atan2(f00Imag,f00Real);
  //if( isZZ ) phi00Val += TMath::Pi();
  Double_t phippVal=atan2(fppImag,fppReal);
  Double_t phimmVal=atan2(fmmImag,fmmReal);

  Double_t betaValSquared = (1.-(pow(m1-m2,2)/pow(mX,2)))*(1.-(pow(m1+m2,2)/pow(mX,2)));
  Double_t betaVal = sqrt(betaValSquared);
  
  Double_t term1Coeff = (pow(m1,3))/( (pow(m1,2)-pow(mZ,2))*(pow(m1,2)-pow(mZ,2))+pow(mZ,2)*pow(gamZ,2) );
  Double_t term2Coeff = (pow(m2,3))/( (pow(m2,2)-pow(mZ,2))*(pow(m2,2)-pow(mZ,2))+pow(mZ,2)*pow(gamZ,2) );

  switch(code)
    {
      
      // projections to h2 
    case 3:
      {
	// FOR THE INTEGRATIONS, ONLY THESE LINES SHOULD CHANGE
	// cform --> still have to change "sqrt" and "cos" and "Pi" and "Power" and "R1" and "R2"
	Double_t term1A = (-32*f00Val*TMath::Pi()*(-1 + TMath::Power(h2,2)))/3.;
	Double_t term1B = (16*(fmmVal + fppVal)*TMath::Pi()*(1 + TMath::Power(h2,2)))/3.;
	Double_t term1C = (32*(fmmVal - fppVal)*TMath::Pi()*R2Val*h2)/3.;
	Double_t term1D = 0;
	Double_t term1E = 0;
	Double_t term1F = 0;
	Double_t term1 = betaVal*term1Coeff*term2Coeff*(term1A+term1B+term1C+term1D+term1E+term1F);

	Double_t accp = 1.;
	return term1*accp;
      }
      // projections to h1 
    case 4:
      {
	// FOR THE INTEGRATIONS, ONLY THESE LINES SHOULD CHANGE
	// cform --> still have to change "sqrt" and "cos" and "Pi" and "Power" and "R1Val" and "R2"
	Double_t term1A = (-32*f00Val*TMath::Pi()*(-1 + TMath::Power(h1,2)))/3.;
	Double_t term1B = (16*(fmmVal + fppVal)*TMath::Pi()*(1 + TMath::Power(h1,2)))/3.;
	Double_t term1C = (32*(fmmVal - fppVal)*TMath::Pi()*R1Val*h1)/3.;
	Double_t term1D = 0;
	Double_t term1E = 0;
	Double_t term1F = 0;
	Double_t term1 = betaVal*term1Coeff*term2Coeff*(term1A+term1B+term1C+term1D+term1E+term1F);
        
	Double_t accp = 1.;
	return term1*accp;
      }
      // projections to Phi
    case 5:
      {
	// FOR THE INTEGRATIONS, ONLY THESE LINES SHOULD CHANGE
	// cform --> still have to change "sqrt" and "cos" and "Pi" and "Power" and "R1" and "R2"
	Double_t term1A = (64*f00Val)/9.;
	Double_t term1B = (64*(fmmVal + fppVal))/9.;
	Double_t term1C = 0;
	Double_t term1D = sqrt(f00Val)*sqrt(fppVal)*TMath::Power(TMath::Pi(),2)*R1Val*R2Val*cos(Phi + phi00Val + phippVal);
	Double_t term1E = sqrt(f00Val)*sqrt(fmmVal)*TMath::Power(TMath::Pi(),2)*R1Val*R2Val*cos(Phi - phi00Val - phimmVal);
	Double_t term1F = (32*sqrt(fmmVal)*sqrt(fppVal)*cos(2*Phi - phimmVal + phippVal))/9.;
	Double_t term1 = betaVal*term1Coeff*term2Coeff*(term1A+term1B+term1C+term1D+term1E+term1F);
	Double_t accp = 1.;
	return term1*accp;
      }
      // projections to mz1/mz2
    case 6:
      {
	// FOR THE INTEGRATIONS, ONLY THESE LINES SHOULD CHANGE
	// cform --> still have to change "sqrt" and "cos" and "Pi" and "Power" and "R1" and "R2"
	Double_t term1A = (128*f00Val*TMath::Pi())/9.;
	Double_t term1B = (128*(fmmVal + fppVal)*TMath::Pi())/9.;
	Double_t term1C = 0;
	Double_t term1D = 0;
	Double_t term1E = 0;
	Double_t term1F = 0;
	Double_t term1 = betaVal*term1Coeff*term2Coeff*(term1A+term1B+term1C+term1D+term1E+term1F);

	Double_t accp = 1.;
	return term1*accp;
      }
    }
    assert(0) ;
    return 0 ;
}


