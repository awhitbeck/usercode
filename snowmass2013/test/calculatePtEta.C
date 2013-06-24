



void calculatePtEta(){

  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();
  //gStyle->SetOptStat(1);

  TChain* t = new TChain("SelectedTree");
  t->Add("/export/d1/scratch/snowmass2013/ilcsamples/signal/spin0/Events_20130618/unweighted_unpol_g1_1M_false.root");

  float costheta1,costheta2,phi;
  float l1pt,l2pt;
  float l1eta,l2eta;
  float ptlplus,ptlminus;
  float etalplus,etalminus;
  float ZMass,HMass;

  float s = 250.;
  float mH = 125.;
  float mZ = 91.18;

  t->SetBranchAddress("costheta1",&costheta1);
  t->SetBranchAddress("costheta2",&costheta2);
  t->SetBranchAddress("phi",&phi);
  t->SetBranchAddress("phi",&phi);
  t->SetBranchAddress("ptlplus",&ptlplus);
  t->SetBranchAddress("ptlminus",&ptlminus);
  t->SetBranchAddress("etalplus",&etalplus);
  t->SetBranchAddress("etalminus",&etalminus);
  //t->SetBranchAddress("ZMass",&mZ);
  //t->SetBranchAddress("HMass",&mH);
 
  //TH1F* difflep1 = new TH1F("difflep1",";#Delta p_{T}/p_{T};",100,-1,1);
  TH1F* difflep1 = new TH1F("difflep1",";#Delta #eta/#eta;",100,-1,1);
  //TH1F* difflep2 = new TH1F("difflep2",";#Delta p_{T}/p_{T};",100,-1,1);
  TH1F* difflep2 = new TH1F("difflep2",";#Delta #eta/#eta;",100,-1,1);
  difflep1->SetLineWidth(2);
  difflep1->SetLineColor(2);

  for(int i = 0 ; i<t->GetEntries(); i++){
   
    t->GetEntry(i);
 
    float pZsquare = (s*s+mZ*mZ-mH*mH)*(s*s+mZ*mZ-mH*mH)/(2*s)/(2*s) - mZ*mZ;
    float pZ = sqrt(pZsquare);
    
    float plep = mZ/2.;

    float gammaZ = sqrt(mZ*mZ+pZ*pZ)/mZ;
    float betaZ  = sqrt(1-1/(gammaZ*gammaZ));

    float gammaH = sqrt(mH*mH+pZ*pZ)/mH;
    float betaH  = sqrt(1-1/(gammaH*gammaH));

    TLorentzVector Z4vec(gammaZ*mZ*betaZ*sqrt(1-costheta1*costheta1),
			 0.0,
			 gammaZ*mZ*betaZ*costheta1,
			 gammaZ*mZ);

    TLorentzVector lepM4vec(.5*mZ*cos(phi)*sqrt(1-costheta2*costheta2),
			    .5*mZ*sin(phi)*sqrt(1-costheta2*costheta2),
			    .5*mZ*costheta2,
			    .5*mZ);

    TLorentzVector lepP4vec(-.5*mZ*cos(phi)*sqrt(1-costheta2*costheta2),
			    -.5*mZ*sin(phi)*sqrt(1-costheta2*costheta2),
			    -.5*mZ*costheta2,
			    .5*mZ);
    
    TLorentzRotation ZToZ;

    ZToZ.Boost(0,0,betaZ);
    ZToZ.RotateY(-TMath::Pi()/2.+costheta1);
	       
    lepP4vec = ZToZ*lepP4vec;
    lepM4vec = ZToZ*lepM4vec;
    /*
    cout << " = = = = = = = = = = = = = = = = = " << endl;
    cout << "costheta1: " << costheta1 << endl;
    cout << "costheta2: " << costheta2 << endl;
    cout << "phi: " << phi << endl;
    cout << "pZ2: " << pZsquare << endl;
    cout << "pZ: " << pZ << endl;
    cout << "plep: " << plep << endl;

    cout << "------------------------" << endl;
    cout << "zp: " << Z4vec.P() << endl;
    cout << "zpx: " << Z4vec.Px() << endl;
    cout << "zpy: " << Z4vec.Py() << endl;
    cout << "zpz: " << Z4vec.Pz() << endl;
    cout << "zE: "  << Z4vec.E()  << endl;

    cout << "------------------------" << endl;
    cout << "llpx: " << lepM4vec.Px()+lepP4vec.Px() << endl;
    cout << "llpy: " << lepM4vec.Py()+lepP4vec.Py() << endl;
    cout << "llpz: " << lepM4vec.Pz()+lepP4vec.Pz() << endl;
    cout << "llE: " << lepM4vec.E()+lepP4vec.E() << endl;
    cout << "------------------------" << endl;
    cout << "l1px: " << lepM4vec.Px() << endl;
    cout << "l1py: " << lepM4vec.Py() << endl;
    cout << "l1pz: " << lepM4vec.Pz() << endl;
    cout << "l1E: " << lepM4vec.E() << endl;
    cout << "------------------------" << endl;
    cout << "l2px: " << lepP4vec.Px() << endl;
    cout << "l2py: " << lepP4vec.Py() << endl;
    cout << "l2pz: " << lepP4vec.Pz() << endl;
    cout << "l2E: " <<  lepP4vec.E() << endl;
    cout << "------------------------" << endl;
    

    cout << "ptlplus: "   << ptlplus   << endl;
    cout << "ptlminus: "  << ptlminus  << endl;
    cout << "etalplus: "  << etalplus  << endl;
    cout << "etalminus: " << etalminus << endl;
    
    cout << "pt: "  << lepP4vec.Pt()  << endl;
    cout << "eta: " << lepP4vec.Eta() << endl;
    cout << "pt: "  << lepM4vec.Pt()  << endl;
    cout << "eta: " << lepM4vec.Eta() << endl;
    */
    //difflep1->Fill((lepM4vec.Pt()-ptlminus)/ptlminus);
    difflep1->Fill((lepM4vec.Eta()-etalminus)/etalminus);
    //difflep2->Fill((lepP4vec.Pt()-ptlplus)/ptlminus);
    difflep2->Fill((lepP4vec.Eta()-etalplus)/etalminus);
  }

  difflep1->Draw();
  difflep2->Draw("SAME");

  TLegend* leg = new TLegend(.3,.7,.5,.9);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  
  leg->AddEntry(difflep1,"l^{-}","l");
  leg->AddEntry(difflep2,"l^{+}","l");

  leg->Draw();
}
