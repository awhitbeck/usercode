


void embeddedToys(int nEvts=50, int nToys=100){
  
  Playground myPG(126.,true);

  TH1F* fa3 = new TH1F("hist",";f_{a3};Number of Toys",30,0,1);
  TH1F* fa2 = new TH1F("hist",";f_{a2};Number of Toys",30,0,1);

  myPG.loadTree("../../../../datafiles/8TeV_H126_SMHiggs_2e2mu.root","angles");

  for(int i = 0 ; i<nToys ; i++){

    // set initial values
    myPG.scalar->fa2->setVal(0.0);
    myPG.scalar->fa3->setVal(0.0);
    myPG.scalar->phia2->setVal(0.0);
    myPG.scalar->phia3->setVal(0.0);

    cout << "=======================" << endl;
    cout << "=======================" << endl;
    cout << "=========" << i << "========" << endl;

    if(myPG.generate(nEvts,false)==0){
      myPG.fitData(true);

      fa2->Fill(myPG.scalar->fa2->getVal());
      fa3->Fill(myPG.scalar->fa3->getVal());
    }

  }

  TCanvas* can = new TCanvas("can","can",600,300);
  can->Divide(2,1);
  can->cd(1);
  fa2->Draw();
  can->cd(2);
  fa3->Draw();

  //myPG.projectPDF(PlaygroundHelpers::kcosthetastar,20,false);

}
