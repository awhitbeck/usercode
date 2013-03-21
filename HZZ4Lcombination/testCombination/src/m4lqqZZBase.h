#ifndef M4L_QQZZ_BASE
#define M4L_QQZZ_BASE

#include "HiggsAnalysis/CombinedLimit/interface/HZZ4LRooPdfs.h"
#include "TString.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooFormulaVar.h"
//#include "HiggsCSandWidth.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class m4lqqZZBase{

public:

  TString channel;
  TString sqrts;
  TString tag;
  TString append;

  RooRealVar *m4l;

  //nuisance parameters
  // ---- none ----
  //
 
  RooRealVar *a0,*a1,*a2,*a3,*a4,*a5,*a6;
  RooRealVar *a7,*a8,*a9,*a10,*a11,*a12,*a13;
 

  //HiggsCSandWidth* myCS;

  RooqqZZPdf_v2 *m4lModel;  

  m4lqqZZBase(){};
  m4lqqZZBase(TString channel_, TString sqrts_, TString tag_, RooRealVar* m4l_);
  void initializePDFs(char* inputs);
  string getInputString(char* inputs,string inputTag);

};

#endif
