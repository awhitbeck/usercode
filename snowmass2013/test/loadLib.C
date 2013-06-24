{

  //gSystem->AddIncludePath("-I$CMSSW_BASE/src/");
  //gSystem->AddIncludePath("-I$ROOFITSYS/include/");
 
  // tools for X->ZZ studies
  gROOT->LoadMacro("../src/RooSpinZero_7DComplex_withAccep.cc+");
  //gROOT->LoadMacro("../src/AngularPdfFactory.cc+");
  gROOT->LoadMacro("../src/ScalarPdfFactory.cc+");
  gROOT->LoadMacro("../src/Playground.cc+");

  // tools for ILC studies 
  gROOT->LoadMacro("../src/RooSpinZero_5D_ZH.cc+");
  gROOT->LoadMacro("../src/RooSpinZero_3D_ZH.cc+");
  gROOT->LoadMacro("../src/ScalarPdfFactoryZH.cc+");
  gROOT->LoadMacro("../src/PlaygroundZH.cc+");

  // loading tdr style for plots
  gROOT->ProcessLine(".L  tdrstyle.C");
  setTDRStyle();  

}
