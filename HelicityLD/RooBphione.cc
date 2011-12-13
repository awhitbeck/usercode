#include <iostream>
#include <math.h>

#include "RooBphione.h"
#include "RooRealVar.h"
#include "RooRealConstant.h"

ClassImp(RooBphione) 

 RooBphione::RooBphione(const char *name, const char *title, 
			RooAbsReal& _x,
			RooAbsReal& _para2,
			RooAbsReal& _para4,
			RooAbsReal& _acca2,
			RooAbsReal& _acca4,
			RooAbsReal& _acca6,
			RooAbsReal& _acca8) :
   RooAbsPdf(name,title), 
   x("x","x",this,_x),
   para2("para2","para2",this,_para2),
   para4("para4","para4",this,_para4),
   acca2("acca2","acca2",this,_acca2),
   acca4("acca4","acca4",this,_acca4),
   acca6("acca6","acca6",this,_acca6),
   acca8("acca8","acca8",this,_acca8)
 { 
 } 


 RooBphione::RooBphione(const RooBphione& other, const char* name) :  
   RooAbsPdf(other,name), 
   x("x",this,other.x),
   para2("para2",this,other.para2),
   para4("para4",this,other.para4),
   acca2("acca2",this,other.acca2),
   acca4("acca4",this,other.acca4),
   acca6("acca6",this,other.acca6),
   acca8("acca8",this,other.acca8)
 { 
 } 
 double RooBphione::evaluate() const 
 { 
   double accp = acca2*cos(x)+acca4*cos(2.0*x)+acca6*cos(4.0*x)+acca8;

   double heli = 1.0+para2*pow(x,2)+para4*pow(x,4);

   double heliCorr = heli*accp;

   if((heliCorr<=0.0)) heliCorr = 0.0;

   return heliCorr;

 } 

