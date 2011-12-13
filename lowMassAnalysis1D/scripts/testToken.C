#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<double>* testToken(char* fileName="~/2l2jHelicity/CMSSW_4_2_2/src/HiggsAnalysis/CombinedLimit/test/lowMassAnalysis/makeDataCards/datacards_20111115_newPUapprox_125/150/hzz2l2q_ee0b.input.root"){

  string line="";
  char* str="";
  char* temp="";
  vector<double>* rates;

  cout << " opening file " << endl;

  ifstream blah("hzz2l2q_ee0b.150.txt");

  if(blah.is_open()){
    while(blah.good()){
      blah >> line;
      if(line=="rate"){
	blah >> line;
	
      }
    }

    blah.close();
    
  }else 
    cout << "ERROR opening file... " << endl;

  return 0;

}
