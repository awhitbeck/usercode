#ifndef LIKELIHOOD_DISCRIMINANT
#define LIKELIHOOD_DISCRIMINANT

#include "RooRealVar.h"
#include "RooAbsPdf.h"

class LikelihoodDiscriminant{

public:

  RooRealVar* z1mass_rrv;
  RooRealVar* z2mass_rrv;
  RooRealVar* costheta1_rrv;
  RooRealVar* costheta2_rrv;
  RooRealVar* phi_rrv;
  RooRealVar* costhetastar_rrv;
  RooRealVar* phistar1_rrv;
  RooRealVar* mzz_rrv;

  LikelihoodDiscriminant();
  ~LikelihoodDiscriminant();

  void checkZorder(double& z1mass, double& z2mass,
		   double& costhetastar, double& costheta1,
				   double& costheta2, double& phi,
				   double& phistar1);
  
};

LikelihoodDiscriminant::LikelihoodDiscriminant(){

  z1mass_rrv = new RooRealVar("z1mass","m_{Z1}",0,180);
  z2mass_rrv = new RooRealVar("z2mass","m_{Z2}",0,120);
  costheta1_rrv = new RooRealVar("costheta1","cos#theta_{1}",-1,1);
  costheta2_rrv = new RooRealVar("costheta2","cos#theta_{2}",-1,1);
  phi_rrv= new RooRealVar("phi","#Phi",-3.1415,3.1415);
  costhetastar_rrv = new RooRealVar("costhetastar","cos#theta^{*}",-1,1);
  phistar1_rrv= new RooRealVar("phistar1","#Phi^{*}_{1}",-3.1415,3.1415);
  mzz_rrv= new RooRealVar("mzz","mZZ",80,1000);

}

LikelihoodDiscriminant::~LikelihoodDiscriminant(){

  delete z1mass_rrv;
  delete z2mass_rrv;
  delete costheta1_rrv;
  delete costheta2_rrv;
  delete phi_rrv;
  delete costhetastar_rrv;
  delete phistar1_rrv;
  delete mzz_rrv;

}

void LikelihoodDiscriminant::checkZorder(double& z1mass, double& z2mass,
			    double& costhetastar, double& costheta1,
			    double& costheta2, double& phi,
			    double& phistar1){
  
  double tempZ1mass=z1mass;
  double tempZ2mass=z2mass;
  double tempH1=costheta1;
  double tempH2=costheta2;
  double tempHs=costhetastar;
  double tempPhi1=phistar1;
  double tempPhi=phi;

  if(z2mass>z1mass){

    z1mass=tempZ2mass;
    z2mass=tempZ1mass;
    costhetastar=-tempHs;
    costheta1=tempH2;
    costheta2=tempH1;
    phi=tempPhi;
    phistar1=-tempPhi1-tempPhi;
    if(phistar1>3.1415)
      phistar1=phistar1-2*3.1415;
    if(phistar1<-3.1415)
      phistar1=phistar1+2*3.1415;

  }else

    return;

}

#endif



