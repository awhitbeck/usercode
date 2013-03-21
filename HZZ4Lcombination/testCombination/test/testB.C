
using namespace RooFit;

void testB(){

  RooRealVar *mH = new RooRealVar("mH","mH",125);
  RooRealVar *m4l = new RooRealVar("m4l","m4l",105,140);
  RooPlot *plot = m4l->frame();

  m4lSignalBase *ggH[6];

  m4lqqZZBase *qqZZ[6];
  m4lggZZBase *ggZZ[6];
  m4lZXBase *ZX[6];

  TString chan[3]={"4mu","4e","2e2mu"};
  char temp[250];

  for(int i=2; i<3; i++){

    cout << i << endl;

    ggH[i] = new m4lSignalBase(chan[i],"8TeV","ggH",mH,m4l);
    qqZZ[i] = new m4lqqZZBase(chan[i],"8TeV","qqZZ",m4l);
    ggZZ[i] = new m4lggZZBase(chan[i],"8TeV","ggZZ",m4l);
    ZX[i] = new m4lZXBase(chan[i],"8TeV","ZX",m4l);

    sprintf(temp,"SM_inputs_8TeV/inputs_%s.txt",chan[i].Data());
    cout << temp << endl;

    ggH[i]->initializePDFs(temp);
    qqZZ[i]->initializePDFs(temp);
    ggZZ[i]->initializePDFs(temp);
    ZX[i]->initializePDFs(temp);

    ggH[i]->m4lModel->plotOn(plot,LineColor(i+1));
    qqZZ[i]->m4lModel->plotOn(plot,LineColor(i+1),LineStyle(2));
    ggZZ[i]->m4lModel->plotOn(plot,LineColor(i+1),LineStyle(4));
    ZX[i]->m4lModel->plotOn(plot,LineColor(i+5),LineStyle(3));

  }

  for(int i=0; i<3; i++){

    cout << i << endl;

    ggH[i] = new m4lSignalBase(chan[i],"7TeV","ggH",mH,m4l);
    qqZZ[i] = new m4lqqZZBase(chan[i],"7TeV","qqZZ",m4l);
    ggZZ[i] = new m4lggZZBase(chan[i],"7TeV","ggZZ",m4l);
    ZX[i] = new m4lZXBase(chan[i],"7TeV","ZX",m4l);

    sprintf(temp,"SM_inputs_8TeV/inputs_%s.txt",chan[i].Data());
    cout << temp << endl;

    ggH[i]->initializePDFs(temp);
    qqZZ[i]->initializePDFs(temp);
    ggZZ[i]->initializePDFs(temp);
    ZX[i]->initializePDFs(temp);

    ggH[i]->m4lModel->plotOn(plot,LineColor(i+4));
    qqZZ[i]->m4lModel->plotOn(plot,LineColor(i+4),LineStyle(2));
    ggZZ[i]->m4lModel->plotOn(plot,LineColor(i+4),LineStyle(4));
    ZX[i]->m4lModel->plotOn(plot,LineColor(i+8),LineStyle(3));

  }

  TCanvas* can = new TCanvas("can","can",500,500);
  
  plot->Draw();
  
  can->SaveAs("compareChannels_m4lShape.png");

}
