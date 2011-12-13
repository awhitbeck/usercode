#include "backgroundModel.h"

using namespace RooFit;
using namespace std;

backgroundModel::backgroundModel(){

  mZZ = new RooRealVar("mZZ","m_{ZZ}",120.,170.); 
  
  p1 = new RooRealVar("p1","p1",0.0,0.,100000.);		 
  p2 = new RooRealVar("p2","p2",.4,0.,100000.);		 
  p3 = new RooRealVar("p3","p3",3.,0.,100000.);		 
  p4 = new RooRealVar("p4","p4",3.,0.,100000.);            
  
  background = new RooBernstein("background","background",*mZZ,RooArgList(*p1,*p2,*p3,*p4));


}

backgroundModel::backgroundModel(double p1Val_,double p2Val_,double p3Val_,double p4Val_){

  if(p1Val_<0.)
    p1Val_=0.;
  if(p2Val_<0.)
    p2Val_=0.;
  if(p3Val_<0.)
    p3Val_=0.;
  if(p4Val_<0.)
    p4Val_=0.;
  
  mZZ = new RooRealVar("mZZ","m_{ZZ}",120.,170.); 
							 
  p1 = new RooRealVar("p1","p1",p1Val_,0.,100000.);		 
  p2 = new RooRealVar("p2","p2",p2Val_,0.,100000.);		 
  p3 = new RooRealVar("p3","p3",p3Val_,0.,100000.);		 
  p4 = new RooRealVar("p4","p4",p4Val_,0.,100000.);            
  
  background = new RooBernstein("background","background",*mZZ,RooArgList(*p1,*p2,*p3,*p4));

}

