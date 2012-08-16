



void getTailSignif(char *fileName="", char* treeName="hypTuple"){

  TChain* t = new TChain(treeName);
  
  t->Add(fileName);
  
  TH1F* S_H0, *S_H1, *SigEst; 
  
  TCanvas* can = new TCanvas("can","can",600,600);

  t->Draw("S_H0>>S_H0(10000,0,100)");
  t->Draw("S_H1>>S_H1(10000,0,100)");
  t->Draw("sqrt(S_H1)>>SigEst(1000,0,100)");

  S_H0 = (TH1F*) gDirectory->Get("S_H0");
  S_H1 = (TH1F*) gDirectory->Get("S_H1");  
  SigEst = (TH1F*) gDirectory->Get("SigEst");
  
  int i=1;
  double frac=0;

  while(frac<.50000){

    if(i>1000000) break;
    frac = S_H1->Integral(0,i)/S_H1->Integral(0,1000000);
    i++;

  }

  double average=0;
  
  for(int j=1; j<=1000; j++){
    average+=SigEst->GetBinCenter(j)*SigEst->Integral(j,j);
  }

  average = average / SigEst->Integral();

  cout << "Sanity check, frac: " << frac << endl;
  cout << "========================== " << endl;
  double Median = S_H1->GetBinCenter(i);
  cout << "Median of H1: " << S_H1->GetBinCenter(i) << endl;
  cout << "Number of H0 toys beyond H1 median: " << S_H0->Integral(i,1000000) << endl;
  cout << "Fraction of H0 toys beyond H1 median: " << S_H0->Integral(i,1000000)/S_H0->Integral(0,1000000) << endl;
  cout << "Significance: " << ROOT::Math::normal_quantile_c(S_H0->Integral(i,1000000)/S_H0->Integral(0,1000000),1.0) << endl;
  cout << "Mean Significance: " << average << endl;


  S_H0->Rebin(100);
  S_H1->Rebin(100);
  S_H1->SetLineColor(4);
  S_H1->Draw("");
  S_H0->Draw("SAME");

  TLine* line = new TLine(Median,0,Median,S_H1->GetMaximum());
  line->SetLineColor(2);
  line->Draw("SAME");


}
