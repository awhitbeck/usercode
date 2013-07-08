using namespace RooFit;

void prepareHistoForModelPdf(TString inputFileName, TString treeName, TString outputFileName, TString varName){
 TChain* sigTree = new TChain(treeName);
  sigTree->Add(inputFileName);
  
  if(!sigTree){
    cout<<"ERROR could not find the tree!"<<endl;
    return;
  }
  float psMELA;
  sigTree->SetBranchAddress(varName,&psMELA);

  TFile* newFile = new TFile(outputFileName,"RECREATE");
  TH1F *h_KD = new TH1F("h_KD", "h_KD", 1000, 0, 1);

  for(int iEvt=0; iEvt<sigTree->GetEntries(); iEvt++){
   sigTree->GetEntry(iEvt);
   
   h_KD->Fill(psMELA);
   }
 
  newFile->cd();
  h_KD->Write();
  newFile->Close();
}

