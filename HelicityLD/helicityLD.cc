#include "helicityLD.h"

using namespace RooFit;
using namespace std;

signalModel::signalModel(){

  double mH=200;

  cosTheta1      = new RooRealVar("cosTheta1",  "cos#theta_{1}",  -1.,1.);
  cosTheta2      = new RooRealVar("cosTheta2",  "cos#theta_{2}",  0.,1.);
  phi     = new RooRealVar("phi", "#Phi", -3.1415,3.1415);
  cosThetaS      = new RooRealVar("cosThetaS",  "cos#theta^{*}",  -1.,1.);
  phi1    = new RooRealVar("phi1","#Phi_{1}",-3.1415,3.1415); 
  fppVal  = new RooRealVar("fppVal",   "fppVal",   0.,1.);
  fppVal->setConstant(kTRUE);
  fmmVal  = new RooRealVar("fmmVal",   "fmmVal",   0.,1.);
  fmmVal->setConstant(kTRUE);
  fpmVal  = new RooRealVar("fpmVal",   "fpmVal",   0.,1.);
  fpmVal->setConstant(kTRUE);
  fp0Val  = new RooRealVar("fp0Val",   "fp0Val",   0.,1.);
  fp0Val->setConstant(kTRUE);
  f0mVal  = new RooRealVar("f0mVal",   "f0mVal",   0.,1.);  
  f0mVal->setConstant(kTRUE);
  phippVal= new RooRealVar("phippVal", "phippVal", 0.,10.);
  phippVal->setConstant(kTRUE);
  phimmVal= new RooRealVar("phimmVal", "phimmVal", 0.,10.);
  phimmVal->setConstant(kTRUE);
  phipmVal= new RooRealVar("phipmVal", "phipmVal", 0.,10.);
  phipmVal->setConstant(kTRUE);
  phip0Val= new RooRealVar("phip0Val", "phip0Val", 0.,10.);
  phip0Val->setConstant(kTRUE);
  phi0mVal= new RooRealVar("phi0mVal", "phi0mVal", 0.,10.);
  phi0mVal->setConstant(kTRUE);
  fz1Val  = new RooRealVar("fz1Val",   "fz1Val",   0.,10.);
  fz1Val->setConstant(kTRUE);
  fz2Val  = new RooRealVar("fz2Val",   "fz2Val",   0.,10.);
  fz2Val->setConstant(kTRUE);
  R1Val   = new RooRealVar("R1Val",    "R1Val",    0.,10.);
  R1Val->setConstant(kTRUE);
  R2Val   = new RooRealVar("R2Val",    "R2Val",    0.,10.);
  R2Val->setConstant(kTRUE);
	  
  para2   = new RooRealVar("para2",  "para2",   -1000.,1000.  );
  para4   = new RooRealVar("para4",  "para4",   -1000.,1000.  );
  para6   = new RooRealVar("para6",  "para6",   -1000.,1000.  );
  para6->setConstant(kTRUE);
  para8   = new RooRealVar("para8",  "para8",   -1000.,1000.  );
  para8->setConstant(kTRUE);
  acca0   = new RooRealVar("acca0",  "acca0",   -1000.,1000.  );
  acca1   = new RooRealVar("acca1",  "acca1",   -1000.,1000.  );
  acca2   = new RooRealVar("acca2",  "acca2",   -1000.,1000.  );
  acca4   = new RooRealVar("acca4",  "acca4",   -1000.,1000.  );
  acca4->setConstant(kTRUE);
  a2      = new RooRealVar("a2",     "a2",      -1000.,1000.  );
  a4      = new RooRealVar("a4",     "a4",      -1000.,1000.  );
  cutOff  = new RooRealVar("cutOff", "cutOff",  0.,2.  );
  g       = new RooRealVar("g",      "g",       0.,1000.  );
  b2      = new RooRealVar("b2",     "b2",      -1000.,1000.  );
  b4      = new RooRealVar("b4",     "b4",      -1000.,1000.  );
  N       = new RooRealVar("N",      "N",       -1000.,1000.  );
  N->setConstant(kTRUE);

  signal = new RooPentaSpinTwo("signal","signal",
			       *cosTheta1,     
			       *cosTheta2,     
			       *phi,    
			       *cosThetaS,     
			       *phi1,
			       *fppVal,
			       *fmmVal,
			       *fpmVal,
			       *fp0Val,
			       *f0mVal,
			       *phippVal,
			       *phimmVal,
			       *phipmVal,
			       *phip0Val,
			       *phi0mVal,
			       *fz1Val, 
			       *fz2Val,
			       *R1Val,
			       *R2Val,  
			       *para2,  
			       *para4,  
			       *para6,  
			       *para8,  
			       *acca0,  
			       *acca1,  
			       *acca2,  
			       *acca4,  
			       *a2,     
			       *a4,     
			       *cutOff, 
			       *g,      
			       *b2,     
			       *b4,     
			       *N 
			       );
			       
  setVars(mH);

}

void signalModel::setVars(double mH){

  fppVal->setVal(.005);  
  fmmVal->setVal(.005);  
  fpmVal->setVal(0.);  
  fp0Val->setVal(0.);  
  f0mVal->setVal(0.);
  
  phippVal->setVal(3.14159);
  phimmVal->setVal(3.14159);
  phipmVal->setVal(0.);
  phip0Val->setVal(0.);
  phi0mVal->setVal(0.);

  fz1Val->setVal(.4);
  fz2Val->setVal(.4);  
  R1Val->setVal(.15);   
  R2Val->setVal(0.);   


  double para2_0= -0.819042;
  double para2_1= 0.0016744;
  double para2_2=0.;
  
  para2->setVal(para2_0+para2_1*mH+para2_2*mH*mH);   

  double para4_0= 1.0512;
  double para4_1= -0.00317063;
  double para4_2=0.;

  para4->setVal(para4_0+para4_1*mH+para4_2*mH*mH);   


  para6->setVal(0.);   


  para8->setVal(0.);   
  
  double acca0_0=990.;
  double acca0_1=0.;
  double acca0_2=0.;

  acca0->setVal(acca0_0+acca0_1*mH+acca0_2*mH*mH);   

  double acca1_0= -45.5558;
  double acca1_1= 0.0945721;
  double acca1_2=0.;

  acca1->setVal(acca1_0+acca1_1*mH+acca1_2*mH*mH);   

  double acca2_0= -173.089;
  double acca2_1= 0.172187;
  double acca2_2=0.;

  acca2->setVal(acca2_0+acca2_1*mH+acca2_2*mH*mH);   


  acca4->setVal(0.);   

  double a2_0= -2.36442;
  double a2_1= 0.00556876;
  double a2_2=0.;

  a2->setVal(a2_0+a2_1*mH+a2_2*mH*mH);      

  double a4_0= 1.50929;
  double a4_1= -0.00584568;
  double a4_2=0.;

  a4->setVal(a4_0+a4_1*mH+a4_2*mH*mH);      

  double cutOff_0= 1.31613;
  double cutOff_1= -0.000331295;
  double cutOff_2=0.;

  cutOff->setVal(cutOff_0+cutOff_1*mH+cutOff_2*mH*mH);  

  double g_0= 0.00503465;
  double g_1= -5.96656e-06;
  double g_2= 0.;

  g->setVal(g_0+g_1*mH+g_2*mH*mH);       

  double b2_0=.13;
  double b2_1=0.;
  double b2_2=0.;

  b2->setVal(b2_0+b2_1*mH+b2_2*mH*mH);      

  double b4_0= -1.02326;
  double b4_1= 0.00136598;
  double b4_2=0.;

  b4->setVal(b4_0+b4_1*mH+b4_2*mH*mH);      

  N->setVal(8.);       

}

backgroundModel::backgroundModel(){

  double mH=200;

  cosTheta1 = new RooRealVar("cosTheta1","cos#theta_{1}", -1.,1. );   
  h1_para2  = new RooRealVar("h1_para2", "h1_para2",  -1000.,1000.);
  h1_para4  = new RooRealVar("h1_para4", "h1_para4",  -1000.,1000.);	    
  h1_acca2  = new RooRealVar("h1_acca2", "h1_acca2",  -1000.,1000.);
  h1_acca4  = new RooRealVar("h1_acca4", "h1_acca4",  -1000.,1000.);	 
  h1_acca4->setConstant(kTRUE);
			     		 	 							
  h1PDF     = new RooBtheta("h1PDF",    "h1PDF",
			     *cosTheta1,
			     *h1_para2,
			     *h1_para4,
			     *h1_acca2,
			     *h1_acca4
			     );

  cosTheta2 = new RooRealVar("cosTheta2","cos#theta_{2}",-1.,1.);
  h2_acca0  = new RooRealVar("h2_acca0", "h2_acca0", -1000.,1000.);
  h2_acca0->setConstant(kTRUE);
  h2_acca2  = new RooRealVar("h2_acca2", "h2_acca2", -1000.,1000.);
  h2_acca4  = new RooRealVar("h2_acca4", "h2_acca4", -1000.,1000.);
  h2_g      = new RooRealVar("h2_g",     "h2_g",     0.,1000.);
  h2_cutOff = new RooRealVar("h2_cutOff","h2_cutOff",0,2.);
  
  h2PDF = new RooBkg2l2jHL("h2PDF","h2PDF",
			   *cosTheta2,
			   *h2_acca0,
			   *h2_acca2,
			   *h2_acca4,
			   *h2_g,
			   *h2_cutOff
			   );

  cosThetaS  = new RooRealVar("cosThetaS","cos#theta^{*}",-1.,1.);
  hs_para2   = new RooRealVar("hs_para2" ,"hs_para2" ,-1000.,1000.);
  hs_para4   = new RooRealVar("hs_para4" ,"hs_para4" ,-1000.,1000.);
  hs_acca2   = new RooRealVar("hs_acca2" ,"hs_acca2" ,-1000.,1000.);
  hs_acca2->setConstant(kTRUE);
  hs_acca4   = new RooRealVar("hs_acca4" ,"hs_acca4" ,-1000.,1000.);
  hs_acca4->setConstant(kTRUE); 
		  
  hsPDF      = new RooBtheta("hsPDF","hsPDF",
			     *cosThetaS,
			     *hs_para2,
			     *hs_para4,
			     *hs_acca2,
			     *hs_acca4
			     );

  phi       = new RooRealVar("phi"      ,"#Phi"      ,-3.1415,3.1415); 
  phi_para2 = new RooRealVar("phi_para2","phi_para2",-1000.,1000.);   
  phi_para2->setConstant(kTRUE);
  phi_para4 = new RooRealVar("phi_para4","phi_para4",-1000.,1000.);   
  phi_para4->setConstant(kTRUE);
  phi_acca2 = new RooRealVar("phi_acca2","phi_acca2",-1000.,1000.);   
  phi_acca4 = new RooRealVar("phi_acca4","phi_acca4",-1000.,1000.);   
  phi_acca6 = new RooRealVar("phi_acca6","phi_acca6",-1000.,1000.);   
  phi_acca6->setConstant(kTRUE);
  phi_acca8 = new RooRealVar("phi_acca8","phi_acca8",-1000.,1000.);   

  phiPDF    = new RooBphione("phiPDF","phiPDF",
			    *phi,
			    *phi_para2,
			    *phi_para4,
			    *phi_acca2,
			    *phi_acca4,
			    *phi_acca6,
			    *phi_acca8
			      );

  phi1       = new RooRealVar("phi1"      ,"#Phi_{1}"      ,-3.1415,3.1415); 
  phi1_para2 = new RooRealVar("phi1_para2","phi1_para2",-1000.,1000.);   
  phi1_para2->setConstant(kTRUE);
  phi1_para4 = new RooRealVar("phi1_para4","phi1_para4",-1000.,1000.);   
  phi1_para4->setConstant(kTRUE);
  phi1_acca2 = new RooRealVar("phi1_acca2","phi1_acca2",-1000.,1000.);   
  phi1_acca4 = new RooRealVar("phi1_acca4","phi1_acca4",-1000.,1000.);   
  phi1_acca6 = new RooRealVar("phi1_acca6","phi1_acca6",-1000.,1000.);   
  phi1_acca6->setConstant(kTRUE);
  phi1_acca8 = new RooRealVar("phi1_acca8","phi1_acca8",-1000.,1000.);   

  phi1PDF = new RooBphione("phi1PDF","phi1PDF",
			   *phi1,
			   *phi1_para2,
			   *phi1_para4,
			   *phi1_acca2,
			   *phi1_acca4,
			   *phi1_acca6,
			   *phi1_acca8  
			   );

  background = new RooProdPdf("background","background",RooArgList(*phi1PDF,*phiPDF,*hsPDF,*h2PDF,*h1PDF));

  setVars(mH);

}

void backgroundModel::setVars(double mH){

  double h1acca2_0= 0.575536;
  double h1acca2_1=0.000852046;
  double h1acca2_2=0.;

  h1_acca2->setVal(h1acca2_0+h1acca2_1*mH+h1acca2_2*mH*mH);

  double h1acca4_0=0.0;
  double h1acca4_1=0.0;
  double h1acca4_2=0.0;

  h1_acca4->setVal(h1acca4_0+h1acca4_1*mH+h1acca4_2*mH*mH);

  double h1para2_0=0.382343;
  double h1para2_1=-0.000626345;
  double h1para2_2=0.;

  h1_para2->setVal(h1para2_0+h1para2_1*mH+h1para2_2*mH*mH);

  double h1para4_0= -1.07047;
  double h1para4_1= 0.000670441;
  double h1para4_2=0.;

  h1_para4->setVal(h1para4_0+h1para4_1*mH+h1para4_2*mH*mH);  

  double h2acca0_0= 2.93756;
  double h2acca0_1= -0.0239946;
  double h2acca0_2= 3.61955e-05;

  h2_acca0->setVal(h2acca0_0+h2acca0_1*mH+h2acca0_2*mH*mH);

  double h2acca2_0= -1.01298;
  double h2acca2_1= .0047841;
  double h2acca2_2= 0.;

    h2_acca2->setVal(h2acca2_0+h2acca2_1*mH+h2acca2_2*mH*mH);

  double h2acca4_0= -0.357522;
  double h2acca4_1= -0.00109411;
  double h2acca4_2=0.;

  h2_acca4->setVal(h2acca4_0+h2acca4_1*mH+h2acca4_2*mH*mH);

  double h2g_0;
  double h2g_1;
  double h2g_2;

  if(mH>=250){
    h2g_0= .0594631;
    h2g_1= -.0000696859;
    h2g_2=0.0;
  }else{
    h2g_0= .488962;
    h2g_1= -.00177202;
    h2g_2= 0.0;
  }

  h2_g->setVal(h2g_0+h2g_1*mH+h2g_2*mH*mH);

  double h2cutOff_0= .692987;
  double h2cutOff_1= .000206678;
  double h2cutOff_2=0.;

  h2_cutOff->setVal(h2cutOff_0+h2cutOff_1*mH+h2cutOff_2*mH*mH);

  double hsacca2_0=0.0;
  double hsacca2_1=0.0;
  double hsacca2_2=0.0;

  hs_acca2->setVal(hsacca2_0+hsacca2_1*mH+hsacca2_2*mH*mH);

  double hsacca4_0=0.0;
  double hsacca4_1=0.0;
  double hsacca4_2=0.0;

  hs_acca4->setVal(hsacca4_0+hsacca4_1*mH+hsacca4_2*mH*mH);  

  double hspara2_0= 1.84553;
  double hspara2_1= -0.0100222;
  double hspara2_2=0.;  

  hs_para2->setVal(hspara2_0+hspara2_1*mH+hspara2_2*mH*mH);

  double hspara4_0=-7.39537;
  double hspara4_1= 0.0403784;
  double hspara4_2=0.;

  hs_para4->setVal(hspara4_0+hspara4_1*mH+hspara4_2*mH*mH);

  phi_acca6->setVal(0.);
  phi_para2->setVal(0.);
  phi_para4->setVal(0.);

  double pacca8_0= 14.6416;
  double pacca8_1= 0.010085;
  double pacca8_2=0.;

  phi_acca8->setVal(pacca8_0+pacca8_1*mH+pacca8_2*mH*mH);

  double pacca2_0= 0.107773;
  double pacca2_1= -0.000254654;
  double pacca2_2=0.;

  phi_acca2->setVal(pacca2_0+pacca2_1*mH+pacca2_2*mH*mH);

  double pacca4_0= -1.78714;
  double pacca4_1= 0.0037279;
  double pacca4_2=0.;

  phi_acca4->setVal(pacca4_0+pacca4_1*mH+pacca4_2*mH*mH);

  phi1_acca6->setVal(0.0);
  phi1_para2->setVal(0.0);
  phi1_para4->setVal(0.0);

  double p1acca8_0 = 100.;
  double p1acca8_1 = 0.;
  double p1acca8_2 = 0.;

  phi1_acca8->setVal(p1acca8_0+p1acca8_1*mH+p1acca8_2*mH*mH);

  double p1acca2_0= 4.39047;
  double p1acca2_1= -0.0254559;
  double p1acca2_2= 3.56541e-05;

  phi1_acca2->setVal(p1acca2_0+p1acca2_1*mH+p1acca2_2*mH*mH);

  double p1acca4_0= 11.6347;
  double p1acca4_1= -0.0750686;
  double p1acca4_2= 8.39119e-05;

  phi1_acca4->setVal(p1acca4_0+p1acca4_1*mH+p1acca4_2*mH*mH);

}


helicityLD::helicityLD(){

  bkgPDF = new backgroundModel();
  sigPDF = new signalModel();
  
}

void helicityLD::setVars(double mH){

  bkgPDF->setVars(mH);
  sigPDF->setVars(mH);

}

double helicityLD::getLD(double h1,
		  double h2,
		  double hs, 
		  double phi,
		  double phi1,
		  double mZZ){

  bkgPDF->setVars(mZZ);  
  bkgPDF->cosTheta1->setVal(h1);
  bkgPDF->cosTheta2->setVal(h2);
  bkgPDF->cosThetaS->setVal(hs);
  bkgPDF->phi->setVal(phi);
  bkgPDF->phi1->setVal(phi1);

  sigPDF->setVars(mZZ);
  sigPDF->cosTheta1->setVal(h1);
  sigPDF->cosTheta2->setVal(h2);
  sigPDF->cosThetaS->setVal(hs);
  sigPDF->phi->setVal(phi);
  sigPDF->phi1->setVal(phi1);

  return sigPDF->signal->getVal()/
    (bkgPDF->background->getVal()+sigPDF->signal->getVal());

}
