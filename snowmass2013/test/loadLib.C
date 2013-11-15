{

  gSystem->AddIncludePath("-I$CMSSW_BASE/src/");
  gSystem->AddIncludePath("-I$ROOFITSYS/include/");
  
  //gROOT->LoadMacro("../src/RooSpinZero_7DComplex_withAccep_withFepspr.cc+");
  gROOT->LoadMacro("../src/RooSpinZero_3DKD_ZH_Simultaneous_PMF.cc+");
  //gROOT->LoadMacro("../src/ScalarPdfFactory_withFepspr.cc+");
  //gROOT->LoadMacro("fitSimultaneous_7D_fepspr.c+");

  gROOT->LoadMacro("fitSimultaneous_3D_PMF.c+");

  /*
 
  // tools for X->ZZ studies
  gROOT->LoadMacro("../src/RooqqZZ_JHU_ZgammaZZ_fast.cxx+");
  gROOT->LoadMacro("../src/RooSpinZero_7DComplex_withAccep.cc+");
  //gROOT->LoadMacro("../src/AngularPdfFactory.cc+");
  gROOT->LoadMacro("../src/ScalarPdfFactory.cc+");
  gROOT->LoadMacro("../src/Playground.cc+");

  // tools for ILC studies 
  gROOT->LoadMacro("../src/RooSpinZero_5D_ZH.cc+");
  gROOT->LoadMacro("../src/RooSpinZero_3D_ZH.cc+");
  gROOT->LoadMacro("../src/RooSpinZero_3D_ZH_Acc.cc+");
  gROOT->LoadMacro("../src/RooZZ_3D.cc+");
  gROOT->LoadMacro("../src/ScalarPdfFactoryZH.cc+");
  gROOT->LoadMacro("../src/PlaygroundZH.cc+");
  gROOT->LoadMacro("../src/KDcalcZH.cc+");

  // tools for PP->ZH studies
  gROOT->LoadMacro("../src/RooSpinZero_3D_ZH_pp.cc+");
  gROOT->LoadMacro("../src/ScalarPdfFactoryPPZH.cc+");
  gROOT->LoadMacro("../src/PlaygroundPPZH.cc+");
  gROOT->LoadMacro("../src/KDcalcPPZH.cc+");

  // tools for pp->WH jj
  gROOT->LoadMacro("../src/RooSpinZero_3D_WH_pp.cc+");

  // stat tools from summer 2012 paper 
  gROOT->LoadMacro("../src/statsFactory.cc+");
  
  // code to convert fa3
  gROOT->LoadMacro("../src/fa3Converter.cc+");

  // code for the 1D KD
  gROOT->LoadMacro("../src/RooSpinZero_KD_ZH.cc+");
  gROOT->LoadMacro("../src/RooSpinZero_KDInt_ZH.cc+");

  */

  // loading tdr style for plots
  gROOT->ProcessLine(".L  tdrstyle.C");
  setTDRStyle();  

}
