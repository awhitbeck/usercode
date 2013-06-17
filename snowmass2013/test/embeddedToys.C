#include "../src/Playground.cc"

using namespace PlaygroundHelpers;

bool runPureToys=false;
TString outputFileName = "embeddedToys_test.root";

void embeddedToys(int nEvts=50, int nToys=100){
  
  Playground myPG(126.,true);

  // load tree to draw toys from
  myPG.loadTree("/scratch0/hep/cyou/AllSamples/JHUGen_v3.1.7/0+/rootfiles/SMHiggsToZZTo4L_M-125_14TeV_2e2mu.root","SelectedTree");

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


  // configure parameter for which ones
  // you would like to fit
  myPG.scalar->fa2->setConstant(kTRUE);
  myPG.scalar->fa3->setConstant(kFALSE);
  myPG.scalar->phia2->setConstant(kTRUE);
  myPG.scalar->phia3->setConstant(kFALSE);
  //-----------------------------------


  for(int i = 0 ; i<nToys ; i++){

    // set initial values
    myPG.scalar->fa2->setVal(0.0);
    myPG.scalar->fa3->setVal(0.0);
    myPG.scalar->phia2->setVal(0.0);
    myPG.scalar->phia3->setVal(0.0);

    cout << "=======================" << endl;
    cout << "=======================" << endl;
    cout << "=========" << i << "========" << endl;

    if(myPG.generate(nEvts,runPureToys)==kNoError){

      // perform fit
      myPG.fitData(true);

      // get fit results and store to be 
      // saved to TTree
      fa2 = myPG.scalar->fa2->getVal();
      fa2Error = myPG.scalar->fa2->getError();
      fa3 = myPG.scalar->fa3->getVal();
      fa3Error = myPG.scalar->fa3->getError();

      phia2 = myPG.scalar->phia2->getVal();
      phia2Error = myPG.scalar->phia2->getError();
      phia3 = myPG.scalar->phia3->getVal();
      phia3Error = myPG.scalar->phia3->getError();

      // fill TTree
      results->Fill();

    }

  }

  // write tree to output file (ouputFileName set at top)
  TFile *outputFile = new TFile(outputFileName,"RECREATE");
  results->Write();
  outputFile->Close();

}
