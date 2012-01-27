#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "../src/LikelihoodToolbox.cc"
#include "../src/AngularPdfFactory.cc"

using namespace RooFit;

void testLD(double scale=1.0){

  RooRealVar* zzmass = new RooRealVar("zzmass","m_{ZZ}",110,180);
  RooRealVar* z1mass = new RooRealVar("z1mass","m_{Z1}",0,180);
  RooRealVar* z2mass = new RooRealVar("z2mass","m_{Z2}",0,120);
  RooRealVar* costheta1 = new RooRealVar("costheta1","cos#theta_{1}",-1,1);  
  RooRealVar* costheta2 = new RooRealVar("costheta2","cos#theta_{2}",-1,1);
  RooRealVar* phi= new RooRealVar("phi","#Phi",-3.1415,3.1415);

  RooArgSet *argSet = new RooArgSet(*z1mass,*z2mass,*costheta1,*costheta2,*phi);

  vector<RooRealVar*> meas;
  meas.push_back(z1mass);
  meas.push_back(z2mass);
  meas.push_back(costheta1);
  meas.push_back(costheta2);
  meas.push_back(phi);

  cout << "Initializing PDFs" << endl;

  AngularPdfFactory SMHiggs(z1mass,z2mass,costheta1,costheta2,phi,zzmass);
  SMHiggs.makeSMHiggs();
  SMHiggs.makeParamsConst(true);

  AngularPdfFactory PSHiggs(z1mass,z2mass,costheta1,costheta2,phi,zzmass);
  PSHiggs.makePSHiggs();
  PSHiggs.makeParamsConst(true);

  LikelihoodToolbox<RooXZsZs_5D,RooXZsZs_5D> LD(SMHiggs.PDF,PSHiggs.PDF,NULL,meas);

  TFile* file1 = new TFile("../datafiles/SMHiggs_125_JHU.root");
  TTree* tree1 = (TTree*) file1->Get("angles");
  RooDataSet *data1 = new RooDataSet("data1","data1",tree1,RooArgSet(*zzmass,*z1mass,*z2mass,*costheta1,*costheta2,*phi));

  LD.LD_lineColor=4;
  LD.LD_bins=100;
  TH1F *SMLD =new TH1F("SMLD","SMLD",100,0,1);
  LD.makeLDdistribution(*SMLD,data1);

  TFile* file2 = new TFile("../datafiles/PSHiggs_125_JHU.root");
  TTree* tree2 = (TTree*) file2->Get("angles");
  RooDataSet *data2 = new RooDataSet("data2","data2",tree2,RooArgSet(*zzmass,*z1mass,*z2mass,*costheta1,*costheta2,*phi));

  LD.LD_lineColor=2;
  TH1F *PSLD = new TH1F("PSLD","PSLD",100,0,1);
  LD.makeLDdistribution(*PSLD,data2);

  if(SMLD->GetMaximum()>PSLD->GetMaximum()){
    SMLD->Draw();
    PSLD->Draw("SAME");
  }else{
    PSLD->Draw();
    SMLD->Draw("SAME");
  }
  
}
