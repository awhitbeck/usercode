{
//=========Macro generated from canvas: c_fitfunc_btag1mmc_fitfunc_btag1mm-ext/CANFITFUNC
//=========  (Wed Aug 22 07:18:52 2012) by ROOT version5.32/00
   TCanvas *c_fitfunc_btag1mmc_fitfunc_btag1mm-ext = new TCanvas("c_fitfunc_btag1mmc_fitfunc_btag1mm-ext", "CANFITFUNC",0,0,900,900);
   c_fitfunc_btag1mmc_fitfunc_btag1mm-ext->Range(119.375,-2.830833e-06,175.625,0.01330678);
   c_fitfunc_btag1mmc_fitfunc_btag1mm-ext->SetFillColor(0);
   c_fitfunc_btag1mmc_fitfunc_btag1mm-ext->SetBorderMode(0);
   c_fitfunc_btag1mmc_fitfunc_btag1mm-ext->SetBorderSize(2);
   c_fitfunc_btag1mmc_fitfunc_btag1mm-ext->SetFrameBorderMode(0);
   
   TF1 *fit_poly3_btag1mm = new TF1("fit_poly3_btag1mm","pol3",125,170);
   fit_poly3_btag1mm->SetFillColor(19);
   fit_poly3_btag1mm->SetFillStyle(0);
   fit_poly3_btag1mm->SetLineColor(2);
   fit_poly3_btag1mm->SetLineWidth(2);
   fit_poly3_btag1mm->SetChisquare(1.254401e-08);
   fit_poly3_btag1mm->SetNDF(1);
   fit_poly3_btag1mm->GetXaxis()->SetTitle("M_{H}");
   fit_poly3_btag1mm->GetXaxis()->SetLabelFont(42);
   fit_poly3_btag1mm->GetXaxis()->SetLabelSize(0.035);
   fit_poly3_btag1mm->GetXaxis()->SetTitleSize(0.035);
   fit_poly3_btag1mm->GetXaxis()->SetTitleFont(42);
   fit_poly3_btag1mm->GetYaxis()->SetTitle("#varepsilon");
   fit_poly3_btag1mm->GetYaxis()->SetLabelFont(42);
   fit_poly3_btag1mm->GetYaxis()->SetLabelSize(0.035);
   fit_poly3_btag1mm->GetYaxis()->SetTitleSize(0.035);
   fit_poly3_btag1mm->GetYaxis()->SetTitleFont(42);
   fit_poly3_btag1mm->SetParameter(0,0.8082949);
   fit_poly3_btag1mm->SetParError(0,0.0983348);
   fit_poly3_btag1mm->SetParLimits(0,0,0);
   fit_poly3_btag1mm->SetParameter(1,-0.01852147);
   fit_poly3_btag1mm->SetParError(1,0.001984237);
   fit_poly3_btag1mm->SetParLimits(1,0,0);
   fit_poly3_btag1mm->SetParameter(2,0.0001391652);
   fit_poly3_btag1mm->SetParError(2,1.328494e-05);
   fit_poly3_btag1mm->SetParLimits(2,0,0);
   fit_poly3_btag1mm->SetParameter(3,-3.409013e-07);
   fit_poly3_btag1mm->SetParError(3,2.95146e-08);
   fit_poly3_btag1mm->SetParLimits(3,0,0);
   fit_poly3_btag1mm->Draw("L");
   
   TGraph *graph = new TGraph(5);
   graph->SetName("effgr_btag1mm");
   graph->SetTitle("Efficiency vs Mass (mm , btag1)");
   graph->SetFillColor(1);
   graph->SetMarkerStyle(21);
   graph->SetPoint(0,130,0.003449590877);
   graph->SetPoint(1,140,0.007441092283);
   graph->SetPoint(2,150,0.0108307302);
   graph->SetPoint(3,160,0.01110456698);
   graph->SetPoint(4,170,0.006685724482);
   
   TH1F *Graph_effgr_btag1mm5 = new TH1F("Graph_effgr_btag1mm5","Efficiency vs Mass (mm , btag1)",100,126,174);
   Graph_effgr_btag1mm5->SetMinimum(0);
   Graph_effgr_btag1mm5->SetMaximum(0.01187006);
   Graph_effgr_btag1mm5->SetDirectory(0);
   Graph_effgr_btag1mm5->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   Graph_effgr_btag1mm5->SetLineColor(ci);
   Graph_effgr_btag1mm5->GetXaxis()->SetTitle("M_{H}");
   Graph_effgr_btag1mm5->GetXaxis()->SetLabelFont(42);
   Graph_effgr_btag1mm5->GetXaxis()->SetLabelSize(0.035);
   Graph_effgr_btag1mm5->GetXaxis()->SetTitleSize(0.035);
   Graph_effgr_btag1mm5->GetXaxis()->SetTitleFont(42);
   Graph_effgr_btag1mm5->GetYaxis()->SetTitle("#varepsilon");
   Graph_effgr_btag1mm5->GetYaxis()->SetLabelFont(42);
   Graph_effgr_btag1mm5->GetYaxis()->SetLabelSize(0.035);
   Graph_effgr_btag1mm5->GetYaxis()->SetTitleSize(0.035);
   Graph_effgr_btag1mm5->GetYaxis()->SetTitleFont(42);
   Graph_effgr_btag1mm5->GetZaxis()->SetLabelFont(42);
   Graph_effgr_btag1mm5->GetZaxis()->SetLabelSize(0.035);
   Graph_effgr_btag1mm5->GetZaxis()->SetTitleSize(0.035);
   Graph_effgr_btag1mm5->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_effgr_btag1mm5);
   
   
   TF1 *fit_poly3_btag1mm = new TF1("fit_poly3_btag1mm","pol3",126,174);
   fit_poly3_btag1mm->SetFillColor(19);
   fit_poly3_btag1mm->SetFillStyle(0);
   fit_poly3_btag1mm->SetLineColor(2);
   fit_poly3_btag1mm->SetLineWidth(2);
   fit_poly3_btag1mm->SetChisquare(1.254401e-08);
   fit_poly3_btag1mm->SetNDF(1);
   fit_poly3_btag1mm->GetXaxis()->SetLabelFont(42);
   fit_poly3_btag1mm->GetXaxis()->SetLabelSize(0.035);
   fit_poly3_btag1mm->GetXaxis()->SetTitleSize(0.035);
   fit_poly3_btag1mm->GetXaxis()->SetTitleFont(42);
   fit_poly3_btag1mm->GetYaxis()->SetLabelFont(42);
   fit_poly3_btag1mm->GetYaxis()->SetLabelSize(0.035);
   fit_poly3_btag1mm->GetYaxis()->SetTitleSize(0.035);
   fit_poly3_btag1mm->GetYaxis()->SetTitleFont(42);
   fit_poly3_btag1mm->SetParameter(0,0.8082949);
   fit_poly3_btag1mm->SetParError(0,0.0983348);
   fit_poly3_btag1mm->SetParLimits(0,0,0);
   fit_poly3_btag1mm->SetParameter(1,-0.01852147);
   fit_poly3_btag1mm->SetParError(1,0.001984237);
   fit_poly3_btag1mm->SetParLimits(1,0,0);
   fit_poly3_btag1mm->SetParameter(2,0.0001391652);
   fit_poly3_btag1mm->SetParError(2,1.328494e-05);
   fit_poly3_btag1mm->SetParLimits(2,0,0);
   fit_poly3_btag1mm->SetParameter(3,-3.409013e-07);
   fit_poly3_btag1mm->SetParError(3,2.95146e-08);
   fit_poly3_btag1mm->SetParLimits(3,0,0);
   graph->GetListOfFunctions()->Add(fit_poly3_btag1mm);
   graph->Draw("p");
   c_fitfunc_btag1mmc_fitfunc_btag1mm-ext->Modified();
   c_fitfunc_btag1mmc_fitfunc_btag1mm-ext->cd();
   c_fitfunc_btag1mmc_fitfunc_btag1mm-ext->SetSelected(c_fitfunc_btag1mmc_fitfunc_btag1mm-ext);
}
