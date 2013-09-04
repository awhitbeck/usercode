#include <vector>
#include "TH1F.h"
#include "RooRealVar.h"
#include "TChain.h"
#include "TDirectory.h"
#include "../src/templatePDFwithInterf.h"
#include "RooPlot.h"

using namespace std;

void testInterf(){

  TChain* t = new TChain("SelectedTree");
  t->Add("/scratch0/hep/cyou/AllSamples/JHUGen_v3.1.7/0+/rootfiles/SMHiggsToZZTo4L_M-125_14TeV_2e2mu.root");

  t->Draw("helcosthetaZ1>>T1(20,-1.,1.)");
  TH1F* T1 = (TH1F*) gDirectory->Get("T1");
  t->Draw("helcosthetaZ1>>T2(20,-1.,1.)");
  TH1F* T2 = (TH1F*) gDirectory->Get("T2");
  t->Draw("helcosthetaZ1>>T3(20,-1.,1.)");
  TH1F* T3 = (TH1F*) gDirectory->Get("T3");

  cout << T1 << " " << T2 << " " << T3 << endl;
  
  vector<TH1F*> histos;
  histos.push_back(T1);
  histos.push_back(T2);
  histos.push_back(T3);

  RooRealVar x("x","x",0.,-1.,1.);
  RooRealVar coef("coef","coef",0.5,0.,1.);

  templatePDFwithInterf test("test","test",x,histos,coef);

  RooPlot* plot = x.frame();
  test.plotOn(plot);
 
  plot->Draw();

}
