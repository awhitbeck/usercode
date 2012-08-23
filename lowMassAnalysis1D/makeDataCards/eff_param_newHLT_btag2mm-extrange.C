{
//=========Macro generated from canvas: c_fitfunc_btag2mmc_fitfunc_btag2mm-ext/CANFITFUNC
//=========  (Wed Aug 22 07:18:52 2012) by ROOT version5.32/00
   TCanvas *c_fitfunc_btag2mmc_fitfunc_btag2mm-ext = new TCanvas("c_fitfunc_btag2mmc_fitfunc_btag2mm-ext", "CANFITFUNC",0,0,900,900);
   c_fitfunc_btag2mmc_fitfunc_btag2mm-ext->Range(119.375,-0.000302995,175.625,0.004942974);
   c_fitfunc_btag2mmc_fitfunc_btag2mm-ext->SetFillColor(0);
   c_fitfunc_btag2mmc_fitfunc_btag2mm-ext->SetBorderMode(0);
   c_fitfunc_btag2mmc_fitfunc_btag2mm-ext->SetBorderSize(2);
   c_fitfunc_btag2mmc_fitfunc_btag2mm-ext->SetFrameBorderMode(0);
   
   TF1 *fit_poly3_btag2mm = new TF1("fit_poly3_btag2mm","pol3",125,170);
   fit_poly3_btag2mm->SetFillColor(19);
   fit_poly3_btag2mm->SetFillStyle(0);
   fit_poly3_btag2mm->SetLineColor(2);
   fit_poly3_btag2mm->SetLineWidth(2);
   fit_poly3_btag2mm->SetChisquare(6.770858e-08);
   fit_poly3_btag2mm->SetNDF(1);
   fit_poly3_btag2mm->GetXaxis()->SetTitle("M_{H}");
   fit_poly3_btag2mm->GetXaxis()->SetLabelFont(42);
   fit_poly3_btag2mm->GetXaxis()->SetLabelSize(0.035);
   fit_poly3_btag2mm->GetXaxis()->SetTitleSize(0.035);
   fit_poly3_btag2mm->GetXaxis()->SetTitleFont(42);
   fit_poly3_btag2mm->GetYaxis()->SetTitle("#varepsilon");
   fit_poly3_btag2mm->GetYaxis()->SetLabelFont(42);
   fit_poly3_btag2mm->GetYaxis()->SetLabelSize(0.035);
   fit_poly3_btag2mm->GetYaxis()->SetTitleSize(0.035);
   fit_poly3_btag2mm->GetYaxis()->SetTitleFont(42);
   fit_poly3_btag2mm->SetParameter(0,0.4572018);
   fit_poly3_btag2mm->SetParError(0,0.2284604);
   fit_poly3_btag2mm->SetParLimits(0,0,0);
   fit_poly3_btag2mm->SetParameter(1,-0.01008874);
   fit_poly3_btag2mm->SetParError(1,0.00460996);
   fit_poly3_btag2mm->SetParLimits(1,0,0);
   fit_poly3_btag2mm->SetParameter(2,7.332072e-05);
   fit_poly3_btag2mm->SetParError(2,3.086479e-05);
   fit_poly3_btag2mm->SetParLimits(2,0,0);
   fit_poly3_btag2mm->SetParameter(3,-1.747689e-07);
   fit_poly3_btag2mm->SetParError(3,6.857102e-08);
   fit_poly3_btag2mm->SetParLimits(3,0,0);
   fit_poly3_btag2mm->Draw("L");
   
   TGraph *graph = new TGraph(5);
   graph->SetName("effgr_btag2mm");
   graph->SetTitle("Efficiency vs Mass (mm , btag2)");
   graph->SetFillColor(1);
   graph->SetMarkerStyle(21);
   graph->SetPoint(0,130,0.0007878466276);
   graph->SetPoint(1,140,0.002423334168);
   graph->SetPoint(2,150,0.003575863782);
   graph->SetPoint(3,160,0.004285353236);
   graph->SetPoint(4,170,0.002414657967);
   
   TH1F *Graph_effgr_btag2mm6 = new TH1F("Graph_effgr_btag2mm6","Efficiency vs Mass (mm , btag2)",100,126,174);
   Graph_effgr_btag2mm6->SetMinimum(0);
   Graph_effgr_btag2mm6->SetMaximum(0.004635104);
   Graph_effgr_btag2mm6->SetDirectory(0);
   Graph_effgr_btag2mm6->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   Graph_effgr_btag2mm6->SetLineColor(ci);
   Graph_effgr_btag2mm6->GetXaxis()->SetTitle("M_{H}");
   Graph_effgr_btag2mm6->GetXaxis()->SetLabelFont(42);
   Graph_effgr_btag2mm6->GetXaxis()->SetLabelSize(0.035);
   Graph_effgr_btag2mm6->GetXaxis()->SetTitleSize(0.035);
   Graph_effgr_btag2mm6->GetXaxis()->SetTitleFont(42);
   Graph_effgr_btag2mm6->GetYaxis()->SetTitle("#varepsilon");
   Graph_effgr_btag2mm6->GetYaxis()->SetLabelFont(42);
   Graph_effgr_btag2mm6->GetYaxis()->SetLabelSize(0.035);
   Graph_effgr_btag2mm6->GetYaxis()->SetTitleSize(0.035);
   Graph_effgr_btag2mm6->GetYaxis()->SetTitleFont(42);
   Graph_effgr_btag2mm6->GetZaxis()->SetLabelFont(42);
   Graph_effgr_btag2mm6->GetZaxis()->SetLabelSize(0.035);
   Graph_effgr_btag2mm6->GetZaxis()->SetTitleSize(0.035);
   Graph_effgr_btag2mm6->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_effgr_btag2mm6);
   
   
   TF1 *fit_poly3_btag2mm = new TF1("fit_poly3_btag2mm","pol3",126,174);
   fit_poly3_btag2mm->SetFillColor(19);
   fit_poly3_btag2mm->SetFillStyle(0);
   fit_poly3_btag2mm->SetLineColor(2);
   fit_poly3_btag2mm->SetLineWidth(2);
   fit_poly3_btag2mm->SetChisquare(6.770858e-08);
   fit_poly3_btag2mm->SetNDF(1);
   fit_poly3_btag2mm->GetXaxis()->SetLabelFont(42);
   fit_poly3_btag2mm->GetXaxis()->SetLabelSize(0.035);
   fit_poly3_btag2mm->GetXaxis()->SetTitleSize(0.035);
   fit_poly3_btag2mm->GetXaxis()->SetTitleFont(42);
   fit_poly3_btag2mm->GetYaxis()->SetLabelFont(42);
   fit_poly3_btag2mm->GetYaxis()->SetLabelSize(0.035);
   fit_poly3_btag2mm->GetYaxis()->SetTitleSize(0.035);
   fit_poly3_btag2mm->GetYaxis()->SetTitleFont(42);
   fit_poly3_btag2mm->SetParameter(0,0.4572018);
   fit_poly3_btag2mm->SetParError(0,0.2284604);
   fit_poly3_btag2mm->SetParLimits(0,0,0);
   fit_poly3_btag2mm->SetParameter(1,-0.01008874);
   fit_poly3_btag2mm->SetParError(1,0.00460996);
   fit_poly3_btag2mm->SetParLimits(1,0,0);
   fit_poly3_btag2mm->SetParameter(2,7.332072e-05);
   fit_poly3_btag2mm->SetParError(2,3.086479e-05);
   fit_poly3_btag2mm->SetParLimits(2,0,0);
   fit_poly3_btag2mm->SetParameter(3,-1.747689e-07);
   fit_poly3_btag2mm->SetParError(3,6.857102e-08);
   fit_poly3_btag2mm->SetParLimits(3,0,0);
   graph->GetListOfFunctions()->Add(fit_poly3_btag2mm);
   graph->Draw("p");
   c_fitfunc_btag2mmc_fitfunc_btag2mm-ext->Modified();
   c_fitfunc_btag2mmc_fitfunc_btag2mm-ext->cd();
   c_fitfunc_btag2mmc_fitfunc_btag2mm-ext->SetSelected(c_fitfunc_btag2mmc_fitfunc_btag2mm-ext);
}
