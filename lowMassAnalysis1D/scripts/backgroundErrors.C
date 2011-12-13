#include "backgroundModel.h"
#include "RooPlot.h"
#include "TCanvas.h"
#include "TRandom.h"

using namespace RooFit; 

void backgroundErrors(int btag=0, int n=10){


  // for inflating errors
  double multFactor;
  if(btag==0)
    multFactor=1;
  if(btag==1)
    multFactor=2;
  if(btag==2)
    multFactor=6;

  const int numCurves=n;

  backgroundModel* bkgPDF[numCurves];

  double paramCenter[4]={0.,.434045,3.44987,3.07162}; 
  double paramError[4]={0.*multFactor,0.*multFactor,1.04942*multFactor,.739223*multFactor};
  double paramVal[4]={0.,0.,0.,0.};

  TRandom* r = new TRandom();

  for(int iVar=0; iVar<numCurves; iVar++){

    for(int iParam=0; iParam<4; iParam++){
      paramVal[iParam]=r->Gaus(paramCenter[iParam],paramError[iParam]);
      while(!(fabs(paramVal[iParam]-paramCenter[iParam])<=paramError[iParam]))
	paramVal[iParam]=r->Gaus(paramCenter[iParam],paramError[iParam]-paramCenter[iParam]);
    }

    bkgPDF[iVar] = new backgroundModel(paramVal[0],paramVal[1],paramVal[2],paramVal[3]);
    
  }


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
  char cutString_SB[100], cutString[100];
  char cutString_0btag[100],cutString_1btag[100],cutString_2btag[100];
  char cutString_SB0[100],cutString_SB1[100],cutString_SB2[100];
  
  sprintf(cutString_SB,"mZZ<%f&&mZZ>%f&&((mJJ>60&&mJJ<75)||(mJJ>105&&mJJ<130))",highMassCut,lowMassCut);
  sprintf(cutString,"mZZ<%f&&mZZ>%f&&mJJ<105&&mJJ>75",highMassCut,lowMassCut);
  sprintf(cutString_SB0,"mZZ<%f&&mZZ>%f&&((mJJ>60&&mJJ<75)||(mJJ>105&&mJJ<130))&&nBTags==0",highMassCut,lowMassCut);
  sprintf(cutString_SB1,"mZZ<%f&&mZZ>%f&&((mJJ>60&&mJJ<75)||(mJJ>105&&mJJ<130))&&nBTags==1",highMassCut,lowMassCut);
  sprintf(cutString_SB2,"mZZ<%f&&mZZ>%f&&((mJJ>60&&mJJ<75)||(mJJ>105&&mJJ<130))&&nBTags==2&&met<50",highMassCut,lowMassCut);

  sprintf(cutString_0btag,"mZZ<%f&&mZZ>%f&&mJJ>75&&mJJ<105&&nBTags==0",highMassCut,lowMassCut);
  sprintf(cutString_1btag,"mZZ<%f&&mZZ>%f&&mJJ>75&&mJJ<105&&nBTags==1",highMassCut,lowMassCut);
  sprintf(cutString_2btag,"mZZ<%f&&mZZ>%f&&mJJ>75&&mJJ<105&&nBTags==2&&met<50",highMassCut,lowMassCut);

  RooDataSet data_SB("data_SB","data_SB",(TTree*)file->Get("AngularInfo"),RooArgSet(*bkgPDF[0]->mZZ,mLL,mJJ),cutString_SB);
  RooDataSet data("data","data",(TTree*)file->Get("AngularInfo"),RooArgSet(*bkgPDF[0]->mZZ,mLL,mJJ),cutString);
  RooDataSet data_SB0("data_SB0","data_SB0",(TTree*)file->Get("AngularInfo"),RooArgSet(*bkgPDF[0]->mZZ,mLL,mJJ,nBTags),cutString_SB0);
  RooDataSet data_SB1("data_SB1","data_SB1",(TTree*)file->Get("AngularInfo"),RooArgSet(*bkgPDF[0]->mZZ,mLL,mJJ,nBTags),cutString_SB1);
  RooDataSet data_SB2("data_SB2","data_SB2",(TTree*)file->Get("AngularInfo"),RooArgSet(*bkgPDF[0]->mZZ,mLL,mJJ,nBTags,met),cutString_SB2);
  RooDataSet data_0btag("data_0btag","data_0btag",(TTree*)file->Get("AngularInfo"),RooArgSet(*bkgPDF[0]->mZZ,mLL,mJJ,nBTags),cutString_0btag);
  RooDataSet data_1btag("data_1btag","data_1btag",(TTree*)file->Get("AngularInfo"),RooArgSet(*bkgPDF[0]->mZZ,mLL,mJJ,nBTags),cutString_1btag);
  RooDataSet data_2btag("data_2btag","data_2btag",(TTree*)file->Get("AngularInfo"),RooArgSet(*bkgPDF[0]->mZZ,mLL,mJJ,nBTags,met),cutString_2btag);
  //============================

  RooPlot* plot =bkgPDF[0]->mZZ->frame(125,170,20);
  
  if(btag==0)
    data_SB0.plotOn(plot);
  if(btag==1)
    data_SB1.plotOn(plot);
  if(btag==2)
    data_SB2.plotOn(plot);
  if(btag==-1)
    data_SB.plotOn(plot);

  for(int iVar=0; iVar<numCurves; iVar++){
    bkgPDF[iVar]->background->plotOn(plot,LineColor(3));
  }

  backgroundModel nominal(paramCenter[0],paramCenter[1],paramCenter[2],paramCenter[3]);
  nominal.background->plotOn(plot);

  if(btag==0)
    data_SB0.plotOn(plot);
  if(btag==1)
    data_SB1.plotOn(plot);
  if(btag==2)
    data_SB2.plotOn(plot);
  if(btag==-1)
    data_SB.plotOn(plot);


  plot->Draw();

}
