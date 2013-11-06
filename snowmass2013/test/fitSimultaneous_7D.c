#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
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
#include "../src/ScalarPdfFactory_withFepspr.cc"
#include "ZZ4l_125_6_Samples.h"
#include "FitDimensionsList.h"

using namespace RooFit;
using namespace std;

const double totalxsec_gi1[4]={ 1.0, 0.3726, 0.1573, 1.0133 }; // 0+m, 0+h, 0-, 0+m0+h interference net contributions, else is 0


int execute_fitSimultaneous_7D (sample smp, const int erg_tev, const double fixval_fg2=-1, const double fixval_fg4=-1, const int iteration=0, int iteration_oldfirstevent=0, const int ntests=200, const int nevents=600){
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

	RooFit::Verbose(false);
	RooFit::PrintLevel(-1000);
	RooMsgService::instance().setStreamStatus(1,false);
	RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
	cout << "Starting..." << endl;
	string SAMPLE_INPUT_NAME = "ggtoHtoZZ4l_VariousKDs_";
	string OUTPUT_NAME = "2DFits/ggtoHtoZZ4l_125_6_PseudoExp_7D_";
	if(nevents!=600){
		char cnevents[20];
		sprintf(cnevents,"%i%s",nevents,"Events_");
		OUTPUT_NAME = OUTPUT_NAME + cnevents;
	};
	OUTPUT_NAME = OUTPUT_NAME + "Sample_";

	char erg_dir[1000];
	sprintf(erg_dir,"LHC_%iTeV/2DFitAnalysis/",erg_tev);
	string cinput_common = user_dir + erg_dir;

	unsigned int flavor = 0;
	float kDarray[8]={0};
	char* cDarray[8]={"ZZMass","Z1Mass","Z2Mass","costhetastar","helcosthetaZ1","helcosthetaZ2","helphi","phistarZ1"};

	string csample_input = cinput_common + SAMPLE_INPUT_NAME + sample_suffix[smp] + ".root";
	TFile* fsample_input = new TFile(csample_input.c_str(),"read");
	TTree* tree_full = (TTree*) fsample_input->Get("SelectedTree");
	tree_full->SetBranchStatus("*",0);
	for(int t=0;t<8;t++){
		tree_full->SetBranchStatus(cDarray[t],1);
		tree_full->SetBranchAddress(cDarray[t],(kDarray+t));
		cout << cDarray[t] << " is taken." << endl;
	};
	tree_full->SetBranchStatus("flavortype",1);
	tree_full->SetBranchAddress("flavortype",&flavor);

	TTree* toy_tree[ntests];
	int iteration_newfirstevent=iteration_oldfirstevent;
	if(iteration!=0 && iteration_newfirstevent==0){
		int catchup=0;
		while(catchup<(nevents*ntests*iteration)){
			tree_full->GetEntry(iteration_newfirstevent);
			++iteration_newfirstevent;
			if(flavor!=3) continue;
			++catchup;
		};
	};
	for(int test=0;test<ntests;test++){
		toy_tree[test] = new TTree("SelectedToyTree","SelectedToyTree");
		for(int t=0;t<8;t++){
			toy_tree[test]->Branch(cDarray[t],(kDarray+t));
		};

		int ev=0;
		while(ev<nevents){
			tree_full->GetEntry(iteration_newfirstevent);
			++iteration_newfirstevent;
			if(flavor!=3) continue;
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

	RooRealVar* mzz = new RooRealVar("ZZMass","M_{ZZ} (GeV)",125.6,125.6,125.6);
	RooRealVar* z1mass = new RooRealVar("Z1Mass","m_{Z1} (GeV)",20,110);
	RooRealVar* z2mass = new RooRealVar("Z2Mass","m_{Z2} (GeV)",1e-09,65);
	RooRealVar* hs = new RooRealVar("costhetastar","cos#theta^{*}",-1,1);
	RooRealVar* h1 = new RooRealVar("helcosthetaZ1","cos#theta_{Z1}",-1,1);
	RooRealVar* h2 = new RooRealVar("helcosthetaZ2","cos#theta_{Z2}",-1,1);
	RooRealVar* Phi = new RooRealVar("helphi","#Phi",-PI_VAL,PI_VAL);
	RooRealVar* Phi1 = new RooRealVar("phistarZ1","#Phi_{Z1}",-PI_VAL,PI_VAL);
	RooRealVar* measurables[8]={z1mass,z2mass,hs,h1,h2,Phi,Phi1,mzz};

	string str7Dtree = "Fit_7D";
	string histo7D_fg2 = str7Dtree + "_fg2Result";
	string histo7D_fg4 = str7Dtree + "_fg4Result";

	double fg2val=0,fg2err=0,fg4val=0,fg4err=0,minLL=0;
	double phia2val=0,phia2err=0,phia3val=0,phia3err=0;
	TTree* fit_7D = new TTree(str7Dtree.c_str(),str7Dtree.c_str());
	fit_7D->Branch("fg2",&fg2val,"fg2/D");
	fit_7D->Branch("fg2_error",&fg2err,"fg2_error/D");
	fit_7D->Branch("fg4",&fg4val,"fg4/D");
	fit_7D->Branch("fg4_error",&fg4err,"fg4_error/D");
	fit_7D->Branch("phia2",&phia2val,"phia2/D");
	fit_7D->Branch("phia2_error",&phia2err,"phia2_error/D");
	fit_7D->Branch("phia3",&phia3val,"phia3/D");
	fit_7D->Branch("phia3_error",&phia3err,"phia3_error/D");
	fit_7D->Branch("minLL_avg",&minLL,"minLL_avg/D");

	TH1F* hfg2_7D;
	TH1F* hfg4_7D;
	hfg2_7D = new TH1F(histo7D_fg2.c_str(),histo7D_fg2.c_str(),25,0,1);
	hfg2_7D->SetXTitle("f_{g_{2}}");
	hfg2_7D->SetYTitle("Weighed Distribution");
	hfg2_7D->Sumw2();
	hfg4_7D = new TH1F(histo7D_fg4.c_str(),histo7D_fg4.c_str(),25,0,1);
	hfg4_7D->SetXTitle("f_{g_{4}}");
	hfg4_7D->SetYTitle("Weighed Distribution");
	hfg4_7D->Sumw2();

	for(int test=0;test<ntests;test++){

		ScalarPdfFactory_withFepspr* someHiggs = new ScalarPdfFactory_withFepspr(z1mass,z2mass,hs,h1,h2,Phi,Phi1,mzz,2,false);
		someHiggs->_modelParams.fepspr->setVal(gi_phi2_phi4[smp][8]);
		someHiggs->_modelParams.fa2->setVal(gi_phi2_phi4[smp][6]);
		someHiggs->_modelParams.fa3->setVal(gi_phi2_phi4[smp][7]);
		someHiggs->_modelParams.phia2->setVal(gi_phi2_phi4[smp][4]);
		someHiggs->_modelParams.phia3->setVal(gi_phi2_phi4[smp][5]);
		someHiggs->makeParamsConst(true);
		if(gi_phi2_phi4[smp][6]!=0) someHiggs->_modelParams.fa2->setConstant(false);
		if(gi_phi2_phi4[smp][7]!=0) someHiggs->_modelParams.fa3->setConstant(false);
//		if(smp==0 || smp==21) someHiggs->_modelParams.fepspr->setConstant(false);
		if(gi_phi2_phi4[smp][4]!=0) someHiggs->_modelParams.phia2->setConstant(false);
		if(gi_phi2_phi4[smp][5]!=0) someHiggs->_modelParams.phia3->setConstant(false);
/*
		ScalarPdfFactory_withFepspr* someHiggs = new ScalarPdfFactory_withFepspr(z1mass,z2mass,hs,h1,h2,Phi,Phi1,mzz,1,false);
		someHiggs->_modelParams.g1Val->setVal(gi_phi2_phi4[smp][0]);
		someHiggs->_modelParams.g3Val->setVal(gi_phi2_phi4[smp][2]);
		someHiggs->_modelParams.g2Val->setVal(gi_phi2_phi4[smp][1]*cos(gi_phi2_phi4[smp][4]));
		someHiggs->_modelParams.g4Val->setVal(gi_phi2_phi4[smp][3]*cos(gi_phi2_phi4[smp][5]));
		someHiggs->_modelParams.g2ValIm->setVal(gi_phi2_phi4[smp][1]*sin(gi_phi2_phi4[smp][4]));
		someHiggs->_modelParams.g4ValIm->setVal(gi_phi2_phi4[smp][3]*sin(gi_phi2_phi4[smp][5]));
		someHiggs->makeParamsConst(true);
		someHiggs->_modelParams.g2Val->setConstant(false);
		someHiggs->_modelParams.g4Val->setConstant(false);
//		someHiggs->_modelParams.g2ValIm->setConstant(false);
//		someHiggs->_modelParams.g4ValIm->setConstant(false);
*/
		RooDataSet* toy_data_7D = new RooDataSet("toy_data_1D","toy_data_1D", toy_tree[test], RooArgSet(*z1mass,*z2mass,*hs,*h1,*h2,*Phi,*Phi1));
		RooFitResult* sum_res_pdf7D = someHiggs->PDF->fitTo(*toy_data_7D,Save(true),PrintLevel(-1));

		fg2val = someHiggs->_modelParams.fa2->getVal();
		fg2err = someHiggs->_modelParams.fa2->getError();
		fg4val = someHiggs->_modelParams.fa3->getVal();
		fg4err = someHiggs->_modelParams.fa3->getError();
		phia2val = someHiggs->_modelParams.phia2->getVal();
		phia2err = someHiggs->_modelParams.phia2->getError();
		phia3val = someHiggs->_modelParams.phia3->getVal();
		phia3err = someHiggs->_modelParams.phia3->getError();
/*
		double g1fit = someHiggs->_modelParams.g1Val->getVal();
		double reg2fit = someHiggs->_modelParams.g2Val->getVal();
		double reg4fit = someHiggs->_modelParams.g4Val->getVal();
		double img2fit = someHiggs->_modelParams.g2ValIm->getVal();
		double img4fit = someHiggs->_modelParams.g4ValIm->getVal();
		double g1err = someHiggs->_modelParams.g1Val->getError();
		double reg2err = someHiggs->_modelParams.g2Val->getError();
		double reg4err = someHiggs->_modelParams.g4Val->getError();
		double img2err = someHiggs->_modelParams.g2ValIm->getError();
		double img4err = someHiggs->_modelParams.g4ValIm->getError();
*/
/*NOTE: THE LINES BELOW REQUIRE GENERALIZATION!*/
/*
		fg2val = (pow(reg2fit,2.0)*totalxsec_gi1[1])/((pow(reg2fit,2.0)*totalxsec_gi1[1])+(pow(reg4fit,2.0)*totalxsec_gi1[2])+(pow(g1fit,2.0)*totalxsec_gi1[0]));
		fg4val = (pow(reg4fit,2.0)*totalxsec_gi1[2])/((pow(reg2fit,2.0)*totalxsec_gi1[1])+(pow(reg4fit,2.0)*totalxsec_gi1[2])+(pow(g1fit,2.0)*totalxsec_gi1[0]));
		fg2err=0;
		fg4err=0;
*/
		minLL = sum_res_pdf7D->minNll();
		minLL /= nevents;

		fit_7D->Fill();
		hfg2_7D->Fill(fg2val,1.0);
		hfg4_7D->Fill(fg4val,1.0);

		delete toy_data_7D;
		delete someHiggs;
	};

	double hfg2_7Dint = hfg2_7D->Integral();
	hfg2_7D->Scale(1.0/hfg2_7Dint);
	double hfg4_7Dint = hfg4_7D->Integral();
	hfg4_7D->Scale(1.0/hfg4_7Dint);
	foutput->WriteTObject(fit_7D);
	foutput->WriteTObject(hfg2_7D);
	foutput->WriteTObject(hfg4_7D);

	delete hfg2_7D;
	delete hfg4_7D;
	delete fit_7D;

	foutput->Close();
	fsample_input->Close();

	return iteration_newfirstevent;
};

void fitSimultaneous_7D (sample smp, const int erg_tev, const int iteration_min=0, const int iteration_max=5, const int ntests=200, const int nevents=600){
	int lastevent[5]={0,0,0,0,0};
	for(int iter=iteration_min;iter<iteration_max;iter++){
		if(gi_phi2_phi4[smp][6]!=0 && gi_phi2_phi4[smp][7]!=0) lastevent[0] = execute_fitSimultaneous_7D(smp,erg_tev,-1,-1,iter,lastevent[0],ntests,nevents);
/*		lastevent[1] = execute_fitSimultaneous_7D(smp,erg_tev,0,-1,iter,lastevent[1],ntests,nevents);
		lastevent[2] = execute_fitSimultaneous_7D(smp,erg_tev,-1,0,iter,lastevent[2],ntests,nevents);
*/
//		if(gi_phi2_phi4[smp][7]!=0) lastevent[3] = execute_fitSimultaneous_7D(smp,erg_tev,gi_phi2_phi4[smp][6],-1,iter,lastevent[3],ntests,nevents);
//		if(gi_phi2_phi4[smp][6]!=0) lastevent[4] = execute_fitSimultaneous_7D(smp,erg_tev,-1,gi_phi2_phi4[smp][7],iter,lastevent[4],ntests,nevents);
	};
};