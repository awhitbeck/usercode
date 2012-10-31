
double ZXnorm = 1.0268 + 1.9260 + 0.5210 + 1.1377 + 0.3423 + 0.5697;
TString inputDir="/scratch0/hep/whitbeck/4lHelicity/datafiles/Trees_261012/";

void plotSuperMelaProjections(TString drawString="superLD", TString cutString="ZZMass>105.&&ZZMass<140."){

  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();

  TCanvas* can = new TCanvas("can","can",500,500);

  TChain* qqZZ_8 = new TChain("SelectedTree");
  qqZZ_8->Add(inputDir+"PRODFSR_8TeV/4mu/HZZ4lTree_ZZTo*_withSMD_doubleCBonly.root");
  qqZZ_8->Add(inputDir+"PRODFSR_8TeV/4e/HZZ4lTree_ZZTo*_withSMD_doubleCBonly.root");
  qqZZ_8->Add(inputDir+"PRODFSR_8TeV/2e2mu/HZZ4lTree_ZZTo*_withSMD_doubleCBonly.root");
  TChain* qqZZ_7 = new TChain("SelectedTree");
  qqZZ_7->Add(inputDir+"PRODFSR_7TeV/4mu/HZZ4lTree_ZZTo*_withSMD_doubleCBonly.root");
  qqZZ_7->Add(inputDir+"PRODFSR_7TeV/4e/HZZ4lTree_ZZTo*_withSMD_doubleCBonly.root");
  qqZZ_7->Add(inputDir+"PRODFSR_7TeV/2e2mu/HZZ4lTree_ZZTo*_withSMD_doubleCBonly.root");

  TChain* SM_8 = new TChain("SelectedTree");
  SM_8->Add(inputDir+"PRODFSR_8TeV/4mu/HZZ4lTree_H125_withSMD_doubleCBonly.root");
  SM_8->Add(inputDir+"PRODFSR_8TeV/4e/HZZ4lTree_H125_withSMD_doubleCBonly.root");
  SM_8->Add(inputDir+"PRODFSR_8TeV/2e2mu/HZZ4lTree_H125_withSMD_doubleCBonly.root");
  TChain* SM_7 = new TChain("SelectedTree");
  SM_7->Add(inputDir+"PRODFSR_7TeV/4mu/HZZ4lTree_H125_withSMD_doubleCBonly.root");
  SM_7->Add(inputDir+"PRODFSR_7TeV/4e/HZZ4lTree_H125_withSMD_doubleCBonly.root");
  SM_7->Add(inputDir+"PRODFSR_7TeV/2e2mu/HZZ4lTree_H125_withSMD_doubleCBonly.root");

  //TChain* ZX = new TChain("SelectedTree");

  TChain* data = new TChain("data_obs");
  data->Add(inputDir+"PRODFSR_8TeV/data/HZZ4lTree_Double*_withSMD_doubleCBonly.root");
  data->Add(inputDir+"PRODFSR_7TeV/data/HZZ4lTree_Double*_withSMD_doubleCBonly.root");

  data->Draw(drawString+">>datahisto(30,0,1)",cutString);
  TH1F* datahisto = (TH1F*) gDirectory->Get("datahisto");

  TChain* PS_8 = new TChain("SelectedTree");
  PS_8->Add(inputDir+"JHU_8TeV/4mu/HZZ4lTree_jhuPseH125_withSMD_doubleCBonly.root");
  PS_8->Add(inputDir+"JHU_8TeV/4e/HZZ4lTree_jhuPseH125_withSMD_doubleCBonly.root");
  PS_8->Add(inputDir+"JHU_8TeV/2e2mu/HZZ4lTree_jhuPseH125_withSMD_doubleCBonly.root");
  TChain* PS_7 = new TChain("SelectedTree");
  PS_7->Add(inputDir+"JHU_7TeV/4mu/HZZ4lTree_jhuPseH125_withSMD_doubleCBonly.root");
  PS_7->Add(inputDir+"JHU_7TeV/4e/HZZ4lTree_jhuPseH125_withSMD_doubleCBonly.root");
  PS_7->Add(inputDir+"JHU_7TeV/2e2mu/HZZ4lTree_jhuPseH125_withSMD_doubleCBonly.root");
  
  qqZZ_7->Draw(drawString+">>qqZZ_7histo(30,0,1)","5.051*MC_weight*("+cutString+")");
  TH1F* qqZZ_7histo = (TH1F*) gDirectory->Get("qqZZ_7histo");

  qqZZ_8->Draw(drawString+">>qqZZ_8histo(30,0,1)","12.21*MC_weight*("+cutString+")");
  TH1F* qqZZ_8histo = (TH1F*) gDirectory->Get("qqZZ_8histo");

  SM_7->Draw(drawString+">>SM_7histo(30,0,1)","5.051*MC_weight*("+cutString+")");
  TH1F* SM_7histo = (TH1F*) gDirectory->Get("SM_7histo");

  SM_8->Draw(drawString+">>SM_8histo(30,0,1)","12.21*MC_weight*("+cutString+")");
  TH1F* SM_8histo = (TH1F*) gDirectory->Get("SM_8histo");

  PS_8->Draw(drawString+">>PS_8histo(30,0,1)","12.21*MC_weight*("+cutString+")");
  TH1F* PS_8histo = (TH1F*) gDirectory->Get("PS_8histo");

  PS_7->Draw(drawString+">>PS_7histo(30,0,1)","5.051*MC_weight*("+cutString+")");
  TH1F* PS_7histo = (TH1F*) gDirectory->Get("PS_7histo");
 
  //=========================

  datahisto->SetMarkerStyle(8);

  TH1F* qqZZhisto = new TH1F(*qqZZ_7histo);
  qqZZhisto->Add(qqZZ_8histo);
  qqZZhisto->SetLineColor(1);
  qqZZhisto->SetLineWidth(2);
  qqZZhisto->SetFillColor(kCyan+1);

  TH1F* ZXhisto = new TH1F(*qqZZhisto);
  ZXhisto->Scale(ZXnorm/ZXhisto->Integral());
  ZXhisto->SetLineColor(1);
  ZXhisto->SetLineWidth(2);
  ZXhisto->SetFillColor(kGreen+1);

  TH1F* SMhisto = new TH1F(*SM_7histo);
  SMhisto->Add(SM_8histo);
  SMhisto->SetLineColor(2);
  SMhisto->SetLineWidth(2);
  SMhisto->SetFillColor(0);

  TH1F* PShisto = new TH1F(*PS_7histo);
  PShisto->Add(PS_8histo);
  PShisto->SetLineColor(kMagenta+1);
  PShisto->SetLineWidth(2);
  PShisto->SetFillColor(0);
  //==========================

  datahisto->GetXaxis()->SetTitle("super-MELA");
  datahisto->GetYaxis()->SetTitle("Events");

  THStack* stack = new THStack("stack","stack");
  stack->Add(ZXhisto);
  stack->Add(qqZZhisto);
  stack->Add(SMhisto);

  datahisto->Draw("E1");
  stack->Draw("SAME");
  datahisto->Draw("E1same");
  datahisto->Draw("SAMEp");

  gPad->RedrawAxis();

  TLegend* leg = new TLegend(.3,.5,.8,.9);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);

  leg->AddEntry(data,"data","p");
  leg->AddEntry(SMhisto,"0^{+}, m_{H}=125 GeV","l");
  leg->AddEntry(qqZZhisto,"ZZ/Z#gamma^{*}","f");
  leg->AddEntry(ZXhisto,"l^{+}l^{-}+X","f");

  leg->Draw();

}
