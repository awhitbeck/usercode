#include "signalYields.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

signalYields::signalYields(TString channel_, TString sqrts_, 
			   TString tag_, RooRealVar* mH_){

  channel = channel_;
  sqrts = sqrts_;
  tag = tag_;
  append = channel+"_"+sqrts+"_"+tag;

  mH = mH_;

}

void signalYields::initialize(char* inputs, bool applyCSfilter){

  string temp;

  temp = getInputString(inputs,"lumi");
  double lumi = atof(temp.c_str());

  temp =  getInputString(inputs,"signalEff a1");
  a0 = new RooRealVar("a0_sigYield"+append,"a0_sigYield"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"signalEff a2");
  a1 = new RooRealVar("a1_sigYield_"+append,"a1_sigYield"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"signalEff a3");
  a2 = new RooRealVar("a2_sigYield_"+append,"a3_sigYield"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"signalEff a4");
  a3 = new RooRealVar("a3_sigYield_"+append,"a3_sigYield"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"signalEff b1");
  a4 = new RooRealVar("a4_sigYield_"+append,"a4_sigYield"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"signalEff b2");
  a5 = new RooRealVar("a5_sigYield_"+append,"a5_sigYield"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"signalEff b3");
  a6 = new RooRealVar("a6_sigYield_"+append,"a6_sigYield"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"signalEff g1");
  a7 = new RooRealVar("a7_sigYield_"+append,"a7_sigYield"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"signalEff g2");
  a8 = new RooRealVar("a8_sigYield_"+append,"a8_sigYield"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"signalEff g3");
  a9 = new RooRealVar("a9_sigYield_"+append,"a9_sigYield"+append,atof(temp.c_str()),-10000,10000);

  //----------------------
  RooArgList tempArgList(*a0,*a1,*a2,*a3,*a4,*a5,*a6);
  tempArgList.add(*a7);
  tempArgList.add(*a8);
  tempArgList.add(*a9);
  tempArgList.add(*mH);
  efficiency = new RooFormulaVar("sigEfficiency_"+append,"(@0+@1*TMath::Erf((@7-@2)/@3))*(@4+@5*@7+@6*@7*@7)+@8*TMath::Gaus(@7,@9,@10)",tempArgList);
  csFilter = new RooFormulaVar("csFilter_"+append,"0.5+0.5*TMath::Erf((@0 - 80.85)/50.42)",RooArgList(*mH));

  xsBr = buildXsBrHisto();

  char formulaString[250];
  if(applyCSfilter){
    sprintf(formulaString,"@0*@1*@2*%f*1000",lumi);
    signalRate = new RooFormulaVar("signalRate_"+append,formulaString,RooArgList(*efficiency,*csFilter,*xsBr));
  }else{
    sprintf(formulaString,"@0*@1*%f*1000",lumi);
    signalRate = new RooFormulaVar("signalRate_"+append,formulaString,RooArgList(*efficiency,*xsBr));
  }

}


RooHistFunc* signalYields::buildXsBrHisto(){

  edm::FileInPath fip("HiggsAnalysis/Higgs_CS_and_Width/txtFiles/8TeV-ggH.txt");

  string HCSWpath = fip.fullPath();
  HCSWpath = HCSWpath.substr(0,HCSWpath.size()-12);
  HiggsCSandWidth myCSWrhf(HCSWpath);
  TH1F* histXsBr = new TH1F("xsBr_"+append,"xsBr_"+append,8905,109.55,1000.05);

  double BR=0, XS=0;

  for(int i=1; i<=histXsBr->GetNbinsX(); i++){

    BR = myCSWrhf.HiggsBR(13,histXsBr->GetBinCenter(i));
    XS = myCSWrhf.HiggsCS(1,histXsBr->GetBinCenter(i),8);

    histXsBr->SetBinContent(i,XS*BR);

  }

  dataHistXsBr = new RooDataHist("dataHistXsBr_"+append,"dataHistXsBr_"+append,RooArgList(*mH),histXsBr);

  RooHistFunc* rhfXsBr = new RooHistFunc("histFuncXsBr_"+append,"histFuncXsBr_"+append,RooArgList(*mH),*dataHistXsBr);

  return rhfXsBr;

}

string signalYields::getInputString(char* inputs,string inputTag){

  //cout << inputTag << endl;
  string line;
  ifstream inputFile;
  inputFile.open(inputs);

  if(inputFile.fail()) assert(0);

  while(getline(inputFile,line)){
    
    line.erase(line.begin(), find_if(line.begin(), line.end(), not1(ptr_fun<int, int>(isspace)))); 

    if(line[0] == '#' ) continue;
    
    if(line.find(inputTag)<line.size()-1){
      //cout << line.erase(0,inputTag.size()+1) << endl;
      return line.erase(0,inputTag.size()+1);
    }
    
  }
  
  return "-------FAILURE---------";  
  assert(0);

}
