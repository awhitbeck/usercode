#include <algorithm>

void melaDistributions_2D(int index=0,int altModelColor=4){

  gROOT->ProcessLine(".L  ~/tdrstyle.C");
  setTDRStyle();

  TChain* SMHiggs = new TChain("angles");
  TChain* altModel = new TChain("angles");

  TChain* qqZZ = new TChain("angles");

  SMHiggs->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/SMHiggs_store/SMHiggs_125GeV_wResolution_withDiscriminants.root");
  
  string fileName[7]={
    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/psScalar_store/psScalar_125GeV_wResolution_withDiscriminants.root",
    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/minGrav_store/minGrav_125GeV_wResolution_withDiscriminants.root",
    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/vector_store/vector_125GeV_wResolution_withDiscriminants.root",
    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/psVector_store/psVector_125GeV_wResolution_withDiscriminants.root",
    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/2hPlus_store/2hPlus_125GeV_wResolution_withDiscriminants.root",
    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/2hMinus_store/2hMinus_125GeV_wResolution_withDiscriminants.root",
    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/0hPlus_store/0hPlus_125GeV_wResolution_withDiscriminants.root"
  };

  string discrimName[7]={"pseudomelaLD","gravimelaLD","vectormelaLD","psVectormelaLD","2hPlusmelaLD","2hMinusmelaLD","0hPlusmelaLD"};

  string axisTitle[7]={"D_{0^{-}}","D_{2_{m}^{+}}","D_{1^{+}}","D_{1^{-}}","D_{2_{h}^{+}}","D_{2_{h}^{-}}","D_{0_{h}^{+}}"};

  altModel->Add( fileName[index].c_str() );

  qqZZ->Add("/scratch0/hep/whitbeck/4lHelicity/datafiles/7TeV/training/pwgevents_*_wResolution_withDiscriminants.root");

  char drawString[100];

  gStyle->SetPadRightMargin(0.12);

  TCanvas* qqZZcan = new TCanvas("qqZZcan","qqZZcan",600,600);
  TCanvas* SMHcan = new TCanvas("SMHcan","SMHcan",600,600);
  TCanvas* altModelcan = new TCanvas("altModelcan","altModelcan",600,600);

  char* binning;
  char* cutString;
  binning ="(30,110,140,20,0,1)";
  cutString="zzmass>110&&zzmass<140";
  
  sprintf(drawString,"%s:zzmass>>qqZZhisto%s",discrimName[index].c_str(),binning);
  qqZZ->Draw(drawString,cutString);

  sprintf(drawString,"%s:zzmass>>SMHhisto%s",discrimName[index].c_str(),binning);
  SMHiggs->Draw(drawString,cutString);

  sprintf(drawString,"%s:zzmass>>altModelhisto%s",discrimName[index].c_str(),binning);
  altModel->Draw(drawString,cutString);

  TH1F* qqZZhisto = (TH1F*) gDirectory->Get("qqZZhisto");
  qqZZhisto->Scale(1./qqZZhisto->GetMaximum());
  qqZZhisto->GetYaxis()->SetTitle( axisTitle[index].c_str() );
  qqZZhisto->GetXaxis()->SetTitle( "m_{4l}" );
  qqZZhisto->GetYaxis()->CenterTitle();
  qqZZhisto->GetXaxis()->SetNdivisions(-505);
  qqZZhisto->GetYaxis()->SetNdivisions(-505);
  qqZZhisto->GetZaxis()->SetNdivisions(-505);
  qqZZhisto->GetXaxis()->CenterTitle();
  
  TGaxis::SetMaxDigits(3);

  TH1F* SMHhisto = (TH1F*) gDirectory->Get("SMHhisto");
  SMHhisto->Scale(1./SMHhisto->GetMaximum());
  SMHhisto->GetYaxis()->SetTitle( axisTitle[index].c_str() );
  SMHhisto->GetXaxis()->SetTitle( "m_{4l}" );
  SMHhisto->GetYaxis()->CenterTitle();
  SMHhisto->GetXaxis()->SetNdivisions(-505);
  SMHhisto->GetYaxis()->SetNdivisions(-505);
  SMHhisto->GetZaxis()->SetNdivisions(-505);
  SMHhisto->GetXaxis()->CenterTitle();


  TH1F* altModelhisto = (TH1F*) gDirectory->Get("altModelhisto");
  altModelhisto->Scale(1./altModelhisto->GetMaximum());
  altModelhisto->GetYaxis()->SetTitle( axisTitle[index].c_str() );
  altModelhisto->GetXaxis()->SetTitle( "m_{4l}" );
  altModelhisto->GetYaxis()->CenterTitle();
  altModelhisto->GetXaxis()->SetNdivisions(-505);
  altModelhisto->GetYaxis()->SetNdivisions(-505);
  altModelhisto->GetZaxis()->SetNdivisions(-505);
  altModelhisto->GetXaxis()->CenterTitle();

  qqZZcan->cd();
  qqZZhisto->Draw("COLZ");

  SMHcan->cd();
  SMHhisto->Draw("COLZ");

  altModelcan->cd();
  altModelhisto->Draw("COLZ");

  char saveName[100];

  sprintf(saveName,"epsfiles/kinematics_2D_%s_qqZZ.eps",discrimName[index].c_str() );
  qqZZcan->SaveAs(saveName);
  sprintf(saveName,"pngfiles/kinematics_2D_%s_qqZZ.png",discrimName[index].c_str() );
  qqZZcan->SaveAs(saveName);

  sprintf(saveName,"epsfiles/kinematics_2D_%s_SMH.eps",discrimName[index].c_str() );
  SMHcan->SaveAs(saveName);
  sprintf(saveName,"pngfiles/kinematics_2D_%s_SMH.png",discrimName[index].c_str() );
  SMHcan->SaveAs(saveName);

  sprintf(saveName,"epsfiles/kinematics_2D_%s_altModel.eps",discrimName[index].c_str() );
  altModelcan->SaveAs(saveName);
  sprintf(saveName,"pngfiles/kinematics_2D_%s_altModel.png",discrimName[index].c_str() );
  altModelcan->SaveAs(saveName);

}
