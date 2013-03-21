#ifndef SIGNAL_YIELDS
#define SIGNAL_YIELDS

#include "RooFormulaVar.h"
#include "HiggsAnalysis/Higgs_CS_and_Width/include/HiggsCSandWidth.h"
#include "TH1F.h"
#include "RooHistFunc.h"
#include "RooDataHist.h"
#include "RooRealVar.h"
#include "RooArgList.h"
#include <algorithm>
#include <FWCore/ParameterSet/interface/FileInPath.h>

using namespace std;

class signalYields{

 public:

  TString channel;
  TString sqrts;
  TString tag;
  TString append;

  RooRealVar *a0,*a1,*a2,*a3,*a4;
  RooRealVar *a5,*a6,*a7,*a8;
  RooRealVar *a9,*a10;

  RooFormulaVar *efficiency;
  RooFormulaVar *csFilter;
  RooHistFunc *xsBr;
  RooDataHist *dataHistXsBr;
  RooFormulaVar *signalRate;

  RooRealVar* mH;

  signalYields(){};
  signalYields(TString channel_, TString sqrts_, TString tag_,RooRealVar* mH_);

  RooHistFunc* buildXsBrHisto();
  void initialize(char* inputs, bool applyCSfilter);
  string getInputString(char* inputs,string inputTag);

};

#endif
