#ifndef M4L_ZX_BASE
#define M4L_ZX_BASE

#include "RooLandau.h"
#include "TString.h"
#include "RooRealVar.h"
//#include "HiggsCSandWidth.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class m4lZXBase{

public:

  TString channel;
  TString sqrts;
  TString tag;
  TString append;

  RooRealVar *m4l;

  //nuisance parameters
  // ---- none ----
  //
 
  RooRealVar *a0,*a1;
 
  //HiggsCSandWidth* myCS;

  RooLandau *m4lModel;  

  m4lZXBase(){};
  m4lZXBase(TString channel_, TString sqrts_, TString tag_, RooRealVar* m4l_);
  void initializePDFs(char* inputs);
  string getInputString(char* inputs,string inputTag);

};

#endif
