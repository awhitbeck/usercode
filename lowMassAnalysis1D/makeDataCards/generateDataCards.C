
void generateDataCards(){

  gSystem->AddIncludePath("-I$ROOFITSYS/include");
  //gROOT->ProcessLine(".L ../../src/HZZ2L2QRooPdfs.cc+");
  gROOT->ProcessLine(".L PDFs/RooDoubleCB.cc+");
  gROOT->ProcessLine(".L make_roofitfiles.C+");
  gROOT->ProcessLine(".L make_datacards.C+");
  make_datacards();

  }


