#include "m4lSignalBase.h"

using namespace std;

m4lSignalBase::m4lSignalBase(TString channel_, TString sqrts_, TString tag_, RooRealVar* mH_, RooRealVar* m4l_){

  channel = channel_;
  sqrts = sqrts_;
  tag = tag_;
  append = channel+"_"+sqrts+"_"+tag;

  // nuisance parameters
  CMS_zz4l_n = new RooRealVar("CMS_zz4l_n_"+append,"CMS_zz4l_n_"+append,0.0,-10.,10.);
  CMS_zz4l_alpha = new RooRealVar("CMS_zz4l_alpha_"+append,"CMS_zz4l_alpha_"+append,0.0,-10.,10.);
  CMS_zz4l_scale = new RooRealVar("CMS_zz4l_scale_"+append,"CMS_zz4l_scale_"+append,1.0,-10.,10.);
  CMS_zz4l_reso = new RooRealVar("CMS_zz4l_reso_"+append,"CMS_zz4l_reso_"+append,0.0,-10.,10.);
  mH = mH_;
  m4l = m4l_;
  
}

void m4lSignalBase::initializePDFs(char* inputs){

  string temp;
  
  temp =  getInputString(inputs,"signalShape n_CB");
  n_CB_rfv = new RooFormulaVar("n_CB_rfv","n_CB_rfv",(temp+"*(1+@1)").c_str(),RooArgList(*mH,*CMS_zz4l_n));
  temp =  getInputString(inputs,"signalShape n2_CB");
  n2_CB_rfv = new RooFormulaVar("n2_CB_rfv","n2_CB_rfv",temp.c_str(),RooArgList(*mH));
  temp =  getInputString(inputs,"signalShape alpha_CB");
  alpha_CB_rfv = new RooFormulaVar("alpha_CB_rfv","alpha_CB_rfv",(temp).c_str(),RooArgList(*mH,*CMS_zz4l_alpha));
  temp =  getInputString(inputs,"signalShape alpha2_CB");
  alpha2_CB_rfv = new RooFormulaVar("alpha2_CB_rfv","alpha2_CB_rfv",temp.c_str(),RooArgList(*mH));
  temp =  getInputString(inputs,"signalShape mean_CB");
  mean_CB_rfv = new RooFormulaVar("mean_CB_rfv","mean_CB_rfv",(temp+"+(@0*@1)").c_str(),RooArgList(*mH,*CMS_zz4l_scale));
  temp =  getInputString(inputs,"signalShape sigma_CB");
  sigma_CB_rfv = new RooFormulaVar("sigma_CB_rfv","sigma_CB_rfv",(temp+"*(1+@1)").c_str(),RooArgList(*mH,*CMS_zz4l_reso));

  m4lModel = new RooDoubleCB("m4lModel_"+append,"m4lModel"+append,
			     *m4l,
			     *mean_CB_rfv,*sigma_CB_rfv,
			     *alpha_CB_rfv,*n_CB_rfv,
			     *alpha2_CB_rfv,*n2_CB_rfv);

}

string m4lSignalBase::getInputString(char* inputs,string inputTag){

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
