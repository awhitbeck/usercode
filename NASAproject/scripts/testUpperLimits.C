#include "RooGaussian.h"
#include "RooExponential.h"
#include "RooRealVar.h"
#include "../../PDFs/LikelihoodToolbox.cc"
#include "TH1F.h"

void testUpperLimits(){

  //---------------- measurables -------------------

  RooRealVar* mZZ = new RooRealVar("mZZ","mZZ",100,200);
  vector<RooRealVar*> meas;
  meas.push_back(mZZ);

  //---------------- background PDF --------------------

  RooRealVar c("c","c",-.005,-10.,0.);
  c.setConstant(kTRUE);

  RooExponential* exp = new RooExponential("exp","exp",*mZZ,c);

  //---------------- signal PDF -----------------------

  RooRealVar m("m","m",125,100,200);
  m.setConstant(kTRUE);
  RooRealVar w("w","w",5,0,50);
  w.setConstant(kTRUE);

  RooGaussian* gaus =  new RooGaussian("gaus","gaus",*mZZ,m,w);

  //---------------- initialize Toolbox -----------------

  LikelihoodToolbox<RooGaussian,RooExponential> Toolbox(gaus,exp,meas);

  //---------------- plot expected limits ------------------

  TH1F* h = (TH1F*) Toolbox.expectedULDistribution();
  h->Draw();

}
