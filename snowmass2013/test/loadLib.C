{

  //gSystem->AddIncludePath("-I$CMSSW_BASE/src/");
  //gSystem->AddIncludePath("-I$ROOFITSYS/include/");
 
  gROOT->LoadMacro("../src/RooSpinZero_7DComplex.cc+");
  gROOT->LoadMacro("../src/AngularPdfFactory.cc+");
  gROOT->LoadMacro("../src/ScalarPdfFactory.cc+");
  gROOT->LoadMacro("../src/Playground.cc+");

  gROOT->ProcessLine(".L  tdrstyle.C");
  setTDRStyle();  

}
