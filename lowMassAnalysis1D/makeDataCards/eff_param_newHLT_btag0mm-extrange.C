{
//=========Macro generated from canvas: c_fitfunc_btag0mmc_fitfunc_btag0mm-ext/CANFITFUNC
//=========  (Wed Aug 22 07:18:52 2012) by ROOT version5.32/00
   TCanvas *c_fitfunc_btag0mmc_fitfunc_btag0mm-ext = new TCanvas("c_fitfunc_btag0mmc_fitfunc_btag0mm-ext", "CANFITFUNC",0,0,900,900);
   c_fitfunc_btag0mmc_fitfunc_btag0mm-ext->Range(119.375,0.0005787594,175.625,0.03421069);
   c_fitfunc_btag0mmc_fitfunc_btag0mm-ext->SetFillColor(0);
   c_fitfunc_btag0mmc_fitfunc_btag0mm-ext->SetBorderMode(0);
   c_fitfunc_btag0mmc_fitfunc_btag0mm-ext->SetBorderSize(2);
   c_fitfunc_btag0mmc_fitfunc_btag0mm-ext->SetFrameBorderMode(0);
   
   TF1 *fit_poly3_btag0mm = new TF1("fit_poly3_btag0mm","pol3",125,170);
   fit_poly3_btag0mm->SetFillColor(19);
   fit_poly3_btag0mm->SetFillStyle(0);
   fit_poly3_btag0mm->SetLineColor(2);
   fit_poly3_btag0mm->SetLineWidth(2);
   fit_poly3_btag0mm->SetChisquare(3.074622e-06);
   fit_poly3_btag0mm->SetNDF(1);
   fit_poly3_btag0mm->GetXaxis()->SetTitle("M_{H}");
   fit_poly3_btag0mm->GetXaxis()->SetLabelFont(42);
   fit_poly3_btag0mm->GetXaxis()->SetLabelSize(0.035);
   fit_poly3_btag0mm->GetXaxis()->SetTitleSize(0.035);
   fit_poly3_btag0mm->GetXaxis()->SetTitleFont(42);
   fit_poly3_btag0mm->GetYaxis()->SetTitle("#varepsilon");
   fit_poly3_btag0mm->GetYaxis()->SetLabelFont(42);
   fit_poly3_btag0mm->GetYaxis()->SetLabelSize(0.035);
   fit_poly3_btag0mm->GetYaxis()->SetTitleSize(0.035);
   fit_poly3_btag0mm->GetYaxis()->SetTitleFont(42);
   fit_poly3_btag0mm->SetParameter(0,2.939625);
   fit_poly3_btag0mm->SetParError(0,1.539519);
   fit_poly3_btag0mm->SetParLimits(0,0,0);
   fit_poly3_btag0mm->SetParameter(1,-0.06530231);
   fit_poly3_btag0mm->SetParError(1,0.03106499);
   fit_poly3_btag0mm->SetParLimits(1,0,0);
   fit_poly3_btag0mm->SetParameter(2,0.0004780788);
   fit_poly3_btag0mm->SetParError(2,0.0002079876);
   fit_poly3_btag0mm->SetParLimits(2,0,0);
   fit_poly3_btag0mm->SetParameter(3,-1.147776e-06);
   fit_poly3_btag0mm->SetParError(3,4.620773e-07);
   fit_poly3_btag0mm->SetParLimits(3,0,0);
   fit_poly3_btag0mm->Draw("L");
   
   TGraph *graph = new TGraph(5);
   graph->SetName("effgr_btag0mm");
   graph->SetTitle("Efficiency vs Mass (mm , btag0)");
   graph->SetFillColor(1);
   graph->SetMarkerStyle(21);
   graph->SetPoint(0,130,0.007981674746);
   graph->SetPoint(1,140,0.01898554526);
   graph->SetPoint(2,150,0.02604828402);
   graph->SetPoint(3,160,0.02961848304);
   graph->SetPoint(4,170,0.01547423471);
   
   TH1F *Graph_effgr_btag0mm4 = new TH1F("Graph_effgr_btag0mm4","Efficiency vs Mass (mm , btag0)",100,126,174);
   Graph_effgr_btag0mm4->SetMinimum(0);
   Graph_effgr_btag0mm4->SetMaximum(0.03178216);
   Graph_effgr_btag0mm4->SetDirectory(0);
   Graph_effgr_btag0mm4->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   Graph_effgr_btag0mm4->SetLineColor(ci);
   Graph_effgr_btag0mm4->GetXaxis()->SetTitle("M_{H}");
   Graph_effgr_btag0mm4->GetXaxis()->SetLabelFont(42);
   Graph_effgr_btag0mm4->GetXaxis()->SetLabelSize(0.035);
   Graph_effgr_btag0mm4->GetXaxis()->SetTitleSize(0.035);
   Graph_effgr_btag0mm4->GetXaxis()->SetTitleFont(42);
   Graph_effgr_btag0mm4->GetYaxis()->SetTitle("#varepsilon");
   Graph_effgr_btag0mm4->GetYaxis()->SetLabelFont(42);
   Graph_effgr_btag0mm4->GetYaxis()->SetLabelSize(0.035);
   Graph_effgr_btag0mm4->GetYaxis()->SetTitleSize(0.035);
   Graph_effgr_btag0mm4->GetYaxis()->SetTitleFont(42);
   Graph_effgr_btag0mm4->GetZaxis()->SetLabelFont(42);
   Graph_effgr_btag0mm4->GetZaxis()->SetLabelSize(0.035);
   Graph_effgr_btag0mm4->GetZaxis()->SetTitleSize(0.035);
   Graph_effgr_btag0mm4->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_effgr_btag0mm4);
   
   
   TF1 *fit_poly3_btag0mm = new TF1("fit_poly3_btag0mm","pol3",126,174);
   fit_poly3_btag0mm->SetFillColor(19);
   fit_poly3_btag0mm->SetFillStyle(0);
   fit_poly3_btag0mm->SetLineColor(2);
   fit_poly3_btag0mm->SetLineWidth(2);
   fit_poly3_btag0mm->SetChisquare(3.074622e-06);
   fit_poly3_btag0mm->SetNDF(1);
   fit_poly3_btag0mm->GetXaxis()->SetLabelFont(42);
   fit_poly3_btag0mm->GetXaxis()->SetLabelSize(0.035);
   fit_poly3_btag0mm->GetXaxis()->SetTitleSize(0.035);
   fit_poly3_btag0mm->GetXaxis()->SetTitleFont(42);
   fit_poly3_btag0mm->GetYaxis()->SetLabelFont(42);
   fit_poly3_btag0mm->GetYaxis()->SetLabelSize(0.035);
   fit_poly3_btag0mm->GetYaxis()->SetTitleSize(0.035);
   fit_poly3_btag0mm->GetYaxis()->SetTitleFont(42);
   fit_poly3_btag0mm->SetParameter(0,2.939625);
   fit_poly3_btag0mm->SetParError(0,1.539519);
   fit_poly3_btag0mm->SetParLimits(0,0,0);
   fit_poly3_btag0mm->SetParameter(1,-0.06530231);
   fit_poly3_btag0mm->SetParError(1,0.03106499);
   fit_poly3_btag0mm->SetParLimits(1,0,0);
   fit_poly3_btag0mm->SetParameter(2,0.0004780788);
   fit_poly3_btag0mm->SetParError(2,0.0002079876);
   fit_poly3_btag0mm->SetParLimits(2,0,0);
   fit_poly3_btag0mm->SetParameter(3,-1.147776e-06);
   fit_poly3_btag0mm->SetParError(3,4.620773e-07);
   fit_poly3_btag0mm->SetParLimits(3,0,0);
   graph->GetListOfFunctions()->Add(fit_poly3_btag0mm);
   graph->Draw("p");
   c_fitfunc_btag0mmc_fitfunc_btag0mm-ext->Modified();
   c_fitfunc_btag0mmc_fitfunc_btag0mm-ext->cd();
   c_fitfunc_btag0mmc_fitfunc_btag0mm-ext->SetSelected(c_fitfunc_btag0mmc_fitfunc_btag0mm-ext);
}
