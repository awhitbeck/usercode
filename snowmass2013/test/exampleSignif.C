#include "../src/statsFactory.cc"


void exampleSignif(){

  RooRealVar m4l("m4l","m4l",110.,140.);

  RooRealVar mean("mean","mean",125.,100.,150.);
  RooRealVar width("width","width",2.,0.,10.);

  mean.setConstant(kTRUE);
  width.setConstant(kTRUE);

  RooGaussian signal("signal","signal",m4l,mean,width);

  RooRealVar a("a","a",1.);
  RooRealVar b("b","b",0.1);  
  RooPolynomial bkg("bkg","bkg",m4l,RooArgList(a,b),1);

  a.setConstant(kTRUE);
  b.setConstant(kTRUE);

  RooArgSet observables(m4l);

  RooPlot* plot = m4l.frame();
  signal.plotOn(plot);
  bkg.plotOn(plot);
  plot->Draw();

  // pass reference to rooargset of observables, 
  // reference to signal and background pdf, 
  // random seed, and output file to save fit results
  statsFactory myStats(&observables,&signal,&bkg,12345,"test.root");

  // pass expected  nsig, nbkg and number of 
  // pure toys to run...
  myStats.runSignificance(20,50,100);
  

}
