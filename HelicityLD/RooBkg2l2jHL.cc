#include <Riostream.h>
#include <math.h>

#include "RooBkg2l2jHL.h"
#include "RooRealVar.h"
#include "RooRealConstant.h"

ClassImp(RooBkg2l2jHL) 

RooBkg2l2jHL::RooBkg2l2jHL(const char *name, const char *title, 
			      RooAbsReal& _x,
			      RooAbsReal& _acca0,
			      RooAbsReal& _acca2,
			      RooAbsReal& _acca4,
			      RooAbsReal& _g,
			      RooAbsReal& _cutOff) :
                              
   RooAbsPdf(name,title), 
   x("x","x",this,_x),
   acca0("acca0","acca0",this,_acca0),
   acca2("acca2","acca2",this,_acca2),
   acca4("acca4","acca4",this,_acca4),
   g("g","g",this,_g),		 
   cutOff("cutOff","cutOff",this,_cutOff) 
 { 
 } 


 RooBkg2l2jHL::RooBkg2l2jHL(const RooBkg2l2jHL& other, const char* name) :  
   RooAbsPdf(other,name), 
   x("x",this,other.x),
   acca0("acca0",this,other.acca0),
   acca2("acca2",this,other.acca2),
   acca4("acca4",this,other.acca4),
   g("g",this,other.g),		 
   cutOff("cutOff",this,other.cutOff) 

 { 
 } 



 double RooBkg2l2jHL::evaluate() const 
 { 
   return (1.0+acca0*x*x)*(1.0+acca2*x*x+acca4*x*x*x*x)/(1 + exp((-cutOff + x)/g));
 } 

