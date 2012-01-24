#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "AngularPdfFactory.cc"

using namespace RooFit;

void testLD_withDataset(double scale=1.0){

  RooRealVar* zzmass = new RooRealVar("zzmass","m_{ZZ}",110,180);
  RooRealVar* z1mass = new RooRealVar("z1mass","m_{Z1}",0,120);
  RooRealVar* z2mass = new RooRealVar("z2mass","m_{Z2}",0,120);
  RooRealVar* costheta1 = new RooRealVar("costheta1","cos#theta_{1}",-1,1);  
  RooRealVar* costheta2 = new RooRealVar("costheta2","cos#theta_{2}",-1,1);
  RooRealVar* phi= new RooRealVar("phi","#Phi",-3.1415,3.1415);

  RooArgSet *argSet = 0; //new RooArgSet(*z1mass,*z2mass,*costheta1,*costheta2,*phi);

  vector<RooRealVar*> meas;
  meas.push_back(zzmass);
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

  TFile* file = new TFile("/scratch/hep/ntran/HZZ_materials/datafiles/JHUGenFiles/SMHiggs_125_JHU.root");
  TTree* tree = (TTree*) file->Get("angles");
  RooDataSet *data = new RooDataSet("data","data",tree,RooArgSet(*z1mass,*z2mass,*costheta1,*costheta2,*phi,*zzmass));

  TH1F* SMhisto = new TH1F("SMhisto","SMhisto",50,0.,1.);

  for(int iEvt=0; iEvt<data->sumEntries(); iEvt++){

    if(iEvt%1000==0)
      cout << "iEvt: " << iEvt << endl;
    
    zzmass->setVal(((RooRealVar*)data->get(iEvt)->find(zzmass->GetName()))->getVal());
    //cout << "mZZ: " << zzmass->getVal() << endl;
    z1mass->setVal(((RooRealVar*)data->get(iEvt)->find(z1mass->GetName()))->getVal());
    z2mass->setVal(((RooRealVar*) data->get(iEvt)->find(z2mass->GetName()))->getVal());
    costheta1->setVal(((RooRealVar*) data->get(iEvt)->find(costheta1->GetName()))->getVal());
    costheta2->setVal(((RooRealVar*) data->get(iEvt)->find(costheta2->GetName()))->getVal());
    phi->setVal(((RooRealVar*) data->get(iEvt)->find(phi->GetName()))->getVal());
    
    SMhisto->Fill(1/(1+SMHiggs.getVal(zzmass->getVal())/(PSHiggs.getVal(zzmass->getVal())*scale)));
  }
  
  TFile* file2= new TFile("/scratch/hep/ntran/HZZ_materials/datafiles/JHUGenFiles/PSHiggs_125_JHU.root");
  TTree* tree2= (TTree*) file2->Get("angles");
  RooDataSet *data2 = new RooDataSet("data2","data2",tree2,RooArgSet(*z1mass,*z2mass,*costheta1,*costheta2,*phi,*zzmass));
  
  TH1F* PShisto = new TH1F("PShisto","PShisto",50,0.,1.); 

  for(int iEvt=0; iEvt<data2->sumEntries(); iEvt++){

    if(iEvt%1000==0)
      cout << "iEvt: " << iEvt << endl;

    zzmass->setVal(((RooRealVar*)data2->get(iEvt)->find(zzmass->GetName()))->getVal());
    //cout << "mZZ: " << zzmass->getVal() << endl;
    z1mass->setVal(((RooRealVar*)data2->get(iEvt)->find(z1mass->GetName()))->getVal());
    z2mass->setVal(((RooRealVar*) data2->get(iEvt)->find(z2mass->GetName()))->getVal());
    costheta1->setVal(((RooRealVar*) data2->get(iEvt)->find(costheta1->GetName()))->getVal());
    costheta2->setVal(((RooRealVar*) data2->get(iEvt)->find(costheta2->GetName()))->getVal());
    phi->setVal(((RooRealVar*) data2->get(iEvt)->find(phi->GetName()))->getVal());

    PShisto->Fill(1/(1+SMHiggs.getVal(zzmass->getVal())/(PSHiggs.getVal(zzmass->getVal())*scale)));			
  }
  
  TFile* file3= new TFile("/scratch/hep/ntran/HZZ_materials/datafiles/PowhegFiles/EWKZZ4l_Powheg.root");
  TTree* tree3= (TTree*) file3->Get("angles");
  RooDataSet *data3 = new RooDataSet("data3","data3",tree3,RooArgSet(*z1mass,*z2mass,*costheta1,*costheta2,*phi,*zzmass));
  
  TH1F* EWKZZhisto = new TH1F("EWKZZhisto","EWKZZhisto",50,0.,1.); 

  for(int iEvt=0; iEvt<data3->sumEntries(); iEvt++){

    if(iEvt%1000==0)
      cout << "iEvt: " << iEvt << endl;

    zzmass->setVal(((RooRealVar*)data3->get(iEvt)->find(zzmass->GetName()))->getVal());
    //cout << "mZZ: " << zzmass->getVal() << endl;
    z1mass->setVal(((RooRealVar*)data3->get(iEvt)->find(z1mass->GetName()))->getVal());
    z2mass->setVal(((RooRealVar*) data3->get(iEvt)->find(z2mass->GetName()))->getVal());
    costheta1->setVal(((RooRealVar*) data3->get(iEvt)->find(costheta1->GetName()))->getVal());
    costheta2->setVal(((RooRealVar*) data3->get(iEvt)->find(costheta2->GetName()))->getVal());
    phi->setVal(((RooRealVar*) data3->get(iEvt)->find(phi->GetName()))->getVal());

    EWKZZhisto->Fill(1/(1+SMHiggs.getVal(zzmass->getVal())/(PSHiggs.getVal(zzmass->getVal())*scale)));
  }
  
  SMhisto->Scale(1/SMhisto->Integral());
  SMhisto->SetLineColor(2);
  PShisto->Scale(1/PShisto->Integral());
  PShisto->SetLineColor(4);
  
  TCanvas* can = new TCanvas("can","can",400,400);

  if(SMhisto->GetMaximum()>PShisto->GetMaximum()){
    SMhisto->Draw();  
    PShisto->Draw("SAME");
  }else{
    PShisto->Draw();  
    SMhisto->Draw("SAME");
  }    

  EWKZZhisto->Scale(1/EWKZZhisto->Integral());
  EWKZZhisto->Draw("SAME");

  TLegend* leg = new TLegend(.7,.8,.95,.6);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);

  leg->AddEntry(SMhisto,"SM Higgs (0^{+}","l");
  leg->AddEntry(PShisto,"PS Higgs (0^{-}","l");
  leg->AddEntry(EWKZZhisto,"EWK ZZ","l");

  leg->Draw();

  can->SaveAs("LDdistribution.eps"); 

}
