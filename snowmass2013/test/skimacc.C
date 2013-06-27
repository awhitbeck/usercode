//
// This file applies acceptance cuts to the e+e- samples
// 

#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include <iostream>
#include <fstream>
#include "TH2F.h"
#include "TH1F.h"
#include "TString.h"
#include "TRint.h"
#include "TChain.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TAxis.h"
#include "TMath.h"
#include "TCut.h"

void skimacc(TString inputFile = "bkgData/ee_ZZ_llbb_250GeV_25M_false.root") {
  
  TFile* fin = new TFile(inputFile);
  TTree* ch=(TTree*)fin->Get("SelectedTree"); 
  if (ch==0x0) return; 
  
  TString outputFileName = inputFile; 
  outputFileName.ReplaceAll("false","true");

  std::cout << "Reading " << inputFile << ", writing " <<  outputFileName << "\n";
  
  TFile *newfile= new TFile(outputFileName,"recreate");
  TTree* evt_tree=(TTree*) ch->CloneTree(0, "fast");

  float ptlplus_ALT_ = 0.0;
  float etalplus_ALT_ = 0.0;
  float ptlminus_ALT_ = 0.0;
  float etalminus_ALT_ = 0.0;
  float m_zmass_ = 0.;
  float m_hmass_ = 0.;
  
  ch->SetBranchAddress( "ptlplus_ALT"          , &ptlplus_ALT_      );   
  ch->SetBranchAddress( "etalplus_ALT"         , &etalplus_ALT_     );   
  ch->SetBranchAddress( "ptlminus_ALT"         , &ptlminus_ALT_     );   
  ch->SetBranchAddress( "etalminus_ALT"        , &etalminus_ALT_    );   
  ch->SetBranchAddress( "ZMass"                , &m_zmass_      );   
  ch->SetBranchAddress( "HMass"                , &m_hmass_      );   
  
  

  for(int ievt = 0; ievt < ch->GetEntries() ;ievt++){
    ch->GetEntry(ievt); 
    
    if ( ptlplus_ALT_ < 5. ) continue;
    if ( TMath::Abs(etalplus_ALT_) > 2.4 ) continue;
    if ( ptlminus_ALT_ < 5. ) continue;
    if ( TMath::Abs(etalminus_ALT_) > 2.4 ) continue;
    
    evt_tree->Fill();
    
  }

  std::cout << outputFileName << " has " << evt_tree->GetEntries() << " entries; \n";

  
  newfile->cd(); 
  evt_tree->Write(); 
  newfile->Close();
  
}
