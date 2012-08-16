

void compareMixedCPstates(int percentEven=50){
  
  gROOT->ProcessLine(".L  ~/tdrstyle.C");
  setTDRStyle();
  
  TCanvas* can = new TCanvas("can","can",600,600);

  TChain* zero = new TChain("angles");  
  TChain* oneHund = new TChain("angles");
  TChain* x = new TChain("angles");

  oneHund->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/SMHiggs_store/SMHiggs_125GeV_withDiscriminants.root");

  zero->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/psScalar_store/psScalar_125GeV_withDiscriminants.root");

  char fileName[100];
  sprintf(fileName,"../datafiles/mixCPstates/test125_CPeven%iprcnt_wResolution_withDiscriminants.root",percentEven);
  x->Add(fileName);

  if(zero->GetEntries()<=0) cout << "0% file error " << endl;  
  if(oneHund->GetEntries()<=0) cout << "100% file error " << endl;
  if(x->GetEntries()<=0) cout << "x% file error " << endl;

  zero->Draw("pseudomelaLD>>zeroHist(30,0,1)");
  oneHund->Draw("pseudomelaLD>>oneHundHist(30,0,1)");
  x->Draw("pseudomelaLD>>xHist(30,0,1)");

  TH1F* zeroHist = (TH1F*) gDirectory->Get("zeroHist");
  zeroHist->Scale(1/zeroHist->Integral());
  zeroHist->SetLineColor(2);
  zeroHist->SetLineWidth(2);
  zeroHist->GetXaxis()->SetTitle("pseudo-MELA");  
  zeroHist->GetXaxis()->SetNdivisions(-505);  
  zeroHist->GetXaxis()->CenterTitle();
  zeroHist->GetYaxis()->SetTitle("normalized to unity");
  //zeroHist->GetYaxis()->SetNdivisions(-505);  
  zeroHist->GetYaxis()->CenterTitle();

  TH1F* oneHundHist = (TH1F*) gDirectory->Get("oneHundHist");
  oneHundHist->Scale(1/oneHundHist->Integral());
  oneHundHist->SetLineColor(4);
  oneHundHist->SetLineWidth(2);

  TH1F* noIntrfSum = new TH1F(*oneHundHist);
  noIntrfSum->Scale((double)percentEven/100.);
  noIntrfSum->Add(zeroHist,1.-(double)percentEven/100.);
  noIntrfSum->SetLineColor(kMagenta+2);
  noIntrfSum->SetLineStyle(2);
  noIntrfSum->SetLineWidth(2);

  TH1F* xHist = (TH1F*) gDirectory->Get("xHist");
  xHist->Scale(1/xHist->Integral());
  xHist->SetLineColor(kGreen+2);
  xHist->SetLineWidth(2);
  
  zeroHist->Draw("");
  oneHundHist->Draw("SAME");
  xHist->Draw("SAME");
  noIntrfSum->Draw("SAME");

  TLegend* leg = new TLegend(.7,.7,.99,.99);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->AddEntry(oneHundHist,"SM Higgs","l");
  leg->AddEntry(zeroHist,"pseudo-scalar","l");
  sprintf(fileName,"mixed - %i % (w/ intf)",percentEven);
  leg->AddEntry(xHist,fileName,"l");
  sprintf(fileName,"mixed - %i % (w/o intf)",percentEven);
  leg->AddEntry(noIntrfSum,fileName,"l");
  
  leg->Draw();

  sprintf(fileName,"epsfiles/mixedCPstateComparison_%ipercent.eps",percentEven);
  can->SaveAs(fileName);
  sprintf(fileName,"pngfiles/mixedCPstateComparison_%ipercent.png",percentEven);
  can->SaveAs(fileName);

}
