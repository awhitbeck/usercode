#include "LikelihoodDiscriminant.cc"
#include "ScalarPdfFactory.cc"

class pseudoMELA: public LikelihoodDiscriminant{

public:

  ScalarPdfFactory *model;
  ScalarPdfFactory *altModel;


  pseudoMELA();
  ~pseudoMELA();
  double eval(double zzmass, double z1mass, 
	      double z2mass, double costhetstar, 
	      double costheta1, double costheta2, 
	      double phi, double phistar1);

};

pseudoMELA::pseudoMELA(){

  model = new ScalarPdfFactory(z1mass_rrv,z2mass_rrv,costheta1_rrv,costheta2_rrv,phi_rrv,mzz_rrv);
  model->makeSMHiggs();
  model->makeParamsConst(true);

  altModel = new ScalarPdfFactory(z1mass_rrv,z2mass_rrv,costheta1_rrv,costheta2_rrv,phi_rrv,mzz_rrv);

  altModel->makePSHiggs();
  altModel->makeParamsConst(true);

}

pseudoMELA::~pseudoMELA(){

  delete model;
  delete altModel;
 
}

double pseudoMELA::eval(double zzmass, double z1mass, 
		      double z2mass, double costhetastar, 
		      double costheta1, double costheta2, 
		      double phi, double phistar1){

    checkZorder(z1mass,z2mass,costhetastar,costheta1,costheta2,phi,phistar1);

    z1mass_rrv->setVal(z1mass);
    z2mass_rrv->setVal(z2mass);
    costhetastar_rrv->setVal(costhetastar);
    costheta1_rrv->setVal(costheta1);
    costheta2_rrv->setVal(costheta2);
    phi_rrv->setVal(phi);
    phistar1_rrv->setVal(phistar1);

    mzz_rrv->setVal(zzmass);

    //cout << "model: " << model->getVal(zzmass) << endl;
    //cout << "altModel: " << altModel->getVal(zzmass) << endl;
    //cout << "result: " << 1/(1+altModel->getVal(zzmass)/(1*model->getVal(zzmass))) << endl;
    return 1/(1+altModel->getVal(zzmass)/(5*model->getVal(zzmass)));
    
}
