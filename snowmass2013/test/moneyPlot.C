#include "../src/fa3Converter.cc"
#include <vector>

using namespace std;

void moneyPlot(){

  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();
  
  gStyle->SetOptStat(0);

  vector<double> binEdge;
  int counter=0;
  
  binEdge.push_back(1e-5);
  
  while( binEdge[binEdge.size()-1]<=5e-4 ){
    binEdge.push_back(binEdge[binEdge.size()-1]+1e-5);
    counter++;
  }
  
  while( binEdge[binEdge.size()-1]<=5e-2 ){
    binEdge.push_back(binEdge[binEdge.size()-1]+1e-4);
    counter++;
  }
  
  while( binEdge[binEdge.size()-1]<=0.2 ){
    binEdge.push_back(binEdge[binEdge.size()-1]+3e-3);
    counter++;
  }
  
  while( binEdge[binEdge.size()-1]<=1.0 ){
    binEdge.push_back(binEdge[binEdge.size()-1]+1e-2);
    counter++;
  }
  
  const int num=3;

  process processList[num]={kLHC_HZZs_mH125,kLHC_HZZs_mH125,kILC_250_mH125};
  TString fileName[num]   ={"LHCtoys/embeddedToys1DKD_fa3Corr_WithBackgDetEffects_fa3p25_600Evts.root",
			  "LHCtoys/embeddedToys1DKD_fa3Corr_WithBackgDetEffects_fa3p1_6000Evts.root",
			  "ILC250toys/toyresults_embd_model6_fa3_p1_2000signal_200bkg_accfalse.root"};

  TString treeName[num]   ={"results","results","fittree"};

  TString tag[num] = {"LHC, HZZ*, 14 TeV, 300/fb",
		      "LHC, HZZ*, 14 TeV, 3000/fb",
		      "ILC, Z*ZH, 250 GeV, 250/fb"};

  int lineColor[num]={1,2,4};

  fa3Converter* converter[num];
  TChain* tree[num];
  TH1F* histo[num];
  
  TCanvas* can = new TCanvas("can","can",500,500);
  can->SetLogx();

  float fa3;
  double fa3_d;

  TLegend* leg = new TLegend(0.2,0.7,0.6,0.9);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);

  for(int i = 2 ; i>-1; i--){

    converter[i] = new fa3Converter(processList[i],kLHC_HZZs_mH125);
    histo[i] = new TH1F(tag[i],";f_{a3};Number of Pseudo-experiments",counter,&binEdge[0]);
    histo[i]->SetLineColor(lineColor[i]);

    leg->AddEntry(histo[i],tag[i]);

    tree[i] = new TChain(treeName[i]);
    tree[i]->Add(fileName[i]);

    if(i==2)
      tree[i]->SetBranchAddress("fa3",&fa3);
    else
      tree[i]->SetBranchAddress("fa3",&fa3_d);

    for(int iEvt=0; iEvt<tree[i]->GetEntries(); iEvt++){

      tree[i]->GetEntry(iEvt);
      
      if(i==2)
	histo[i]->Fill(converter[i]->getConverted(fa3));
      else
	histo[i]->Fill(converter[i]->getConverted(fa3_d));

    }// end loop over iEvt

    histo[i]->Scale(1./histo[i]->Integral());

    if(i==2)
      histo[i]->Draw();
    else
      histo[i]->Draw("SAME");   

  }// end loop over i

  leg->Draw();

}
