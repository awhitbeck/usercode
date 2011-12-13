#include "RooPlot.h"
#include "backgroundModel.h"
#include "TCanvas.h"

using namespace RooFit;

void compareCurves(int btag=0){

  backgroundModel SBFit(0.0,.43045,3.44987,3.07162);
  backgroundModel sigFit(0.0,.60493,3.56323,3.23295);
  
  RooPlot* plot = sigFit.mZZ->frame(125,170,20);

  //======== get data ==========
  double lowMassCut=120, highMassCut=170;

  RooRealVar mLL("mLL","mZ*",0,120);
  RooRealVar mJJ("mJJ","m_{jj}",50,150);
  RooRealVar nBTags("nBTags","nBTags",-1,10);
  RooRealVar met("met","met",0,1000);

  char* fileName="../../dataFiles/4fbData/summer11_data_4600pb_lowmass.root";
  TFile *file = new TFile(fileName);
  if(!file)
    return;
  char cutString_SB[100],cutString[100];
  char cutString_0btag[100],cutString_1btag[100],cutString_2btag[100];
  sprintf(cutString_SB,"mZZ<%f&&mZZ>%f&&((mJJ>60&&mJJ<75)||(mJJ>105&&mJJ<130))",highMassCut,lowMassCut);
  sprintf(cutString,"mZZ<%f&&mZZ>%f&&mJJ<105&&mJJ>75",highMassCut,lowMassCut);
  sprintf(cutString_0btag,"mZZ<%f&&mZZ>%f&&mJJ>75&&mJJ<105&&nBTags==0",highMassCut,lowMassCut);
  sprintf(cutString_1btag,"mZZ<%f&&mZZ>%f&&mJJ>75&&mJJ<105&&nBTags==1",highMassCut,lowMassCut);
  sprintf(cutString_2btag,"mZZ<%f&&mZZ>%f&&mJJ>75&&mJJ<105&&nBTags==2&&met<50",highMassCut,lowMassCut);

  RooDataSet data_SB("data_SB","data_SB",(TTree*)file->Get("AngularInfo"),RooArgSet(*sigFit->mZZ,mLL,mJJ),cutString_SB);
  RooDataSet data("data","data",(TTree*)file->Get("AngularInfo"),RooArgSet(*sigFit->mZZ,mLL,mJJ),cutString);
  RooDataSet data_0btag("data_0btag","data_0btag",(TTree*)file->Get("AngularInfo"),RooArgSet(*sigFit->mZZ,mLL,mJJ,nBTags),cutString_0btag);
  RooDataSet data_1btag("data_1btag","data_1btag",(TTree*)file->Get("AngularInfo"),RooArgSet(*sigFit->mZZ,mLL,mJJ,nBTags),cutString_1btag);
  RooDataSet data_2btag("data_2btag","data_2btag",(TTree*)file->Get("AngularInfo"),RooArgSet(*sigFit->mZZ,mLL,mJJ,nBTags,met),cutString_2btag);
  //============================

  if(btag==0)
    data_0btag.plotOn(plot);
  if(btag==1)
    data_1btag.plotOn(plot);
  if(btag==2)
    data_2btag.plotOn(plot);
  if(btag==-1)
    data.plotOn(plot);

  sigFit.background->plotOn(plot,LineColor(2));
  SBFit.background->plotOn(plot);

  plot->Draw();

}
