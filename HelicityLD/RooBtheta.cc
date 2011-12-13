#include <iostream>
#include <math.h>

#include "RooBtheta.h"
#include "RooRealVar.h"
#include "RooRealConstant.h"

ClassImp(RooBtheta) 

 RooBtheta::RooBtheta(const char *name, const char *title, 
		      RooAbsReal& _x,
		      RooAbsReal& _para2,
                      RooAbsReal& _para4,
		      RooAbsReal& _acca2,
		      RooAbsReal& _acca4) :
   RooAbsPdf(name,title), 
   x("x","x",this,_x),
   para2("para2","para2",this,_para2),
   para4("para4","para4",this,_para4),
   acca2("acca2","acca2",this,_acca2),
   acca4("acca4","acca4",this,_acca4)
 { 
 } 


 RooBtheta::RooBtheta(const RooBtheta& other, const char* name) :  
   RooAbsPdf(other,name), 
   x("x",this,other.x),
   para2("para2",this,other.para2),
   para4("para4",this,other.para4),
   acca2("acca2",this,other.acca2),
   acca4("acca4",this,other.acca4)
 { 
 } 



 double RooBtheta::evaluate() const 
 { 
   return (1.0+para2*x*x+para4*x*x*x*x)*(1.0+acca2*x*x+acca4*x*x*x*x);
 } 

