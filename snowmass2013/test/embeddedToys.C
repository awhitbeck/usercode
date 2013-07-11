#include "../src/Playground.cc"

//[#0] ERROR:InputArguments -- RooAddPdf::getAnalyticalIntegral(model) WARNING: component PDF PDF   advertises inconsistent set of integrals (e.g. (X,Y) but not X or Y individually.   Distributed analytical integration disabled. Please fix PDF

using namespace PlaygroundHelpers;

enum sample {kScalar_fa3p0,kScalar_fa3p1,kScalar_fa3p5,kScalar_fa3p5phia390,kScalar_fa3p25,kScalar_fa2p1fa3p1,kNumSamples};
TString sampleName[kNumSamples] = {"fa3p0","fa3p1","fa3p5","fa3p5phia390","fa3p25","fa2p1fa3p1"};
enum freeParams {kfa3Only,kfa3phia3Only,kfa2Only,kfa2phia2Only,kfa2fa3Only,kAllFree,kNumScenarios};
TString scenario[kNumScenarios] = {"fa3_free","fa3_phia3_free","fa2_free","fa2_phia2_free","fa2_fa3_free","all_free"};

TString inputFileNames[kNumSamples] = {"/afs/cern.ch/user/s/sbologne/workspace/multidimFit_prophecy/ZZMatrixElement/snowmass2013/test/samples/JHUpsMELA_m2cut_2e2mu/SMHiggsToZZTo4L_M-125_14TeV_2e2mu_withProbabilities.root",
				       "/afs/cern.ch/user/s/sbologne/workspace/multidimFit_prophecy/ZZMatrixElement/snowmass2013/test/samples/JHUpsMELA_m2cut_2e2mu/Higgs0Mf01ph0ToZZTo4L_M-125_14TeV_2e2mu_withProbabilities.root",
				       "/afs/cern.ch/user/s/sbologne/workspace/multidimFit_prophecy/ZZMatrixElement/snowmass2013/test/samples/JHUpsMELA_m2cut_2e2mu/Higgs0Mf05ph0ToZZTo4L_M-125_14TeV_2e2mu_withProbabilities.root",
				       "/afs/cern.ch/user/s/sbologne/workspace/multidimFit_prophecy/ZZMatrixElement/snowmass2013/test/samples/JHUpsMELA_m2cut_2e2mu/Higgs0Mf05ph90ToZZTo4L_M-125_14TeV_2e2mu_withProbabilities.root",
				       "/afs/cern.ch/user/s/sbologne/workspace/multidimFit_prophecy/ZZMatrixElement/snowmass2013/test/samples/analyticalpsMELA/m2cut_2e2mu/XZ-Higgs0Mf025ph0ToZZTo4l_M-125_14TeV_withDiscriminants_2e2mu.root",
				       "/afs/cern.ch/user/s/sbologne/workspace/multidimFit_prophecy/ZZMatrixElement/snowmass2013/test/samples/JHUpsMELA_m2cut_2e2mu/Higgs0Mf01ph0Higgs0PHf01ph0ToZZTo4L_M-125_14TeV_withDiscriminants_2e2mu_withProbabilities.root"};
/*
TString inputFileNames[kNumSamples] = {"/afs/cern.ch/user/s/sbologne/workspace/multidimFit_prophecy/ZZMatrixElement/snowmass2013/test/samples/analyticalpsMELA/m2cut_2e2mu/SMHiggsToZZTo4L_M-125_14TeV_withDiscriminants_2e2mu.root",
				       "/afs/cern.ch/user/s/sbologne/workspace/multidimFit_prophecy/ZZMatrixElement/snowmass2013/test/samples/analyticalpsMELA/m2cut_2e2mu/Higgs0Mf01ph0ToZZTo4L_M-125_14TeV_withDiscriminants_2e2mu.root",
				       "/afs/cern.ch/user/s/sbologne/workspace/multidimFit_prophecy/ZZMatrixElement/snowmass2013/test/samples/analyticalpsMELA/m2cut_2e2mu/Higgs0Mf05ph0ToZZTo4L_M-125_14TeV_withDiscriminants_2e2mu.root",
				       "/afs/cern.ch/user/s/sbologne/workspace/multidimFit_prophecy/ZZMatrixElement/snowmass2013/test/samples/analyticalpsMELA/m2cut_2e2mu/Higgs0Mf05ph90ToZZTo4L_M-125_14TeV_withDiscriminants_2e2mu.root",
				       "/afs/cern.ch/user/s/sbologne/workspace/multidimFit_prophecy/ZZMatrixElement/snowmass2013/test/samples/analyticalpsMELA/m2cut_2e2mu/XZ-Higgs0Mf025ph0ToZZTo4l_M-125_14TeV_withDiscriminants_2e2mu.root",
				       "/afs/cern.ch/user/s/sbologne/workspace/multidimFit_prophecy/ZZMatrixElement/snowmass2013/test/samples/analyticalpsMELA/m2cut_2e2mu/Higgs0Mf01ph0Higgs0PHf01ph0ToZZTo4L_M-125_14TeV_withDiscriminants_2e2mu.root"};
*/

bool fa3Constant [kNumScenarios]   = {false,false,true, true, false,false};
bool fa2Constant [kNumScenarios]   = {true, true, false,false,false,false};
bool phia3Constant [kNumScenarios] = {true, false,true, true, true, false};
bool phia2Constant [kNumScenarios] = {true, true, true, false,true, false};


void embeddedToys(int nEvts=50, int nToys=10,
		  bool runPureToys=false,
		  sample mySample = kScalar_fa3p5, 
		  freeParams myParams = kfa3Only,
		  bool bkg=false, int iteration=0){
  RooRandom::randomGenerator()->SetSeed(iteration+1*129456) ;
 // initialize tree to save toys to 
  TTree* results = new TTree("results","toy results");
  
  double fa3,fa3Error;
  double fa2,fa2Error;
  double phia2, phia2Error;
  double phia3, phia3Error;

  results->Branch("fa3",&fa3,"fa3/D");
  results->Branch("fa3Error",&fa3Error,"fa3Error/D");
  results->Branch("fa2",&fa2,"fa2/D");
  results->Branch("fa2Error",&fa2Error,"fa2Error/D");

  results->Branch("phia3",&phia3,"phia3/D");
  results->Branch("phia3Error",&phia3Error,"phia3Error/D");
  results->Branch("phia2",&phia2,"phia2/D");
 
  results->Branch("phia2Error",&phia2Error,"phia2Error/D");
  //---------------------------------
  
  Playground myPG(125., true, 2, false, bkg,3.75);
  myPG.setEmbeddingCounter(iteration*nEvts*nToys);

  // load tree to draw toys from
  myPG.loadTree(inputFileNames[mySample],"SelectedTree");
  if(bkg)
    myPG.loadTree_bkg("/afs/cern.ch/user/s/sbologne/workspace/multidimFit_prophecy/ZZMatrixElement/snowmass2013/test/samples/analyticalpsMELA/m2cut_2e2mu/pwgevents_hadd_withDiscriminants_2e2mu.root","SelectedTree");

  // configure parameter for which ones
  // you would like to fit
  myPG.scalar->_modelParams.fa2->setConstant(fa2Constant[myParams]);
  myPG.scalar->_modelParams.fa3->setConstant(fa3Constant[myParams]);
  myPG.scalar->_modelParams.phia2->setConstant(phia2Constant[myParams]);
  myPG.scalar->_modelParams.phia3->setConstant(phia3Constant[myParams]);
  //-----------------------------------


  for(int i = 0 ; i<nToys ; i++){

    // set initial values
    myPG.scalar->_modelParams.fa2->setVal(0.0);
    myPG.scalar->_modelParams.fa3->setVal(0.0);
    myPG.scalar->_modelParams.phia2->setVal(0.0);
    myPG.scalar->_modelParams.phia3->setVal(0.0);
    if(mySample == kScalar_fa3p1)
      myPG.scalar->_modelParams.fa3->setVal(0.1);
    if(mySample == kScalar_fa3p25)
      myPG.scalar->_modelParams.fa3->setVal(0.25);
    if(mySample == kScalar_fa3p5)
      myPG.scalar->_modelParams.fa3->setVal(0.5);
    if(mySample == kScalar_fa3p5phia390){
      myPG.scalar->_modelParams.fa3->setVal(0.5);
      myPG.scalar->_modelParams.phia3->setVal(1.570796);
    }
    if(mySample == kScalar_fa2p1fa3p1){
      myPG.scalar->_modelParams.fa2->setVal(0.1);
      myPG.scalar->_modelParams.fa3->setVal(0.1);
    }
 
    cout << "=======================" << endl;
    cout << "=======================" << endl;
    cout << "=========" << i << "========" << endl;

    if(bkg){
      myPG.generate_sigbkg(nEvts,runPureToys);
      // perform fit
      myPG.fitData_sigbkg(true,-1);
    }
    else{
      myPG.generate(nEvts,runPureToys);
      // perform fit
      myPG.fitData(true,-1);
    }

    // get fit results and store to be 
    // saved to TTree
    fa2 = myPG.scalar->_modelParams.fa2->getVal();
    fa2Error = myPG.scalar->_modelParams.fa2->getError();
    fa3 = myPG.scalar->_modelParams.fa3->getVal();
    fa3Error = myPG.scalar->_modelParams.fa3->getError();

    phia2 = myPG.scalar->_modelParams.phia2->getVal();
    phia2Error = myPG.scalar->_modelParams.phia2->getError();
    phia3 = myPG.scalar->_modelParams.phia3->getVal();
    phia3Error = myPG.scalar->_modelParams.phia3->getError();

    // fill TTree
    results->Fill();

  }

  char nEvtsString[100];
  if(bkg)
    sprintf(nEvtsString,"_%iEvtsPlusBkg_%iter",nEvts,iteration);
  else
    sprintf(nEvtsString,"_%iEvts_%iter",nEvts,iteration);

  // write tree to output file (ouputFileName set at top)
  TString outputFileName;
  if(runPureToys)
    outputFileName="pureToys_"+sampleName[mySample]+"_"+scenario[myParams]+nEvtsString+".root";
  else
    outputFileName="embeddedToys_"+sampleName[mySample]+"_"+scenario[myParams]+nEvtsString+".root";

  TFile *outputFile = new TFile(outputFileName,"RECREATE");
  results->Write();
  outputFile->Close();

}
