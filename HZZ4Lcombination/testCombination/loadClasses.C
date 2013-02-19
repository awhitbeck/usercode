{

  gSystem->Load("libHiggsAnalysisCombinedLimit.so");
  gSystem->AddIncludePath("-I$CMSSW_BASE/src/");
  gSystem->AddIncludePath("-I$ROOFITSYS/include/");
  gROOT->ProcessLine(".L m4lSignalBase.cc+");   

}
