/*
 * kFitTotalD = 2 x TotalD for each g_i... Thus, kFitTotalD has to be even.
 * Implementation uses kFitTotalD/2 and produces 2 templates for each g_i separately.
 * Template creation requires upon histogram initialization that the second dimensions (D2) to be the interference terms.
 */

const int kFitTotalD = 8;
char* strFitDim[kFitTotalD] = { // Dimension should not exceed kFitTotalD!!!
	"D_g1_vs_g2_phi0",
	"D_g1_vs_g4_phi0",
	"D_g2_vs_g4_phi0",
	"D_g1_vs_g2_phi0",
	"D_g2intPdf_phi0",
	"D_g4intPdf_phi0",
	"D_g2g4intPdf_phi0",
	"D_g1_vs_g4_phi0"
};
char* str1DFit_title[kFitTotalD] = { // Dimension should not exceed kFitTotalD!!!
	"T_1D_D_g1_vs_g2_phi0",
	"T_1D_D_g1_vs_g4_phi0",
	"T_1D_D_g2_vs_g4_phi0",
	"T_1D_D_g1_vs_g2_phi0_copy",
	"T_1D_D_g2intPdf_phi0",
	"T_1D_D_g4intPdf_phi0",
	"T_1D_D_g2g4intPdf_phi0",
	"T_1D_D_g1_vs_g4_phi0_copy"
};
char* str2DFit_title[kFitTotalD/2] = { // Dimension should not exceed kFitTotalD/2!!!
	"T_2D_g1_vs_g2_phi0",
	"T_2D_g1_vs_g4_phi0",
	"T_2D_g2_vs_g4_phi0",
	"T_2D_Dg2_vs_Dg4"
};
char* str3DFit_title[2] = { // Dimension should not exceed 2!!!
	"T_3D_Dg2Dg4Dg2int",
	"T_3D_Dg2Dg4Dg4int"
};

char* strFitDim_label[kFitTotalD] = { // Dimension should not exceed kFitTotalD!!!
	"D_{0^{+}_{h}}",
	"D_{0^{-}}",
	"D_{0^{+}_{h}-0^{-}}",
	"D_{0^{+}_{h}}",
	"D_{Int}",
	"D_{CP}",
	"D_{0^{+}_{h}-0^{-} Int}",
	"D_{CP}"
};
char* str1DFit_label[kFitTotalD] = { // Dimension should not exceed kFitTotalD!!!
	"1D Fit to D_{0^{+}_{h}}",
	"1D Fit to D_{0^{-}}",
	"1D Fit to D_{0^{+}_{h}-0^{-}}",
	"1D Fit to D_{0^{+}_{h}}",
	"1D Fit to D_{Int}",
	"1D Fit to D_{CP}",
	"1D Fit to D_{0^{+}_{h}-0^{-}, Int}",
	"1D Fit to D_{CP}"
};
char* str2DFit_label[kFitTotalD/2] = { // Dimension should not exceed kFitTotalD/2!!!
	"2D Fit to D_{0^{+}_{h}} and D_{Int}",
	"2D Fit to D_{0^{-}} and D_{CP}",
	"2D Fit to D_{0^{+}_{h}-0^{-}} and D_{0^{+}_{h}-0^{-} Int}",
	"2D Fit to D_{0^{+}_{h}} and D_{0^{-}}"
};
char* str3DFit_label[2] = { // Dimension should not exceed 2!!!
	"3D Fit to D_{0^{+}_{h}}, D_{0^{-}} and D_{Int}",
	"3D Fit to D_{0^{+}_{h}}, D_{0^{-}} and D_{CP}"
};


const int kFitTotalExtraD = 8;
char* strFitExtraDim[kFitTotalExtraD] = { // Dimension should not exceed kFitTotalExtraD!!!
	"D_g1_vs_g2_phi0",
	"D_g1_vs_g4_phi0",
	"D_g2_vs_g4_phi0",
/* Start extra D templates from here, at strFitExtraDim[t=3], everything above is redundant. */
	"D_g1Q2_phi0",
	"D_g2intPdf_phi90",
	"D_g4intPdf_phi90",
	"D_g2g4intPdf_phi90",
	"D_g1Q2intPdf_phi0"
};
char* str1DExtraFit_title[kFitTotalExtraD] = { // Dimension should not exceed kFitTotalExtraD!!!
	"T_1D_D_g1_vs_g2_phi0",
	"T_1D_D_g1_vs_g4_phi0",
	"T_1D_D_g2_vs_g4_phi0",
	"T_1D_D_g1Q2_phi0",
	"T_1D_D_g2intPdf_phi90",
	"T_1D_D_g4intPdf_phi90",
	"T_1D_D_g2g4intPdf_phi90",
	"T_1D_D_g1Q2intPdf_phi0"
};
char* str2DExtraFit_title[kFitTotalExtraD/2] = { // Dimension should not exceed kFitTotalExtraD/2!!!
	"T_2D_g1_vs_g2_withperp",
	"T_2D_g1_vs_g4_withperp",
	"T_2D_g2_vs_g4_withperp",
	"T_2D_g1_vs_g1Q2"
};
char* str3DExtraFit_title[2] = { // Dimension should not exceed 2!!!
	"T_3D_Dg2Dg4Dg2intPerp",
	"T_3D_Dg2Dg4Dg4intPerp"
};

char* strFitExtraDim_label[kFitTotalExtraD] = { // Dimension should not exceed kFitTotalextraD!!!
	"D_{0^{+}_{h}}",
	"D_{0^{-}}",
	"D_{0^{+}_{h}-0^{-}}",
	"D_{#epsilon}",
	"D_{Int_{#perp}}",
	"D_{CP_{#perp}}",
	"D_{0^{+}_{h}-0^{-} Int_{#perp}}",
	"D_{#epsilon Int}"
};
char* str1DExtraFit_label[kFitTotalExtraD] = { // Dimension should not exceed kFitTotalExtraD!!!
	"1D Fit to D_{0^{+}_{h}}",
	"1D Fit to D_{0^{-}}",
	"1D Fit to D_{0^{+}_{h}-0^{-}}",
	"1D Fit to D_{#epsilon}",
	"1D Fit to D_{Int_{#perp}}",
	"1D Fit to D_{CP_{#perp}}",
	"1D Fit to D_{0^{+}_{h}-0^{-}, Int_{#perp}}",
	"1D Fit to D_{#epsilon Int}"
};
char* str2DExtraFit_label[kFitTotalExtraD/2] = { // Dimension should not exceed kFitTotalExtraD/2!!!
	"2D Fit to D_{0^{+}_{h}} and D_{Int_{#perp}}",
	"2D Fit to D_{0^{-}} and D_{CP_{#perp}}",
	"2D Fit to D_{0^{+}_{h}-0^{-}} and D_{0^{+}_{h}-0^{-} Int_{#perp}}",
	"2D Fit to D_{#epsilon} and D_{#epsilon Int _{#perp}}"
};
char* str3DExtraFit_label[2] = { // Dimension should not exceed 2!!!
	"3D Fit to D_{0^{+}_{h}}, D_{0^{-}} and D_{Int_{#perp}}",
	"3D Fit to D_{0^{+}_{h}}, D_{0^{-}} and D_{CP_{#perp}}"
};
