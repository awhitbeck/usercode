{
//=========Macro generated from canvas: c_fitfunc_btag0eec_fitfunc_btag0ee-ext/CANFITFUNC
//=========  (Wed Aug 22 07:18:52 2012) by ROOT version5.32/00
   TCanvas *c_fitfunc_btag0eec_fitfunc_btag0ee-ext = new TCanvas("c_fitfunc_btag0eec_fitfunc_btag0ee-ext", "CANFITFUNC",0,0,900,900);
   c_fitfunc_btag0eec_fitfunc_btag0ee-ext->Range(119.375,0.0005530614,175.625,0.02645275);
   c_fitfunc_btag0eec_fitfunc_btag0ee-ext->SetFillColor(0);
   c_fitfunc_btag0eec_fitfunc_btag0ee-ext->SetBorderMode(0);
   c_fitfunc_btag0eec_fitfunc_btag0ee-ext->SetBorderSize(2);
   c_fitfunc_btag0eec_fitfunc_btag0ee-ext->SetFrameBorderMode(0);
   
   TF1 *fit_poly3_btag0ee = new TF1("fit_poly3_btag0ee","pol3",125,170);
   fit_poly3_btag0ee->SetFillColor(19);
   fit_poly3_btag0ee->SetFillStyle(0);
   fit_poly3_btag0ee->SetLineColor(2);
   fit_poly3_btag0ee->SetLineWidth(2);
   fit_poly3_btag0ee->SetChisquare(9.332279e-08);
   fit_poly3_btag0ee->SetNDF(1);
   fit_poly3_btag0ee->GetXaxis()->SetTitle("M_{H}");
   fit_poly3_btag0ee->GetXaxis()->SetLabelFont(42);
   fit_poly3_btag0ee->GetXaxis()->SetLabelSize(0.035);
   fit_poly3_btag0ee->GetXaxis()->SetTitleSize(0.035);
   fit_poly3_btag0ee->GetXaxis()->SetTitleFont(42);
   fit_poly3_btag0ee->GetYaxis()->SetTitle("#varepsilon");
   fit_poly3_btag0ee->GetYaxis()->SetLabelFont(42);
   fit_poly3_btag0ee->GetYaxis()->SetLabelSize(0.035);
   fit_poly3_btag0ee->GetYaxis()->SetTitleSize(0.035);
   fit_poly3_btag0ee->GetYaxis()->SetTitleFont(42);
   fit_poly3_btag0ee->SetParameter(0,2.631153);
   fit_poly3_btag0ee->SetParError(0,0.2682149);
   fit_poly3_btag0ee->SetParLimits(0,0,0);
   fit_poly3_btag0ee->SetParameter(1,-0.05754261);
   fit_poly3_btag0ee->SetParError(1,0.005412142);
   fit_poly3_btag0ee->SetParLimits(1,0,0);
   fit_poly3_btag0ee->SetParameter(2,0.0004152928);
   fit_poly3_btag0ee->SetParError(2,3.623559e-05);
   fit_poly3_btag0ee->SetParLimits(2,0,0);
   fit_poly3_btag0ee->SetParameter(3,-9.846909e-07);
   fit_poly3_btag0ee->SetParError(3,8.05031e-08);
   fit_poly3_btag0ee->SetParLimits(3,0,0);
   fit_poly3_btag0ee->Draw("L");
   
   TGraph *graph = new TGraph(5);
   graph->SetName("effgr_btag0ee");
   graph->SetTitle("Efficiency vs Mass (ee , btag0)");
   graph->SetFillColor(1);
   graph->SetMarkerStyle(21);
   graph->SetPoint(0,130,0.005659951363);
   graph->SetPoint(1,140,0.01308111195);
   graph->SetPoint(2,150,0.02029911056);
   graph->SetPoint(3,160,0.02268374898);
   graph->SetPoint(4,170,0.01304893475);
   
   TH1F *Graph_effgr_btag0ee1 = new TH1F("Graph_effgr_btag0ee1","Efficiency vs Mass (ee , btag0)",100,126,174);
   Graph_effgr_btag0ee1->SetMinimum(0);
   Graph_effgr_btag0ee1->SetMaximum(0.02438613);
   Graph_effgr_btag0ee1->SetDirectory(0);
   Graph_effgr_btag0ee1->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   Graph_effgr_btag0ee1->SetLineColor(ci);
   Graph_effgr_btag0ee1->GetXaxis()->SetTitle("M_{H}");
   Graph_effgr_btag0ee1->GetXaxis()->SetLabelFont(42);
   Graph_effgr_btag0ee1->GetXaxis()->SetLabelSize(0.035);
   Graph_effgr_btag0ee1->GetXaxis()->SetTitleSize(0.035);
   Graph_effgr_btag0ee1->GetXaxis()->SetTitleFont(42);
   Graph_effgr_btag0ee1->GetYaxis()->SetTitle("#varepsilon");
   Graph_effgr_btag0ee1->GetYaxis()->SetLabelFont(42);
   Graph_effgr_btag0ee1->GetYaxis()->SetLabelSize(0.035);
   Graph_effgr_btag0ee1->GetYaxis()->SetTitleSize(0.035);
   Graph_effgr_btag0ee1->GetYaxis()->SetTitleFont(42);
   Graph_effgr_btag0ee1->GetZaxis()->SetLabelFont(42);
   Graph_effgr_btag0ee1->GetZaxis()->SetLabelSize(0.035);
   Graph_effgr_btag0ee1->GetZaxis()->SetTitleSize(0.035);
   Graph_effgr_btag0ee1->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_effgr_btag0ee1);
   
   
   TF1 *fit_poly3_btag0ee = new TF1("fit_poly3_btag0ee","pol3",126,174);
   fit_poly3_btag0ee->SetFillColor(19);
   fit_poly3_btag0ee->SetFillStyle(0);
   fit_poly3_btag0ee->SetLineColor(2);
   fit_poly3_btag0ee->SetLineWidth(2);
   fit_poly3_btag0ee->SetChisquare(9.332279e-08);
   fit_poly3_btag0ee->SetNDF(1);
   fit_poly3_btag0ee->GetXaxis()->SetLabelFont(42);
   fit_poly3_btag0ee->GetXaxis()->SetLabelSize(0.035);
   fit_poly3_btag0ee->GetXaxis()->SetTitleSize(0.035);
   fit_poly3_btag0ee->GetXaxis()->SetTitleFont(42);
   fit_poly3_btag0ee->GetYaxis()->SetLabelFont(42);
   fit_poly3_btag0ee->GetYaxis()->SetLabelSize(0.035);
   fit_poly3_btag0ee->GetYaxis()->SetTitleSize(0.035);
   fit_poly3_btag0ee->GetYaxis()->SetTitleFont(42);
   fit_poly3_btag0ee->SetParameter(0,2.631153);
   fit_poly3_btag0ee->SetParError(0,0.2682149);
   fit_poly3_btag0ee->SetParLimits(0,0,0);
   fit_poly3_btag0ee->SetParameter(1,-0.05754261);
   fit_poly3_btag0ee->SetParError(1,0.005412142);
   fit_poly3_btag0ee->SetParLimits(1,0,0);
   fit_poly3_btag0ee->SetParameter(2,0.0004152928);
   fit_poly3_btag0ee->SetParError(2,3.623559e-05);
   fit_poly3_btag0ee->SetParLimits(2,0,0);
   fit_poly3_btag0ee->SetParameter(3,-9.846909e-07);
   fit_poly3_btag0ee->SetParError(3,8.05031e-08);
   fit_poly3_btag0ee->SetParLimits(3,0,0);
   graph->GetListOfFunctions()->Add(fit_poly3_btag0ee);
   graph->Draw("p");
   c_fitfunc_btag0eec_fitfunc_btag0ee-ext->Modified();
   c_fitfunc_btag0eec_fitfunc_btag0ee-ext->cd();
   c_fitfunc_btag0eec_fitfunc_btag0ee-ext->SetSelected(c_fitfunc_btag0eec_fitfunc_btag0ee-ext);
}
