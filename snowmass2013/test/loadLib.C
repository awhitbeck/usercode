{
  gSystem->AddIncludePath("-I$CMSSW_BASE/src/");
  gSystem->AddIncludePath("-I$ROOFITSYS/include/");
  gSystem->Load("libZZMatrixElementsnowmass2013.so");
  gROOT->LoadMacro("../src/AngularPdfFactory.h+");
  gROOT->LoadMacro("../src/VectorPdfFactory.h+");
  gROOT->LoadMacro("../src/TensorPdfFactory.h+");

}
