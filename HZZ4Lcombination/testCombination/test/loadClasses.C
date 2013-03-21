{

  gSystem->Load("libHiggsAnalysisCombinedLimit.so");
  gSystem->Load("libHiggsAnalysisHZZ4lCombination.so");
  gSystem->AddIncludePath("-I$CMSSW_BASE/src/");
  gSystem->AddIncludePath("-I$ROOFITSYS/include/");
  gROOT->LoadMacro("../src/m4lSignalBase.h+");
  gROOT->LoadMacro("../src/m4lqqZZBase.h+");
  gROOT->LoadMacro("../src/m4lggZZBase.h+");
  gROOT->LoadMacro("../src/m4lZXBase.h+");
  gROOT->LoadMacro("../src/signalYields.h+");

}
