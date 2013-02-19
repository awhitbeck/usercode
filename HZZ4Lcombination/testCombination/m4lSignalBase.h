#ifndef M4L_SIGNAL_BASE
#define M4L_SIGNAL_BASE

#include "HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"
#include "HiggsAnalysis/CombinedLimit/interface/HZZ4LRooPdfs.h"
#include "TString.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooFormulaVar.h"
//#include "HiggsCSandWidth.h"
//#include "RooDoubleCB.h"
//#include "RooRelBWUFParam.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class m4lSignalBase{

public:

  TString channel;
  TString sqrts;
  TString tag;
  TString append;

  RooRealVar *mH;  
  RooRealVar *m4l;
  RooRealVar *n_CB,*alpha_CB,*n2_CB,*alpha2_CB,*mean_CB,*sigma_CB;
  RooRealVar *mean_BW,*gamma_BW;

  RooRealVar *CMS_zz4l_n,*CMS_zz4l_alpha;
  RooRealVar *CMS_zz4l_scale,*CMS_zz4l_reso;

  RooFormulaVar *n_CB_rfv,*alpha_CB_rfv,*n2_CB_rfv;
  RooFormulaVar *alpha2_CB_rfv,*mean_CB_rfv,*sigma_CB_rfv;

  //HiggsCSandWidth* myCS;

  RooDoubleCB *m4lModel;  

  m4lSignalBase(){};
  m4lSignalBase(TString channel_, TString sqrts_, TString tag_, double mH_);
  void initializePDFs(char* inputs);
  string getInputString(char* inputs,string inputTag);

};

#endif
