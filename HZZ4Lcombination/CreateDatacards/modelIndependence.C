
TString inputDir = "/scratch0/hep/whitbeck/4lHelicity/datafiles/Trees_261012/";

void modelIndependence(){

  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();

  TCanvas* can = new TCanvas("can","can",500,500);

  TChain* qqZZ = new TChain("SelectedTree");
  qqZZ->Add(inputDir+"PRODFSR_8TeV/4mu/HZZ4lTree_ZZTo4mu*.root");
  qqZZ->Add(inputDir+"PRODFSR_8TeV/4e/HZZ4lTree_ZZTo4e*.root");
  qqZZ->Add(inputDir+"PRODFSR_8TeV/2e2mu/HZZ4lTree_ZZTo2e2mu*.root");

  TChain* SM = new TChain("SelectedTree");
  SM->Add(inputDir+"JHU_8TeV/4mu/HZZ4lTree_jhuH125*.root");
  SM->Add(inputDir+"JHU_8TeV/4e/HZZ4lTree_jhuH125*.root");
  SM->Add(inputDir+"JHU_8TeV/2e2mu/HZZ4lTree_jhuH125*.root");

  TChain* PS = new TChain("SelectedTree");
  PS->Add(inputDir+"JHU_8TeV/4e/HZZ4lTree_jhuPseH125*.root");
  PS->Add(inputDir+"JHU_8TeV/4mu/HZZ4lTree_jhuPseH125*.root");
  PS->Add(inputDir+"JHU_8TeV/2e2mu/HZZ4lTree_jhuPseH125*.root");

  TChain* GR = new TChain("SelectedTree");
  GR->Add(inputDir+"JHU_8TeV/4e/HZZ4lTree_jhuGravH125*.root");
  GR->Add(inputDir+"JHU_8TeV/4mu/HZZ4lTree_jhuGravH125*.root");
  GR->Add(inputDir+"JHU_8TeV/2e2mu/HZZ4lTree_jhuGravH125*.root");
  
  qqZZ->Draw("ZZLD>>qqZZhisto(30,0,1)","(ZZMass>110&&ZZMass<140)");
  TH1F* qqZZhisto = (TH1F*) gDirectory->Get("qqZZhisto");
  qqZZhisto->SetLineColor(4);
  qqZZhisto->SetLineWidth(2);
  qqZZhisto->SetFillColor(4);
  qqZZhisto->SetFillStyle(3553);
  qqZZhisto->Scale(1./qqZZhisto->Integral());

  qqZZhisto->GetXaxis()->SetTitle("KD");
  qqZZhisto->GetYaxis()->SetTitle("Normalized To Unity");

  SM->Draw("ZZLD>>SMhisto(30,0,1)","(ZZMass>110&&ZZMass<140)");
  TH1F* SMhisto = (TH1F*) gDirectory->Get("SMhisto");
  SMhisto->SetLineColor(kRed);
  SMhisto->SetLineWidth(2);
  //SMhisto->SetFillColor(4);
  //SMhisto->SetFillStyle(4554);
  SMhisto->Scale(1./SMhisto->Integral());

  PS->Draw("ZZLD>>PShisto(30,0,1)","(ZZMass>110&&ZZMass<140)");
  TH1F* PShisto = (TH1F*) gDirectory->Get("PShisto");
  PShisto->SetLineColor(kGreen+1);
  PShisto->SetLineWidth(2);
  //PShisto->SetFillColor(4);
  //PShisto->SetFillStyle(4554);
  PShisto->Scale(1./PShisto->Integral());

  GR->Draw("ZZLD>>GRhisto(30,0,1)","(ZZMass>110&&ZZMass<140)");
  TH1F* GRhisto = (TH1F*) gDirectory->Get("GRhisto");
  GRhisto->SetLineColor(kMagenta+1);
  GRhisto->SetLineWidth(2);
  //GRhisto->SetFillColor(4);
  //GRhisto->SetFillStyle(4554);
  GRhisto->Scale(1./GRhisto->Integral());

  qqZZhisto->Draw();
  SMhisto->Draw("SAME");
  PShisto->Draw("SAME");
  GRhisto->Draw("SAME");

  TLegend* leg = new TLegend(.4,.6,.8,.9);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);

  leg->AddEntry(qqZZhisto,"ZZ, Z#gamma^{*}","f");
  leg->AddEntry(SMhisto,"J^{P}=0^{+} (SM Higgs)","f");
  leg->AddEntry(PShisto,"J^{P}=0^{-}","f");
  leg->AddEntry(GRhisto,"J^{P}=2^{+}_{min}","f");

  leg->Draw();  

  can->SaveAs("modelIndependence_MELA.eps");
  can->SaveAs("modelIndependence_MELA.png");

}
