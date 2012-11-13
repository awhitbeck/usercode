
double ZXnorm = 1.0268 + 1.9260 + 0.5210 + 1.1377 + 0.3423 + 0.5697;

TString JHUinputDir="/scratch0/hep/whitbeck/4lHelicity/datafiles/Trees_M126_special/Trees_061112_M125/";
TString inputDir="/scratch0/hep/whitbeck/4lHelicity/datafiles/Trees_M126_special/Trees_061112_M126/";

void plotGraviMelaProjections(){

  TString drawString="graviLD";
  TString cutString="ZZMass>106.&&ZZMass<141.&&superLD>.5";

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
  SM_8->Add(inputDir+"PRODFSR_8TeV/4mu/HZZ4lTree_H126_withSMD_doubleCBonly.root");
  SM_8->Add(inputDir+"PRODFSR_8TeV/4e/HZZ4lTree_H126_withSMD_doubleCBonly.root");
  SM_8->Add(inputDir+"PRODFSR_8TeV/2e2mu/HZZ4lTree_H126_withSMD_doubleCBonly.root");
  TChain* SM_7 = new TChain("SelectedTree");
  SM_7->Add(inputDir+"PRODFSR_7TeV/4mu/HZZ4lTree_H126_withSMD_doubleCBonly.root");
  SM_7->Add(inputDir+"PRODFSR_7TeV/4e/HZZ4lTree_H126_withSMD_doubleCBonly.root");
  SM_7->Add(inputDir+"PRODFSR_7TeV/2e2mu/HZZ4lTree_H126_withSMD_doubleCBonly.root");

  //TChain* ZX = new TChain("SelectedTree");

  TChain* data = new TChain("data_obs");
  data->Add(inputDir+"PRODFSR_8TeV/data/HZZ4lTree_Double*_withSMD_doubleCBonly.root");
  data->Add(inputDir+"PRODFSR_7TeV/data/HZZ4lTree_Double*_withSMD_doubleCBonly.root");

  data->Draw(drawString+">>datahisto(30,0,1)",cutString);
  TH1F* datahisto = (TH1F*) gDirectory->Get("datahisto");

  TChain* PS_8 = new TChain("SelectedTree");
  PS_8->Add(JHUinputDir+"JHU_8TeV/4mu/HZZ4lTree_jhuPseH125_withSMD_doubleCBonly.root");
  PS_8->Add(JHUinputDir+"JHU_8TeV/4e/HZZ4lTree_jhuPseH125_withSMD_doubleCBonly.root");
  PS_8->Add(JHUinputDir+"JHU_8TeV/2e2mu/HZZ4lTree_jhuPseH125_withSMD_doubleCBonly.root");
  TChain* PS_7 = new TChain("SelectedTree");
  PS_7->Add(JHUinputDir+"JHU_7TeV/4mu/HZZ4lTree_jhuPseH125_withSMD_doubleCBonly.root");
  PS_7->Add(JHUinputDir+"JHU_7TeV/4e/HZZ4lTree_jhuPseH125_withSMD_doubleCBonly.root");
  PS_7->Add(JHUinputDir+"JHU_7TeV/2e2mu/HZZ4lTree_jhuPseH125_withSMD_doubleCBonly.root");
  
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
  
	datahisto->SetBinErrorOption(TH1::kPoisson);
	datahisto->SetMarkerStyle(20);
	datahisto->SetMarkerSize(1.1);
	
  TH1F* qqZZhisto = new TH1F(*qqZZ_7histo);
  qqZZhisto->Add(qqZZ_8histo);
  qqZZhisto->SetLineColor(1);
  qqZZhisto->SetLineWidth(2);
  qqZZhisto->SetFillColor(kAzure-9);

  double with =(double) qqZZ_8->Draw("ZZMass",cutString);
  double without = (double) qqZZ_8->Draw("ZZMass","ZZMass>106.&&ZZMass<141.");

  cout << " efficiency of superMELA cut: " << with/without << endl;

  TH1F* ZXhisto = new TH1F(*qqZZhisto);
  ZXhisto->Scale(ZXnorm/ZXhisto->Integral()*with/without);
  ZXhisto->SetLineColor(1);
  ZXhisto->SetLineWidth(2);
  ZXhisto->SetFillColor(kGreen-5);

  TH1F* SMhisto = new TH1F(*SM_7histo);
  SMhisto->Add(SM_8histo);
  SMhisto->SetLineColor(kOrange+10);
  SMhisto->SetLineWidth(2);
  SMhisto->SetFillColor(0);

  TH1F* PShisto = new TH1F(*PS_7histo);
  PShisto->Add(PS_8histo);
  PShisto->SetLineColor(kRed+1);
  PShisto->SetLineWidth(2);
  PShisto->SetLineStyle(2);
  PShisto->SetFillColor(0);
  PShisto->Scale(SMhisto->Integral()/PShisto->Integral());

  //==========================

  datahisto->GetXaxis()->SetTitle("gravi-MELA");
  datahisto->GetYaxis()->SetTitle("Events");

  THStack* stackSM = new THStack("stackSM","stackSM");
  stackSM->Add(ZXhisto);
  stackSM->Add(qqZZhisto);
  stackSM->Add(SMhisto);

  THStack* stackPS = new THStack("stackPS","stackPS");
  stackPS->Add(ZXhisto);
  stackPS->Add(qqZZhisto);
  stackPS->Add(PShisto);

  
  // ------------ draw ----------------

  datahisto->Draw("E1");
  stackSM->Draw("SAME");
  stackPS->Draw("SAME");
  datahisto->Draw("E1same");
  datahisto->Draw("SAMEp");

  // ------------ legend ---------------

  TLegend* leg = new TLegend(.5,.65,.85,.9);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);

  leg->AddEntry(datahisto,"data","p");
  leg->AddEntry(SMhisto,"0^{+}, m_{H}=126 GeV","l");
  leg->AddEntry(PShisto,"2^{+}_{min}, m_{H}=126 GeV","l");
  leg->AddEntry(qqZZhisto,"ZZ/Z#gamma^{*}","f");
  leg->AddEntry(ZXhisto,"Z+X","f");

  leg->Draw();

  datahisto->Draw("E1same");
  gPad->RedrawAxis();

  // -------- plot header --------------

  TPaveText *pt = new TPaveText(0.1577181,0.9562937,0.9580537,0.9947552,"brNDC");
  pt->SetBorderSize(0);
  pt->SetTextAlign(12);
  pt->SetFillStyle(0);
  pt->SetTextFont(42);
  pt->SetTextSize(0.03);
  TText *text = pt->AddText(0.01,0.5,"CMS preliminary");
  text = pt->AddText(0.3,0.6,"#sqrt{s} = 7 TeV, L = 5.1 fb^{-1}  #sqrt{s} = 8 TeV, L = 12.2 fb^{-1}");
  pt->Draw();   
  

  // ------------ save 

  can->SaveAs("graviMELAproj_superMELA_p5.png");
  can->SaveAs("graviMELAproj_superMELA_p5.eps");
  can->SaveAs("graviMELAproj_superMELA_p5.root");
  
}
