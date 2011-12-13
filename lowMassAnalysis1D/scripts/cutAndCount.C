#include <cstdlib>
#include <iostream>
#include "RooRealVar.h"
#include "TTree.h"
#include "TFile.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TCanvas.h"
#include "RooWorkspace.h"
#include "RooPolynomial.h"
#include "RooBernstein.h"

void cutAndCount(){

  cout << "initializing measurables..." <<endl;

  RooRealVar mLL("mLL","mZ*",0,120);
  RooRealVar mZZ("mZZ","m_{ZZ}",lowMassCut,highMassCut);
  RooRealVar mJJ("mJJ","m_{jj}",50,150);
  RooRealVar nBTags("nBTags","nBTags",-1,10);
  RooRealVar met("met","met",0,1000);
  RooRealVar lep("lep","lep",0,1000);
  RooRealVar p1("p1","p1",0,0.,100000);
  p1.setConstant(kTRUE);
  RooRealVar p2("p2","p2",4.34045e-01,0.,100000);
  p2.setConstant(kTRUE);
  RooRealVar p3("p3","p3",3.44987e+00,0.,10000);
  //p3.setConstant(kTRUE);
  RooRealVar p4("p4","p4",3.07162e+00,0.,10000);
  //p4.setConstant(kTRUE);

  RooBernstein background("background","background",mZZ,RooArgList(p1,p2,p3,p4));

  cout << "loading data..." << endl;

  char cutString_0btag[100],cutString_1btag[100],cutString_2btag[100];
  sprintf(cutString_2btag,"mZZ<%f&&mZZ>%f&&mJJ>75&&mJJ<105&&nBTags==2&&lep==%i&&met<50",highMassCut,lowMassCut,leptType);

  cout << "calculating expected data..." << endl;

  cout << "====================== mZZ sidebands scaling ================== " << endl;
  cout << "Generated events based on background shape according to exact distribution...." << endl;

  RooBernstein normalizer(background,"normalizer");
  RooDataHist* fauxData = normalizer.generateBinned(RooArgSet(CMS_hzz2l2q_mZZ),1000,kTRUE);
  cout << "Generated Number of events: " << fauxData->sumEntries() << endl;
  char sidebandCutString[50];
  sprintf(sidebandCutString,"(CMS_hzz2l2q_mZZ>125&&CMS_hzz2l2q_mZZ<%f)||(CMS_hzz2l2q_mZZ>%f\
&&CMS_hzz2l2q_mZZ<170)",lowMassCut,highMassCut);
  cout << "Sideband selection: " << sidebandCutString << endl;
  alpha = fauxData->sumEntries()/fauxData->sumEntries(sidebandCutString);
  cout << "alpha: " << alpha << endl;
  if(btag==2)
    sprintf(sidebandCutString,"((CMS_hzz2l2q_mZZ>125&&CMS_hzz2l2q_mZZ<%f)||(CMS_hzz2l2q_mZZ\
>%f&&CMS_hzz2l2q_mZZ<170))&&mZjj<105&&mZjj>75&&nBTags==2&&met<50",lowMassCut,highMassCut);
  else
    sprintf(sidebandCutString,"((CMS_hzz2l2q_mZZ>125&&CMS_hzz2l2q_mZZ<%f)||(CMS_hzz2l2q_mZZ>%f&&CMS_hzz2l2q_mZZ<170))&&mZjj<105&&mZjj>75&&nBTags==%i&&leptType==%i",lowMassCut,highMassCut,btag,chan);
  RooDataSet *mZZSidebandData=new RooDataSet("mZZSidebandData","mZZSidebandData",(TTree*)file->Get("AngularInfo"),
                                             RooArgSet(CMS_hzz2l2q_mZZ,nBTags,mZjj,leptType,met),sidebandCutString);
  cout << "Number of Events in Sideband: " <<  mZZSidebandData->sumEntries() << endl;
  if(btag<2)
    exp_yield = mZZSidebandData->sumEntries()*alpha;
  else
    exp_yield = mZZSidebandData->sumEntries()*alpha*.5;
  cout << "Number of expected events [125,170]: " << exp_yield << endl;



}
