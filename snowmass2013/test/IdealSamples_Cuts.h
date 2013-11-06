/* Cuts to ideal events
 * Used for templates and embedded toy fits
 */

char* idealdetector_cut[4]={
	"(flavortype==3)", // 2e2mu all
	"(flavortype==0 || flavortype==1)", // 4+4mu all
	"(flavortype==3 && l1m_pT>5 && l2m_pT>5 && l1p_pT>5 && l2p_pT>5 && abs(l1m_eta)<2.4 && abs(l2m_eta)<2.4 && abs(l1p_eta)<2.4 && abs(l2p_eta)<2.4)",
	"((flavortype==0 || flavortype==1) && l1m_pT>5 && l2m_pT>5 && l1p_pT>5 && l2p_pT>5 && abs(l1m_eta)<2.4 && abs(l2m_eta)<2.4 && abs(l1p_eta)<2.4 && abs(l2p_eta)<2.4)"
};

char* idealdetector_cut_title[4]={
	"",
	"4e4mu",
	"2e2mu_selected",
	"4e4mu_selected"
};
char* idealdetector_cut_label[4]={
	"",
	"4e+4mu",
	"2e-2mu After Selection",
	"4e+4mu After Selection"
};
float idealdetector_pT_selection[2][4]={ // The order goes as l1-, l1+, l2-, l2+
	{5,5,5,5},
	{5,5,5,5}
};
float idealdetector_eta_selection[2][4]={ // The order goes as l1-, l1+, l2-, l2+
	{2.4,2.4,2.4,2.4},
	{2.4,2.4,2.4,2.4}
};
