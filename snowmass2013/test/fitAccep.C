#include "../src/Playground.cc"
#include <vector>

//using namespace PlaygroundHelpers;

void fitAccep(){

  Playground myPG(125.);

  TChain* t = new TChain("SelectedTree");
  t->Add("/scratch0/hep/cyou/Reso_test_0618/0+/SMHiggsToZZTo4L_M-125_14TeV_0_true_wResolution_2e2mu.root");
  
  RooRealVar* z1mass = new RooRealVar("Z1Mass","m_{Z1}",90.,12.,120.);
  RooRealVar* z2mass = new RooRealVar("Z2Mass","m_{Z2}",25.,12.,120.);
  RooRealVar* costhetastar = new RooRealVar("costhetastar","cos#theta*",0.,-1.,1.);  
  RooRealVar* costheta1 = new RooRealVar("helcosthetaZ1","cos#theta_{1}",0.,-1.,1.);
  RooRealVar* costheta2 = new RooRealVar("helcosthetaZ2","cos#theta_{2}",0.,-1.,1.);  
  RooRealVar* phi = new RooRealVar("helphi","#Phi",0.,-TMath::Pi(),TMath::Pi());
  RooRealVar* phi1 = new RooRealVar("phistarZ1","#Phi_{1}",0.,-TMath::Pi(),TMath::Pi());

  RooRealVar* pt1 = new RooRealVar("l1m_pT","pt1",0.,1000.);
  RooRealVar* pt2 = new RooRealVar("l1p_pT","pt2",0.,1000.);
  RooRealVar* pt3 = new RooRealVar("l2m_pT","pt3",0.,1000.);
  RooRealVar* pt4 = new RooRealVar("l2p_pT","pt4",0.,1000.);

  RooRealVar* eta1 = new RooRealVar("l1m_eta","eta1",-4.,4.);
  RooRealVar* eta2 = new RooRealVar("l1p_eta","eta2",-4.,4.);
  RooRealVar* eta3 = new RooRealVar("l2m_eta","eta3",-4.,4.);
  RooRealVar* eta4 = new RooRealVar("l2p_eta","eta4",-4.,4.);  

  RooArgSet min(*z1mass,*z2mass,*costheta1,*costheta2,*costhetastar,*phi,*phi1);
  RooArgSet full(min);
  full.add(*pt1);
  full.add(*pt2);
  full.add(*pt3);
  full.add(*pt4);

  full.add(*eta1);
  full.add(*eta2);
  full.add(*eta3);
  full.add(*eta4);

  RooDataSet* data = new RooDataSet("data","data",t,
				    full,
				    "l1m_pT>5.0&&l1p_pT>5.0&&l2m_pT>5.0&&l2p_pT>5.0&&abs(l1m_eta)<2.4&&abs(l1p_eta)<2.4&&abs(l2m_eta)<2.4&&abs(l2p_eta)<2.4&&Z2Mass>12.0"
				    );

  data.reduce(min);

  myPG.loadTree(data);

  myPG.scalar->_modelParams.fa2->setVal(0.0);
  myPG.scalar->_modelParams.fa2->setConstant(true);
  myPG.scalar->_modelParams.fa3->setVal(0.0);
  myPG.scalar->_modelParams.fa3->setConstant(true);

  myPG.scalar->_modelParams.phia2->setVal(0.0);
  myPG.scalar->_modelParams.phia2->setConstant(true);
  myPG.scalar->_modelParams.phia3->setVal(0.0);
  myPG.scalar->_modelParams.phia3->setConstant(true);  

  bool fitPhi = false;
  //myPG.scalar->_accepParams.aPhi->setConstant(!fitPhi);
  myPG.scalar->_accepParams.bPhi->setConstant(!fitPhi);
  myPG.scalar->_accepParams.cPhi->setConstant(!fitPhi);
  //myPG.scalar->_accepParams.dPhi->setConstant(!fitPhi);

  bool fitPhi1 = false;
  //myPG.scalar->_accepParams.aPhi1->setConstant(!fitPhi1);
  myPG.scalar->_accepParams.bPhi1->setConstant(!fitPhi1);
  myPG.scalar->_accepParams.cPhi1->setConstant(!fitPhi1);
  //myPG.scalar->_accepParams.dPhi1->setConstant(!fitPhi1);

  bool fitH1 = false;
  //myPG.scalar->_accepParams.aH1->setConstant(!fitH1);
  myPG.scalar->_accepParams.bH1->setConstant(!fitH1);
  //myPG.scalar->_accepParams.cH1->setConstant(!fitH1);
  //myPG.scalar->_accepParams.dH1->setConstant(!fitH1);

  bool fitH2 = false;
  //myPG.scalar->_accepParams.aH2->setConstant(!fitH2);
  myPG.scalar->_accepParams.bH2->setConstant(!fitH2);
  //myPG.scalar->_accepParams.cH2->setConstant(!fitH2);
  //myPG.scalar->_accepParams.dH2->setConstant(!fitH2);

  bool fitHs = false;
  //myPG.scalar->_accepParams.aHs->setConstant(!fitHs);
  myPG.scalar->_accepParams.bHs->setConstant(!fitHs);
  //myPG.scalar->_accepParams.cHs->setConstant(!fitHs);
  //myPG.scalar->_accepParams.dHs->setConstant(!fitHs);

  bool fitM1 = false;
  //myPG.scalar->_accepParams.aM1->setConstant(!fitM1);
  myPG.scalar->_accepParams.bM1->setConstant(!fitM1);
  myPG.scalar->_accepParams.cM1->setConstant(!fitM1);
  //myPG.scalar->_accepParams.dM1->setConstant(!fitM1);

  bool fitM2 = false;
  //myPG.scalar->_accepParams.aM2->setConstant(!fitM2);
  myPG.scalar->_accepParams.bM2->setConstant(!fitM2);
  myPG.scalar->_accepParams.cM2->setConstant(!fitM2);
  //myPG.scalar->_accepParams.dM2->setConstant(!fitM2);

  myPG.generate(50000,false);
  //myPG.fitData(true);

  TCanvas* can =new TCanvas("can","can",800,400);
  can->Divide(4,2);

  can->cd(1);
  myPG.projectPDF(PlaygroundHelpers::kz1mass,80,true);
  can->cd(2);
  myPG.projectPDF(PlaygroundHelpers::kz2mass,80,true);
  can->cd(3);
  myPG.projectPDF(PlaygroundHelpers::kcostheta1,20,true);
  can->cd(4);
  myPG.projectPDF(PlaygroundHelpers::kcostheta2,20,true);
  can->cd(5);
  myPG.projectPDF(PlaygroundHelpers::kcosthetastar,20,true);
  can->cd(6);
  myPG.projectPDF(PlaygroundHelpers::kphi,20,true);
  can->cd(7);
  myPG.projectPDF(PlaygroundHelpers::kphi1,20,true);

}
