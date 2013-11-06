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


int execute_fitSimultaneous_7D (sample smp, const int erg_tev, const int iteration=0, int iteration_oldfirstevent=0, bool pmf_applied=true, const int ntests=200, const int nevents=600){
	RooFit::Verbose(false);
	RooFit::PrintLevel(-1000);
	RooMsgService::instance().setStreamStatus(1,false);
	RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
	cout << "Starting..." << endl;
	string SAMPLE_INPUT_NAME = "ggtoHtoZZ4l_VariousKDs_";
	string OUTPUT_NAME = "ggtoHtoZZ4l_125_6_PseudoExp_feps_7D_";
	if(!pmf_applied) OUTPUT_NAME = "ggtoHtoZZ4l_125_6_PseudoExp_feps_7D_noPMF_";
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

	string coutput = OUTPUT_NAME + sampleName[smp];
	char citeration[100];
	sprintf(citeration,"_%i",iteration);
	coutput = coutput + citeration;
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
	string histo7D_fepspr = str7Dtree + "_fepsprResult";

	double fepsprval,fepsprerr,minLL;
	TTree* fit_7D = new TTree(str7Dtree.c_str(),str7Dtree.c_str());
	fit_7D->Branch("fepspr",&fepsprval,"fepspr/D");
	fit_7D->Branch("fepspr_error",&fepsprerr,"fepspr_error/D");
	fit_7D->Branch("minLL_avg",&minLL,"minLL_avg/D");

	TH1F* hfepspr_7D;
	hfepspr_7D = new TH1F(histo7D_fepspr.c_str(),histo7D_fepspr.c_str(),25,0,1);
	hfepspr_7D->SetXTitle("#epsilon");
	hfepspr_7D->SetYTitle("Weighed Distribution");
	hfepspr_7D->Sumw2();

	for(int test=0;test<ntests;test++){

		ScalarPdfFactory_withFepspr* someHiggs = new ScalarPdfFactory_withFepspr(z1mass,z2mass,hs,h1,h2,Phi,Phi1,mzz,1,false,pmf_applied);
		someHiggs->_modelParams.fepspr->setVal(gi_phi2_phi4[smp][8]);
		someHiggs->_modelParams.g1Val->setVal(gi_phi2_phi4[smp][0]);
		someHiggs->_modelParams.g3Val->setVal(gi_phi2_phi4[smp][2]);
		someHiggs->_modelParams.g2Val->setVal(gi_phi2_phi4[smp][1]*cos(gi_phi2_phi4[smp][4]));
		someHiggs->_modelParams.g4Val->setVal(gi_phi2_phi4[smp][3]*cos(gi_phi2_phi4[smp][5]));
		someHiggs->_modelParams.g2ValIm->setVal(gi_phi2_phi4[smp][1]*sin(gi_phi2_phi4[smp][4]));
		someHiggs->_modelParams.g4ValIm->setVal(gi_phi2_phi4[smp][3]*sin(gi_phi2_phi4[smp][5]));
		someHiggs->makeParamsConst(true);
		someHiggs->_modelParams.fepspr->setConstant(false);

		RooDataSet* toy_data_7D = new RooDataSet("toy_data_1D","toy_data_1D", toy_tree[test], RooArgSet(*z1mass,*z2mass,*hs,*h1,*h2,*Phi,*Phi1));
		RooFitResult* sum_res_pdf7D = someHiggs->PDF->fitTo(*toy_data_7D,Save(true),PrintLevel(-1));

		fepsprval = someHiggs->_modelParams.fepspr->getVal();
		fepsprerr = someHiggs->_modelParams.fepspr->getError();
		minLL = sum_res_pdf7D->minNll();
		minLL /= nevents;

		fit_7D->Fill();
		hfepspr_7D->Fill(fepsprval,1.0);

		delete toy_data_7D;
		delete someHiggs;
	};

	double hfepspr_7Dint = hfepspr_7D->Integral();
	hfepspr_7D->Scale(1.0/hfepspr_7Dint);

	foutput->WriteTObject(fit_7D);
	foutput->WriteTObject(hfepspr_7D);

	delete hfepspr_7D;
	delete fit_7D;

	foutput->Close();
	fsample_input->Close();

	return iteration_newfirstevent;
};

void fitSimultaneous_7D_fepspr (sample smp, const int erg_tev, const int iteration_min=0, const int iteration_max=5, bool pmf_applied=true, const int ntests=200, const int nevents=600){
	int lastevent[5]={0,0,0,0,0};
	for(int iter=iteration_min;iter<iteration_max;iter++){
		lastevent[0] = execute_fitSimultaneous_7D(smp,erg_tev,iter,lastevent[0],pmf_applied,ntests,nevents);
	};
};
