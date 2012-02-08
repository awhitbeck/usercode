#include <string>
#include "TFile.h"
#include <fstream>
#include "TH2F.h"
#include <sstream>

using namespace std;

void genTemplate(bool isSig=true){


  TFile* inputTempFile;
  if(isSig)
    inputTempFile = new TFile("../datafiles/Dsignal.root");
  else
    inputTempFile = new TFile("../datafiles/Dbackground.root");

  TH2F* mzzDTemplate;
  if(inputTempFile)
    mzzDTemplate = (TH2F*) inputTempFile->Get("h_mzzD");

  if(!inputTempFile)
    return;

  double integral[35]; 
  double checkInt;
  string lines="";
  stringstream convert;

  // ---------------------------------------
  // get normalization 
  // ---------------------------------------

  for(int i=0; i<35; i++){
    integral[i]=0;
    for(int j=0; j<100; j++){
      integral[i]+=mzzDTemplate->GetBinContent(i+1,j+1);
    }
    cout << "integral (" << i << "): " << integral[i] << endl;
  }

  //=====================================
  // apply norm and write to file 
  //=====================================

  for(int i=0; i<35; i++){
    checkInt=0;
    for(int j=0; j<100; j++){

      lines+=" PmzzD[";
      convert << i; 
      lines+=convert.str()+"][";
      convert.str("");
      convert << j;
      lines+=convert.str()+"]=";
      convert.str("");
      if(integral[i]>0){
	convert << (double)mzzDTemplate->GetBinContent(i+1,j+1);
	checkInt[i]+=(double)mzzDTemplate->GetBinContent(i+1,j+1)/integral[i];
      }
      else
	convert << 0.00;
      lines+=convert.str()+";  ";
      convert.str("");

      if(j%10==0)
	lines+="\n";

    }
    cout << "checkInt: " << checkInt << endl;
  }
  
  ifstream templateFile;
  if(isSig)
    templateFile.open("../PDFs/RooNASAModelSig.tpl");
  else
    templateFile.open("../PDFs/RooNASAModelBkg.tpl");
  string templateLine="";
  ofstream newFile;
  if(isSig)
    newFile.open("../PDFs/RooNASAModelSig.cc",ios::out);
  else
    newFile.open("../PDFs/RooNASAModelBkg.cc",ios::out);

  while(templateFile.good() && !templateFile.eof()){

    getline(templateFile,templateLine);
    size_t found=templateLine.find("<INSERT ARRAY>");

    if(found!=string::npos){
      newFile << lines  << endl;
    }else{
      newFile << templateLine << endl;
    }
   
  }

  templateFile.close();
  newFile.close();

}
