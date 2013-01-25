// source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.04/x86_64-slc5-gcc43-opt/root/bin/thisroot.sh

bool plotData=false;
bool plotAltSig=false;

double ZXnorm = 21;//HCP:1.0268 + 1.9260 + 0.5210 + 1.1377 + 0.3423 + 0.5697;
TString inputDir="/tmp/sbologne/Trees_061112_M126/";
TString JHUinputDir="/tmp/sbologne/Trees_061112_M126/";

void plotSuperMelaProjections(TString drawString="(p0plus_VAJHU*p0plus_m4l)/((p0plus_VAJHU*p0plus_m4l)+(bkg_VAMCFMNorm*bkg_m4l))",
			      TString cutString="ZZMass>106.&&ZZMass<141."){

  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();

  TCanvas* can = new TCanvas("can","can",500,500);

  gStyle->SetOptStat(0);

  TChain* qqZZ_8 = new TChain("SelectedTree");
  qqZZ_8->Add(inputDir+"PRODFSR_8TeV/4mu/HZZ4lTree_ZZTo*_withSMD_doubleCBonly_withProbabilities.root");
  qqZZ_8->Add(inputDir+"PRODFSR_8TeV/4e/HZZ4lTree_ZZTo*_withSMD_doubleCBonly_withProbabilities.root");
  qqZZ_8->Add(inputDir+"PRODFSR_8TeV/2e2mu/HZZ4lTree_ZZTo*_withSMD_doubleCBonly_withProbabilities.root");
  TChain* qqZZ_7 = new TChain("SelectedTree");
  qqZZ_7->Add(inputDir+"PRODFSR_7TeV/4mu/HZZ4lTree_ZZTo*_withSMD_doubleCBonly_withProbabilities.root");
  qqZZ_7->Add(inputDir+"PRODFSR_7TeV/4e/HZZ4lTree_ZZTo*_withSMD_doubleCBonly_withProbabilities.root");
  qqZZ_7->Add(inputDir+"PRODFSR_7TeV/2e2mu/HZZ4lTree_ZZTo*_withSMD_doubleCBonly_withProbabilities.root");

  TChain* SM_8 = new TChain("SelectedTree");
  SM_8->Add(inputDir+"PRODFSR_8TeV/4mu/HZZ4lTree_H126_withSMD_doubleCBonly_withProbabilities.root");
  SM_8->Add(inputDir+"PRODFSR_8TeV/4e/HZZ4lTree_H126_withSMD_doubleCBonly_withProbabilities.root");
  SM_8->Add(inputDir+"PRODFSR_8TeV/2e2mu/HZZ4lTree_H126_withSMD_doubleCBonly_withProbabilities.root");
  TChain* SM_7 = new TChain("SelectedTree");
  SM_7->Add(inputDir+"PRODFSR_7TeV/4mu/HZZ4lTree_H126_withSMD_doubleCBonly_withProbabilities.root");
  SM_7->Add(inputDir+"PRODFSR_7TeV/4e/HZZ4lTree_H126_withSMD_doubleCBonly_withProbabilities.root");
  SM_7->Add(inputDir+"PRODFSR_7TeV/2e2mu/HZZ4lTree_H126_withSMD_doubleCBonly_withProbabilities.root");

  //TChain* ZX = new TChain("SelectedTree");

  TChain* data = new TChain("SelectedTree");
  data->Add(inputDir+"PRODFSR_8TeV/data/HZZ4lTree_Double*_withSMD_doubleCBonly_withProbabilities.root");
  data->Add(inputDir+"PRODFSR_7TeV/data/HZZ4lTree_Double*_withSMD_doubleCBonly_withProbabilities.root");

  data->Draw(drawString+">>datahisto(30,0,1)",cutString);
  TH1F* datahisto = (TH1F*) gDirectory->Get("datahisto");

  TChain* PS_8 = new TChain("SelectedTree");
  PS_8->Add(JHUinputDir+"JHU_8TeV_withInterf/4mu/HZZ4lTree_jhuPseH126_withSMD_doubleCBonly_withProbabilities.root");
  PS_8->Add(JHUinputDir+"JHU_8TeV_withInterf/4e/HZZ4lTree_jhuPseH126_withSMD_doubleCBonly_withProbabilities.root");
  PS_8->Add(JHUinputDir+"JHU_8TeV_withInterf/2e2mu/HZZ4lTree_jhuPseH126_withSMD_doubleCBonly_withProbabilities.root");
  TChain* PS_7 = new TChain("SelectedTree");
  PS_7->Add(JHUinputDir+"JHU_7TeV_withInterf/4mu/HZZ4lTree_jhuPseH126_withSMD_doubleCBonly_withProbabilities.root");
  PS_7->Add(JHUinputDir+"JHU_7TeV_withInterf/4e/HZZ4lTree_jhuPseH126_withSMD_doubleCBonly_withProbabilities.root");
  PS_7->Add(JHUinputDir+"JHU_7TeV_withInterf/2e2mu/HZZ4lTree_jhuPseH126_withSMD_doubleCBonly_withProbabilities.root");
  
  qqZZ_7->Draw(drawString+">>qqZZ_7histo(30,0,1)","5.051*MC_weight*("+cutString+")");
  TH1F* qqZZ_7histo = (TH1F*) gDirectory->Get("qqZZ_7histo");

  qqZZ_8->Draw(drawString+">>qqZZ_8histo(30,0,1)","19.6*MC_weight*("+cutString+")");
  TH1F* qqZZ_8histo = (TH1F*) gDirectory->Get("qqZZ_8histo");

  SM_7->Draw(drawString+">>SM_7histo(30,0,1)","5.051*MC_weight*("+cutString+")");
  TH1F* SM_7histo = (TH1F*) gDirectory->Get("SM_7histo");

  SM_8->Draw(drawString+">>SM_8histo(30,0,1)","19.6*MC_weight*("+cutString+")");
  TH1F* SM_8histo = (TH1F*) gDirectory->Get("SM_8histo");

  PS_8->Draw(drawString+">>PS_8histo(30,0,1)","19.6*MC_weight*("+cutString+")");
  TH1F* PS_8histo = (TH1F*) gDirectory->Get("PS_8histo");

  PS_7->Draw(drawString+">>PS_7histo(30,0,1)","5.051*MC_weight*("+cutString+")");
  TH1F* PS_7histo = (TH1F*) gDirectory->Get("PS_7histo");
 
  //=========================
	
  //datahisto->SetBinErrorOption(TH1::kPoisson);
  datahisto->SetMarkerStyle(20);
  datahisto->SetMarkerSize(1.1);
	
  TH1F* qqZZhisto = new TH1F(*qqZZ_7histo);
  qqZZhisto->Add(qqZZ_8histo);
  qqZZhisto->SetLineColor(1);
  qqZZhisto->SetLineWidth(2);
  qqZZhisto->SetFillColor(kAzure-9);

  TH1F* ZXhisto = new TH1F(*qqZZhisto);
  ZXhisto->Scale(ZXnorm/ZXhisto->Integral());
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
  PShisto->SetFillColor(0);
  //==========================

  datahisto->GetXaxis()->SetTitle("it{D}_{bkg}");
  datahisto->GetYaxis()->SetTitle("Events");

  THStack* stack = new THStack("stack","stack");
  stack->Add(ZXhisto);
  stack->Add(qqZZhisto);
  stack->Add(SMhisto);
  stack->Draw("");
  stack->GetXaxis()->SetTitle("#it{D}_{bkg}");
  stack->GetYaxis()->SetTitle("Events");  

  if(plotData){
    datahisto->Draw("E1");
    stack->Draw("SAME");
    datahisto->Draw("E1same");
    datahisto->Draw("SAMEp");
  }else{
    stack->Draw();
  }

  // --------------- legend ----------------

  TLegend* leg = new TLegend(.3,.65,.65,.9);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);

  if(plotData) leg->AddEntry(datahisto,"data","p");
  leg->AddEntry(SMhisto,"0^{+}, m_{H}=126 GeV","l");
  leg->AddEntry(qqZZhisto,"ZZ/Z#gamma^{*}","f");
  leg->AddEntry(ZXhisto,"Z+X","f");

  leg->Draw();

  if(plotData) datahisto->Draw("E1same");
  gPad->RedrawAxis();

  // -------- plot header --------------

  TPaveText *pt = new TPaveText(0.1577181,0.9562937,0.9580537,0.9947552,"brNDC");
  pt->SetBorderSize(0);
  pt->SetTextAlign(12);
  pt->SetFillStyle(0);
  pt->SetTextFont(42);
  pt->SetTextSize(0.03);
  TText *text = pt->AddText(0.01,0.5,"CMS preliminary");
  text = pt->AddText(0.3,0.6,"#sqrt{s} = 7 TeV, L = 5.1 fb^{-1}  #sqrt{s} = 8 TeV, L = 19.6 fb^{-1}");
  pt->Draw();   

  // ---------- save ----------
  
  can->SaveAs("superMELAproj.png");
  can->SaveAs("superMELAproj.eps");
  can->SaveAs("superMELAproj.root");
  
}
