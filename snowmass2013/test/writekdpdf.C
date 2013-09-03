// 
// This code writes out the lookup table for the kd based histogram
// 

using namespace RooFit ;

void writekdpdf()
{
  bool withAcc = false;
  TString accName = "false";
  if ( withAcc ) 
    accName = "true";

  FILE *fpdf;
  fpdf = fopen ("RooSpinZero_KD_ZH.cc", "w");
  
  //
  //  1D KD observable bin and ranges
  // 
  const int nbins = 100;
  double xMin = 0.3;
  double xMax = 1.;
  
  double zeroplus[nbins];
  double zerominus[nbins];

  //
  // 0+ 
  // 
  TChain *zeroplusTree = new TChain("SelectedTree");
  TString zeroplusFileName = Form("samples/ee_ZH/unweighted_unpol_g1_1M_%s.root", accName.Data());
  zeroplusTree->Add(zeroplusFileName);
  std::cout << "Reading " << zeroplusFileName << "\n";
  assert(zeroplusTree);

  TH1F *hist_zeroplus = new TH1F("hist_zeroplus", "hist_zeroplus", nbins, xMin, xMax);
  zeroplusTree->Project("hist_zeroplus", "pseudoMELA"); 
  hist_zeroplus->Scale(1./hist_zeroplus->Integral());
  

  for ( int i = 1; i < nbins+1 ; i++ ) {
    zeroplus[i-1] = hist_zeroplus->GetBinContent(i) * nbins / (xMax - xMin);
    fprintf(fpdf, "   zeroplus[%i] = %.6f;\n", i-1, zeroplus[i-1]);

  }


  //
  // 0-
  //
  TChain *zerominusTree = new TChain("SelectedTree");
  TString zerominusFileName = Form("samples/ee_ZH/unweighted_unpol_g4_1M_%s.root", accName.Data());
  zerominusTree->Add(zerominusFileName);
  // std::cout << "Reading " << zerominusFileName << "\n";
  assert(zerominusTree);

  TH1F *hist_zerominus = new TH1F("hist_zerominus", "hist_zerominus", nbins, xMin, xMax);
  zerominusTree->Project("hist_zerominus", "pseudoMELA"); 
  hist_zerominus->Scale(1./hist_zerominus->Integral());
  
  for ( int i = 1; i < nbins+1 ; i++ ) {
    zerominus[i-1] = hist_zerominus->GetBinContent(i)* nbins / (xMax - xMin);
    fprintf(fpdf, "   zerominus[%i] = %.6f;\n", i-1, zerominus[i-1]);
  }

  double kdval = 0.45;
  int bin_index = hist_zeroplus->FindBin(kdval); 
  printf("bin %i: 0+ %.6f,\t 0- %.6f\n", bin_index, hist_zeroplus->GetBinContent(bin_index)*nbins/(xMax - xMin), hist_zerominus->GetBinContent(bin_index)*nbins/(xMax-xMin));
  
  double bin_width = (xMax - xMin) / nbins;
  double array_index = (kdval - xMin) / bin_width; 
  printf("from array %i: 0+ %.6f,\t 0- %.6f\n", array_index, zeroplus[array_index], zerominus[array_index]); 
 

}


