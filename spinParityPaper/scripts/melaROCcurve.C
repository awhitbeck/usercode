




TGraph* makeROCcurve(char* drawVar="gravimelaLD", char* fileTag="minGrav", 
		     const int bins=30, float start=0, float end=1,
		     int lineColor=1, int lineStyle=1, int lineWidth=2){

  gROOT->ProcessLine(".L  ~/tdrstyle.C");
  setTDRStyle();

  TChain* SMHtree = new TChain("angles");
  SMHtree->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/SMHiggs_store/SMHiggs_125GeV_wResolution_withDiscriminants.root");

  TChain* PStree = new TChain("angles");

  char fileName[150];
  sprintf(fileName,"/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/%s_store/%s_125GeV_wResolution_withDiscriminants.root",fileTag,fileTag);
  PStree->Add(fileName);
  
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
  ROC->GetXaxis()->SetTitle("#epsilon_{sig}");
  ROC->GetYaxis()->SetTitle("#epsilon_{alt sig}");
  delete SMHtree;
  delete PStree;

  return ROC;

}


void melaROCcurve(char* varName="gravimelaLD",char* fileTag="minGrav",
		  char* legendName="MELA"){

  TCanvas* can  = new TCanvas("can","can",400,400);


  TGraph* andrewMELA = makeROCcurve(varName,fileTag,30,0,1,              1,1);
  TGraph* z1mass = makeROCcurve("z1mass",fileTag,100,60,120,                  2,2);
  TGraph* z2mass = makeROCcurve("-z2mass",fileTag,100,-80,-12,                3,3);
  TGraph* costheta1 = makeROCcurve("-abs(costheta1)",fileTag,30,-1,0,     4,4);
  TGraph* costheta2 = makeROCcurve("-abs(costheta2)",fileTag,30,-1,0,     6,6);
  TGraph* costhetastar=makeROCcurve("abs(costhetastar)",fileTag,30,0,1,     7,7);
  TGraph* phi=makeROCcurve("abs(abs(phi)-3.1415/2.)",fileTag,30,0,2,       2,1);
  TGraph* phistar1=makeROCcurve("-abs(abs(phistar1)-3.1415/2.)",fileTag,30,-2,0,       4,1);
  

  andrewMELA->Draw("AC");
  
  z1mass->Draw("sameC");
  z2mass->Draw("sameC");
  costheta1->Draw("sameC");
  costheta2->Draw("sameC");
  costhetastar->Draw("sameC");
  phi->Draw("sameC");  
  phistar1->Draw("sameC");
  

  TLegend* leg = new TLegend(.2,.6,.5,.9);
  leg->SetFillColor(0);
  
  leg->AddEntry(andrewMELA,legendName,"l");
    
  leg->AddEntry(z1mass,"m_{Z1}","l");
  leg->AddEntry(z2mass,"m_{Z2}","l");
  leg->AddEntry(costheta1,"cos#theta_{1}","l");
  leg->AddEntry(costheta2,"cos#theta_{2}","l");
  leg->AddEntry(costhetastar,"cos#theta^{*}","l");
  leg->AddEntry(phi,"#Phi","l");
  leg->AddEntry(phistar1,"#Phi^{*}","l");
  

  leg->Draw();

  char saveName[100];
  sprintf(saveName,"epsfiles/%s_ROCcurve.eps",varName);
  can->SaveAs(saveName);
  sprintf(saveName,"pngfiles/%s_ROCcurve.png",varName);
  can->SaveAs(saveName);

}
