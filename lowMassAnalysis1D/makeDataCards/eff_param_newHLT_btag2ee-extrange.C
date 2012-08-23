{
//=========Macro generated from canvas: c_fitfunc_btag2eec_fitfunc_btag2ee-ext/CANFITFUNC
//=========  (Wed Aug 22 07:18:52 2012) by ROOT version5.32/00
   TCanvas *c_fitfunc_btag2eec_fitfunc_btag2ee-ext = new TCanvas("c_fitfunc_btag2eec_fitfunc_btag2ee-ext", "CANFITFUNC",0,0,900,900);
   c_fitfunc_btag2eec_fitfunc_btag2ee-ext->Range(119.375,-0.000416761,175.625,0.003891136);
   c_fitfunc_btag2eec_fitfunc_btag2ee-ext->SetFillColor(0);
   c_fitfunc_btag2eec_fitfunc_btag2ee-ext->SetBorderMode(0);
   c_fitfunc_btag2eec_fitfunc_btag2ee-ext->SetBorderSize(2);
   c_fitfunc_btag2eec_fitfunc_btag2ee-ext->SetFrameBorderMode(0);
   
   TF1 *fit_poly3_btag2ee = new TF1("fit_poly3_btag2ee","pol3",125,170);
   fit_poly3_btag2ee->SetFillColor(19);
   fit_poly3_btag2ee->SetFillStyle(0);
   fit_poly3_btag2ee->SetLineColor(2);
   fit_poly3_btag2ee->SetLineWidth(2);
   fit_poly3_btag2ee->SetChisquare(5.545405e-09);
   fit_poly3_btag2ee->SetNDF(1);
   fit_poly3_btag2ee->GetXaxis()->SetTitle("M_{H}");
   fit_poly3_btag2ee->GetXaxis()->SetLabelFont(42);
   fit_poly3_btag2ee->GetXaxis()->SetLabelSize(0.035);
   fit_poly3_btag2ee->GetXaxis()->SetTitleSize(0.035);
   fit_poly3_btag2ee->GetXaxis()->SetTitleFont(42);
   fit_poly3_btag2ee->GetYaxis()->SetTitle("#varepsilon");
   fit_poly3_btag2ee->GetYaxis()->SetLabelFont(42);
   fit_poly3_btag2ee->GetYaxis()->SetLabelSize(0.035);
   fit_poly3_btag2ee->GetYaxis()->SetTitleSize(0.035);
   fit_poly3_btag2ee->GetYaxis()->SetTitleFont(42);
   fit_poly3_btag2ee->SetParameter(0,0.2856379);
   fit_poly3_btag2ee->SetParError(0,0.06538163);
   fit_poly3_btag2ee->SetParLimits(0,0,0);
   fit_poly3_btag2ee->SetParameter(1,-0.006386031);
   fit_poly3_btag2ee->SetParError(1,0.001319295);
   fit_poly3_btag2ee->SetParLimits(1,0,0);
   fit_poly3_btag2ee->SetParameter(2,4.686856e-05);
   fit_poly3_btag2ee->SetParError(2,8.832999e-06);
   fit_poly3_btag2ee->SetParLimits(2,0,0);
   fit_poly3_btag2ee->SetParameter(3,-1.124092e-07);
   fit_poly3_btag2ee->SetParError(3,1.96239e-08);
   fit_poly3_btag2ee->SetParLimits(3,0,0);
   fit_poly3_btag2ee->Draw("L");
   
   TGraph *graph = new TGraph(5);
   graph->SetName("effgr_btag2ee");
   graph->SetTitle("Efficiency vs Mass (ee , btag2)");
   graph->SetFillColor(1);
   graph->SetMarkerStyle(21);
   graph->SetPoint(0,130,0.0005607244675);
   graph->SetPoint(1,140,0.001802212209);
   graph->SetPoint(2,150,0.002841532929);
   graph->SetPoint(3,160,0.003315751441);
   graph->SetPoint(4,170,0.00223889295);
   
   TH1F *Graph_effgr_btag2ee3 = new TH1F("Graph_effgr_btag2ee3","Efficiency vs Mass (ee , btag2)",100,126,174);
   Graph_effgr_btag2ee3->SetMinimum(0);
   Graph_effgr_btag2ee3->SetMaximum(0.003591254);
   Graph_effgr_btag2ee3->SetDirectory(0);
   Graph_effgr_btag2ee3->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   Graph_effgr_btag2ee3->SetLineColor(ci);
   Graph_effgr_btag2ee3->GetXaxis()->SetTitle("M_{H}");
   Graph_effgr_btag2ee3->GetXaxis()->SetLabelFont(42);
   Graph_effgr_btag2ee3->GetXaxis()->SetLabelSize(0.035);
   Graph_effgr_btag2ee3->GetXaxis()->SetTitleSize(0.035);
   Graph_effgr_btag2ee3->GetXaxis()->SetTitleFont(42);
   Graph_effgr_btag2ee3->GetYaxis()->SetTitle("#varepsilon");
   Graph_effgr_btag2ee3->GetYaxis()->SetLabelFont(42);
   Graph_effgr_btag2ee3->GetYaxis()->SetLabelSize(0.035);
   Graph_effgr_btag2ee3->GetYaxis()->SetTitleSize(0.035);
   Graph_effgr_btag2ee3->GetYaxis()->SetTitleFont(42);
   Graph_effgr_btag2ee3->GetZaxis()->SetLabelFont(42);
   Graph_effgr_btag2ee3->GetZaxis()->SetLabelSize(0.035);
   Graph_effgr_btag2ee3->GetZaxis()->SetTitleSize(0.035);
   Graph_effgr_btag2ee3->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_effgr_btag2ee3);
   
   
   TF1 *fit_poly3_btag2ee = new TF1("fit_poly3_btag2ee","pol3",126,174);
   fit_poly3_btag2ee->SetFillColor(19);
   fit_poly3_btag2ee->SetFillStyle(0);
   fit_poly3_btag2ee->SetLineColor(2);
   fit_poly3_btag2ee->SetLineWidth(2);
   fit_poly3_btag2ee->SetChisquare(5.545405e-09);
   fit_poly3_btag2ee->SetNDF(1);
   fit_poly3_btag2ee->GetXaxis()->SetLabelFont(42);
   fit_poly3_btag2ee->GetXaxis()->SetLabelSize(0.035);
   fit_poly3_btag2ee->GetXaxis()->SetTitleSize(0.035);
   fit_poly3_btag2ee->GetXaxis()->SetTitleFont(42);
   fit_poly3_btag2ee->GetYaxis()->SetLabelFont(42);
   fit_poly3_btag2ee->GetYaxis()->SetLabelSize(0.035);
   fit_poly3_btag2ee->GetYaxis()->SetTitleSize(0.035);
   fit_poly3_btag2ee->GetYaxis()->SetTitleFont(42);
   fit_poly3_btag2ee->SetParameter(0,0.2856379);
   fit_poly3_btag2ee->SetParError(0,0.06538163);
   fit_poly3_btag2ee->SetParLimits(0,0,0);
   fit_poly3_btag2ee->SetParameter(1,-0.006386031);
   fit_poly3_btag2ee->SetParError(1,0.001319295);
   fit_poly3_btag2ee->SetParLimits(1,0,0);
   fit_poly3_btag2ee->SetParameter(2,4.686856e-05);
   fit_poly3_btag2ee->SetParError(2,8.832999e-06);
   fit_poly3_btag2ee->SetParLimits(2,0,0);
   fit_poly3_btag2ee->SetParameter(3,-1.124092e-07);
   fit_poly3_btag2ee->SetParError(3,1.96239e-08);
   fit_poly3_btag2ee->SetParLimits(3,0,0);
   graph->GetListOfFunctions()->Add(fit_poly3_btag2ee);
   graph->Draw("p");
   c_fitfunc_btag2eec_fitfunc_btag2ee-ext->Modified();
   c_fitfunc_btag2eec_fitfunc_btag2ee-ext->cd();
   c_fitfunc_btag2eec_fitfunc_btag2ee-ext->SetSelected(c_fitfunc_btag2eec_fitfunc_btag2ee-ext);
}
