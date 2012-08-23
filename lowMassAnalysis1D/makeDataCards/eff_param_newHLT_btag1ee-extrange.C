{
//=========Macro generated from canvas: c_fitfunc_btag1eec_fitfunc_btag1ee-ext/CANFITFUNC
//=========  (Wed Aug 22 07:18:52 2012) by ROOT version5.32/00
   TCanvas *c_fitfunc_btag1eec_fitfunc_btag1ee-ext = new TCanvas("c_fitfunc_btag1eec_fitfunc_btag1ee-ext", "CANFITFUNC",0,0,900,900);
   c_fitfunc_btag1eec_fitfunc_btag1ee-ext->Range(119.375,-0.0003291794,175.625,0.01067312);
   c_fitfunc_btag1eec_fitfunc_btag1ee-ext->SetFillColor(0);
   c_fitfunc_btag1eec_fitfunc_btag1ee-ext->SetBorderMode(0);
   c_fitfunc_btag1eec_fitfunc_btag1ee-ext->SetBorderSize(2);
   c_fitfunc_btag1eec_fitfunc_btag1ee-ext->SetFrameBorderMode(0);
   
   TF1 *fit_poly3_btag1ee = new TF1("fit_poly3_btag1ee","pol3",125,170);
   fit_poly3_btag1ee->SetFillColor(19);
   fit_poly3_btag1ee->SetFillStyle(0);
   fit_poly3_btag1ee->SetLineColor(2);
   fit_poly3_btag1ee->SetLineWidth(2);
   fit_poly3_btag1ee->SetChisquare(2.291068e-07);
   fit_poly3_btag1ee->SetNDF(1);
   fit_poly3_btag1ee->GetXaxis()->SetTitle("M_{H}");
   fit_poly3_btag1ee->GetXaxis()->SetLabelFont(42);
   fit_poly3_btag1ee->GetXaxis()->SetLabelSize(0.035);
   fit_poly3_btag1ee->GetXaxis()->SetTitleSize(0.035);
   fit_poly3_btag1ee->GetXaxis()->SetTitleFont(42);
   fit_poly3_btag1ee->GetYaxis()->SetTitle("#varepsilon");
   fit_poly3_btag1ee->GetYaxis()->SetLabelFont(42);
   fit_poly3_btag1ee->GetYaxis()->SetLabelSize(0.035);
   fit_poly3_btag1ee->GetYaxis()->SetTitleSize(0.035);
   fit_poly3_btag1ee->GetYaxis()->SetTitleFont(42);
   fit_poly3_btag1ee->SetParameter(0,0.9090254);
   fit_poly3_btag1ee->SetParError(0,0.4202504);
   fit_poly3_btag1ee->SetParLimits(0,0,0);
   fit_poly3_btag1ee->SetParameter(1,-0.02023523);
   fit_poly3_btag1ee->SetParError(1,0.00847997);
   fit_poly3_btag1ee->SetParLimits(1,0,0);
   fit_poly3_btag1ee->SetParameter(2,0.0001482989);
   fit_poly3_btag1ee->SetParError(2,5.677544e-05);
   fit_poly3_btag1ee->SetParLimits(2,0,0);
   fit_poly3_btag1ee->SetParameter(3,-3.561757e-07);
   fit_poly3_btag1ee->SetParError(3,1.261356e-07);
   fit_poly3_btag1ee->SetParLimits(3,0,0);
   fit_poly3_btag1ee->Draw("L");
   
   TGraph *graph = new TGraph(5);
   graph->SetName("effgr_btag1ee");
   graph->SetTitle("Efficiency vs Mass (ee , btag1)");
   graph->SetFillColor(1);
   graph->SetMarkerStyle(21);
   graph->SetPoint(0,130,0.002122569131);
   graph->SetPoint(1,140,0.005635328591);
   graph->SetPoint(2,150,0.008030966856);
   graph->SetPoint(3,160,0.009174770676);
   graph->SetPoint(4,170,0.004927345086);
   
   TH1F *Graph_effgr_btag1ee2 = new TH1F("Graph_effgr_btag1ee2","Efficiency vs Mass (ee , btag1)",100,126,174);
   Graph_effgr_btag1ee2->SetMinimum(0);
   Graph_effgr_btag1ee2->SetMaximum(0.009879991);
   Graph_effgr_btag1ee2->SetDirectory(0);
   Graph_effgr_btag1ee2->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   Graph_effgr_btag1ee2->SetLineColor(ci);
   Graph_effgr_btag1ee2->GetXaxis()->SetTitle("M_{H}");
   Graph_effgr_btag1ee2->GetXaxis()->SetLabelFont(42);
   Graph_effgr_btag1ee2->GetXaxis()->SetLabelSize(0.035);
   Graph_effgr_btag1ee2->GetXaxis()->SetTitleSize(0.035);
   Graph_effgr_btag1ee2->GetXaxis()->SetTitleFont(42);
   Graph_effgr_btag1ee2->GetYaxis()->SetTitle("#varepsilon");
   Graph_effgr_btag1ee2->GetYaxis()->SetLabelFont(42);
   Graph_effgr_btag1ee2->GetYaxis()->SetLabelSize(0.035);
   Graph_effgr_btag1ee2->GetYaxis()->SetTitleSize(0.035);
   Graph_effgr_btag1ee2->GetYaxis()->SetTitleFont(42);
   Graph_effgr_btag1ee2->GetZaxis()->SetLabelFont(42);
   Graph_effgr_btag1ee2->GetZaxis()->SetLabelSize(0.035);
   Graph_effgr_btag1ee2->GetZaxis()->SetTitleSize(0.035);
   Graph_effgr_btag1ee2->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_effgr_btag1ee2);
   
   
   TF1 *fit_poly3_btag1ee = new TF1("fit_poly3_btag1ee","pol3",126,174);
   fit_poly3_btag1ee->SetFillColor(19);
   fit_poly3_btag1ee->SetFillStyle(0);
   fit_poly3_btag1ee->SetLineColor(2);
   fit_poly3_btag1ee->SetLineWidth(2);
   fit_poly3_btag1ee->SetChisquare(2.291068e-07);
   fit_poly3_btag1ee->SetNDF(1);
   fit_poly3_btag1ee->GetXaxis()->SetLabelFont(42);
   fit_poly3_btag1ee->GetXaxis()->SetLabelSize(0.035);
   fit_poly3_btag1ee->GetXaxis()->SetTitleSize(0.035);
   fit_poly3_btag1ee->GetXaxis()->SetTitleFont(42);
   fit_poly3_btag1ee->GetYaxis()->SetLabelFont(42);
   fit_poly3_btag1ee->GetYaxis()->SetLabelSize(0.035);
   fit_poly3_btag1ee->GetYaxis()->SetTitleSize(0.035);
   fit_poly3_btag1ee->GetYaxis()->SetTitleFont(42);
   fit_poly3_btag1ee->SetParameter(0,0.9090254);
   fit_poly3_btag1ee->SetParError(0,0.4202504);
   fit_poly3_btag1ee->SetParLimits(0,0,0);
   fit_poly3_btag1ee->SetParameter(1,-0.02023523);
   fit_poly3_btag1ee->SetParError(1,0.00847997);
   fit_poly3_btag1ee->SetParLimits(1,0,0);
   fit_poly3_btag1ee->SetParameter(2,0.0001482989);
   fit_poly3_btag1ee->SetParError(2,5.677544e-05);
   fit_poly3_btag1ee->SetParLimits(2,0,0);
   fit_poly3_btag1ee->SetParameter(3,-3.561757e-07);
   fit_poly3_btag1ee->SetParError(3,1.261356e-07);
   fit_poly3_btag1ee->SetParLimits(3,0,0);
   graph->GetListOfFunctions()->Add(fit_poly3_btag1ee);
   graph->Draw("p");
   c_fitfunc_btag1eec_fitfunc_btag1ee-ext->Modified();
   c_fitfunc_btag1eec_fitfunc_btag1ee-ext->cd();
   c_fitfunc_btag1eec_fitfunc_btag1ee-ext->SetSelected(c_fitfunc_btag1eec_fitfunc_btag1ee-ext);
}
