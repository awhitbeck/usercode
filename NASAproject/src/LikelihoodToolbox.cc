#include "RooPlot.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TRandom.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooAddPdf.h"
#include "RooFitResult.h"
#include "RooArgSet.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace RooFit;

template <class T,class U>

class LikelihoodToolbox{

  T* signal;
  T* altSignal;
  U* background;
  RooAbsPdf* model, *altModel;
  vector<RooRealVar*> measurables;
  vector<RooRealVar*> signalParams;
  vector<RooRealVar*> backgroundParams;
  TTree* data;
  RooDataSet* dataSet;

public: 

  bool   sepParams_generateFromSignal;   
  int    sepParams_numEvents;		   
  int    sepParams_numSignal;		   
  int    sepParams_nToys;		   
  int    sepParams_bins;		   
  double sepParams_rangeLow;		   
  double sepParams_rangeHigh;		   
  int    sepParams_lineColor1;	   
  int    sepParams_lineStyle1;	   
  int    sepParams_lineColor2;	   
  int    sepParams_lineStyle2;	   
  double ulParams_numBkgEvents;	   
  double ulParams_sclFactor;		   
  double ulParams_meanBackground;	   
  int    ulParams_steps;		   
  int    ulParams_numToys;		   
  int    ulParams_seed;
  int    ulParams_lineColor;		   
  int    ulParams_lineStyle;             
  int    LD_bins;     
  int    LD_lineColor;
  int    LD_lineStyle;
  bool   debug;

  LikelihoodToolbox(T* sig, vector<RooRealVar*> sigParams, U* bkg, vector<RooRealVar*> bkgParams, vector<RooRealVar*> meas){
    signal=sig;
    background=bkg;
    measurables=meas;
    signalParams=sigParams;
    backgroundParams=bkgParams;
    sepParams_generateFromSignal=true;   
    sepParams_numEvents=100;		   
    sepParams_numSignal=10;		   
    sepParams_nToys=100;		   
    sepParams_bins=40;		   
    sepParams_rangeLow=-20;		   
    sepParams_rangeHigh=20;		   
    sepParams_lineColor1=2;	   
    sepParams_lineStyle1=1;	   
    sepParams_lineColor2=4;	   
    sepParams_lineStyle2=1;	   
    ulParams_numBkgEvents=100;	   
    ulParams_sclFactor=.4;		   
    ulParams_meanBackground=100;	   
    ulParams_steps=100;		   
    ulParams_numToys=100;		   
    ulParams_seed=12345;
    ulParams_lineColor=4;		   
    ulParams_lineStyle=1;             
    LD_bins=50;     
    LD_lineColor=1;
    LD_lineStyle=1;
    debug=false;
  };
  LikelihoodToolbox(T* sig, U* bkg, vector<RooRealVar*> meas){
    signal=sig;
    background=bkg;
    measurables=meas;
    sepParams_generateFromSignal=true;   
    sepParams_numEvents=100;		   
    sepParams_numSignal=10;		   
    sepParams_nToys=100;		   
    sepParams_bins=40;		   
    sepParams_rangeLow=-20;		   
    sepParams_rangeHigh=20;		   
    sepParams_lineColor1=2;	   
    sepParams_lineStyle1=1;	   
    sepParams_lineColor2=4;	   
    sepParams_lineStyle2=1;	   
    ulParams_numBkgEvents=100;	   
    ulParams_sclFactor=.4;		   
    ulParams_meanBackground=100;	   
    ulParams_steps=100;		   
    ulParams_numToys=100;		   
    ulParams_seed=12345;
    ulParams_lineColor=4;		   
    ulParams_lineStyle=1;             
    LD_bins=50;     
    LD_lineColor=1;
    LD_lineStyle=1;
    debug=false;
  };
  LikelihoodToolbox(T* sig, T* altSig, U* bkg, vector<RooRealVar*> meas){
    signal=sig;
    altSignal=altSig;
    background=bkg;
    measurables=meas;
    sepParams_generateFromSignal=true;   
    sepParams_numEvents=100;		   
    sepParams_numSignal=10;		   
    sepParams_nToys=100;		   
    sepParams_bins=40;		   
    sepParams_rangeLow=-20;		   
    sepParams_rangeHigh=20;		   
    sepParams_lineColor1=2;	   
    sepParams_lineStyle1=1;	   
    sepParams_lineColor2=4;	   
    sepParams_lineStyle2=1;	   
    ulParams_numBkgEvents=100;	   
    ulParams_sclFactor=.4;		   
    ulParams_meanBackground=100;	   
    ulParams_steps=100;		   
    ulParams_numToys=100;		   
    ulParams_seed=12345;
    ulParams_lineColor=4;		   
    ulParams_lineStyle=1;             
    LD_bins=50;     
    LD_lineColor=1;
    LD_lineStyle=1;
    debug=false;
  };
  T* getSignal(){return signal;};
  T* getAltSignal(){return signal;};
  U* getBackground(){return background;};
  double getSignalProb(){return signal->getVal();};
  double getBackgroundProb(){return background->getVal();};

  //=============================================================

  double getLD(RooAbsPdf* pdf1, RooAbsPdf* pdf2){

    if(debug)
      cout << "getLD" << endl;

    if(!pdf1 || !pdf2){
      cout << "ERROR: at least one pdf seems to be not initialized" << endl;
      return -1.;
    }

    return 1/(1+pdf1->getVal()/pdf2->getVal());
  };

  //=============================================================

  void setMeas(vector<double> var){

  if(measurables.size() != var.size()){
    cout << "ERROR: var size does not match measurables size" << endl;
    return;
  }

  for(unsigned int i=0; i<var.size(); i++){
    measurables[i]->setVal(var[i]);
  }
  };

  //=============================================================

  void    addDataTree(TTree* t){data=t;};
  void    addDataSet(RooDataSet* d){dataSet=d;};

  //=============================================================

  double  getUpperLimit(){
    double numBkgEvents=ulParams_numBkgEvents;
    double sclFactor=ulParams_sclFactor;

    double stepSize = numBkgEvents*sclFactor/ulParams_steps;    

    RooRealVar nsig("nsig","nsig",0,0,2000);
    RooRealVar nbkg("nbkg","nbkg",numBkgEvents,0,2000);
    nsig.setConstant(kTRUE);
    nbkg.setConstant(kFALSE);

    model = new RooAddPdf("model","model",RooArgList(*background,*signal),RooArgList(nbkg,nsig));
    
    RooFitResult* r;

    TH1F* h_chi2Scan = new TH1F( "h_chi2Scan","h_chi2Scan", ulParams_steps, 0., numBkgEvents*sclFactor );
    TH1F* h_chi2Scan_reScl = new TH1F( "h_chi2Scan_reScl","h_chi2Scan_reScl", ulParams_steps, 0., numBkgEvents*sclFactor );
    TH1F* h_likeliScan_reScl = new TH1F( "h_likeliScan_reScl","h_likeliScan_reScl", ulParams_steps, 0., numBkgEvents*sclFactor );

    for(int j=0; j<ulParams_steps;j++){
      nsig.setVal(j*stepSize);
      
      r= (RooFitResult*) model->fitTo(*dataSet,Extended(kTRUE),Minos(kTRUE),Save(kTRUE),Verbose(kFALSE),PrintLevel(-1),Warnings(kFALSE));

      h_chi2Scan->SetBinContent(j+1,r->minNll() );

    }// end loop over j (signal yield)

    std::cout << "min Val of chi2 Scan: " << h_chi2Scan->GetMinimum() << std::endl;
    Double_t minR = h_chi2Scan->GetMinimum();
    for (int aa = 1; aa <= ulParams_steps; aa++){
      Double_t curR = h_chi2Scan->GetBinContent( aa );
      h_chi2Scan_reScl->SetBinContent( aa, curR - minR ); 
      h_likeliScan_reScl->SetBinContent( aa, exp(minR - curR) );
    }

    delete h_chi2Scan;
    delete h_chi2Scan_reScl;
    delete r;
    double UL= getNUL95( h_likeliScan_reScl );
    delete h_likeliScan_reScl;
    return UL;
  };

  //=============================================================

  double getNUL95( TH1F* histo ){
    
    // first integrate
    Double_t totIntegral = 0;
    for (int i = 1; i <= histo->GetNbinsX(); i++){
      //std::cout << histo->GetBinContent( i ) << std::endl;
      totIntegral += histo->GetBinContent( i );
    }
    //std::cout << "total integral: " << totIntegral << std::endl;
    
    Double_t limit95 = totIntegral*0.95;
    Double_t ctrForLimit = 0;
    Int_t stoppingBin = 0;
    for (int i = 1; i <= histo->GetNbinsX(); i++){
      ctrForLimit += histo->GetBinContent( i );
      if (ctrForLimit >= limit95){
	stoppingBin = i;
			break;
      }
    }
    std::cout << "ctrForLimit: " << ctrForLimit << ", stopping Bin: " << stoppingBin << std::endl;
    
    return histo->GetBinCenter( stoppingBin );
  };
  //=============================================================

  TH1F*   expectedULDistribution(){

    double meanBackground=ulParams_meanBackground;
    int numToys=ulParams_numToys; 
    int seed=ulParams_seed;
    double sclFactor=ulParams_sclFactor;

    TRandom r(seed);

    TH1F* ULdist = new TH1F("ULdist","ULdist",100,0,4*sqrt(meanBackground));
    ULdist->SetLineColor(ulParams_lineColor);
    ULdist->SetLineStyle(ulParams_lineStyle);

    for(int iToy=0; iToy<numToys; iToy++){
      
      cout << "toy: " << iToy << endl;

      generateData(background,r.Poisson(meanBackground));
      cout << "Data generated..." << endl;
      ULdist->Fill(getUpperLimit());
    }

    return ULdist;

  };

  //=============================================================

  void generateData(RooAbsPdf* pdf, int numEvents){
    
        if(measurables.size()==0){
      cout << "ERROR: measurables appears to be empty.  I don't know how to generate events for you." << endl;
      return;
    }
      RooArgSet genVars(*measurables[0]);
      cout << "genVars: added " << measurables[0]->GetTitle() << endl;

      //build RooArgSet from measurables
      for(unsigned int iMeas=1; iMeas<measurables.size(); iMeas++){
	genVars.add(*measurables[iMeas]);
	cout << "genVars: added " << measurables[iMeas]->GetTitle() << endl;
      }

      cout << "Generating!" << endl;

      dataSet = (RooDataSet*) pdf->generate(genVars,numEvents);
  };

  //=============================================================

  TH1F* makeLnLRPlot(bool generateFromSignal=true){

    int numEvents=sepParams_numEvents;
    int numSignal=sepParams_numSignal;
    int nToys=sepParams_nToys;

    TH1F* hLnLR = new TH1F("hLnLR","hLnLR",sepParams_bins,sepParams_rangeLow,sepParams_rangeHigh);
    if(generateFromSignal){
      hLnLR->SetLineColor(sepParams_lineColor1);
      hLnLR->SetLineStyle(sepParams_lineStyle1);
    }else{
      hLnLR->SetLineColor(sepParams_lineColor2);
      hLnLR->SetLineStyle(sepParams_lineStyle2);
    }


    RooRealVar f("f","f",numSignal/numEvents,0,10);
    f.setConstant(kTRUE);
    model = new RooAddPdf("model","model",*signal,*background,f);
    altModel = new RooAddPdf("altModel","altModel",*altSignal,*background,f);

    for(int iToy=0; iToy<nToys; iToy++){

      cout << "Toy: " << iToys << endl;

      if(generateFromSignal)
	generateData(model, numEvents);
      else
	generateData(altModel, numEvents);


      hLnLR->Fill( getLnLikelihoodRatio() );  

    }// end loop over toys

    return hLnLR;

  };

  //=============================================================

  double  getLnLikelihoodRatio(){

    RooFitResult* r1,*r2;
    
    r1 = (RooFitResult*) model->fitTo(*dataSet,Minos(kTRUE),Save(kTRUE),Verbose(kFALSE),PrintLevel(-1),Warnings(kFALSE));
    r2 = (RooFitResult*) altModel->fitTo(*dataSet,Minos(kTRUE),Save(kTRUE),Verbose(kFALSE),PrintLevel(-1),Warnings(kFALSE));

    cout << r1->minNll() - r2->minNll() << endl;

    return (r1->minNll() - r2->minNll());
    
  };

  // The following methods have not yet been implemented 
  // and simply serve as an outline for things to be 
  // developed. 

  TH1F* makeLDdistribution(){

    if(debug)
      cout << "makeLDdistribution:" << endl;
 
    TH1F* LDdist = new TH1F("LDdist","LDdist",LD_bins,0,1);
    LDdist->SetLineColor(LD_lineColor);
    LDdist->SetLineStyle(LD_lineStyle);

    if(debug)
      cout << "histo initialized" << endl;

    if(!data){
      cout << "There is no data load.  Please use the method LikelihoodToolbox::addDataTree(TTree* t)" << endl;
      return 0;
    }

    if(debug)
      cout << "setting branch addresses" << endl;
    
    vector<double> val;
    for(int iMeas=0; iMeas<measurables.size(); iMeas++){
      val.push_back(0);
      data->SetBranchAddress(measurables[iMeas]->GetName(),&val[iMeas]);
    }

    for(int iEvt=0; iEvt<data->GetEntries(); iEvt++){

      if(debug)
	cout << "iEvt: " << iEvt << endl;

      data->GetEntry(iEvt);
      
      //set measurables
      for(int iMeas=0; iMeas<measurables.size(); iMeas++){

	cout << measurables[iMeas]->GetName() << ": " << val[iMeas] << endl;
	measurables[iMeas]->setVal(val[iMeas]);

      }

      cout << "signal prob: " << signal->getVal() << endl;
      cout << "altsignal prob: " << altSignal->getVal() << endl;
      cout << "LD: " << 1/(1+(signal->getVal()/altSignal->getVal())) << endl;

      LDdist->Fill(1/(1+(signal->getVal()/altSignal->getVal())));
      
    }// end loop over events

    return LDdist;
  };

  TGraph* makeROCcurve(){return 0;};
  TH1F*   makeLikelihoodRatio(){return 0;};

};

