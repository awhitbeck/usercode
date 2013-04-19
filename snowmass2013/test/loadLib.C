{

  gSystem->AddIncludePath("-I$CMSSW_BASE/src/");
  gSystem->AddIncludePath("-I$ROOFITSYS/include/");
  gSystem->Load("libZZMatrixElementsnowmass2013.so");
  gROOT->LoadMacro("../src/AngularPdfFactory.cc+");
  gROOT->LoadMacro("../src/ScalarPdfFactory.cc+");

  gROOT->ProcessLine(".L  ~/tdrstyle.C");
  setTDRStyle();  

}
