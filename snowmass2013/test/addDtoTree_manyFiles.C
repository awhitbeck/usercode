//THIS HAS TO BE USED INSIDE THE MELA PACKAGE!!!!!!!!
// FOLLOW THE INSTRUCTIONS IN: 
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/MELAProject
using namespace RooFit;


void addDtoTree_manyFiles(char* inputFile,int max=-1, int LHCsqrts=8){

  gSystem->Load("/afs/cern.ch/user/y/yygao/public/libmcfm.so");
  gSystem->Load("$CMSSW_BASE/lib/slc5_amd64_gcc462/libZZMatrixElementMELA.so");
  gROOT->LoadMacro("../interface/PseudoMELA.h+");

  PseudoMELA myPseudoMELA;

  RooMsgService::instance().getStream(1).removeTopic(NumIntegration);

  char inputFileName[100];
  char outputFileName[150];
  sprintf(inputFileName,"%s.root",inputFile);

  TChain* sigTree = new TChain("SelectedTree");
  sigTree->Add(inputFileName);
  
  if(!sigTree){
    cout<<"ERROR could not find the tree!"<<endl;
    return;
  }


  float m1,m2,mzz,h1,h2,hs,phi,phi1;
  float pt4l, Y4l;
  float psig, pbkg, psMELA;

  // -------- CJLST TREES ---------------
  sigTree->SetBranchAddress("Z2Mass",&m2);
  sigTree->SetBranchAddress("Z1Mass",&m1);
  sigTree->SetBranchAddress("ZZMass",&mzz);
  sigTree->SetBranchAddress("costhetastar",&hs);
  sigTree->SetBranchAddress("helcosthetaZ1",&h1);
  sigTree->SetBranchAddress("helcosthetaZ2",&h2);
  sigTree->SetBranchAddress("helphi",&phi);
  sigTree->SetBranchAddress("phistarZ1",&phi1);

  //---------------------------------------*/
  //for (int j=0;j<136; j++){
   for (int j=0;j<68; j++){
     TTree* newTree = new TTree("SelectedTree","SelectedTree");
     
     newTree->Branch("psMELA",&psMELA,"psMELA/F");  
     newTree->Branch("Z2Mass",&m2,"Z2Mass/F");
     newTree->Branch("Z1Mass",&m1,"Z1Mass/F");
     newTree->Branch("ZZMass",&mzz,"ZZMass/F");
     newTree->Branch("costhetastar",&hs,"costhetastar/F");
     newTree->Branch("helcosthetaZ1",&h1,"helcosthetaZ1/F");
     newTree->Branch("helcosthetaZ2",&h2,"helcosthetaZ2/F");
     newTree->Branch("helphi",&phi,"helphi/F");
     newTree->Branch("phistarZ1",&phi1,"phistarZ1/F");
     for(int iEvt=(j*500); iEvt<500*(j+1); iEvt++){
       
      if(iEvt>=sigTree->GetEntries()) {
	cout<<"tree endeed at sample "<<j<<" with event "<<iEvt<<endl;
	break;
      }

    if(iEvt%1000==0) 
      cout << "event: " << iEvt << endl;

    sigTree->GetEntry(iEvt);

    // --------------------------------

    
    /*cout << mzz << endl;
    cout << m1 << endl;
   cout << m2 << endl;
    cout << h1 << endl;
    cout << h2 << endl;
    cout << hs << endl;
    cout << phi << endl;
    cout << phi1 << endl;
    */

    myPseudoMELA.computeKD((float)mzz,(float)m1,(float)m2,(float)hs,(float)h1,(float)h2,(float)phi,(float)phi1,psMELA,psig,pbkg);
    newTree->Fill();

   }

    sprintf(outputFileName,"%s_withDiscriminants_%i.root",inputFile,j);
  TFile* newFile = new TFile(outputFileName,"RECREATE");
  newFile->cd();
  newTree->Write("SelectedTree"); 
  newFile->Close();
  }

}

