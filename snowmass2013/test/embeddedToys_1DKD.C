using namespace RooFit;

enum sample {kScalar_fa3p0,kScalar_fa3p1,kScalar_fa3p5,kNumSamples};
TString sampleName[kNumSamples] = {"fa3p0","fa3p1","fa3p5"};
TString inputFileNames[kNumSamples] = {"/tmp/sbologne/14TeV/SMHiggsToZZTo4L_M-125_14TeV_2e2mu_withProbabilities.root",
				       "/tmp/sbologne/14TeV/Higgs0Mf01ph0ToZZTo4L_M-125_14TeV_2e2mu_withProbabilities.root",
				       "/tmp/sbologne/14TeV/Higgs0Mf05ph0ToZZTo4L_M-125_14TeV_2e2mu_withProbabilities.root"};

void embeddedToys_1DKD(int nEvts=50, int nToys=100,
		  sample mySample = kScalar_fa3p0){
  

  RooRealVar* kd = new RooRealVar("psMELA","psMELA",0,1);
  kd->setBins(50);
  RooPlot* kdframe1 = kd->frame();
  
  // 0- template
  TFile f1("KDdistribution_ps.root", "READ"); 
  TH2F *h_KD_ps = (TH2F*)f1.Get("h_KD");
  h_KD_ps->SetName("h_KD_ps");
  RooDataHist rdh_KD_ps("rdh_KD_ps","rdh_KD_ps",RooArgList(*kd),h_KD_ps);
  RooHistPdf pdf_KD_ps("pdf_KD_ps","pdf_KD_ps",RooArgList(*kd),rdh_KD_ps); 

  // 0+ template
  TFile f2("KDdistribution_sm.root", "READ"); 
  TH2F *h_KD_sm = (TH2F*)f2.Get("h_KD");
  h_KD_sm->SetName("h_KD_sm");
  RooDataHist rdh_KD_sm("rdh_KD_sm","rdh_KD_sm",RooArgList(*kd),h_KD_sm);
  RooHistPdf pdf_KD_sm("pdf_KD_sm","pdf_KD_sm",RooArgList(*kd),rdh_KD_sm); 

  RooRealVar rrv_fa3("fa3","fa3",0.5,0.,1.);  //free parameter of the model
  RooAddPdf model("model","ps+sm",pdf_KD_ps,pdf_KD_sm,rrv_fa3);  
  rrv_fa3.setConstant(kFALSE);
  double fa3Val=-99;
  if (mySample == kScalar_fa3p0)
    fa3Val=0.;
  else if (mySample == kScalar_fa3p1)
    fa3Val=0.1;
  else if (mySample == kScalar_fa3p5)
    fa3Val=0.5;
  else{
    cout<<"fa3Val not correct!"<<endl;
      return 0;
  }

  TCanvas* c = new TCanvas("modelPlot","modelPlot",400,400);
  rdh_KD_ps.plotOn(kdframe1,LineColor(kBlack));
  pdf_KD_ps.plotOn(kdframe1,LineColor(kBlack));
  rdh_KD_sm.plotOn(kdframe1,LineColor(kBlue));
  pdf_KD_sm.plotOn(kdframe1,LineColor(kBlue));
  model.plotOn(kdframe1,LineColor(kRed));
  kdframe1->Draw();

  
  TChain* myChain = new TChain("SelectedTree");
  myChain->Add(inputFileNames[mySample]);
  
  if(!myChain || myChain->GetEntries()<=0) {
    cout<<"error in the tree"<<endl;
    return 0;
  }
  
   RooDataSet* data = new RooDataSet("data","data",myChain,RooArgSet(*kd),"");
  //MISSING THE CUT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    cout << "Number of events in data: " << data->numEntries() << endl;
  
  // initialize tree to save toys to 
  TTree* results = new TTree("results","toy results");
  
  double fa3,fa3Error, fa3Pull;
  double fa2,fa2Error, fa2Pull;
  double phia2, phia2Error, phia2Pull;
  double phia3, phia3Error, phia3Pull;

  results->Branch("fa3",&fa3,"fa3/D");
  results->Branch("fa3Error",&fa3Error,"fa3Error/D");
  results->Branch("fa3Pull",&fa3Pull,"fa3Pull/D");

  //---------------------------------

  RooDataSet* toyData;
  int embedTracker;
  RooArgSet *tempEvent;

  RooFitResult *toyfitresults;
  RooRealVar *r_fa3;

  for(int i = 0 ; i<nToys ; i++){

    //if(toyData) delete toyData;
    toyData = new RooDataSet("toyData","toyData",RooArgSet(*kd));

    if(nEvts+embedTracker > data->sumEntries()){
      cout << "Playground::generate() - ERROR!!! Playground::data does not have enough events to fill toy!!!!  bye :) " << endl;
      toyData = NULL;
      return 0;
    }

    for(int iEvent=0; iEvent<nEvts; iEvent++){
      cout << "generating event: " << iEvent << " embedTracker: " << embedTracker << endl;
      tempEvent = (RooArgSet*) data->get(embedTracker);
      toyData->add(*tempEvent);
      embedTracker++;
    }

    toyfitresults =model.fitTo(*toyData,Save());
    cout<<toyfitresults<<endl;
    r_fa3 = (RooRealVar *) toyfitresults->floatParsFinal().find("fa3");

    fa3 = r_fa3->getVal();
    fa3Error = r_fa3->getError();
    fa3Pull = (r_fa3->getVal() - fa3Val) / r_fa3->getError();

    // fill TTree
    results->Fill();

  }

  char nEvtsString[100];
  sprintf(nEvtsString,"_%iEvts",nEvts);

  // write tree to output file (ouputFileName set at top)
  TFile *outputFile = new TFile("embeddedToys1DKD_"+sampleName[mySample]+nEvtsString+".root","RECREATE");
  results->Write();
  outputFile->Close();

}
