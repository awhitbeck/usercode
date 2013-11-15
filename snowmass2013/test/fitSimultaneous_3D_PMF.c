#include <iostream>
#include <fstream>
#include <vector>
#include "TH1F.h"
#include "TH3F.h"
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
#include "../src/RooSpinZero_3DKD_ZH_Simultaneous_PMF.h"
#include "ZZ4l_125_6_Samples.h"
#include "FitDimensionsList.h"
#include "IdealSamples_Cuts.h"

using namespace RooFit;
using namespace std;

const double totalxsec_gi1[4]={ 1.0, 0.3726, 0.1573, 1.0133 }; // 0+m, 0+h, 0-, 0+m0+h interference net contributions, else is 0


int execute_fitSimultaneous_3D (sample smp, const int erg_tev, const double fixval_fg2=-1, const double fixval_fg4=-1, const int iteration=0, int iteration_oldfirstevent=0, const int event_selection=0, const int ntests=200, const int nevents=600){
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
	string OUTPUT_NAME = "ggtoHtoZZ4l_125_6_PseudoExp_3D_PMF_";
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
	TH3F* h3D[6*2];
	for(int f=0;f<6;f++){
		string cinput = cinput_common + INPUT_NAME + sampleName[f] + ".root";
		finput[f] = new TFile(cinput.c_str(),"read");

		for(int t=0;t<2;t++){
			string templatename = str3DFit_title[t];
			if(event_selection!=0) templatename = templatename + "_" + idealdetector_cut_title[event_selection];
			h3D[6*t+f] = (TH3F*) finput[f]->Get(templatename.c_str());
			*h3D[6*t+f] = *h3D[6*t+f] * (sample_xsec_ratio[f]);
		};
	};
	for(int t=0;t<2;t++){
		h3D[6*t+3]->Add(h3D[6*t+1],(-pow(gi_phi2_phi4[3][1],2.0)));
		h3D[6*t+4]->Add(h3D[6*t+2],(-pow(gi_phi2_phi4[4][3],2.0)));
		h3D[6*t+5]->Add(h3D[6*t+2],-1.0);
		h3D[6*t+3]->Add(h3D[6*t+0],-1.0);
		h3D[6*t+4]->Add(h3D[6*t+0],-1.0);
		h3D[6*t+5]->Add(h3D[6*t+1],(-pow(gi_phi2_phi4[5][1],2.0)));

		h3D[6*t+5]->Scale((gi_phi2_phi4[3][1]*gi_phi2_phi4[4][3]/gi_phi2_phi4[5][1]));

		h3D[6*t+0]->Scale(pow(gi_phi2_phi4[3][0],2.0));
		h3D[6*t+1]->Scale(pow(gi_phi2_phi4[3][1],2.0));
		h3D[6*t+2]->Scale(pow(gi_phi2_phi4[4][3],2.0));

		h3D[6*t+3]->Scale(1.0/(gi_phi2_phi4[3][1]*sqrt(sample_xsec_ratio[0]*sample_xsec_ratio[1])));
		h3D[6*t+4]->Scale(1.0/(gi_phi2_phi4[4][3]*sqrt(sample_xsec_ratio[0]*sample_xsec_ratio[2])));
		h3D[6*t+5]->Scale(1.0/(gi_phi2_phi4[3][1]*gi_phi2_phi4[4][3]*sqrt(sample_xsec_ratio[2]*sample_xsec_ratio[1])));

		h3D[6*t+0]->Scale(1.0/(sample_xsec_ratio[0]*pow(gi_phi2_phi4[3][0],2.0)));
		h3D[6*t+1]->Scale(1.0/(sample_xsec_ratio[1]*pow(gi_phi2_phi4[3][1],2.0)));
		h3D[6*t+2]->Scale(1.0/(sample_xsec_ratio[2]*pow(gi_phi2_phi4[4][3],2.0)));
	};

	unsigned int flavor = 0;
	double kDarray[4]={0};
	float lepton_pT[4]={0};
	float lepton_eta[4]={0};

	string csample_input = cinput_common + SAMPLE_INPUT_NAME + sample_suffix[smp] + ".root";
	TFile* fsample_input = new TFile(csample_input.c_str(),"read");
	TTree* tree_full = (TTree*) fsample_input->Get("SelectedTree");
	tree_full->SetBranchStatus("*",0);

	for(int t=0;t<2;t++){
		tree_full->SetBranchStatus(strFitDim[t],1);
		tree_full->SetBranchAddress(strFitDim[t],(kDarray+t));
		tree_full->SetBranchStatus(strFitDim[kFitTotalD/2+t],1);
		tree_full->SetBranchAddress(strFitDim[kFitTotalD/2+t],(kDarray+t+2));
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
		for(int t=0;t<2;t++){
			toy_tree[test]->Branch(strFitDim[t],(kDarray+t));
			toy_tree[test]->Branch(strFitDim[t+kFitTotalD/2],(kDarray+t+2));
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

	string coutput = OUTPUT_NAME + sampleName[smp];
	char citeration[100];
	sprintf(citeration,"_%i",iteration);
	coutput = coutput + citeration;
	if(fix_fg2 || fix_fg4) coutput = coutput + "_Fixed";
	if(fix_fg2) coutput = coutput + cfixval_fg2;
	if(fix_fg4) coutput = coutput + cfixval_fg4;
	coutput = coutput + ".root";
	TFile* foutput = new TFile(coutput.c_str(),"recreate");
	for(int t=0;t<2;t++){
		for(int f=0;f<6;f++){
			string oldname = h3D[6*t+f]->GetName();
			string newname = oldname + "_" + sample_suffix[f];
			if(f>=3) newname = newname + "Int";
			h3D[6*t+f]->SetName(newname.c_str());
		};
	};
	for(int t=0;t<2;t++){
		string kd1_name;
		string kd2_name;
		string kd3_name;

		string histo3D_fg2;
		string histo3D_fg4;

		string str3Dtree = "Fit_3D_";

		kd1_name = strFitDim[0];
		kd2_name = strFitDim[1];
		kd3_name = strFitDim[t+kFitTotalD/2];

		RooRealVar* kD1;
		RooRealVar* kD2;
		RooRealVar* kD3;
		kD1 = new RooRealVar(kd1_name.c_str(),kd1_name.c_str(),0.0,1.0);
		kD2 = new RooRealVar(kd2_name.c_str(),kd2_name.c_str(),0,1.0);
		kD3 = new RooRealVar(kd3_name.c_str(),kd3_name.c_str(),-1.0,1.0);

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

		vector<TH3F*> histos_3D;
		RooSpinZero_3DKD_ZH_Simultaneous_PMF* pdf_3D;

		double fg2val,fg2err,fg4val,fg4err,minLL;

		foutput->cd();
		TTree* fit_3D;
		TH1F* hfg2_3D;
		TH1F* hfg4_3D;

		str3Dtree = str3Dtree + str3DFit_title[t];
		histo3D_fg2 = str3Dtree + "_fg2Result";
		histo3D_fg4 = str3Dtree + "_fg4Result";
		hfg2_3D = new TH1F(histo3D_fg2.c_str(),histo3D_fg2.c_str(),25,-1,1);
		hfg2_3D->SetXTitle("f_{g_{2}}");
		hfg2_3D->SetYTitle("Weighed Distribution");
		hfg2_3D->Sumw2();
		hfg4_3D = new TH1F(histo3D_fg4.c_str(),histo3D_fg4.c_str(),25,-1,1);
		hfg4_3D->SetXTitle("f_{g_{4}}");
		hfg4_3D->SetYTitle("Weighed Distribution");
		hfg4_3D->Sumw2();

		for(int f=0;f<6;f++) histos_3D.push_back(h3D[6*t+f]);
		pdf_3D = new RooSpinZero_3DKD_ZH_Simultaneous_PMF("pdf_3D","pdf_3D",*kD1,*kD2,*kD3,*f_g2,*f_g4,histos_3D,false,fix_fg2,fix_fg4);

		fit_3D = new TTree(str3Dtree.c_str(),str3Dtree.c_str());
		fit_3D->Branch("fg2",&fg2val,"fg2/D");
		fit_3D->Branch("fg2_error",&fg2err,"fg2_error/D");
		fit_3D->Branch("fg4",&fg4val,"fg4/D");
		fit_3D->Branch("fg4_error",&fg4err,"fg4_error/D");
		fit_3D->Branch("minLL_avg",&minLL,"minLL_avg/D");

		for(int test=0;test<ntests;test++){
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

			RooDataSet* toy_data_3D;

			toy_data_3D = new RooDataSet("toy_data_3D","toy_data_3D", toy_tree[test], RooArgSet(*kD1,*kD2,*kD3));
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

			RooFitResult* sum_res_pdf3D = pdf_3D->fitTo(*toy_data_3D,Save(true),PrintLevel(-1));
			minLL = sum_res_pdf3D->minNll();
			minLL /= nevents;

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

			hfg2_3D->Fill(fg2val,1.0);
			hfg4_3D->Fill(fg4val,1.0);

			fit_3D->Fill();

			delete toy_data_3D;
		};
//			cout << "Finished test: " << test << endl;

		double hfg2_3Dint = hfg2_3D->Integral();
		hfg2_3D->Scale(1.0/hfg2_3Dint);
		double hfg4_3Dint = hfg4_3D->Integral();
		hfg4_3D->Scale(1.0/hfg4_3Dint);
		foutput->WriteTObject(fit_3D);
		foutput->WriteTObject(hfg2_3D);
		foutput->WriteTObject(hfg4_3D);

		delete f_g4;
		delete f_g2;
		delete kD1;
		delete kD2;
		delete kD3;
		delete fit_3D;
		delete hfg2_3D;
		delete hfg4_3D;

//		cout << "Finished dimension: " << t << endl;
	};


	foutput->Close();
	fsample_input->Close();
	for(int f=0;f<6;f++){
		finput[f]->Close();
	};
	return iteration_newfirstevent;
};

void fitSimultaneous_3D_PMF (sample smp, const int erg_tev, const int iteration_min=0, const int iteration_max=5, const int event_selection=0, const int ntests=200, const int nevents=600){
	int lastevent[5]={0,0,0,0,0};
	for(int iter=iteration_min;iter<iteration_max;iter++){
		lastevent[0] = execute_fitSimultaneous_3D(smp,erg_tev,-1,-1,iter,lastevent[0],event_selection,ntests,nevents);
//		lastevent[3] = execute_fitSimultaneous_3D(smp,erg_tev,gi_phi2_phi4[smp][6],-1,iter,lastevent[3],event_selection,ntests,nevents);
//		lastevent[4] = execute_fitSimultaneous_3D(smp,erg_tev,-1,gi_phi2_phi4[smp][7],iter,lastevent[4],event_selection,ntests,nevents);
	};
};
