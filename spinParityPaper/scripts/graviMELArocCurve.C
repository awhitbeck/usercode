




TGraph* makeROCcurve(char* drawVar="gravimelaLD", 
		     const int bins=30, float start=0, float end=1,
		     int lineColor=1, int lineStyle=1, int lineWidth=2){

  TChain* SMHtree = new TChain("angles");
  SMHtree->Add("../datafiles/SMHiggs_125_JHU_v3_wResolution_withDiscriminants_withDiscriminants.root");

  TChain* PStree = new TChain("angles");
  PStree->Add("../datafiles/7T_125G_4l_wResolution_withDiscriminants_withDiscriminants.root");
  
  TH1F *SMHhisto, *PShisto;
  
  char drawString[150];

  sprintf(drawString,"%s>>SMHhisto(%i,%f,%f)",drawVar,bins,start,end);
  SMHtree->Draw(drawString,"(zzmass>100)");
  sprintf(drawString,"%s>>PShisto(%i,%f,%f)",drawVar,bins,start,end);
  PStree->Draw(drawString,"(zzmass>100)");
  
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


void graviMELArocCurve(){


  TGraph* andrewMELA = makeROCcurve("gravimelaLD",30,0,1,              1,1);
  TGraph* saraMELA = makeROCcurve("melaLD",30,0,1,              2,2);

  //TGraph* z1mass = makeROCcurve("Z1Mass",100,60,120,                  2,2);
  //TGraph* z2mass = makeROCcurve("-Z2Mass",100,-80,-12,                3,3);
  TGraph* costheta1 = makeROCcurve("-abs(costheta1)",30,-1,0,     4,4);
  TGraph* costheta2 = makeROCcurve("-abs(costheta2)",30,-1,0,     6,6);
  TGraph* costhetastar=makeROCcurve("abs(costhetastar)",30,0,1,     7,7);
  TGraph* phi=makeROCcurve("abs(abs(phi)-3.1415/2.)",30,0,2,       2,1);
  TGraph* phistar1=makeROCcurve("-abs(abs(phistar1)-3.1415/2.)",30,-2,0,       4,1);
  

  andrewMELA->Draw("AC");
  saraMELA->Draw("SAMEC");

  
  //z1mass->Draw("sameC");
  //z2mass->Draw("sameC");
  costheta1->Draw("sameC");
  costheta2->Draw("sameC");
  costhetastar->Draw("sameC");
  phi->Draw("sameC");  
  phistar1->Draw("sameC");
  

  TLegend* leg = new TLegend(.7,.7,.9,.9);
  leg->SetFillColor(0);
  
  leg->AddEntry(andrewMELA,"gravi-MELA (Andrew)","l");
  leg->AddEntry(saraMELA,"gravi-MELA (Sara)","l");

  
  //leg->AddEntry(z1mass,"m_{Z1}","l");
  //leg->AddEntry(z2mass,"m_{Z2}","l");
  leg->AddEntry(costheta1,"cos#theta_{1}","l");
  leg->AddEntry(costheta2,"cos#theta_{2}","l");
  leg->AddEntry(costhetastar,"cos#theta^{*}","l");
  leg->AddEntry(phi,"#Phi","l");
  leg->AddEntry(phistar1,"#Phi^{*}","l");
  

  leg->Draw();

}
