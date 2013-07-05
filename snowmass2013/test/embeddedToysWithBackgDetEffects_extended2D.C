using namespace RooFit;

enum sample {kScalar_fa3p0,kScalar_fa3p1,kScalar_fa3p5,kScalar_fa3p5phia390,kScalar_fa3p25,kNumSamples};
TString sampleName[kNumSamples] = {"fa3p0","fa3p1","fa3p5","fa3p5phia390","fa3p25"};
TString inputFileNames[kNumSamples] = {"samples/analyticalpsMELA/withResolution/SMHiggsToZZTo4L_M-125_14TeV_wResolution_withDiscriminants_cutDetector.root",
				       "samples/analyticalpsMELA/withResolution/Higgs0Mf01ph0ToZZTo4L_M-125_14TeV_wResolution_withDiscriminants_cutDetector.root",
				       "samples/analyticalpsMELA/withResolution/Higgs0Mf05ph0ToZZTo4L_M-125_14TeV_wResolution_withDiscriminants_cutDetector.root",
				       "",
				       "samples/analyticalpsMELA/withResolution/XZ-Higgs0Mf025ph0ToZZTo4l_M-125_14TeV_wResolution_withDiscriminants_cutDetector.root"
				       };

void embeddedToysWithBackgDetEffects_extended2D(int nEvts=600, int nToys=100,
						sample mySample = kScalar_fa3p5, 
						int nbackgroundEvts=160, int counter=0){
  

  RooRealVar* kd = new RooRealVar("psMELA","psMELA",0,1);
  RooRealVar* mzz = new RooRealVar("ZZMass","ZZMass",106,141);
  kd->setBins(100);
  mzz->setBins(100);
  RooPlot* kdframe = kd->frame();
  RooPlot* mzzframe = mzz->frame();
  
  // 0- template
  TFile f1("KDMZZdistribution_ps_analytical_detEff.root", "READ"); 
  TH2F *h_KD_ps = (TH2F*)f1.Get("h_KD");
  h_KD_ps->SetName("h_KD_ps");
  RooDataHist rdh_KD_ps("rdh_KD_ps","rdh_KD_ps",RooArgList(*mzz,*kd),h_KD_ps);
  RooHistPdf pdf_KD_ps("pdf_KD_ps","pdf_KD_ps",RooArgList(*mzz,*kd),rdh_KD_ps); 

  // 0+ template
  TFile f2("KDMZZdistribution_sm_analytical_detEff.root", "READ"); 
  TH2F *h_KD_sm = (TH2F*)f2.Get("h_KD");
  h_KD_sm->SetName("h_KD_sm");
  RooDataHist rdh_KD_sm("rdh_KD_sm","rdh_KD_sm",RooArgList(*mzz,*kd),h_KD_sm);
  RooHistPdf pdf_KD_sm("pdf_KD_sm","pdf_KD_sm",RooArgList(*mzz,*kd),rdh_KD_sm); 

  // backg template
  TFile f3("KDMZZdistribution_bkg_analytical_detEff.root", "READ"); 
  TH2F *h_KD_bkg = (TH2F*)f3.Get("h_KD");
  h_KD_bkg->SetName("h_KD_bkg");
  RooDataHist rdh_KD_bkg("rdh_KD_bkg","rdh_KD_bkg",RooArgList(*mzz,*kd),h_KD_bkg);
  RooHistPdf pdf_KD_bkg("pdf_KD_bkg","pdf_KD_bkg",RooArgList(*mzz,*kd),rdh_KD_bkg); 

  //Define signal model with 0+, 0- mixture
  RooRealVar rrv_fa3("fa3","fa3",0.5,0.,1.);  //free parameter of the model
  RooFormulaVar rfv_fa3Obs("fa3obs","1/ (1 + (1/@0 - 1)*0.99433)",RooArgList(rrv_fa3));
  RooAddPdf modelSignal("modelSignal","ps+sm",pdf_KD_ps,pdf_KD_sm,rfv_fa3Obs);  
  rrv_fa3.setConstant(kFALSE);
  
 //PDF signal+bakground with nevts for realistic calibration of significance
  RooRealVar nsig("nsig","nsig",nEvts,0,10000);
  RooRealVar nbkg("nbkg","nbkg",nbackgroundEvts,0,20000);
  RooAddPdf model("model","background+modelSignal",RooArgList(modelSignal,pdf_KD_bkg),RooArgList(nsig,nbkg));

  //Set the values of free parameters to compute pulls
  double fa3Val=-99;
  if (mySample == kScalar_fa3p0)
    fa3Val=0.;
  else if (mySample == kScalar_fa3p1)
    fa3Val=0.1;
  else if (mySample == kScalar_fa3p5 || mySample == kScalar_fa3p5phia390)
    fa3Val=0.5;
  else if (mySample == kScalar_fa3p25)
    fa3Val=0.25;
  else{
    cout<<"fa3Val not correct!"<<endl;
      return 0;
  }

  //Plot the models
  /*TCanvas* c = new TCanvas("model2DPlot_detBkg","model2DPlot_detBkg",400,400);
  c->Divide(2);
  c->cd(1);
  rdh_KD_ps.plotOn(kdframe,LineColor(kBlack),MarkerColor(kBlack));
  pdf_KD_ps.plotOn(kdframe,LineColor(kBlack),RooFit::Name("pseudo"));
  //rdh_KD_sm.plotOn(kdframe,LineColor(kBlue),MarkColor(kBlue));
  pdf_KD_sm.plotOn(kdframe,LineColor(kBlue),RooFit::Name("SM"));
  //rdh_KD_bkg.plotOn(kdframe,LineColor(kGreen),LineColor(kGreen));
  pdf_KD_bkg.plotOn(kdframe,LineColor(kGreen),RooFit::Name("bkg"));
  modelSignal.plotOn(kdframe,LineColor(kRed),RooFit::Name("signal_fa3p5"));
  model.plotOn(kdframe,LineColor(kOrange),RooFit::Name("signal+background"));
  c->cd(1);
  rdh_KD_ps.plotOn(mzzframe,LineColor(kBlack),MarkerColor(kBlack));
  pdf_KD_ps.plotOn(mzzframe,LineColor(kBlack),RooFit::Name("pseudo"));
  //rdh_KD_sm.plotOn(mzzframe,LineColor(kBlue),MarkColor(kBlue));
  pdf_KD_sm.plotOn(mzzframe,LineColor(kBlue),RooFit::Name("SM"));
  //rdh_KD_bkg.plotOn(mzzframe,LineColor(kGreen),LineColor(kGreen));
  pdf_KD_bkg.plotOn(mzzframe,LineColor(kGreen),RooFit::Name("bkg"));
  modelSignal.plotOn(mzzframe,LineColor(kRed),RooFit::Name("signal_fa3p5"));
  model.plotOn(mzzframe,LineColor(kOrange),RooFit::Name("signal+background"));
  TLegend *leg = new TLegend (0.7,0.6,0.95,0.8);
  leg->AddEntry(kdframe->findObject("pseudo"),"0-","L");
  leg->AddEntry(kdframe->findObject("SM"),"0+","L");
  leg->AddEntry(kdframe->findObject("bkg"),"bkg","L");
  leg->AddEntry(kdframe->findObject("signal_fa3p5"),"signal fa3=0.5","L");
  leg->AddEntry(kdframe->findObject("signal+background"),"signal + bkg","L");
  kdframe->Draw();
  leg->SetFillColor(kWhite);
  leg->Draw("same");
  c->SaveAs("model2DPlot_detBkg.eps");
  c->SaveAs("model2DPlot_detBkg.png");

  */
  //Load the trees into the datasets
  TChain* myChain = new TChain("SelectedTree");
  myChain->Add(inputFileNames[mySample]);
  if(!myChain || myChain->GetEntries()<=0) {
    cout<<"error in the tree"<<endl;
    return 0;
  }
  RooDataSet* data = new RooDataSet("data","data",myChain,RooArgSet(*mzz,*kd),"");

  TChain* myChain_bkg = new TChain("SelectedTree");
  myChain_bkg->Add("samples/analyticalpsMELA/withResolution/pwgevents_mllCut10_smeared_withDiscriminants_2e2mu_cutDetector.root");
  myChain_bkg->Add("samples/analyticalpsMELA/withResolution/pwgevents_mllCut4_wResolution_withDiscriminants_cutDetector.root");
  if(!myChain_bkg || myChain_bkg->GetEntries()<=0) {
    cout<<"error in the tree"<<endl;
    return 0;
  }
  RooDataSet* data_bkg = new RooDataSet("data_bkg","data_bkg",myChain_bkg,RooArgSet(*mzz,*kd),"");

  cout << "Number of events in data sig: " << data->numEntries() << endl;
  cout << "Number of events in data bkg: " << data_bkg->numEntries() << endl;
  
  // Initialize tree to save toys to 
  TTree* results = new TTree("results","toy results");
  
  double fa3,fa3Error, fa3Pull;
  double sigFrac,sigFracError, sigFracPull;
  double significance, nsignal, nbackground, nbackground0;

  results->Branch("fa3",&fa3,"fa3/D");
  results->Branch("fa3Error",&fa3Error,"fa3Error/D");
  results->Branch("fa3Pull",&fa3Pull,"fa3Pull/D");
  results->Branch("significance",&significance,"significance/D");
  results->Branch("nsignal",&nsignal,"nsignal/D");
  results->Branch("nbackground",&nbackground,"nbackground/D");
  results->Branch("nbackground0",&nbackground0,"nbackground0/D");

  //---------------------------------

  RooDataSet* toyData;
  int embedTracker=nEvts*counter;
  int embedTracker_bkg=nbackgroundEvts*counter;
  RooArgSet *tempEvent;

  RooFitResult *toyfitresults;
  RooFitResult *toyfitresults_sigBkg;
  RooFitResult *toyfitresults_bkgOnly;
  RooRealVar *r_fa3;

  for(int i = 0 ; i<nToys ; i++){
    cout <<i<<"<-----------------------------"<<endl;
    //if(toyData) delete toyData;
    toyData = new RooDataSet("toyData","toyData",RooArgSet(*mzz,*kd));
    toyData_bkgOnly = new RooDataSet("toyData_bkgOnly","toyData_bkgOnly",RooArgSet(*mzz,*kd));

    if(nEvts+embedTracker > data->sumEntries()){
      cout << "Playground::generate() - ERROR!!! Playground::data does not have enough events to fill toy!!!!  bye :) " << endl;
      toyData = NULL;
      abort();
      return 0;
    }
    if(nbackgroundEvts+embedTracker_bkg > data_bkg->sumEntries()){
      cout << "Playground::generate() - ERROR!!! Playground::data does not have enough events to fill toy!!!!  bye :) " << endl;
      toyData = NULL;
      abort();
      return 0;
    }

    for(int iEvent=0; iEvent<nEvts; iEvent++){
      if(iEvent==1)
	cout << "generating event: " << iEvent << " embedTracker: " << embedTracker << endl;
      tempEvent = (RooArgSet*) data->get(embedTracker);
      toyData->add(*tempEvent);
      embedTracker++;
    }
    for(int iEvent=0; iEvent<nbackgroundEvts; iEvent++){
      if(iEvent==1)
	cout << "generating bkg event: " << iEvent << " embedTracker bkg: " << embedTracker_bkg << endl;
      tempEvent = (RooArgSet*) data_bkg->get(embedTracker_bkg);
      toyData->add(*tempEvent);
      embedTracker_bkg++;
    }

    //toyfitresults =model.fitTo(*toyData,Save(),Extended(kTRUE));

    //Calibration of singificance
    nsig.setVal(nEvts);
    nbkg.setVal(nbackgroundEvts);
    nsig.setConstant(kFALSE); nbkg.setConstant(kFALSE);
    toyfitresults_sigBkg =model.fitTo(*toyData,Save(),Extended(kTRUE));
    nsignal=nsig.getVal();
    nbackground=nbkg.getVal();

    r_fa3 = (RooRealVar *) toyfitresults_sigBkg->floatParsFinal().find("fa3");

    fa3 = r_fa3->getVal();
    fa3Error = r_fa3->getError();
    fa3Pull = (r_fa3->getVal() - fa3Val) / r_fa3->getError();


    nsig.setVal(0.00000001); nsig.setConstant(kTRUE); nbkg.setConstant(kFALSE);
    toyfitresults_bkgOnly =model.fitTo(*toyData,Save(),Extended(kTRUE));
    nbackground0=nbkg.getVal();

    significance = sqrt(2*fabs(toyfitresults_sigBkg->minNll() - toyfitresults_bkgOnly->minNll()));

    // fill TTree
    results->Fill();
  }

  char nEvtsString[100];
  sprintf(nEvtsString,"_%iEvts_%iEvtsBkg_%iiter",nEvts,nbackgroundEvts,counter);

  // write tree to output file (ouputFileName set at top)
  TFile *outputFile = new TFile("embeddedToys2DExt_fa3Corr_WithBackgDetEffects_"+sampleName[mySample]+nEvtsString+".root","RECREATE");
  results->Write();
  outputFile->Close();

}
