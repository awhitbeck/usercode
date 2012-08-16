




TGraph* makeROCcurve(char* drawVar="ZZpseudoLD", 
		     const int bins=30, float start=0, float end=1,
		     int lineColor=1, int lineStyle=1, int lineWidth=2){

  TChain* SMHtree = new TChain("SelectedTree");
  SMHtree->Add("CJLSTtrees_June21_2012/7plus8TeV_FSR/HZZ*Tree_H125_*TeV.root");

  TChain* PStree = new TChain("SelectedTree");
  PStree->Add("CJLSTtree_Jun25_2012/JHUsignal/HZZ*Tree_H125jhuPse.root");
  
  TH1F *SMHhisto, *PShisto;
  
  char drawString[150];

  sprintf(drawString,"%s>>SMHhisto(%i,%f,%f)",drawVar,bins,start,end);
  SMHtree->Draw(drawString,"MC_weight*(ZZMass>100&&ZZLD>.5)");
  sprintf(drawString,"%s>>PShisto(%i,%f,%f)",drawVar,bins,start,end);
  PStree->Draw(drawString,"MC_weight*(ZZMass>100&&ZZLD>.5)");
  
  SMHhisto = (TH1F*) gDirectory->Get("SMHhisto");
  SMHhisto->Scale(1/SMHhisto->Integral());
  PShisto = (TH1F*) gDirectory->Get("PShisto");
  PShisto->Scale(1/PShisto->Integral());

  double effSMH[bins],effPS[bins];

  for(int i=0; i<bins; i++){

    effSMH[i] = SMHhisto->Integral(i+1, bins);
    effPS[i] = PShisto->Integral(i+1, bins);

  }

  TGraph* ROC = new TGraph(bins,effSMH,effPS);
  ROC->SetLineColor(lineColor);
  ROC->SetLineStyle(lineStyle);
  ROC->SetLineWidth(lineWidth);
  ROC->GetXaxis()->SetTitle("#epsilon_{0^{+}}");
  ROC->GetYaxis()->SetTitle("#epsilon_{0^{-}}");
  delete SMHtree;
  delete PStree;

  return ROC;

}


void pseudoMELArocCurve(){


  TGraph* pseudoMELA = makeROCcurve("ZZpseudoLD",30,0,1,              1,1);
  TGraph* z1mass = makeROCcurve("Z1Mass",100,60,120,                  2,2);
  TGraph* z2mass = makeROCcurve("-Z2Mass",100,-80,-12,                3,3);
  TGraph* costheta1 = makeROCcurve("-abs(helcosthetaZ1)",30,-1,0,     4,4);
  TGraph* costheta2 = makeROCcurve("-abs(helcosthetaZ2)",30,-1,0,     6,6);
  TGraph* costhetastar=makeROCcurve("-abs(costhetastar)",30,-1,0,     7,7);
  TGraph* phi=makeROCcurve("abs(abs(helphi)-3.1415/2.)",30,0,2,       1,2);
  
  pseudoMELA->Draw("AC");
  z1mass->Draw("sameC");
  z2mass->Draw("sameC");
  costheta1->Draw("sameC");
  costheta2->Draw("sameC");
  costhetastar->Draw("sameC");
  phi->Draw("sameC");

  TLegend* leg = new TLegend(.7,.7,.9,.9);
  leg->SetFillColor(0);
  
  leg->AddEntry(pseudoMELA,"pseudo-MELA","l");
  leg->AddEntry(z1mass,"m_{Z1}","l");
  leg->AddEntry(z2mass,"m_{Z2}","l");
  leg->AddEntry(costheta1,"cos#theta_{1}","l");
  leg->AddEntry(costheta2,"cos#theta_{2}","l");
  leg->AddEntry(costhetastar,"cos#theta^{*}","l");
  leg->AddEntry(phi,"#Phi","l");

  leg->Draw();

}
