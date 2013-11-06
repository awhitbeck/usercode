#include <iostream>
#include <fstream>
#include <vector>
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooMsgService.h"
#include "RooFitResult.h"
#include "TList.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "../src/RooSpinZero_KDInt_ZH_Simultaneous_PMF.h"
#include "../src/RooSpinZero_KD_ZH_Simultaneous_PMF.h"
#include "ZZ4l_125_6_Samples.h"
#include "FitDimensionsList.h"
#include "IdealSamples_Cuts.h"

using namespace RooFit;
using namespace std;

const double totalxsec_gi1[4]={ 1.0, 0.3726, 0.1573, 1.0133 }; // 0+m, 0+h, 0-, 0+m0+h interference net contributions, else is 0


int execute_fitSimultaneous_2D (sample smp, const int erg_tev, const double fixval_fg2=-1, const double fixval_fg4=-1, const int iteration=0, int iteration_oldfirstevent=0, const int event_selection=0, const int ntests=200, const int nevents=600){
	bool fix_fg2=false, fix_fg4=false;
	char cfix_fg2[]="_fg2";
	char cfix_fg4[]="_fg4";
	char cfixval_fg2[10];
	char cfixval_fg4[10];
	if(fixval_fg2>=0.0 && fixval_fg2<=1.0){
		fix_fg2=true;
		if(fixval_fg2==gi_phi2_phi4[smp][6]) sprintf(cfixval_fg2,"%s%s",cfix_fg2,"Sample");
		else sprintf(cfixval_fg2,"%s%.0f",cfix_fg2,10*fixval_fg2);
	};
	if(fixval_fg4>=0.0 && fixval_fg4<=1.0){
		fix_fg4=true;
		if(fixval_fg4==gi_phi2_phi4[smp][7]) sprintf(cfixval_fg4,"%s%s",cfix_fg4,"Sample");
		else sprintf(cfixval_fg4,"%s%.0f",cfix_fg4,10*fixval_fg4);
	};
	if((fix_fg2 && fix_fg4) || ntests<0){
		cout << "No fit is possible!" << endl;
		abort();
	};
	string selection_title=idealdetector_cut_title[event_selection];

	RooFit::Verbose(false);
	RooFit::PrintLevel(-1000);
	RooMsgService::instance().setStreamStatus(1,false);
	RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
	cout << "Starting..." << endl;
	string INPUT_NAME = "Templates/ggtoHtoZZ4l_125_6_IntTmps_Dg_Dgint_";
	string SAMPLE_INPUT_NAME = "ggtoHtoZZ4l_VariousKDs_";
	string OUTPUT_NAME = "2DFits/ggtoHtoZZ4l_125_6_PseudoExp_PMF_withPerp_";
	if(nevents!=600){
		char cnevents[20];
		sprintf(cnevents,"%i%s",nevents,"Events_");
		OUTPUT_NAME = OUTPUT_NAME + cnevents;
	};
	if(event_selection!=0){
		OUTPUT_NAME = OUTPUT_NAME + selection_title;
		OUTPUT_NAME = OUTPUT_NAME + "_";
	};
	OUTPUT_NAME = OUTPUT_NAME + "Sample_";

	char erg_dir[1000];
	sprintf(erg_dir,"LHC_%iTeV/2DFitAnalysis/",erg_tev);
	string cinput_common = user_dir + erg_dir;

	TFile* finput[6];
	TH1F* h1D[6*(kFitTotalExtraD/2-1)]; // "0+m","0+h","0-","0+m0+h","0+m0-","0+h0-"
	TH2F* h2D[6*(kFitTotalExtraD/2-1)];
	for(int f=0;f<6;f++){
		string cinput = cinput_common + INPUT_NAME + sampleName[f] + ".root";
		finput[f] = new TFile(cinput.c_str(),"read");

		for(int t=0;t<(kFitTotalExtraD/2-1);t++){
			string templatename = str1DExtraFit_title[t+kFitTotalExtraD/2];
			if(event_selection!=0) templatename = templatename + "_" + idealdetector_cut_title[event_selection];
			h1D[6*t+f] = (TH1F*) finput[f]->Get(templatename.c_str());
			*h1D[6*t+f] = *h1D[6*t+f] * (sample_xsec_ratio[f]);
		};
		for(int t=0;t<(kFitTotalExtraD/2-1);t++){
			string templatename = str2DExtraFit_title[t];
			if(event_selection!=0) templatename = templatename + "_" + idealdetector_cut_title[event_selection];
			h2D[6*t+f] = (TH2F*) finput[f]->Get(templatename.c_str());
			*h2D[6*t+f] = *h2D[6*t+f] * (sample_xsec_ratio[f]);
		};
	};
	for(int t=0;t<(kFitTotalExtraD/2-1);t++){
		h1D[6*t+3]->Add(h1D[6*t+1],(-pow(gi_phi2_phi4[3][1],2.0)));
		h1D[6*t+4]->Add(h1D[6*t+2],(-pow(gi_phi2_phi4[4][3],2.0)));
		h1D[6*t+5]->Add(h1D[6*t+2],-1.0);
		h1D[6*t+3]->Add(h1D[6*t+0],-1.0);
		h1D[6*t+4]->Add(h1D[6*t+0],-1.0);
		h1D[6*t+5]->Add(h1D[6*t+1],(-pow(gi_phi2_phi4[5][1],2.0)));

		h1D[6*t+5]->Scale((gi_phi2_phi4[3][1]*gi_phi2_phi4[4][3]/gi_phi2_phi4[5][1]));

		h1D[6*t+0]->Scale(pow(gi_phi2_phi4[3][0],2.0));
		h1D[6*t+1]->Scale(pow(gi_phi2_phi4[3][1],2.0));
		h1D[6*t+2]->Scale(pow(gi_phi2_phi4[4][3],2.0));

		h1D[6*t+3]->Scale(1.0/(gi_phi2_phi4[3][1]*sqrt(sample_xsec_ratio[0]*sample_xsec_ratio[1])));
		h1D[6*t+4]->Scale(1.0/(gi_phi2_phi4[4][3]*sqrt(sample_xsec_ratio[0]*sample_xsec_ratio[2])));
		h1D[6*t+5]->Scale(1.0/(gi_phi2_phi4[3][1]*gi_phi2_phi4[4][3]*sqrt(sample_xsec_ratio[2]*sample_xsec_ratio[1])));

		h1D[6*t+0]->Scale(1.0/(sample_xsec_ratio[0]*pow(gi_phi2_phi4[3][0],2.0)));
		h1D[6*t+1]->Scale(1.0/(sample_xsec_ratio[1]*pow(gi_phi2_phi4[3][1],2.0)));
		h1D[6*t+2]->Scale(1.0/(sample_xsec_ratio[2]*pow(gi_phi2_phi4[4][3],2.0)));

		h2D[6*t+3]->Add(h2D[6*t+1],(-pow(gi_phi2_phi4[3][1],2.0)));
		h2D[6*t+4]->Add(h2D[6*t+2],(-pow(gi_phi2_phi4[4][3],2.0)));
		h2D[6*t+5]->Add(h2D[6*t+2],-1.0);
		h2D[6*t+3]->Add(h2D[6*t+0],-1.0);
		h2D[6*t+4]->Add(h2D[6*t+0],-1.0);
		h2D[6*t+5]->Add(h2D[6*t+1],(-pow(gi_phi2_phi4[5][1],2.0)));

		h2D[6*t+5]->Scale((gi_phi2_phi4[3][1]*gi_phi2_phi4[4][3]/gi_phi2_phi4[5][1]));

		h2D[6*t+0]->Scale(pow(gi_phi2_phi4[3][0],2.0));
		h2D[6*t+1]->Scale(pow(gi_phi2_phi4[3][1],2.0));
		h2D[6*t+2]->Scale(pow(gi_phi2_phi4[4][3],2.0));

		h2D[6*t+3]->Scale(1.0/(gi_phi2_phi4[3][1]*sqrt(sample_xsec_ratio[0]*sample_xsec_ratio[1])));
		h2D[6*t+4]->Scale(1.0/(gi_phi2_phi4[4][3]*sqrt(sample_xsec_ratio[0]*sample_xsec_ratio[2])));
		h2D[6*t+5]->Scale(1.0/(gi_phi2_phi4[3][1]*gi_phi2_phi4[4][3]*sqrt(sample_xsec_ratio[2]*sample_xsec_ratio[1])));

		h2D[6*t+0]->Scale(1.0/(sample_xsec_ratio[0]*pow(gi_phi2_phi4[3][0],2.0)));
		h2D[6*t+1]->Scale(1.0/(sample_xsec_ratio[1]*pow(gi_phi2_phi4[3][1],2.0)));
		h2D[6*t+2]->Scale(1.0/(sample_xsec_ratio[2]*pow(gi_phi2_phi4[4][3],2.0)));
	};

	unsigned int flavor = 0;
	double kDarray[kFitTotalExtraD]={0};
	float lepton_pT[4]={0};
	float lepton_eta[4]={0};

	string csample_input = cinput_common + SAMPLE_INPUT_NAME + sample_suffix[smp] + ".root";
	TFile* fsample_input = new TFile(csample_input.c_str(),"read");
	TTree* tree_full = (TTree*) fsample_input->Get("SelectedTree");
	tree_full->SetBranchStatus("*",0);
	for(int t=0;t<kFitTotalExtraD;t++){
		if(t==kFitTotalExtraD/2-1 || t==kFitTotalExtraD-1) continue;
		tree_full->SetBranchStatus(strFitExtraDim[t],1);
		tree_full->SetBranchAddress(strFitExtraDim[t],(kDarray+t));
		cout << strFitDim[t] << " is taken." << endl;
	};
	tree_full->SetBranchStatus("flavortype",1);
	tree_full->SetBranchAddress("flavortype",&flavor);

	tree_full->SetBranchStatus("l1m_pT",1);
	tree_full->SetBranchStatus("l1p_pT",1);
	tree_full->SetBranchStatus("l2m_pT",1);
	tree_full->SetBranchStatus("l2p_pT",1);
	tree_full->SetBranchStatus("l1m_eta",1);
	tree_full->SetBranchStatus("l1p_eta",1);
	tree_full->SetBranchStatus("l2m_eta",1);
	tree_full->SetBranchStatus("l2p_eta",1);
	tree_full->SetBranchAddress("l1m_pT",(lepton_pT+0));
	tree_full->SetBranchAddress("l1p_pT",(lepton_pT+1));
	tree_full->SetBranchAddress("l2m_pT",(lepton_pT+2));
	tree_full->SetBranchAddress("l2p_pT",(lepton_pT+3));
	tree_full->SetBranchAddress("l1m_eta",(lepton_eta+0));
	tree_full->SetBranchAddress("l1p_eta",(lepton_eta+1));
	tree_full->SetBranchAddress("l2m_eta",(lepton_eta+2));
	tree_full->SetBranchAddress("l2p_eta",(lepton_eta+3));

	TTree* toy_tree[ntests];
	int iteration_newfirstevent=iteration_oldfirstevent;
	if(iteration!=0 && iteration_newfirstevent==0){
		int catchup=0;
		while(catchup<(nevents*ntests*iteration)){
			tree_full->GetEntry(iteration_newfirstevent);
			++iteration_newfirstevent;

			bool skip_event = false;
			if( (event_selection==0 || event_selection==2) && flavor!=3 ) skip_event = true;
			if( (event_selection==1 || event_selection==3) && !(flavor==0 || flavor==1) ) skip_event = true;
			if(event_selection>=2){
				for(int lep=0;lep<4;lep++){
					if(lepton_pT[lep]<=idealdetector_pT_selection[event_selection-2][lep]) skip_event=true;
					if(abs(lepton_eta[lep])>=idealdetector_eta_selection[event_selection-2][lep]) skip_event=true;
				};
			};
			if(skip_event) continue;

			++catchup;
		};
	};
	for(int test=0;test<ntests;test++){
		toy_tree[test] = new TTree("SelectedToyTree","SelectedToyTree");
		for(int t=0;t<kFitTotalExtraD;t++){
			if(t==kFitTotalExtraD/2-1 || t==kFitTotalExtraD-1) continue;
			toy_tree[test]->Branch(strFitExtraDim[t],(kDarray+t));
		};

		int ev=0;
		while(ev<nevents){
			tree_full->GetEntry(iteration_newfirstevent);
			++iteration_newfirstevent;

			bool skip_event = false;
			if( (event_selection==0 || event_selection==2) && flavor!=3 ) skip_event = true;
			if( (event_selection==1 || event_selection==3) && !(flavor==0 || flavor==1) ) skip_event = true;
			if(event_selection>=2){
				for(int lep=0;lep<4;lep++){
					if(lepton_pT[lep]<=idealdetector_pT_selection[event_selection-2][lep]) skip_event=true;
					if(abs(lepton_eta[lep])>=idealdetector_eta_selection[event_selection-2][lep]) skip_event=true;
				};
			};
			if(skip_event) continue;

			++ev;
			toy_tree[test]->Fill();
		};
	};
	cout << "Old iteration was " << iteration_oldfirstevent << endl;
	cout << "New iteration was " << iteration_newfirstevent << endl;

	string coutput = cinput_common + OUTPUT_NAME + sampleName[smp];
	char citeration[100];
	sprintf(citeration,"_%i",iteration);
	coutput = coutput + citeration;
	if(fix_fg2 || fix_fg4) coutput = coutput + "_Fixed";
	if(fix_fg2) coutput = coutput + cfixval_fg2;
	if(fix_fg4) coutput = coutput + cfixval_fg4;
	coutput = coutput + ".root";
	TFile* foutput = new TFile(coutput.c_str(),"recreate");
	for(int t=0;t<(kFitTotalExtraD/2-1);t++){
		for(int f=0;f<6;f++){
			string oldname = h1D[6*t+f]->GetName();
			string newname = oldname + "_" + sample_suffix[f];
			if(f>=3) newname = newname + "Int";
			h1D[6*t+f]->SetName(newname.c_str());
//			foutput->WriteTObject(h1D[6*t+f]);
		};

		for(int f=0;f<6;f++){
			string oldname = h2D[6*t+f]->GetName();
			string newname = oldname + "_" + sample_suffix[f];
			if(f>=3) newname = newname + "Int";
			h2D[6*t+f]->SetName(newname.c_str());
//			foutput->WriteTObject(h2D[6*t+f]);
		};
	};
	for(int t=0;t<(kFitTotalExtraD/2-1);t++){
		string kd1_name;
		string kd12_name;
		string kd2_name;

		string histo1D_fg2;
		string histo1D_fg4;
		string histo2D_fg2;
		string histo2D_fg4;

		string str1Dtree = "Fit_1D_";
		string str2Dtree = "Fit_2D_";

		TH1F* hfg2_1D;
		TH1F* hfg4_1D;
		TH1F* hfg2_2D;
		TH1F* hfg4_2D;

		str1Dtree = str1Dtree + str1DExtraFit_title[t+kFitTotalExtraD/2];

		kd1_name = strFitExtraDim[t];
		kd2_name = strFitExtraDim[t+kFitTotalExtraD/2];

		RooRealVar* kD1;
		RooRealVar* kD2;
		kD1 = new RooRealVar(kd1_name.c_str(),kd1_name.c_str(),0.0,1.0);
		kD2 = new RooRealVar(kd2_name.c_str(),kd2_name.c_str(),-1.0,1.0);


		histo1D_fg2 = str1Dtree + "_fg2Result";
		histo1D_fg4 = str1Dtree + "_fg4Result";
		hfg2_1D = new TH1F(histo1D_fg2.c_str(),histo1D_fg2.c_str(),25,-1,1);
		hfg2_1D->SetXTitle("f_{g_{2}}");
		hfg2_1D->SetYTitle("Weighed Distribution");
		hfg2_1D->Sumw2();
		hfg4_1D = new TH1F(histo1D_fg4.c_str(),histo1D_fg4.c_str(),25,-1,1);
		hfg4_1D->SetXTitle("f_{g_{4}}");
		hfg4_1D->SetYTitle("Weighed Distribution");
		hfg4_1D->Sumw2();

		RooRealVar* f_g2;
		RooRealVar* f_g4;

		if(fix_fg2){
			f_g2 = new RooRealVar("fg2","fg2",fixval_fg2,fixval_fg2,fixval_fg2);
			f_g4 = new RooRealVar("fg4","fg4",gi_phi2_phi4[smp][7],-1.0+fixval_fg2,1.0-fixval_fg2);
		}
		else if(fix_fg4){
			f_g2 = new RooRealVar("fg2","fg2",gi_phi2_phi4[smp][6],-1.0+fixval_fg4,1.0-fixval_fg4);
			f_g4 = new RooRealVar("fg4","fg4",fixval_fg4,fixval_fg4,fixval_fg4);
		}
		else{
			f_g2 = new RooRealVar("fg2","fg2",(gi_phi2_phi4[smp][6]+gi_phi2_phi4[smp][7]),-1.0,1.0);
			f_g4 = new RooRealVar("fg4","fg4",gi_phi2_phi4[smp][7]/(gi_phi2_phi4[smp][6]+gi_phi2_phi4[smp][7]),-1.0,1.0);
		};
/*

		if(fix_fg2){
			f_g2 = new RooRealVar("fg2","fg2",fixval_fg2,fixval_fg2,fixval_fg2);
			f_g4 = new RooRealVar("fg4","fg4",gi_phi2_phi4[smp][7],0,1.0-fixval_fg2);
		}
		else if(fix_fg4){
			f_g2 = new RooRealVar("fg2","fg2",gi_phi2_phi4[smp][6],0,1.0-fixval_fg4);
			f_g4 = new RooRealVar("fg4","fg4",fixval_fg4,fixval_fg4,fixval_fg4);
		}
		else{
			f_g2 = new RooRealVar("fg2","fg2",(gi_phi2_phi4[smp][6]+gi_phi2_phi4[smp][7]),0,1.0);
			f_g4 = new RooRealVar("fg4","fg4",gi_phi2_phi4[smp][7]/(gi_phi2_phi4[smp][6]+gi_phi2_phi4[smp][7]),0,1.0);
		};
*/
		vector<TH1F*> histos_1D;
		vector<TH2F*> histos_2D;
		for(int f=0;f<6;f++) histos_1D.push_back(h1D[6*t+f]);

		RooSpinZero_KD_ZH_Simultaneous_PMF* pdf_1D;
		RooSpinZero_KDInt_ZH_Simultaneous_PMF* pdf_2D;
		pdf_1D = new RooSpinZero_KD_ZH_Simultaneous_PMF("pdf_1D","pdf_1D",*kD2,*f_g2,*f_g4,histos_1D,false,fix_fg2,fix_fg4);

		double fg2val,fg2err,fg4val,fg4err,minLL;

		foutput->cd();
		TTree* fit_1D = new TTree(str1Dtree.c_str(),str1Dtree.c_str());
		TTree* fit_2D;
		fit_1D->Branch("fg2",&fg2val,"fg2/D");
		fit_1D->Branch("fg2_error",&fg2err,"fg2_error/D");
		fit_1D->Branch("fg4",&fg4val,"fg4/D");
		fit_1D->Branch("fg4_error",&fg4err,"fg4_error/D");
		fit_1D->Branch("minLL_avg",&minLL,"minLL_avg/D");

		str2Dtree = str2Dtree + str2DExtraFit_title[t];
		histo2D_fg2 = str2Dtree + "_fg2Result";
		histo2D_fg4 = str2Dtree + "_fg4Result";
		hfg2_2D = new TH1F(histo2D_fg2.c_str(),histo2D_fg2.c_str(),25,-1,1);
		hfg2_2D->SetXTitle("f_{g_{2}}");
		hfg2_2D->SetYTitle("Weighed Distribution");
		hfg2_2D->Sumw2();
		hfg4_2D = new TH1F(histo2D_fg4.c_str(),histo2D_fg4.c_str(),25,-1,1);
		hfg4_2D->SetXTitle("f_{g_{4}}");
		hfg4_2D->SetYTitle("Weighed Distribution");
		hfg4_2D->Sumw2();

		for(int f=0;f<6;f++) histos_2D.push_back(h2D[6*t+f]);
		pdf_2D = new RooSpinZero_KDInt_ZH_Simultaneous_PMF("pdf_2D","pdf_2D",*kD1,*kD2,*f_g2,*f_g4,histos_2D,false,fix_fg2,fix_fg4);

		fit_2D = new TTree(str2Dtree.c_str(),str2Dtree.c_str());
		fit_2D->Branch("fg2",&fg2val,"fg2/D");
		fit_2D->Branch("fg2_error",&fg2err,"fg2_error/D");
		fit_2D->Branch("fg4",&fg4val,"fg4/D");
		fit_2D->Branch("fg4_error",&fg4err,"fg4_error/D");
		fit_2D->Branch("minLL_avg",&minLL,"minLL_avg/D");

		for(int test=0;test<ntests;test++){
//			cout << "Initializing test: " << test << endl;

/*
			if(!fix_fg2) f_g2->setVal(0);
			if(!fix_fg4) f_g4->setVal(0);
*/
			if(fix_fg2){
				f_g4->setVal(gi_phi2_phi4[smp][7]);
			}
			else if(fix_fg4){
				f_g2->setVal(gi_phi2_phi4[smp][6]);
			}
			else{
				f_g2->setVal((gi_phi2_phi4[smp][6]+gi_phi2_phi4[smp][7]));
				f_g4->setVal(gi_phi2_phi4[smp][7]/(gi_phi2_phi4[smp][6]+gi_phi2_phi4[smp][7]));
			};

			RooDataSet* toy_data_1D;
			RooDataSet* toy_data_2D;
			toy_data_1D = new RooDataSet("toy_data_1D","toy_data_1D", toy_tree[test], RooArgSet(*kD2));
			RooFitResult* sum_res_pdf1D = pdf_1D->fitTo(*toy_data_1D,Save(true),PrintLevel(-1));
			minLL = sum_res_pdf1D->minNll();
			minLL /= nevents;

//			if( t<(kFitTotalExtraD/2-1) ) cout << "Embedded Toy " << test << "in 1D: " << kD1->GetName() << endl;
//			else cout << "Embedded Toy " << test << "in 1D: " << kD12->GetName() << endl;

			fg2val = (f_g2->getVal());
			fg2err = (f_g2->getError());
			fg4val = (f_g4->getVal());
			fg4err = (f_g4->getError());

			if(!fix_fg2 && !fix_fg4){
				double fg2newval=fg2val*(1-abs(fg4val));
				double fg4newval=abs(fg2val)*fg4val;
				double fg2newerr=sqrt(pow((fg2err*(1-abs(fg4val))),2.0)+pow((fg4err*fg2val),2.0));
				double fg4newerr=sqrt(pow((fg2err*fg4val),2.0)+pow((fg4err*abs(fg2val)),2.0));
//				double fg2newerr=sqrt(pow((fg2err*(1-fg4val))+(fg4err*fg2val),2.0));
//				double fg4newerr=sqrt(pow((fg2err*fg4val)+(fg4err*fg2val),2.0));
				fg2val=fg2newval;
				fg2err=fg2newerr;
				fg4val=fg4newval;
				fg4err=fg4newerr;
			};

			fit_1D->Fill();

			hfg2_1D->Fill(fg2val,1.0);
			hfg4_1D->Fill(fg4val,1.0);

			delete toy_data_1D;

			toy_data_2D = new RooDataSet("toy_data_2D","toy_data_2D", toy_tree[test], RooArgSet(*kD1,*kD2));
/*
			if(!fix_fg2) f_g2->setVal(0);
			if(!fix_fg4) f_g4->setVal(0);
*/
			if(fix_fg2){
				f_g4->setVal(gi_phi2_phi4[smp][7]);
			}
			else if(fix_fg4){
				f_g2->setVal(gi_phi2_phi4[smp][6]);
			}
			else{
				f_g2->setVal((gi_phi2_phi4[smp][6]+gi_phi2_phi4[smp][7]));
				f_g4->setVal(gi_phi2_phi4[smp][7]/(gi_phi2_phi4[smp][6]+gi_phi2_phi4[smp][7]));
			};

			RooFitResult* sum_res_pdf2D = pdf_2D->fitTo(*toy_data_2D,Save(true),PrintLevel(-1));
			minLL = sum_res_pdf2D->minNll();
			minLL /= nevents;

//			cout << "Embedded Toy " << test << "in 2D: " << kD1->GetName() << '\t' << kD2->GetName() << endl;

			fg2val = (f_g2->getVal());
			fg2err = (f_g2->getError());
			fg4val = (f_g4->getVal());
			fg4err = (f_g4->getError());

			if(!fix_fg2 && !fix_fg4){
				double fg2newval=fg2val*(1-abs(fg4val));
				double fg4newval=abs(fg2val)*fg4val;
				double fg2newerr=sqrt(pow((fg2err*(1-abs(fg4val))),2.0)+pow((fg4err*fg2val),2.0));
				double fg4newerr=sqrt(pow((fg2err*fg4val),2.0)+pow((fg4err*abs(fg2val)),2.0));
//				double fg2newerr=sqrt(pow((fg2err*(1-fg4val))+(fg4err*fg2val),2.0));
//				double fg4newerr=sqrt(pow((fg2err*fg4val)+(fg4err*fg2val),2.0));
				fg2val=fg2newval;
				fg2err=fg2newerr;
				fg4val=fg4newval;
				fg4err=fg4newerr;
			};

			hfg2_2D->Fill(fg2val,1.0);
			hfg4_2D->Fill(fg4val,1.0);

			fit_2D->Fill();

			delete toy_data_2D;
//			cout << "Finished test: " << test << endl;
		};
		double hfg2_1Dint = hfg2_1D->Integral();
		hfg2_1D->Scale(1.0/hfg2_1Dint);
		double hfg4_1Dint = hfg4_1D->Integral();
		hfg4_1D->Scale(1.0/hfg4_1Dint);
		foutput->WriteTObject(fit_1D);
		foutput->WriteTObject(hfg2_1D);
		foutput->WriteTObject(hfg4_1D);

		double hfg2_2Dint = hfg2_2D->Integral();
		hfg2_2D->Scale(1.0/hfg2_2Dint);
		double hfg4_2Dint = hfg4_2D->Integral();
		hfg4_2D->Scale(1.0/hfg4_2Dint);
		foutput->WriteTObject(fit_2D);
		foutput->WriteTObject(hfg2_2D);
		foutput->WriteTObject(hfg4_2D);
		
		delete f_g4;
		delete f_g2;
		delete kD1;
		delete fit_1D;
		delete hfg2_1D;
		delete hfg4_1D;
		delete kD2;
		delete fit_2D;
		delete hfg2_2D;
		delete hfg4_2D;

//		cout << "Finished dimension: " << t << endl;
	};


	foutput->Close();
	fsample_input->Close();
	for(int f=0;f<6;f++){
		finput[f]->Close();
	};
	return iteration_newfirstevent;
};

void fitSimultaneous_2D_PMF_withPerp (sample smp, const int erg_tev, const int iteration_min=0, const int iteration_max=5, const int event_selection=0, const int ntests=200, const int nevents=600){
	int lastevent[5]={0,0,0,0,0};
	for(int iter=iteration_min;iter<iteration_max;iter++){
/*		lastevent[0] = execute_fitSimultaneous_2D(smp,erg_tev,-1,-1,iter,lastevent[0],event_selection,ntests,nevents);
		lastevent[1] = execute_fitSimultaneous_2D(smp,erg_tev,0,-1,iter,lastevent[1],event_selection,ntests,nevents);
		lastevent[2] = execute_fitSimultaneous_2D(smp,erg_tev,-1,0,iter,lastevent[2],event_selection,ntests,nevents);
*/		lastevent[3] = execute_fitSimultaneous_2D(smp,erg_tev,gi_phi2_phi4[smp][6],-1,iter,lastevent[3],event_selection,ntests,nevents);
		lastevent[4] = execute_fitSimultaneous_2D(smp,erg_tev,-1,gi_phi2_phi4[smp][7],iter,lastevent[4],event_selection,ntests,nevents);
	};
};