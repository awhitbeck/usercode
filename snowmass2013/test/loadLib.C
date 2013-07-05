{

  //gSystem->AddIncludePath("-I$CMSSW_BASE/src/");
  //gSystem->AddIncludePath("-I$ROOFITSYS/include/");
 
  // tools for X->ZZ studies
  gROOT->LoadMacro("../src/RooqqZZ_JHU_ZgammaZZ_fast.cxx+");
  gROOT->LoadMacro("../src/RooSpinZero_7DComplex_withAccep.cc+");
  //gROOT->LoadMacro("../src/AngularPdfFactory.cc+");
  gROOT->LoadMacro("../src/ScalarPdfFactory.cc+");
  gROOT->LoadMacro("../src/Playground.cc+");

  // tools for ILC studies 
  gROOT->LoadMacro("../src/RooSpinZero_5D_ZH.cc+");
  gROOT->LoadMacro("../src/RooSpinZero_3D_ZH.cc+");
  gROOT->LoadMacro("../src/ScalarPdfFactoryZH.cc+");
  gROOT->LoadMacro("../src/PlaygroundZH.cc+");
  gROOT->LoadMacro("../src/RooZZ_3D.cc+");

  // stat tools from summer 2012 paper 
  gROOT->LoadMacro("../src/statsFactory.cc+");
  
  // code to convert fa3
  gROOT->LoadMacro("../src/fa3Converter.cc+");

  // loading tdr style for plots
  gROOT->ProcessLine(".L  tdrstyle.C");
  setTDRStyle();  

}
