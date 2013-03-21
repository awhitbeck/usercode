
using namespace RooFit;

void testA(){

  RooRealVar *mH[10];
  RooRealVar *m4l = new RooRealVar("m4l","m4l",105,140);
  RooPlot *plot = m4l->frame();

  m4lSignalBase *ggH[10];
  signalYields *ggHNorm[10];

  m4lqqZZBase *qqZZ = new m4lqqZZBase("4mu","8TeV","qqZZ",m4l);
  qqZZ->initializePDFs("SM_inputs_8TeV/inputs_4mu.txt");
  qqZZ->m4lModel->plotOn(plot,FillColor(kBlue+2));

  for(int i=0; i<10; i++){

    TString temp="mH_";
    temp+=i;

    mH[i] = new RooRealVar(temp,temp,115.0+i*2.0,100,1000);
    ggH[i] = new m4lSignalBase("4mu","8TeV","ggH",mH[i],m4l);
    ggHNorm[i] = new signalYields("4mu","8TeV","ggH",mH[i]);

    ggH[i]->initializePDFs("SM_inputs_8TeV/inputs_4mu.txt");
    ggHNorm[i]->initialize("SM_inputs_8TeV/inputs_4mu.txt",true);
    cout << mH[i]->getVal() << endl;
    cout << ggHNorm[i]->xsBr << endl;
    cout << ggHNorm[i]->xsBr->getVal() << endl;
    cout << ggHNorm[i]->signalRate << endl;
    cout << ggHNorm[i]->signalRate->getVal() << endl;
    ggH[i]->m4lModel->plotOn(plot,LineColor(i+1),Normalization(ggHNorm[i].signalRate->getVal()));
 
  }

  TCanvas* can = new TCanvas("can","can",500,500);
  
  plot->Draw();
  
  can->SaveAs("signal_m4lShape.png");

}
