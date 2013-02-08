#include <algorithm>

double ZXnorm =  1.6 + 1.06 + 0.78 + 0.73 + 0.35 + 0.28 ;//1.0086+0.3826+0.4505+1.3097+0.8967+1.9897;

TString JHUinputDir="/tmp/sbologne/130205/";
TString inputDir="/tmp/sbologne/130205/";

enum model {k0minus,k0hplus,k1minus,k1plus,k2mplus_gg,k2mplus_qqb,kNUM};

TString saveTag[6]={"0minus",
		    "0hplus",
		    "1minus",
		    "1plus",
		    "2mplus_gg",
		    "2mplus_qqbar"};

TString sampleName[6]={"jhuGenV2PseH126",
		       "jhuGenV2ScaHH126",
		       "jhuGenV2Vec1MH126",
		       "jhuGenV2Vec1PH126",
		       "jhuGenV2GravH126",
		       "jhuGenV2qqGravH126"};

TString legendTag[6]={"J^{P}=0^{-}",
		      "J^{P}=0^{+}_{h}",
		      "J^{P}=1^{-}",
		      "J^{P}=1^{+}",
		      "J^{P}=2^{+}_{m} (gg)",
		      "J^{P}=2^{+}_{m} (q#bar{q})"};		      


TString axisLabel[6]={"#it{D}_{0^{-}}",
		      "#it{D}_{0^{+}_{h}}",
		      "#it{D}(1^{-})",
		      "#it{D}(1^{+})",
		      "#it{D}_{2^{+}_{m} (gg)}",
		      "#it{D}_{2^{+}_{m} (q#bar{q})}"};		      

TString drawString[6]={"p0plus_VAJHU/(p0plus_VAJHU+p0minus_VAJHU)",
		       "p0plus_VAJHU/(p0plus_VAJHU+p0hplus_VAJHU)",
		       "p0plus_VAJHU/(p0plus_VAJHU+p1_VAJHU)",
		       "p0plus_VAJHU/(p0plus_VAJHU+p1plus_VAJHU)",
		       "p0plus_VAJHU/(p0plus_VAJHU+p2_VAJHU)",
		       "p0plus_VAJHU/(p0plus_VAJHU+p2qqb_VAJHU)"};

TString cutString;

bool applySuperKDcut=true;

void plotKDprojections(model myModel){

  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();

  TCanvas* can = new TCanvas("can","can",500,500);
  
  if(applySuperKDcut)
    cutString="ZZMass>106.&&ZZMass<141.&&p0plus_VAJHU*p0plus_m4l/(p0plus_VAJHU*p0plus_m4l+bkg_VAMCFMNorm*bkg_m4l)>.5";
  else
    cutString="ZZMass>106.&&ZZMass<141.";

  TChain* qqZZ_8 = new TChain("SelectedTree");
  qqZZ_8->Add(inputDir+"PRODFSR_8TeV/4mu/HZZ4lTree_ZZTo*.root");
  qqZZ_8->Add(inputDir+"PRODFSR_8TeV/4e/HZZ4lTree_ZZTo*.root");
  qqZZ_8->Add(inputDir+"PRODFSR_8TeV/2mu2e/HZZ4lTree_ZZTo*.root");
  TChain* qqZZ_7 = new TChain("SelectedTree");
  qqZZ_7->Add(inputDir+"PRODFSR_8TeV/4mu/HZZ4lTree_ZZTo*.root");
  qqZZ_7->Add(inputDir+"PRODFSR_8TeV/4e/HZZ4lTree_ZZTo*.root");
  qqZZ_7->Add(inputDir+"PRODFSR_8TeV/2mu2e/HZZ4lTree_ZZTo*.root");

  TChain* SM_8 = new TChain("SelectedTree");
  SM_8->Add(inputDir+"JHU_8TeV/4mu/HZZ4lTree_jhuGenV2H126.root");
  SM_8->Add(inputDir+"JHU_8TeV/4e/HZZ4lTree_jhuGenV2H126.root");
  SM_8->Add(inputDir+"JHU_8TeV/2mu2e/HZZ4lTree_jhuGenV2H126.root");
  TChain* SM_7 = new TChain("SelectedTree");
  SM_7->Add(inputDir+"JHU_8TeV/4mu/HZZ4lTree_jhuGenV2H126.root");
  SM_7->Add(inputDir+"JHU_8TeV/4e/HZZ4lTree_jhuGenV2H126.root");
  SM_7->Add(inputDir+"JHU_8TeV/2mu2e/HZZ4lTree_jhuGenV2H126.root");

  TChain* POWHEG_8 = new TChain("SelectedTree");
  POWHEG_8->Add(inputDir+"PRODFSR_8TeV/4mu/HZZ4lTree_H126.root");
  POWHEG_8->Add(inputDir+"PRODFSR_8TeV/4e/HZZ4lTree_H126.root");
  POWHEG_8->Add(inputDir+"PRODFSR_8TeV/2mu2e/HZZ4lTree_H126.root");

  TChain* POWHEG_7 = new TChain("SelectedTree");
  POWHEG_7->Add(inputDir+"PRODFSR_8TeV/4mu/HZZ4lTree_H126.root");
  POWHEG_7->Add(inputDir+"PRODFSR_8TeV/4e/HZZ4lTree_H126.root");
  POWHEG_7->Add(inputDir+"PRODFSR_8TeV/2mu2e/HZZ4lTree_H126.root");

  TChain* PS_8 = new TChain("SelectedTree");
  PS_8->Add(JHUinputDir+"JHU_8TeV/4mu/HZZ4lTree_"+sampleName[myModel]+".root");
  PS_8->Add(JHUinputDir+"JHU_8TeV/4e/HZZ4lTree_"+sampleName[myModel]+".root");
  PS_8->Add(JHUinputDir+"JHU_8TeV/2mu2e/HZZ4lTree_"+sampleName[myModel]+".root");
  TChain* PS_7 = new TChain("SelectedTree");
  PS_7->Add(JHUinputDir+"JHU_8TeV/4mu/HZZ4lTree_"+sampleName[myModel]+".root");
  PS_7->Add(JHUinputDir+"JHU_8TeV/4e/HZZ4lTree_"+sampleName[myModel]+".root");
  PS_7->Add(JHUinputDir+"JHU_8TeV/2mu2e/HZZ4lTree_"+sampleName[myModel]+".root");

  TChain* data = new TChain("SelectedTree");
  data->Add(inputDir+"PRODFSR_8TeV/data/HZZ4lTree_DoubleMu.root");
  data->Add(inputDir+"PRODFSR_8TeV/data/HZZ4lTree_DoubleOr.root");
  data->Add(inputDir+"PRODFSR_8TeV/data/HZZ4lTree_DoubleEle.root");
  data->Add(inputDir+"PRODFSR/data/HZZ4lTree_DoubleMu.root");
  data->Add(inputDir+"PRODFSR/data/HZZ4lTree_DoubleOr.root");
  data->Add(inputDir+"PRODFSR/data/HZZ4lTree_DoubleEle.root");

  data->Draw(drawString[myModel]+">>datahisto(30,0,1)",cutString);
  TH1F* datahisto = (TH1F*) gDirectory->Get("datahisto");
  
  qqZZ_7->Draw(drawString[myModel]+">>qqZZ_7histo(30,0,1)","5.051*MC_weight*("+cutString+")");
  TH1F* qqZZ_7histo = (TH1F*) gDirectory->Get("qqZZ_7histo");

  qqZZ_8->Draw(drawString[myModel]+">>qqZZ_8histo(30,0,1)","19.61*MC_weight*("+cutString+")");
  TH1F* qqZZ_8histo = (TH1F*) gDirectory->Get("qqZZ_8histo");

  POWHEG_7->Draw(drawString[myModel]+">>POWHEG_7histo(30,0,1)","5.051*MC_weight*("+cutString+")");
  TH1F* POWHEG_7histo = (TH1F*) gDirectory->Get("POWHEG_7histo");

  POWHEG_8->Draw(drawString[myModel]+">>POWHEG_8histo(30,0,1)","19.61*MC_weight*("+cutString+")");
  TH1F* POWHEG_8histo = (TH1F*) gDirectory->Get("POWHEG_8histo");

  SM_7->Draw(drawString[myModel]+">>SM_7histo(30,0,1)","5.051*MC_weight*("+cutString+")");
  TH1F* SM_7histo = (TH1F*) gDirectory->Get("SM_7histo");
  SM_7histo->Scale(POWHEG_7histo->Integral()/SM_7histo->Integral());

  SM_8->Draw(drawString[myModel]+">>SM_8histo(30,0,1)","19.61*MC_weight*("+cutString+")");
  TH1F* SM_8histo = (TH1F*) gDirectory->Get("SM_8histo");
  SM_8histo->Scale(POWHEG_8histo->Integral()/SM_8histo->Integral());

  PS_8->Draw(drawString[myModel]+">>PS_8histo(30,0,1)","19.61*MC_weight*("+cutString+")");
  TH1F* PS_8histo = (TH1F*) gDirectory->Get("PS_8histo");
  PS_8histo->Scale(POWHEG_8histo->Integral()/PS_8histo->Integral());

  PS_7->Draw(drawString[myModel]+">>PS_7histo(30,0,1)","5.051*MC_weight*("+cutString+")");
  TH1F* PS_7histo = (TH1F*) gDirectory->Get("PS_7histo");
  PS_7histo->Scale(POWHEG_7histo->Integral()/PS_7histo->Integral());
 
  //=========================
  
  //datahisto->SetBinErrorOption(TH1::kPoisson);
  datahisto->SetMarkerStyle(20);
  datahisto->SetMarkerSize(1.1);

  if(!qqZZ_7histo || !qqZZ_8histo) assert(0) ;

  TH1F* qqZZhisto = new TH1F(*qqZZ_7histo);
  qqZZhisto->Add(qqZZ_8histo);
  qqZZhisto->SetLineColor(1);
  qqZZhisto->SetLineWidth(2);
  qqZZhisto->SetFillColor(kAzure-9);

  //double with =(double) qqZZ_8->Draw("ZZMass",cutString);
  //double without = (double) qqZZ_8->Draw("ZZMass","ZZMass>106.&&ZZMass<141.");
  
  double with =(double) qqZZ_8->Draw("ZZMass","MC_weight*("+cutString+")");
  double without = (double) qqZZ_8->Draw("ZZMass","MC_weight*(ZZMass>106.&&ZZMass<141.)");

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
  
  datahisto->GetXaxis()->SetTitle(axisLabel[myModel]);
  datahisto->GetYaxis()->SetTitle("Events");
  datahisto->SetTitle("");

  THStack* stackSM = new THStack("stackSM","stackSM");
  ZXhisto->SetTitle("");
  qqZZhisto->SetTitle("");
  SMhisto->SetTitle("");
  stackSM->Add(ZXhisto);
  stackSM->Add(qqZZhisto);
  stackSM->Add(SMhisto);
  stackSM->SetTitle("");

  THStack* stackPS = new THStack("stackPS","stackPS");
  PShisto->SetTitle("");
  stackPS->Add(ZXhisto);
  stackPS->Add(qqZZhisto);
  stackPS->Add(PShisto);
  stackPS->SetTitle("");

  // ------------ draw ----------------

  datahisto->GetYaxis()->SetRangeUser(0,max(stackSM->GetMaximum(),datahisto->GetBinContent(datahisto->GetMaximumBin())+datahisto->GetBinError(datahisto->GetMaximumBin()))*1.5);
  datahisto->Draw("E1");

  //stackSM->Draw("");
  //stackSM->GetXaxis()->SetTitle(axisLabel[myModel]);
  //stackSM->GetYaxis()->SetTitle("Events");
  //stackSM->SetMaximum(stackSM->GetMaximum()*1.5);  
  
  stackSM->Draw("SAME");
  stackPS->Draw("SAME");
  datahisto->Draw("E1same");
  datahisto->Draw("SAMEp");

  // ------------ legend ---------------

  TLegend* leg = new TLegend(.2,.65,.45,.9);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);

  leg->AddEntry(datahisto,"data","p");
  leg->AddEntry(SMhisto,"0^{+}, m_{H}=126 GeV","l");
  leg->AddEntry(PShisto,legendTag[myModel]+", m_{H}=126 GeV","l");
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
  text = pt->AddText(0.3,0.6,"#sqrt{s} = 7 TeV, L = 5.1 fb^{-1}  #sqrt{s} = 8 TeV, L = 19.6 fb^{-1}");
  pt->Draw();   
  

  // ------------ save 


  if(applySuperKDcut){
    can->SetName("KD_"+saveTag[myModel]+"_Proj_superKDcut");
    can->SaveAs("KD_"+saveTag[myModel]+"_Proj_superKDcut.png");
    can->SaveAs("KD_"+saveTag[myModel]+"_Proj_superKDcut.eps");
    can->SaveAs("KD_"+saveTag[myModel]+"_Proj_superKDcut.root");
  }else{
    can->SetName("KD_"+saveTag[myModel]+"_Proj");
    can->SaveAs("KD_"+saveTag[myModel]+"_Proj.png");
    can->SaveAs("KD_"+saveTag[myModel]+"_Proj.eps");
    can->SaveAs("KD_"+saveTag[myModel]+"_Proj.root");
  }


}

void plotAll(){

  for(model i = 0 ; i<model::kNUM ; i++){

    plotKDprojections(i);

  }

}
