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

using namespace RooFit;

void checkShapeErrors(double nP3=0., double nP4=0.,int btag=0){

  int mH=150;
  int leptType=0;
  double lowMassCut=125;
  double highMassCut=170;

  cout << "initializing measurables..." <<endl;

  RooRealVar mLL("mLL","mZ*",0,120);
  RooRealVar mZZ("mZZ","m_{ZZ}",120,170);
  RooRealVar mJJ("mJJ","m_{jj}",50,150);
  RooRealVar nBTags("nBTags","nBTags",-1,10);
  RooRealVar met("met","met",0,1000);
  RooRealVar lep("lep","lep",0,1000);
  
  cout << "initializing parameters... " << endl;

  // 1  p3           3.45850e+00   1.04901e+00   3.82633e-03  -1.53360e+00
  // 2  p4           3.03334e+00   7.23529e-01   2.81900e-03  -1.53596e+00

  double p3Error=1.04901e+00;
  double p4Error=7.23529e-01; 

  RooRealVar p1("p1","p1",0,0.,100000);
  RooRealVar p2("p2","p2",4.34045e-01,0.,100000);
  RooRealVar p3("p3","p3",3.44987e+00,0.,10000);
  RooRealVar p4("p4","p4",3.07162e+00,0.,10000);

  RooBernstein background("background","background",mZZ,RooArgList(p1,p2,p3,p4));


  RooRealVar p1A("p1A","p1A",0,0.,100000);
  RooRealVar p2A("p2A","p2A",4.34045e-01,0.,100000);
  RooRealVar p3A("p3A","p3A",(3.44987e+00+p3Error*nP3)<0?0.:3.44987e+00+p3Error*nP3,0.,10000);
  RooRealVar p4A("p4A","p4A",(3.07162e+00+p4Error*nP4)<0?0.:3.07162e+00+p4Error*nP4,0.,10000);

  RooBernstein backgroundA("backgroundA","backgroundA",mZZ,RooArgList(p1A,p2A,p3A,p4A));

  char* fileName="../dataFiles/4fbData/summer11_data_4600pb_lowmass.root";
			
  TFile *file = new TFile(fileName);
  char cutString_SB2[100];
  char cutString_SB1[100];
  char cutString_SB0[100];

  sprintf(cutString_SB2,"mZZ<%f&&mZZ>%f&&((mJJ>60&&mJJ<75)||(mJJ>105&&mJJ<130))&&nBTags==2&&met<50",highMassCut,lowMassCut);    
  sprintf(cutString_SB1,"mZZ<%f&&mZZ>%f&&((mJJ>60&&mJJ<75)||(mJJ>105&&mJJ<130))&&nBTags==1",highMassCut,lowMassCut);
  sprintf(cutString_SB0,"mZZ<%f&&mZZ>%f&&((mJJ>60&&mJJ<75)||(mJJ>105&&mJJ<130))&&nBTags==0",highMassCut,lowMassCut);    
  RooDataSet data_SB2("data_SB2","data_SB2",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags,met),cutString_SB2);
  RooDataSet data_SB1("data_SB1","data_SB1",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags,met),cutString_SB1);
  RooDataSet data_SB0("data_SB0","data_SB0",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags,met),cutString_SB0);

  RooPlot *plotmZZ=mZZ.frame(125,170,20);
  if(btag==0)
    data_SB0.plotOn(plotmZZ);
  if(btag==1)
    data_SB1.plotOn(plotmZZ);
  if(btag==2)
    data_SB2.plotOn(plotmZZ);
  background.plotOn(plotmZZ);
  backgroundA.plotOn(plotmZZ,LineColor(2));

  plotmZZ->Draw();

}
