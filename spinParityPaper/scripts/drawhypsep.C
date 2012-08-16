//
// This code draws the various plots for the signal separtion
// run by root -l drawhypsep.C 
// 

#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TAxis.h"
#include <iostream>
#include "TH2F.h"
#include "TH1F.h"
#include "TString.h"
#include "TRint.h"
#include "TChain.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLine.h"
#include "TLegend.h"
#include "TCut.h"
#include "THStack.h"
#include "TNtuple.h"

void setTDRStyle() ;

int getMedianBin(TH1F *h);

void drawhypsep(char* discrimName="pseudomelaLD", int intLumi=22, int nbins=50, double xmax=20.)
{

  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();

  gROOT->ForceStyle();
  
  TString fileName = Form("hypSep_22fb_fullStats/SMHiggs_vs_altModel_%s_%ifb_*.root", discrimName, intLumi );

  TChain* hypTuple = new TChain("hypTuple");
  hypTuple->Add(fileName);
  assert(hypTuple);
  
  /*
  TFile *file = new TFile(fileName, "READ");
  std::cout << "Opening " << fileName << "\n";
  TNtuple *hypTuple = (TNtuple*) file->Get("hypTuple");
  assert(hypTuple);
  */

  double xmin = xmax*-1;
   
  TCut cut("nSigFitH1>0.001");

  TH1F *S_H0 = new TH1F("S_H0", "S_H0", nbins, xmin, xmax);
  hypTuple->Project("S_H0", "S_H0", cut);
  
  double mean_S_H0 = S_H0->GetMean();
  std::cout << "mean_S_H0 = " << mean_S_H0 << "\t";
  
  TH1F *S_H1 = new TH1F("S_H1", "S_H1", nbins, xmin, xmax);
  hypTuple->Project("S_H1", "S_H1", cut);
  
  double mean_S_H1 = S_H1->GetMean();
  std::cout << "mean_S_H1 = " << mean_S_H1 << "\n";

  //
  // Calculate separating in terms of sigma based on S_H0 and S_H1
  // 
  
  int nBins = S_H0->GetNbinsX();
  double norm0 = S_H0->Integral( 1, nBins );
  double norm1 = S_H1->Integral( 1, nBins );
  double int0(0.), int1(0.);
  double diff = 10.;
  double coverage = 0.;

  int nbin_eq = 0;
  double temp1, temp0;
  for (int i = 1; i <= nBins; i++){
    
    //int0 = S_H0->Integral(1,i)/norm0;
    int1 = S_H1->Integral(1,i)/norm0;
    //int1 = S_H1->Integral(i,nBins)/norm1;
    int0 = S_H0->Integral(i,nBins)/norm1;
    
    if (fabs(int0-int1) < diff){
      diff = fabs(int0-int1);
      temp1=int1;
      temp0=int0;
      coverage = 0.5*(int0+int1);
      nbin_eq = i;
    }
  }  
  cout<<temp0<<"   "<<temp1<<"  "<<endl;
  std::cout << "coverage : " << coverage << ", for bin " << nbin_eq << "\n";
  //double sepH = 2*ROOT::Math::normal_quantile_c(1.0 - coverage, 1.0);
  double sepH = 2*ROOT::Math::normal_quantile_c(coverage, 1.0);
  std::cout << "histogram separatino is: " <<  sepH << ", with sigma coverage: " << coverage << std::endl;
  
  // 
  // Calculate the fraction of events of the hypothesis 2 in the hyp1's plot
  //
  int bin_median_H1 = getMedianBin(S_H1);
  std::cout << "bin_median_H1 = " << bin_median_H1 << "\n";
  
  double frac_H0_beyondH1Median = S_H0->Integral(bin_median_H1, nBins) / norm0;
  double sepH0vsH1 = ROOT::Math::normal_quantile_c(frac_H0_beyondH1Median, 1.0);
  std::cout << "frac of H0 histogram beyond the H1 median " << frac_H0_beyondH1Median << ", correspond to " << sepH0vsH1 << " sigma\n";
  

  int bin_median_H0 = getMedianBin(S_H0);
  std::cout << "bin_median_H0 = " << bin_median_H0 << "\n";
  
  double frac_H1_beyondH0Median = S_H1->Integral(1, bin_median_H0) / norm1;
  double sepH1vsH0 = ROOT::Math::normal_quantile_c(frac_H1_beyondH0Median, 1.0);
  std::cout << "frac of H1 histogram beyond the H0 median " << frac_H1_beyondH0Median << ", correspond to " << sepH1vsH0 << " sigma\n";
  //
  // Plotting stuff
  // 
  
  // set line color marker color etc
  S_H0->SetLineColor(kRed);
  S_H0->SetMarkerColor(kRed);
  S_H0->SetMarkerStyle(8);
  S_H1->SetMarkerStyle(24);
  S_H1->SetLineColor(kBlue);
  S_H1->SetMarkerColor(kBlue);
  S_H0->SetXTitle("-2ln(L_{1}/L_{2})");
  S_H1->SetXTitle("-2ln(L_{1}/L_{2})");
  S_H0->SetYTitle("experiments");
  S_H1->SetYTitle("experiments");
  S_H0->GetXaxis()->CenterTitle();
  S_H1->GetXaxis()->CenterTitle();
  S_H0->GetYaxis()->CenterTitle();
  S_H1->GetYaxis()->CenterTitle();
  S_H1->GetXaxis()->SetTitleOffset(1.0);
  S_H0->GetXaxis()->SetTitleOffset(1.0);
  S_H1->GetYaxis()->SetTitleOffset(1.0);
  S_H0->GetYaxis()->SetTitleOffset(1.0);
  TGaxis::SetMaxDigits(3);

  //added by me  

  TH1F *S_H1_tail = new TH1F("S_H1_tail", "S_H1_tail", nbin_eq, xmin, S_H1->GetBinCenter(nbin_eq)+S_H1->GetBinWidth(nbin_eq)/2.);
  for (int i=0;i<=nbin_eq;i++){
    S_H1_tail->SetBinContent(i,S_H1->GetBinContent(i));
  }
  
  S_H1_tail->SetFillColor(kBlue);
  S_H1_tail->SetLineColor(kWhite);
  S_H1_tail->SetFillStyle(3244);

  double xmaxH1=S_H1->GetBinCenter(bin_median_H0)+S_H1->GetBinWidth(bin_median_H0)/2.;
  TH1F *S_H1_tailSmall = new TH1F("S_H1_tailSmall", "S_H1_tailSmall", (int)((xmaxH1-xmin)/S_H1->GetBinWidth(1)), xmin, xmaxH1);
  for (int i=0;i<=(bin_median_H0);i++){
      S_H1_tailSmall->SetBinContent(i,S_H1->GetBinContent(i));
  }
  S_H1_tailSmall->SetFillColor(kBlue);
  S_H1_tailSmall->SetLineColor(kBlue);

  TH1F *S_H0_tail = new TH1F("S_H0_tail", "S_H0_tail", nbin_eq, S_H0->GetBinCenter(nbin_eq)+S_H0->GetBinWidth(nbin_eq)/2.,xmax);
  for (int i=1;i<=nbin_eq;i++){
      S_H0_tail->SetBinContent(i,S_H0->GetBinContent(i+nbin_eq));
  }
  S_H0_tail->SetFillColor(kRed);
  S_H0_tail->SetLineColor(kWhite);
  S_H0_tail->SetFillStyle(3244);

  double xminH0=S_H0->GetBinCenter(bin_median_H1)-S_H0->GetBinWidth(bin_median_H1)/2.,
    TH1F *S_H0_tailSmall = new TH1F("S_H0_tailSmall", "S_H0_tailSmall", (int)((xmax-xminH0)/S_H0->GetBinWidth(1)),xminH0, xmax);
  for (int i=0;i<=bin_median_H1;i++){
    S_H0_tailSmall->SetBinContent(i,S_H0->GetBinContent(i+bin_median_H1-1));
  }
  S_H0_tailSmall->SetFillColor(kRed);
  S_H0_tailSmall->SetLineColor(kRed);

  TCanvas *c1 = new TCanvas("c1", "c1",800,800);

  float yMax_S = S_H0->GetMaximum();
  yMax_S = yMax_S > S_H1->GetMaximum() ? yMax_S : S_H1->GetMaximum();
  S_H0->GetYaxis()->SetRangeUser(0.0001,(yMax_S * 1.2));

  double xline= S_H0->GetBinCenter(nbin_eq)+ S_H0->GetBinWidth(1)/2.;
  double yline= S_H0->GetMaximum(10000);
  TLine *line = new TLine(xline,yMax_S*1.2,xline,0);
  double xline0=  S_H0->GetBinCenter(bin_median_H0 )+ S_H0->GetBinWidth(1)/2.;
  line->SetLineWidth(2);
  TLine *line0 = new TLine(xline0,yMax_S*1.2,xline0,0);
  line0->SetLineStyle(2);
  line0->SetLineWidth(2);
  double xline1=  S_H1->GetBinCenter(bin_median_H1) - S_H1->GetBinWidth(1)/2.;
  TLine *line1 = new TLine(xline1,yMax_S*1.2,xline1,0);
  line1->SetLineStyle(2);
  line1->SetLineWidth(2);

  //S_H0->Fit("gaus");
  //S_H1->Fit("gaus");
  S_H0->Draw("e");
  S_H1_tail->Draw("same");
  S_H0_tail->Draw("same");
  S_H1_tailSmall->Draw("same");
  S_H0_tailSmall->Draw("same");
  S_H0->Draw("samee");
  S_H1->Draw("samee");
  line->Draw("same");
  line0->Draw("same");
  line1->Draw("same");

  /*leg->Draw("same");
  tex_sepH->Draw("same");
  tex_sepH0vsH1->Draw("same");
  tex_sepH1vsH0->Draw("same");*/
  cout<<"Here"<<endl;

  c1->SaveAs(Form("epsfiles/hypsep_%s_%i.eps", discrimName, intLumi));
  c1->SaveAs(Form("pngfiles/hypsep_%s_%i.png", discrimName, intLumi));


  //delete S_H0;
  //delete S_H1;
  //delete c1;

}


int getMedianBin(TH1F *h) {
  
  if ( h == 0 ) {
    return 0;
  }
  int nBins = h->GetNbinsX();
  
  double norm = h->Integral(1, nBins);
  int bin_median = 1;
  double diff = 1;

  for ( int i = 1; i < nBins; i++) {
    double frac = h->Integral(1, i) / norm;
    double diff_bin = fabs(frac - 0.5 );
    if ( diff_bin < diff ) {
      diff = diff_bin;
      bin_median = i;
    }
    // std::cout << "Bin " << i << ", frac " << frac << ", diff_bin " << diff_bin << "\n";
  }
  
  return bin_median;
}
