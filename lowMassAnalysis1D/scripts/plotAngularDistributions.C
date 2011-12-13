#include "TFile.h"
#include "TTree.h"
#include "RooRealVar.h"
#include <vector>
#include <string>
#include "RooDataSet.h"
#include "TCanvas.h"
#include "RooPlot.h"

using namespace std;
using namespace RooFit;

void plotAngularDistributions(
			 char* sigFileName="../dataFiles/4fbData/summer11_data_lowmass.root",
			 char* bkgFileName="../dataFiles/4fbData/summer11_SMHiggs_150GeV_lowmass.root",
			 int mZZlowCut=120,
			 int mZZhighCut=180
			 ){


  RooRealVar mZZ("mZZ","mZZ",mZZlowCut,mZZhighCut);
  RooRealVar mJJ("mJJ","mJJ",60,130);
  RooRealVar nBTags("nBTags","nBTags",-1.2,2.2);
  RooRealVar met("met","met",0,1000);
  RooRealVar weight("weight","weight",0,1000);
  RooRealVar cosThetaStar("cosThetaStar","cos(#theta*)",-1,1);
  RooRealVar cosTheta1("cosTheta1","cos(#theta_{1})",-1,1);
  RooRealVar cosTheta2("cosTheta2","cos(#theta_{2})",0,1);
  RooRealVar phi("phi","#Phi",-3.1415,3.1415);
  RooRealVar phiStar1("phiStar1","#Phi*_{1}",-3.1415,3.1415);

  vector<string> saveFile;
  saveFile.push_back("costhetastar.eps");
  saveFile.push_back("costheta1.eps");
  saveFile.push_back("costheta2.eps");
  saveFile.push_back("phi.eps");
  saveFile.push_back("phistar1.eps");
  

  TFile* sigFile = new TFile(sigFileName);
  if(!sigFile){
    std::cout << "ERROR: no file - " << sigFileName << std::endl;
    return;
  }
  TFile* bkgFile = new TFile(bkgFileName);
  if(!bkgFile){
    std::cout << "ERROR: no file - " << sigFileName << std::endl;
    return;
  }

  //------------- Load data ---------------

  TTree* sigTree = (TTree*) sigFile->Get("AngularInfo");
  TTree* bkgTree = (TTree*) bkgFile->Get("AngularInfo");
  if(!bkgTree || !sigTree){
    cout << "ERROR: one or more trees could not be loaded" << endl;
    return;
  }
  std::cout << sigTree << std::endl;
  std::cout << bkgTree << std::endl;

  char cutString_sig[100],cutString_bkg[100];
  sprintf(cutString_sig,"mZZ>%i&&mZZ<%i&&mJJ<105&&mJJ>75",mZZlowCut,mZZhighCut);  
  sprintf(cutString_bkg,"mZZ>%i&&mZZ<%i&&((mJJ<60&&mJJ<75)||(mJJ<150&&mJJ>105))",mZZlowCut,mZZhighCut);

  RooDataSet data_sig("data_sig","data_sig",sigTree,
		      RooArgSet(mZZ,mJJ,weight,cosThetaStar,cosTheta1,cosTheta2,phi,phiStar1),
		      cutString_sig,"weight");
  RooDataSet data_bkg("data_bkg","data_bkg",bkgTree,
		      RooArgSet(mZZ,mJJ,weight,cosThetaStar,cosTheta1,cosTheta2,phi,phiStar1),
		      cutString_bkg);

  //============== plot ===================
  RooPlot* plot[5];
  plot[0] = new RooPlot(cosThetaStar,-1,1,20);
  plot[1] = new RooPlot(cosTheta1,-1,1,20);
  plot[2] = new RooPlot(cosTheta2,0,1,10);
  plot[3] = new RooPlot(phi,-3.1415,3.1415,20);
  plot[4] = new RooPlot(phiStar1,-3.1415,3.1415,20);

  data_sig.plotOn(plot[0],DataError(RooAbsData::SumW2), MarkerColor(4),Normalization(1/data_sig.sumEntries()));
  data_bkg.plotOn(plot[0], MarkerColor(2),Normalization(1/data_bkg.sumEntries()));
  data_sig.plotOn(plot[1],DataError(RooAbsData::SumW2), MarkerColor(4),Normalization(1/data_sig.sumEntries()));
  data_bkg.plotOn(plot[1], MarkerColor(2),Normalization(1/data_bkg.sumEntries()));
  data_sig.plotOn(plot[2],DataError(RooAbsData::SumW2), MarkerColor(4),Normalization(1/data_sig.sumEntries()));
  data_bkg.plotOn(plot[2], MarkerColor(2),Normalization(1/data_bkg.sumEntries()));
  data_sig.plotOn(plot[3],DataError(RooAbsData::SumW2), MarkerColor(4),Normalization(1/data_sig.sumEntries()));
  data_bkg.plotOn(plot[3], MarkerColor(2),Normalization(1/data_bkg.sumEntries()));
  data_sig.plotOn(plot[4],DataError(RooAbsData::SumW2), MarkerColor(4),Normalization(1/data_sig.sumEntries()));
  data_bkg.plotOn(plot[4], MarkerColor(2),Normalization(1/data_bkg.sumEntries()));

  char cName[2];

  TCanvas* c[5];
  for(int i=0; i<5; i++){
    sprintf(cName,"c%i",i);
    c[i] = new TCanvas(cName,cName,400,400);
    plot[i]->Draw();
    c[i]->SaveAs(saveFile.at(i).c_str());
  }
  
}
