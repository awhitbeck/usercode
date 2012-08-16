


void run_addDtoTree(char* fileName){

  gROOT->ProcessLine(".L ../PDFs/RooXZsZs_5D.cxx+");
  gROOT->ProcessLine(".L ../PDFs/RooSpinOne_7D.cxx+");
  gROOT->ProcessLine(".L ../PDFs/RooSpinTwo_7D.cxx+");
  gROOT->ProcessLine(".L ../src/AngularPdfFactory.cc+");
  gROOT->ProcessLine(".L ../src/ScalarPdfFactory.cc+");
  gROOT->ProcessLine(".L ../src/VectorPdfFactory.cc+");
  gROOT->ProcessLine(".L ../src/TensorPdfFactory.cc+");
  //gROOT->ProcessLine(".L ../src/pseudoMELA.cc+");
  //gROOT->ProcessLine(".L ../src/graviMELA.cc+");
  gROOT->ProcessLine(".L ../PDFs/RooqqZZ_JHU.cxx+");
  gROOT->ProcessLine(".L MELA.C+");

  addDtoTree(fileName);

}
