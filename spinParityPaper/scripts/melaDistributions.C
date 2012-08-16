#include <algorithm>

void melaDistributions(int index=0,int altModelColor=4){

  gROOT->ProcessLine(".L  ~/tdrstyle.C");
  setTDRStyle();

  TChain* SMHiggs = new TChain("angles");
  TChain* altModel = new TChain("angles");
  TChain* minGrav = new TChain("angles");
  TChain* psScalar = new TChain("angles");
  TChain* TwohPlus = new TChain("angles");

  TChain* qqZZ = new TChain("angles");

  SMHiggs->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/SMHiggs_store/SMHiggs_125GeV_wResolution_withDiscriminants.root");
  
  string fileName[9]={
    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/psScalar_store/psScalar_125GeV_wResolution_withDiscriminants.root",
    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/psScalar_store/psScalar_125GeV_wResolution_withDiscriminants.root",
    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/psScalar_store/psScalar_125GeV_wResolution_withDiscriminants.root",
    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/minGrav_store/minGrav_125GeV_wResolution_withDiscriminants.root",
    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/vector_store/vector_125GeV_wResolution_withDiscriminants.root",
    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/psVector_store/psVector_125GeV_wResolution_withDiscriminants.root",
    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/2hPlus_store/2hPlus_125GeV_wResolution_withDiscriminants.root",
    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/2hMinus_store/2hMinus_125GeV_wResolution_withDiscriminants.root",
    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/0hPlus_store/0hPlus_125GeV_wResolution_withDiscriminants.root"
  };

  string discrimName[9]={"zzmass","melaLD","pseudomelaLD","gravimelaLD","vectormelaLD","psVectormelaLD","2hPlusmelaLD","2hMinusmelaLD","0hPlusmelaLD"};

  string axisTitle[9]={"m_{4l}","D_{bkg}","D_{0^{-}}","D_{2_{m}^{+}}","D_{1^{+}}","D_{1^{-}}","D_{2_{h}^{+}}","D_{2_{h}^{-}}","D_{0_{h}^{+}}"};

  altModel->Add( fileName[index].c_str() );
  minGrav->Add( fileName[3].c_str() );
  psScalar->Add( fileName[0].c_str() );
  TwohPlus->Add( fileName[6].c_str() );

  qqZZ->Add("/scratch0/hep/whitbeck/4lHelicity/datafiles/7TeV/training/pwgevents_*_wResolution_withDiscriminants.root");

  char drawString[100];

  TCanvas* can = new TCanvas("can","can",600,600);
    
  char* binning;
  char* cutString;

  if(index==0){
    binning ="(40,110,140)";
    cutString="";
  }else{
    binning ="(20,0,1)";
    cutString="zzmass>120&&zzmass<130";
  }
  
  sprintf(drawString,"%s>>qqZZhisto%s",discrimName[index].c_str(),binning);
  qqZZ->Draw(drawString,cutString);

  sprintf(drawString,"%s>>SMHhisto%s",discrimName[index].c_str(),binning);
  SMHiggs->Draw(drawString,cutString);

  sprintf(drawString,"%s>>altModelhisto%s",discrimName[index].c_str(),binning);
  altModel->Draw(drawString,cutString);

  sprintf(drawString,"%s>>minGravhisto%s",discrimName[index].c_str(),binning);
  minGrav->Draw(drawString,cutString);

  sprintf(drawString,"%s>>psScalarhisto%s",discrimName[index].c_str(),binning);
  psScalar->Draw(drawString,cutString);

  sprintf(drawString,"%s>>TwohPlushisto%s",discrimName[index].c_str(),binning);
  TwohPlus->Draw(drawString,cutString);


  TH1F* qqZZhisto = (TH1F*) gDirectory->Get("qqZZhisto");
  qqZZhisto->Scale(1./qqZZhisto->Integral());
  qqZZhisto->GetXaxis()->SetTitle( axisTitle[index].c_str() );
  qqZZhisto->GetYaxis()->SetTitle( "" );
  qqZZhisto->GetYaxis()->CenterTitle();
  qqZZhisto->GetXaxis()->SetNdivisions(-505);
  qqZZhisto->GetYaxis()->SetNdivisions(-505);
  qqZZhisto->GetXaxis()->CenterTitle();
  qqZZhisto->SetLineColor(1);
  qqZZhisto->SetLineWidth(2);
  qqZZhisto->SetMarkerStyle(8);
  qqZZhisto->SetMarkerColor(1);
  qqZZhisto->SetMarkerSize(1.0);
  
  TGaxis::SetMaxDigits(3);

  TH1F* SMHhisto = (TH1F*) gDirectory->Get("SMHhisto");
  SMHhisto->Scale(1./SMHhisto->Integral());
  SMHhisto->SetLineColor(2);
  SMHhisto->SetLineWidth(2);
  SMHhisto->SetLineStyle(1);
  SMHhisto->SetMarkerStyle(4);
  SMHhisto->SetMarkerColor(2);
  SMHhisto->SetMarkerSize(1.0);

  TH1F* altModelhisto = (TH1F*) gDirectory->Get("altModelhisto");
  altModelhisto->Scale(1./altModelhisto->Integral());
  altModelhisto->SetLineColor(altModelColor);
  altModelhisto->SetLineWidth(2);
  altModelhisto->SetLineStyle(1);
  altModelhisto->SetMarkerStyle(32);
  altModelhisto->SetMarkerColor(altModelColor);
  altModelhisto->SetMarkerSize(1.2);

  TH1F* minGravhisto = (TH1F*) gDirectory->Get("minGravhisto");
  minGravhisto->Scale(1./minGravhisto->Integral());
  minGravhisto->SetLineColor(kBlue);
  minGravhisto->SetLineWidth(2);
  minGravhisto->SetLineStyle(1);
  minGravhisto->SetMarkerStyle(32);
  minGravhisto->SetMarkerColor(kBlue);
  minGravhisto->SetMarkerSize(1.2);

  TH1F* psScalarhisto = (TH1F*) gDirectory->Get("psScalarhisto");
  psScalarhisto->Scale(1./psScalarhisto->Integral());
  psScalarhisto->SetLineColor(kMagenta+1);
  psScalarhisto->SetLineWidth(2);
  psScalarhisto->SetLineStyle(1);
  psScalarhisto->SetMarkerStyle(25);
  psScalarhisto->SetMarkerColor(kMagenta+1);
  psScalarhisto->SetMarkerSize(1.2);

  TH1F* TwohPlushisto = (TH1F*) gDirectory->Get("TwohPlushisto");
  TwohPlushisto->Scale(1./TwohPlushisto->Integral());
  TwohPlushisto->GetXaxis()->SetTitle( axisTitle[index].c_str() );
  TwohPlushisto->GetYaxis()->SetTitle( "" );
  TwohPlushisto->GetYaxis()->CenterTitle();
  TwohPlushisto->GetXaxis()->SetNdivisions(-505);
  TwohPlushisto->GetYaxis()->SetNdivisions(-505);
  TwohPlushisto->GetXaxis()->CenterTitle();
  TwohPlushisto->SetLineColor(kGreen+3);
  TwohPlushisto->SetLineWidth(2);
  TwohPlushisto->SetLineStyle(1);
  TwohPlushisto->SetMarkerStyle(27);
  TwohPlushisto->SetMarkerColor(kGreen+3);
  TwohPlushisto->SetMarkerSize(1.2);

  qqZZhisto->GetYaxis()->SetRangeUser(0, 1.2*max( max(altModelhisto->GetMaximum(),SMHhisto->GetMaximum()) , qqZZhisto->GetMaximum()  ) );
  
  if(index!=1){
    qqZZhisto->Draw("hist");
    qqZZhisto->Draw("sameP");
    SMHhisto->Draw("sameHIST");
    SMHhisto->Draw("sameP");
    if(index>1){
      altModelhisto->Draw("sameHIST");
      altModelhisto->Draw("sameP");
    }
    
  }else{
    qqZZhisto->Draw("hist");
    qqZZhisto->Draw("sameP");

    TwohPlushisto->Draw("SAMEhist");
    TwohPlushisto->Draw("sameP");
    psScalarhisto->Draw("SAMEhist");
    psScalarhisto->Draw("sameP");
    minGravhisto->Draw("SAMEhist");
    minGravhisto->Draw("sameP");

    SMHhisto->Draw("SAMEhist");
    SMHhisto->Draw("sameP");


  }

  char saveName[100];
  sprintf(saveName,"epsfiles/kinematics_1D_%s.eps",discrimName[index].c_str());
  can->SaveAs(saveName);
  sprintf(saveName,"pngfiles/kinematics_1D_%s.png",discrimName[index].c_str());
  can->SaveAs(saveName);

}
