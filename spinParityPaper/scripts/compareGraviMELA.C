


compareGraviMELA(){

  TFile* gravFile = new TFile("../datafiles/7T_125G_4l_wResolution_withDiscriminants_withDiscriminants.root");
  TFile* higgsFile = new TFile("../datafiles/SMHiggs_125_JHU_v3_wResolution_withDiscriminants_withDiscriminants.root");

  TTree* grav = (TTree*) gravFile->Get("angles");
  TTree* higgs = (TTree*) higgsFile->Get("angles");

  TCanvas* can =new TCanvas("can","can",600,600);

  grav->Draw("melaLD>>sara_grav(30,0,1)");
  grav->Draw("gravimelaLD>>drew_grav(30,0,1)");

  higgs->Draw("melaLD>>sara_higgs(30,0,1)");
  higgs->Draw("gravimelaLD>>drew_higgs(30,0,1)");

  sara_grav->SetLineColor(4);
  sara_grav->SetLineWidth(2);
  sara_grav->SetLineStyle(2);

  sara_higgs->SetLineColor(2);
  sara_higgs->SetLineWidth(2);
  sara_higgs->SetLineStyle(2);

  drew_grav->SetLineColor(4);
  drew_grav->SetLineWidth(2);
  drew_grav->SetLineStyle(1);

  drew_higgs->SetLineColor(2);
  drew_higgs->SetLineWidth(2);
  drew_higgs->SetLineStyle(1);

  sara_grav->DrawNormalized();
  sara_higgs->DrawNormalized("SAME");

  drew_grav->DrawNormalized("SAME");
  drew_higgs->DrawNormalized("SAME");

  TLegend* leg = new TLegend(.5,.5,.8,.8);

  leg->SetFillColor(0);
  
  leg->AddEntry(drew_grav,"2_{m}^{+} - analytic","l");
  leg->AddEntry(drew_higgs,"0_{m}^{+} - analytic","l");
  leg->AddEntry(sara_grav,"2_{m}^{+} - template","l");
  leg->AddEntry(sara_higgs,"0_{m}^{+} - template","l");

  leg->Draw();

}


