#include <iostream>
#include "RooRealVar.h"
#include "RooPentaSpinTwo.h"
#include "RooBtheta.h"
#include "RooBphione.h"
#include "RooBkg2l2jHL.h"
#include "RooProdPdf.h"

class signalModel{

 public: 

  signalModel();

  void setVars(double mH);
  
  RooRealVar* cosTheta1;     
  RooRealVar* cosTheta2;     
  RooRealVar* phi;    
  RooRealVar* cosThetaS;     
  RooRealVar* phi1;   
	      		      
  RooRealVar* fppVal; 	 
  RooRealVar* fmmVal; 	 
  RooRealVar* fpmVal; 	 
  RooRealVar* fp0Val; 	 
  RooRealVar* f0mVal; 	 
  RooRealVar* phippVal;	 
  RooRealVar* phimmVal;	 
  RooRealVar* phipmVal;	 
  RooRealVar* phip0Val;	 
  RooRealVar* phi0mVal;	 
  RooRealVar* fz1Val; 	 
  RooRealVar* fz2Val; 	 
  RooRealVar* R1Val;  	 
  RooRealVar* R2Val;     
	      		      
  RooRealVar* para2;  
  RooRealVar* para4;  
  RooRealVar* para6;  
  RooRealVar* para8;  
  RooRealVar* acca0;  
  RooRealVar* acca1;  
  RooRealVar* acca2;  
  RooRealVar* acca4;  
  RooRealVar* a2;     
  RooRealVar* a4;     
  RooRealVar* cutOff; 
  RooRealVar* g;      
  RooRealVar* b2;     
  RooRealVar* b4;     
  RooRealVar* N;      

  RooPentaSpinTwo* signal;

};


class backgroundModel{

 public: 

  backgroundModel();

  void setVars(double mH);

  RooRealVar* cosTheta1;    
  RooRealVar* h1_para2;	    
  RooRealVar* h1_para4;	    
  RooRealVar* h1_acca2;	    
  RooRealVar* h1_acca4;	    
	      			    
  RooBtheta*  h1PDF;        

  RooRealVar* cosTheta2;
  RooRealVar* h2_acca0;
  RooRealVar* h2_acca2;
  RooRealVar* h2_acca4;
  RooRealVar* h2_g;
  RooRealVar* h2_cutOff;
  
  RooBkg2l2jHL* h2PDF;

  RooRealVar* cosThetaS;
  RooRealVar* hs_para2;
  RooRealVar* hs_para4;
  RooRealVar* hs_acca2;
  RooRealVar* hs_acca4;

  RooBtheta* hsPDF;

  RooRealVar* phi;
  RooRealVar* phi_para2;
  RooRealVar* phi_para4;
  RooRealVar* phi_acca2;
  RooRealVar* phi_acca4;
  RooRealVar* phi_acca6;
  RooRealVar* phi_acca8;

  RooBphione* phiPDF;

  RooRealVar* phi1;
  RooRealVar* phi1_para2;
  RooRealVar* phi1_para4;
  RooRealVar* phi1_acca2;
  RooRealVar* phi1_acca4;
  RooRealVar* phi1_acca6;
  RooRealVar* phi1_acca8;

  RooBphione* phi1PDF;

  RooProdPdf* background;

};


class helicityLD{
  
 public:
  helicityLD();
  
  void setVars(double mH);
  double getLD(double h1, double h2, double hs, double phi, double phi1, double mZZ);

  backgroundModel* bkgPDF;
  signalModel* sigPDF;

};
