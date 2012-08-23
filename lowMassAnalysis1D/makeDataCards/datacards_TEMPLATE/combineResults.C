#include "TTree.h"
#include "TFile.h"
#include <vector>
#include <algorithm>
#include <cstring>
#include "TChain.h"
#include <iostream>
#include "plot_golfcourse.C"

using namespace std;

double getObsLimit(char* fileName="130/higgsCombine6channels.MarkovChainMC.mH130.12345.root"){

  char chainInput[50];
  sprintf(chainInput,"%s/limit",fileName);

  cout << "get OBS Limit: " << endl;
  cout << chainInput << endl;

  TChain* tree = new TChain("tree","tree");
  if(tree->Add(chainInput)<=0)
    return -1.;
  double l,m;
  tree->SetBranchAddress("limit",&l);
  tree->GetEntry(0);
  delete tree;
  return l;

}
  

double calcQuantiles(char* fileName="130/higgsCombine1btag.MarkovChainMC.mH130.*.root", float quantile=.50){

  if(quantile < 0. || quantile > 1.){
    return -1.;
  }

  char chainInput[50];
  sprintf(chainInput,"%s/limit",fileName);

  cout << "get Quantile: " << endl;
  cout << chainInput << endl;

  TChain* tree = new TChain("tree","tree");
  if(tree->Add(chainInput)<=0)
    return -1.;

  double l;
  vector<double> limits;
  tree->SetBranchAddress("limit",&l);
  int numToys=tree->GetEntries();

  for(int iToy=0; iToy<tree->GetEntries(); iToy++){
    tree->GetEntry(iToy);
    limits.push_back(l);
  }

  sort(limits.begin(),limits.end());
  delete tree;

  return limits.at((int)(quantile*numToys));
}

void combineResults(char* tag="6channels"){

  char outFile[50];
  sprintf(outFile,"combineResults%s.MarkovChainMC.root",tag);

  TFile* f = new TFile(outFile,"RECREATE");
  TTree* t = new TTree("combineResults","combineResults");
  
  char* massList[37]={"130","131","132","133","134","135","136","137","138","139","140","141",
		      "142","143","144","145","146","147","148","149","150","151","152","153",
		      "154","155","156","157","158","159","160","162","164","166","168","170"};

  double masses[37]={130,131,132,133,134,135,136,137,138,139,140,141,
		   142,143,144,145,146,147,148,149,150,151,152,153,
		   154,155,156,157,158,159,160,162,164,166,168,170};
  double mass,l;
  float q;
  
  t->Branch("mh",&mass);
  t->Branch("quantileExpected",&q);
  t->Branch("limit",&l);
  
  char fileName[50];
  char obsFile[50];

  
  for(int i=0; i<36; i++){

    sprintf(fileName,"%s/higgsCombine%s.MarkovChainMC.mH%s.*.root",massList[i],tag,massList[i]);
    sprintf(obsFile,"%s/higgsCombine%s.MarkovChainMC.mH%s.12345.root",massList[i],tag,massList[i]);

    cout << fileName << endl;
    cout << obsFile << endl;

    mass=masses[i];
    q=-1.00;
    l=getObsLimit(obsFile);
    t->Fill();
    q=.025;
    l=calcQuantiles(fileName,.025);
    t->Fill();
    q=.975;
    l=calcQuantiles(fileName,.975);
    t->Fill();
    q=.50;
    l=calcQuantiles(fileName,.50);
    t->Fill();
    q=.84;
    l=calcQuantiles(fileName,.84);
    t->Fill();
    q=.16;
    l=calcQuantiles(fileName,.16);
    t->Fill();

  }
  f->cd();
  t->Write("limit");
  f->Close();

  plot_golfcourse(tag);

}
