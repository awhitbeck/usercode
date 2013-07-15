#include "TString.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"

void printup() 
{    

  float sqrtsVal = 1000.;
  bool withAcceptance = false;
  TString accName = "false";
  if (withAcceptance )  accName = "true";
  
  TString toyName = Form("pure_f_3_0_%.0fGeV_acc%s_fixphia3", sqrtsVal, accName.Data());

  TString fileName = Form("toyresults_%s.root", toyName.Data());
  TFile *inputFile = new TFile(fileName, "READ");
  std::cout << "opening " << fileName << "\n";

  TTree* results= (TTree*) inputFile->Get("fittree");
  assert(results);
  
  float fa3,fa3Error;

  results->SetBranchAddress("fa3",&fa3);
  results->SetBranchAddress("fa3_err",&fa3Error);

  TH1F *h_fa3 = new TH1F("h_fa3", "h_fa3", 50000, 0, 1);
  TH1F *h_fa3_pull = new TH1F("h_fa3_pull", "h_fa3_pull", 100, -5, 5);
  TH1F *h_fa3_error = new TH1F("h_fa3_error", "h_fa3_error", 100, 0, 1);
 
  double fa2Val=0, fa3Val=0, phia3Val=0, phia2Val=0;
  
  for(int iEvt=0; iEvt<results->GetEntries(); iEvt++){
    
    if(iEvt>=results->GetEntries()) break;
    results->GetEntry(iEvt);
    h_fa3->Fill(fa3);
    h_fa3_error->Fill(fa3Error);
    h_fa3_pull->Fill((fa3-fa3Val)/fa3Error);
    
    }

  double pvalue68=0, pvalue50=0, pvalue95=0, dummypvalue=-99, nextpvalue=-99;
  bool found50=false, found68=false, found95=false;
  for(int i=0;  i<h_fa3->GetNbinsX(); i++){
    if(i%1000==1)
      cout<<i<<endl;
    dummypvalue=h_fa3->Integral(0,i)/h_fa3->Integral();
    nextpvalue=h_fa3->Integral(0,i+1)/h_fa3->Integral();
    if(TMath::Abs(dummypvalue-0.50)<TMath::Abs(nextpvalue-0.50) && !found50){
      pvalue50=h_fa3->GetBinCenter(i);
      found50=true;
    }
    if(TMath::Abs(dummypvalue-0.68)<TMath::Abs(nextpvalue-0.68) && !found68){
      pvalue68=h_fa3->GetBinCenter(i);
      found68=true;
    }
    if(fabs(dummypvalue-0.95)<fabs(nextpvalue-0.95) && !found95){
      pvalue95=h_fa3->GetBinCenter(i);
      found95=true;
      break;
    }
  }
  
  cout << "fa3  50% : " << pvalue50 <<endl;
  cout << "fa3  68% : " << pvalue68 << endl;
  cout << "fa3  95% : " << pvalue95 << endl;
  cout << "fa3 mean : " << h_fa3->GetMean()<<endl;
  cout << "fa3 RMS : " << h_fa3->GetRMS() << endl;

  gROOT->ProcessLine(".L tdrstyle.C");
  gROOT->ProcessLine("setTDRStyle();");
  gROOT->ForceStyle();

  TCanvas *c1 = new TCanvas("c1","c1",800,800);
  c1->SetLogy();
  h_fa3->Rebin(20);
  h_fa3->GetXaxis()->SetRangeUser(0,0.1);
  h_fa3->SetXTitle("f_{3}");
  h_fa3->SetYTitle("experiments");
  h_fa3->Draw("hist");
  
  c1->SaveAs(Form("fitplots/fa3_fitresults_%s.eps", toyName.Data()));
  c1->SaveAs(Form("fitplots/fa3_fitresults_%s.png", toyName.Data()));

  delete c1;


 
}
  
