#include "m4lggZZBase.h"

using namespace std;

m4lggZZBase::m4lggZZBase(TString channel_, TString sqrts_, TString tag_, RooRealVar* m4l_){

  channel = channel_;
  sqrts = sqrts_;
  tag = tag_;
  append = channel+"_"+sqrts+"_"+tag;

  m4l = m4l_;
  
}

void m4lggZZBase::initializePDFs(char* inputs){

  string temp;

  temp =  getInputString(inputs,"ggZZshape a0_bkgd");
  a0 = new RooRealVar("a0_ggZZ"+append,"a0_ggZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"ggZZshape a1_bkgd");
  a1 = new RooRealVar("a1_ggZZ"+append,"a1_ggZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"ggZZshape a2_bkgd");
  a2 = new RooRealVar("a2_ggZZ"+append,"a2_ggZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"ggZZshape a3_bkgd");
  a3 = new RooRealVar("a3_ggZZ"+append,"a3_ggZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"ggZZshape a4_bkgd");
  a4 = new RooRealVar("a4_ggZZ"+append,"a4_ggZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"ggZZshape a5_bkgd");
  a5 = new RooRealVar("a5_ggZZ"+append,"a5_ggZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"ggZZshape a6_bkgd");
  a6 = new RooRealVar("a6_ggZZ"+append,"a6_ggZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"ggZZshape a7_bkgd");
  a7 = new RooRealVar("a7_ggZZ"+append,"a7_ggZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"ggZZshape a8_bkgd");
  a8 = new RooRealVar("a8_ggZZ"+append,"a8_ggZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"ggZZshape a9_bkgd");
  a9 = new RooRealVar("a9_ggZZ"+append,"a9_ggZZ"+append,atof(temp.c_str()),-10000,10000);

  m4lModel = new RooggZZPdf_v2("m4lModel_"+append,"m4lModel"+append,
			       *m4l,
			       *a0,*a1,*a2,*a3,*a4,
			       *a5,*a6,*a7,*a8,*a9);


}

string m4lggZZBase::getInputString(char* inputs,string inputTag){

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
