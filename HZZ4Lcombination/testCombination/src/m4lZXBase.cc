#include "m4lZXBase.h"

using namespace std;

m4lZXBase::m4lZXBase(TString channel_, TString sqrts_, TString tag_, RooRealVar* m4l_){

  channel = channel_;
  sqrts = sqrts_;
  tag = tag_;
  append = channel+"_"+sqrts+"_"+tag;

  m4l = m4l_;
  
}

void m4lZXBase::initializePDFs(char* inputs){

  string temp;

  temp =  getInputString(inputs,"zjetsShape mean");
  a0 = new RooRealVar("a0_ZX"+append,"a0_ZX"+append,atof(temp.c_str()),-10000,10000);

  temp =  getInputString(inputs,"zjetsShape sigma");
  a1 = new RooRealVar("a1_ZX"+append,"a1_ZX"+append,atof(temp.c_str()),-10000,10000);

  m4lModel = new RooLandau("m4lModel_"+append,"m4lModel"+append,
			   *m4l,
			   *a0,*a1);

}

string m4lZXBase::getInputString(char* inputs,string inputTag){

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
