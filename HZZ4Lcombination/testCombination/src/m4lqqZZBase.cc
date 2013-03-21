#include "m4lqqZZBase.h"

using namespace std;

m4lqqZZBase::m4lqqZZBase(TString channel_, TString sqrts_, TString tag_, RooRealVar* m4l_){

  channel = channel_;
  sqrts = sqrts_;
  tag = tag_;
  append = channel+"_"+sqrts+"_"+tag;

  m4l = m4l_;
  
}

void m4lqqZZBase::initializePDFs(char* inputs){

  string temp;

  temp =  getInputString(inputs,"qqZZshape a0_bkgd");
  a0 = new RooRealVar("a0_qqZZ"+append,"a0_qqZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"qqZZshape a1_bkgd");
  a1 = new RooRealVar("a1_qqZZ"+append,"a1_qqZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"qqZZshape a2_bkgd");
  a2 = new RooRealVar("a2_qqZZ"+append,"a2_qqZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"qqZZshape a3_bkgd");
  a3 = new RooRealVar("a3_qqZZ"+append,"a3_qqZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"qqZZshape a4_bkgd");
  a4 = new RooRealVar("a4_qqZZ"+append,"a4_qqZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"qqZZshape a5_bkgd");
  a5 = new RooRealVar("a5_qqZZ"+append,"a5_qqZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"qqZZshape a6_bkgd");
  a6 = new RooRealVar("a6_qqZZ"+append,"a6_qqZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"qqZZshape a7_bkgd");
  a7 = new RooRealVar("a7_qqZZ"+append,"a7_qqZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"qqZZshape a8_bkgd");
  a8 = new RooRealVar("a8_qqZZ"+append,"a8_qqZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"qqZZshape a9_bkgd");
  a9 = new RooRealVar("a9_qqZZ"+append,"a9_qqZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"qqZZshape a10_bkgd");
  a10 = new RooRealVar("a10_qqZZ"+append,"a10_qqZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"qqZZshape a11_bkgd");
  a11 = new RooRealVar("a11_qqZZ"+append,"a11_qqZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"qqZZshape a12_bkgd");
  a12 = new RooRealVar("a12_qqZZ"+append,"a12_qqZZ"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"qqZZshape a13_bkgd");
  a13 = new RooRealVar("a13_qqZZ"+append,"a13_qqZZ"+append,atof(temp.c_str()),-10000,10000);

  m4lModel = new RooqqZZPdf_v2("m4lModel_"+append,"m4lModel"+append,
		     *m4l,
		     *a0,*a1,*a2,*a3,*a4,
		     *a5,*a6,*a7,*a8,*a9,
		     *a10,*a11,*a12,*a13);

}

string m4lqqZZBase::getInputString(char* inputs,string inputTag){

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
