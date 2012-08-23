void convert_FP_tree(){

  // TFile *dfile = new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_TRACKERALIGN/HIP/bonato/HtoZZto2L2J-crab/trees_FP_Summer11/HZZlljjRM_DATA_800pb_optLD_looseBTags_v2_ALL_FP.root");
  // TFile *dfile = new TFile("/afs/cern.ch/user/w/whitbeck/scratch0/HZZlljjRM_DATA_1fb_optLD_looseBTags_v2_ALL.root");//859
  //TFile *dfile = new TFile("/home/whitbeck/2l2jHelicity/dataFiles/4fbData/summer11_data_lowmass.root");//3800 pb data
  TFile *dfile = new TFile("/home/whitbeck/2l2jHelicity/dataFiles/4fbData/summer11_data_4600pb_lowmass.root");//4600 pb data
  TTree *t=(TTree*)dfile->Get("AngularInfo");
  cout<<"Loaded tree "<<t->GetName()<<endl;
  double leptTypeIN, nBTagsIN, metIN;
  double mZZIN, mZjjIN,eventWeightIN;
  cout<<"addressing"<<endl;
  t->SetBranchAddress("mZZ", &mZZIN);
  t->SetBranchAddress("mJJ", &mZjjIN);
  t->SetBranchAddress("nBTags", &nBTagsIN);
  t->SetBranchAddress("lep", &leptTypeIN);
  t->SetBranchAddress("weight", &eventWeightIN);
  t->SetBranchAddress("met", &metIN);
  
  
  double leptTypeOUT, nBTagsOUT, metOUT;
  double mZZOUT, mZjjOUT,eventWeightOUT;
  TFile *fout=new TFile("./convertedTree.root","RECREATE");
  TTree *tout=new TTree("AngularInfo","Converted from SB");
  tout->Branch("CMS_hzz2l2q_mZZ",&mZZOUT,"CMS_hzz2l2q_ZZ/D");
  tout->Branch("mZjj",&mZjjOUT,"mZjj/D");
  tout->Branch("leptType",&leptTypeOUT,"leptType/D");
  tout->Branch("nBTags",&nBTagsOUT,"nBTags/D");
  tout->Branch("eventWeight",&eventWeightOUT,"eventWeight/D");
  tout->Branch("met",&metOUT,"met/D");
  
  cout<<"start"<<endl;
  for(int i=0;i<t->GetEntries();i++){
    t->GetEntry(i);
    leptTypeOUT=double(leptTypeIN);
    nBTagsOUT=double(nBTagsIN);
    mZZOUT=double(mZZIN);
    mZjjOUT=double(mZjjIN);
    eventWeightOUT=double(eventWeightIN);
    metOUT=double(metIN);
    tout->Fill();
  }
  cout<<"finished"<<endl;
  tout->Write();
  delete tout;
  delete fout;
  
}
