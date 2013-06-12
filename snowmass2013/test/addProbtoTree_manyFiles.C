///////////////////////////////
// Add Probabilities to tree //
///////////////////////////////

using namespace RooFit;
bool includePathIsSet = false;

void addProbtoTree_manyFiles(char* inputFile,int flavor, int max=-1, int LHCsqrts=8){
  //flavor: 1:4e, 2:4mu, 3:2e2mu


  gSystem->Load("$CMSSW_BASE/src/ZZMatrixElement/MELA/data/$SCRAM_ARCH/libmcfm.so");
  gSystem->Load("$CMSSW_BASE/lib/slc5_amd64_gcc462/libZZMatrixElementMELA.so");

  // set up path for local cmssw includes
  // as well as roofit
  if (!includePathIsSet) {
    TString path = gSystem->GetIncludePath();
    path += "-I$CMSSW_BASE/src/ ";
    path += "-I$ROOFITSYS/include/ ";
    gSystem->SetIncludePath(path.Data());

    // this is awkward, but want to protect against 
    // multiple additions to the base include path
    // if this function is invoked multiple times per session
    includePathIsSet = true;
  }

  gROOT->LoadMacro("$CMSSW_BASE/src/ZZMatrixElement/MELA/interface/Mela.h+");

  Mela myMELA(LHCsqrts,flavor);
  
  RooMsgService::instance().getStream(1).removeTopic(NumIntegration);

  char inputFileName[500];
  char outputFileName[500];
  sprintf(inputFileName,"%s.root",inputFile);

  TFile* sigFile = new TFile(inputFileName);
  TTree* sigTree=0;
  if(sigFile)
    sigTree = (TTree*) sigFile->Get("SelectedTree");
  if(!sigTree){
    //2nd try with the name of data obs tree
    sigTree = (TTree*) sigFile->Get("data_obs");
    if(!sigTree){
      cout<<"ERROR could not find the tree!"<<endl;
      return;
    }
  }


  float m1,m2,mzz,h1,h2,hs,phi,phi1;  //angles
  float	p0plus_VAJHU,p0minus_VAJHU, psMELA;

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
  for (int j=0;j<136; j++){
    cout<<"Preparing files "<<j<<endl;
  // for (int j=0;j<68; j++){
    sprintf(outputFileName,"%s_withProbabilities_%i.root",inputFile,j);
    TFile* newFile = new TFile(outputFileName,"RECREATE");
    TTree* newTree = new TTree("SelectedTree","SelectedTree");
    //TTree* newTree = (TTree*) sigTree->CloneTree(0,"fast");
    newTree->Branch("psMELA",&psMELA,"psMELA/F");  
    newTree->Branch("Z2Mass",&m2,"Z2Mass/F");
    newTree->Branch("Z1Mass",&m1,"Z1Mass/F");
    newTree->Branch("ZZMass",&mzz,"ZZMass/F");
    newTree->Branch("costhetastar",&hs,"costhetastar/F");
    newTree->Branch("helcosthetaZ1",&h1,"helcosthetaZ1/F");
    newTree->Branch("helcosthetaZ2",&h2,"helcosthetaZ2/F");
    newTree->Branch("helphi",&phi,"helphi/F");
    newTree->Branch("phistarZ1",&phi1,"phistarZ1/F");
    newTree->Branch("p0plus_VAJHU",&p0plus_VAJHU,"p0plus_VAJHU/F");  // higgs, vector algebra, JHUgen
    newTree->Branch("p0minus_VAJHU",&p0minus_VAJHU,"p0minus_VAJHU/F");  // pseudoscalar, vector algebra, JHUgen
     
    for(int iEvt=(j*500); iEvt<500*(j+1); iEvt++){
  
      if(iEvt>=sigTree->GetEntries()) break;
    
      // if ( iEvt != 2 ) continue;
      if(iEvt%1000==1) {
	cout<<"---------\n event: "<<iEvt<<endl;
      }
      sigTree->GetEntry(iEvt);

   
      // 
      // JHU Gen based signal calculations 
      // 
    
      // 0+
      myMELA.setProcess(TVar::HZZ_4l, TVar::JHUGen, TVar::GG);
      myMELA.computeP(mzz, m1, m2, 
		      hs,h1,h2,phi,phi1,flavor, p0plus_VAJHU);
    

      // 0-
      myMELA.setProcess(TVar::PSHZZ_4l, TVar::JHUGen, TVar::GG);
      myMELA.computeP(mzz, m1, m2, 
		      hs,h1,h2,phi,phi1,flavor, p0minus_VAJHU);
    
      psMELA = p0plus_VAJHU/(p0plus_VAJHU+p0minus_VAJHU);
  
      newTree->Fill();
    
    }
  
    newFile->cd();
    newTree->Write("SelectedTree"); 
    newFile->Close();
  
  }
}

