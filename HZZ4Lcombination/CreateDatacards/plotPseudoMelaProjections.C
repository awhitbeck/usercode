#include <vector>
#include <algorithm>

TString inputDir="root://lxcms02//data/Higgs/rootuplesOut/191012/";
TString dataDir="CMSdata/";

void plotPseudoMelaProjections(){

  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();

  gStyle->SetOptStat(0);

  TChain* data = new TChain("data_obs");
  data->Add(dataDir+"/hzz*12.21.root");
  data->Add(dataDir+"/hzz*5.051.root");

  TChain* qqZZ = new TChain("SelectedTree");
  qqZZ->Add(inputDir+"PRODFSR/4mu/HZZ4lTree_ZZTo*.root");
  qqZZ->Add(inputDir+"PRODFSR/4e/HZZ4lTree_ZZTo*.root");
  qqZZ->Add(inputDir+"PRODFSR/2mu2e/HZZ4lTree_ZZTo*.root");
  qqZZ->Add(inputDir+"PRODFSR_8TeV/4mu/HZZ4lTree_ZZTo*.root");
  qqZZ->Add(inputDir+"PRODFSR_8TeV/4e/HZZ4lTree_ZZTo*.root");
  qqZZ->Add(inputDir+"PRODFSR_8TeV/2mu2e/HZZ4lTree_ZZTo*.root");
  //TChain* ZX = new TChain("SelectedTree");
  
  TChain* SM = new TChain("SelectedTree");
  SM->Add(inputDir+"PRODFSR/4mu/HZZ4lTree_H125.root");
  SM->Add(inputDir+"PRODFSR/4e/HZZ4lTree_H125.root");
  SM->Add(inputDir+"PRODFSR/2mu2e/HZZ4lTree_H125.root");
  SM->Add(inputDir+"PRODFSR_8TeV/4mu/HZZ4lTree_H125.root");
  SM->Add(inputDir+"PRODFSR_8TeV/4e/HZZ4lTree_H125.root");
  SM->Add(inputDir+"PRODFSR_8TeV/2mu2e/HZZ4lTree_H125.root");

  TChain* PS = new TChain("SelectedTree");
  PS->Add(inputDir+"JHU/4mu/HZZ4lTree_jhuPseH125.root");
  PS->Add(inputDir+"JHU/4e/HZZ4lTree_jhuPseH125.root");
  PS->Add(inputDir+"JHU/2mu2e/HZZ4lTree_jhuPseH125.root");
  PS->Add(inputDir+"JHU_8TeV/4mu/HZZ4lTree_jhuPseH125.root");
  PS->Add(inputDir+"JHU_8TeV/4e/HZZ4lTree_jhuPseH125.root");
  PS->Add(inputDir+"JHU_8TeV/2mu2e/HZZ4lTree_jhuPseH125.root");

  TH1F* qqZZhisto, *SMhisto, *PShisto, *datahisto;

  TCanvas* can = new TCanvas("can","can",400,400);
  
  qqZZ->Draw("ZZpseudoLD>>qqZZhisto(30,0,1)","MC_weight*(ZZMass>120&&ZZMass<130)");
  SM->Draw("ZZpseudoLD>>SMhisto(30,0,1)","MC_weight*(ZZMass>120&&ZZMass<130)");
  PS->Draw("ZZpseudoLD>>PShisto(30,0,1)","MC_weight*(ZZMass>120&&ZZMass<130)");
  data->Draw("CMS_zz4l_KD>>datahisto(30,0,1)","(ZZMass>120&&ZZMass<130)");

  qqZZhisto = (TH1F*) gDirectory->Get("qqZZhisto");
  PShisto   = (TH1F*) gDirectory->Get("PShisto");
  SMhisto   = (TH1F*) gDirectory->Get("SMhisto");
  datahisto   = (TH1F*) gDirectory->Get("datahisto");

  cout << " data: " << datahisto->Integral() << endl;

  style(*qqZZhisto,4);
  style(*SMhisto,2);
  style(*PShisto,3,1,true);


  PShisto->GetXaxis()->SetTitle("pseudo-Mela");
  PShisto->GetYaxis()->SetTitle("Events");

  vector<double> maxHeight;
  maxHeight.push_back(PShisto->GetMaximum());
  maxHeight.push_back(qqZZhisto->GetMaximum());  
  maxHeight.push_back(SMhisto->GetMaximum());
  maxHeight.push_back(datahisto->GetMaximum());

  //maxHeight.sort(maxHeight.begin(),maxHeight.end());

  //PShisto->GetYaxis()->SetRangeUser(0,maxHeight[maxHeight.size()-1]*1.5);
  PShisto->GetYaxis()->SetRangeUser(0,8);
  
  PShisto->Draw();
  SMhisto->Draw("SAME");
  qqZZhisto->Draw("SAME");
  datahisto->Draw("SAMEpe");

  TLegend* leg = new TLegend(.2,.45,.6,.9,"120< m_{4l}<130 GeV, 5.051+12.21 fb^{-1}");
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  
  leg->AddEntry(PShisto,"0^{-} m_{H}=125 GeV","f");  
  leg->AddEntry(SMhisto,"0^{+} m_{H}=125 GeV","l");
  leg->AddEntry(qqZZhisto,"qqZZ GeV","l");
  leg->AddEntry(qqZZhisto,"data","p");

  leg->Draw();
  
  can->SaveAs("pseudoMELAdistribution.eps");
  can->SaveAs("pseudoMELAdistribution.png");

}

void style(TH1F & histo,int color=1,int style=1,bool fill=false, int fillStyle=5335){

  histo.Scale(18/histo.Integral());
  histo.SetLineColor(color);
  histo.SetLineStyle(style);
  histo.SetLineWidth(2);
  if(fill){
    histo.SetFillColor(color);
    histo.SetFillStyle(fillStyle);
  }

}
