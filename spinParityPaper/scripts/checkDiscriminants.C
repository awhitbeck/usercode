#include <algorithm>
#include "TH1F.h"

void checkDiscriminants(){

  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();

  TChain* SMHiggs = new TChain("angles");
  TChain* psScalar = new TChain("angles");
  TChain* minGrav = new TChain("angles");
  TChain* vector = new TChain("angles");
  TChain* psVector = new TChain("angles");
  TChain* TwohPlus = new TChain("angles");
  TChain* TwohMinus = new TChain("angles");
  TChain* ZerohPlus = new TChain("angles");

  TChain* qqZZ = new TChain("angles");

  SMHiggs->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/SMHiggs_store/SMHiggs_125GeV_wResolution_withDiscriminants.root");
  psScalar->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/psScalar_store/psScalar_125GeV_wResolution_withDiscriminants.root");
  minGrav->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/minGrav_store/minGrav_125GeV_wResolution_withDiscriminants.root");
  vector->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/vector_store/vector_125GeV_wResolution_withDiscriminants.root");
  psVector->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/psVector_store/psVector_125GeV_wResolution_withDiscriminants.root");
  TwohPlus->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/2hPlus_store/2hPlus_125GeV_wResolution_withDiscriminants.root");
  TwohMinus->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/2hMinus_store/2hMinus_125GeV_wResolution_withDiscriminants.root");
  ZerohPlus->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/0hPlus_store/0hPlus_125GeV_wResolution_withDiscriminants.root");

  qqZZ->Add("/scratch0/hep/whitbeck/4lHelicity/datafiles/7TeV/training/pwgevents_*_wResolution_withDiscriminants.root");

  if(qqZZ->GetEntries()<=0) return;

  TCanvas* can2 = new TCanvas("can2","can2",1200,600);

  SMHiggs->Draw("pseudomelaLD>>pseudoMELA_model(30,0,1)","zzmass>120&&zzmass<130");
  psScalar->Draw("pseudomelaLD>>pseudoMELA_altModel(30,0,1)","zzmass>120&&zzmass<130");
  qqZZ->Draw("pseudomelaLD>>pseudoMELA_bkg(30,0,1)","zzmass>120&&zzmass<130");

  TH1F* A1 = (TH1F*) gDirectory->Get("pseudoMELA_model");
  TH1F* A2 = (TH1F*) gDirectory->Get("pseudoMELA_altModel");
  TH1F* A3 = (TH1F*) gDirectory->Get("pseudoMELA_bkg");
  A1->SetLineColor(4);  
  A2->SetLineColor(2);  
  A3->SetLineColor(kGreen+3);
  A1->GetXaxis()->SetTitle("pseudo-MELA");

  SMHiggs->Draw("gravimelaLD>>graviMELA_model(30,0,1)","zzmass>120&&zzmass<130");
  minGrav->Draw("gravimelaLD>>graviMELA_altModel(30,0,1)","zzmass>120&&zzmass<130");
  qqZZ->Draw("gravimelaLD>>graviMELA_bkg(30,0,1)","zzmass>120&&zzmass<130");

  TH1F* B1 = (TH1F*) gDirectory->Get("graviMELA_model");
  TH1F* B2 = (TH1F*) gDirectory->Get("graviMELA_altModel");
  TH1F* B3 = (TH1F*) gDirectory->Get("graviMELA_bkg");
  B1->SetLineColor(4);  
  B2->SetLineColor(2);
  B3->SetLineColor(kGreen+3);
  B1->GetXaxis()->SetTitle("gravi-MELA");

  SMHiggs->Draw("vectormelaLD>>vectorMELA_model(30,0,1)","zzmass>120&&zzmass<130");
  vector->Draw("vectormelaLD>>vectorMELA_altModel(30,0,1)","zzmass>120&&zzmass<130");
  qqZZ->Draw("vectormelaLD>>vectorMELA_bkg(30,0,1)","zzmass>120&&zzmass<130");

  TH1F* C1 = (TH1F*) gDirectory->Get("vectorMELA_model");
  TH1F* C2 = (TH1F*) gDirectory->Get("vectorMELA_altModel");
  TH1F* C3 = (TH1F*) gDirectory->Get("vectorMELA_bkg");
  C1->SetLineColor(4);  
  C2->SetLineColor(2);
  C3->SetLineColor(kGreen+3);
  C1->GetXaxis()->SetTitle("vector-MELA");

  SMHiggs->Draw("psVectormelaLD>>psVectorMELA_model(30,0,1)","zzmass>120&&zzmass<130");
  psVector->Draw("psVectormelaLD>>psVectorMELA_altModel(30,0,1)","zzmass>120&&zzmass<130");
  qqZZ->Draw("psVectormelaLD>>psVectorMELA_bkg(30,0,1)","zzmass>120&&zzmass<130");

  TH1F* D1 = (TH1F*) gDirectory->Get("psVectorMELA_model");
  TH1F* D2 = (TH1F*) gDirectory->Get("psVectorMELA_altModel");
  TH1F* D3 = (TH1F*) gDirectory->Get("psVectorMELA_bkg");
  D1->SetLineColor(4);  
  D2->SetLineColor(2);
  D3->SetLineColor(kGreen+3);
  D1->GetXaxis()->SetTitle("pseudo-vector-MELA");

  SMHiggs->Draw("2hPlusmelaLD>>2hPlusMELA_model(30,0,1)","zzmass>120&&zzmass<130");
  TwohPlus->Draw("2hPlusmelaLD>>2hPlusMELA_altModel(30,0,1)","zzmass>120&&zzmass<130");
  qqZZ->Draw("2hPlusmelaLD>>2hPlusMELA_bkg(30,0,1)","zzmass>120&&zzmass<130");

  TH1F* E1 = (TH1F*) gDirectory->Get("2hPlusMELA_model");
  TH1F* E2 = (TH1F*) gDirectory->Get("2hPlusMELA_altModel");
  TH1F* E3 = (TH1F*) gDirectory->Get("2hPlusMELA_bkg");
  E1->SetLineColor(4);  
  E2->SetLineColor(2);  
  E3->SetLineColor(kGreen+3);
  E1->GetXaxis()->SetTitle("2hPlus-MELA");

  SMHiggs->Draw("2hMinusmelaLD>>2hMinusMELA_model(30,0,1)","zzmass>120&&zzmass<130");
  TwohMinus->Draw("2hMinusmelaLD>>2hMinusMELA_altModel(30,0,1)","zzmass>120&&zzmass<130");
  qqZZ->Draw("2hMinusmelaLD>>2hMinusMELA_bkg(30,0,1)","zzmass>120&&zzmass<130");

  TH1F* F1 = (TH1F*) gDirectory->Get("2hMinusMELA_model");
  TH1F* F2 = (TH1F*) gDirectory->Get("2hMinusMELA_altModel");
  TH1F* F3 = (TH1F*) gDirectory->Get("2hMinusMELA_bkg");
  F1->SetLineColor(4);  
  F2->SetLineColor(2);  
  F3->SetLineColor(kGreen+3);
  F1->GetXaxis()->SetTitle("2hMinus-MELA");

  SMHiggs->Draw("0hPlusmelaLD>>0hPlusMELA_model(30,0,1)","zzmass>120&&zzmass<130");
  ZerohPlus->Draw("0hPlusmelaLD>>0hPlusMELA_altModel(30,0,1)","zzmass>120&&zzmass<130");
  qqZZ->Draw("0hPlusmelaLD>>0hPlusMELA_bkg(30,0,1)","zzmass>120&&zzmass<130");

  TH1F* G1 = (TH1F*) gDirectory->Get("0hPlusMELA_model");
  TH1F* G2 = (TH1F*) gDirectory->Get("0hPlusMELA_altModel");
  TH1F* G3 = (TH1F*) gDirectory->Get("0hPlusMELA_bkg");
  G1->SetLineColor(4);  
  G2->SetLineColor(2);  
  G3->SetLineColor(kGreen+3);
  G1->GetXaxis()->SetTitle("0hPlus-MELA");

  delete can2; 

  TCanvas* can = new TCanvas("can","can",1200,600);

  can->Divide(4,2);

  cout << " Drawing A's " << endl;

  can->cd(1);
  A1->Scale(1/A1->Integral());
  A2->Scale(1/A2->Integral());
  A3->Scale(1/A3->Integral());
  A1->GetYaxis()->SetRangeUser(0,max(A1->GetMaximum(),A2->GetMaximum())*1.3);
  A1->Draw();
  A2->Draw("SAME");
  A3->Draw("SAME");

  cout << " Drawing B's " << endl;

  can->cd(2);
  B1->Scale(1/B1->Integral());
  B2->Scale(1/B2->Integral());
  B3->Scale(1/B3->Integral());
  B1->GetYaxis()->SetRangeUser(0,max(B1->GetMaximum(),B2->GetMaximum())*1.3);
  B1->Draw();
  B2->Draw("SAME");
  B3->Draw("SAME");
 
  cout << " Drawing C's " << endl;

  can->cd(3);
  C1->Scale(1/C1->Integral());
  C2->Scale(1/C2->Integral());
  C3->Scale(1/C3->Integral());
  C1->GetYaxis()->SetRangeUser(0,max(C1->GetMaximum(),C2->GetMaximum())*1.3);
  C1->Draw();
  C2->Draw("SAME");
  C3->Draw("SAME");

  cout << " Drawing D's " << endl;

  can->cd(4);
  D1->Scale(1/D1->Integral());
  D2->Scale(1/D2->Integral());
  D3->Scale(1/D3->Integral());
  D1->GetYaxis()->SetRangeUser(0,max(D1->GetMaximum(),D2->GetMaximum())*1.3);
  D1->Draw();
  D2->Draw("SAME");
  D3->Draw("SAME");

  can->cd(5);
  E1->Scale(1/E1->Integral());
  E2->Scale(1/E2->Integral());
  E3->Scale(1/E3->Integral());
  E1->GetYaxis()->SetRangeUser(0,max(D1->GetMaximum(),E2->GetMaximum())*1.3);
  E1->Draw();
  E2->Draw("SAME");
  E3->Draw("SAME");

  can->cd(6);
  F1->Scale(1/F1->Integral());
  F2->Scale(1/F2->Integral());
  F3->Scale(1/F3->Integral());
  F1->GetYaxis()->SetRangeUser(0,max(D1->GetMaximum(),F2->GetMaximum())*1.3);
  F1->Draw();
  F2->Draw("SAME");
  F3->Draw("SAME");

  can->cd(7);
  G1->Scale(1/G1->Integral());
  G2->Scale(1/G2->Integral());
  G3->Scale(1/G3->Integral());
  G1->GetYaxis()->SetRangeUser(0,max(D1->GetMaximum(),G2->GetMaximum())*1.3);
  G1->Draw();
  G2->Draw("SAME");
  G3->Draw("SAME");

  TLegend* leg = new TLegend(.2,.2,.9,.9);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->AddEntry(G1,"SMHiggs","l");
  leg->AddEntry(G2,"Alternative Model","l");
  leg->AddEntry(G3,"qqZZ","l");

  can->cd(8);
  leg->Draw();

}


/*
addDtoTree("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/psScalar_store/psScalar_125GeV_wResolution"); addDtoTree("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/vector_store/vector_125GeV_wResolution"); addDtoTree("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/psVector_store/psVector_125GeV_wResolution"); addDtoTree("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/minGrav_store/minGrav_125GeV_wResolution"); addDtoTree("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/SMHiggs_store/SMHiggs_125GeV_wResolution");
*/
