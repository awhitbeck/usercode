#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif

#include <vector>
#include <string>
#include <iostream>
#include "RooAbsPdf.h"
#include "RooAddPdf.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooFitResult.h"
#include "TRandom.h"
#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TH1F.h"

using namespace std;
using namespace RooFit;

class statsFactory {
    
public:
    
    //Default constructor
    
    statsFactory(RooArgSet* set, RooAbsPdf* pdf1, RooAbsPdf* pdf2, std::string outputFileName="test.root");
    ~statsFactory();

    RooArgSet* observables; 
    RooAbsPdf* H0pdf;
    RooAbsPdf* H1pdf;
    RooRealVar* nsig;
    RooRealVar* nbkg;
    RooAddPdf* totalPdf;

    TFile* fout;

    
    void runSignificance(double nSig, double nBkg, int nToys);
  void runSignificance(double nSig, double nBkg, int nToys,RooDataSet* sigPoolData,RooDataSet* bkgPoolData);
    void runUpperLimit(double nSig, double nBkg, int nToys);
    
    double getNUL95( TH1F* histo );


    
};


// CONSTRUCTOR
statsFactory::statsFactory(RooArgSet* set, RooAbsPdf* pdf1, RooAbsPdf* pdf2, std::string outputFileName){
    
    std::cout << "in the constructor!" << std::endl;
    observables = set;
    H0pdf = pdf1;
    H1pdf = pdf2;
    fout = new TFile( outputFileName.c_str(), "RECREATE");

    nsig = new RooRealVar("nsig","number of signal events",0.,300) ;
    nbkg = new RooRealVar("nbkg","number of background events",0.,300) ;
    //Construct composite PDF
    totalPdf = new RooAddPdf("totalPdf","totalPdf",RooArgList(*H0pdf,*H1pdf),RooArgList(*nsig,*nbkg));
    
}

// DESTRUCTOR
statsFactory::~statsFactory(){
    
    fout->Close();
    
}

// SIGNIFICANCE CALCULATION WITH EMBEDDED
void statsFactory::runSignificance(double nSig, double nBkg, int nToys,
				   RooDataSet* sigPoolData,RooDataSet* bkgPoolData){
    
   //Extended Likelihood Formalism
    double nsignal=nSig;
    double nbackground=nBkg;
    double nPoissSig,nPoissBkg;
    int sigPlaceHolder=0;
    int bkgPlaceHolder=0;

    nsig->setVal(nsignal);
    nbkg->setVal(nbackground);

    // --- ntuple
    TNtuple* signifTuple = new TNtuple("signifTuple","signifTuple", "sig:nll0:nll1:nPoiss:nSigFit:nBkgFit"); 
    TRandom rng;
    
    std::cout << "Performing " << nToys << " toys..." << std::endl;
    for (int i = 0; i < nToys; i++){

        if(i%100==0) cout << "toy number " << i << endl;

        nPoissSig = rng.Poisson(nsignal);
        nPoissBkg = rng.Poisson(nbackground);
        nsig->setVal(nsignal);
        nbkg->setVal(nbackground);
        //--------------------------------------------------------------------------------------------
        // generating dataset

        RooDataSet* data = new RooDataSet("data","data",*observables);
	RooArgSet *temp;
	for(int iSig=0; iSig<nPoissSig; iSig++){

	  if(sigPlaceHolder>=sigPoolData->sumEntries()){
	    cout << "Sorry, we ran out of signal events.  Bye!" << endl;
	    return;
	  }else{
	    temp = (RooArgSet*)(sigPoolData->get(sigPlaceHolder));
	    sigPlaceHolder++;
	  }

	  data->add(*temp);
	  
	}

	for(int iBkg=0; iBkg<nPoissBkg; iBkg++){

	  if(bkgPlaceHolder>=bkgPoolData->sumEntries()){
	    cout << "Sorry, we ran out of signal events.  Bye!" << endl;
	    return;
	  }else{
	    temp = (RooArgSet*)(bkgPoolData->get(bkgPlaceHolder));
	    bkgPlaceHolder++;
	  }

	  data->add(*temp);

	}

	//--------------------------------------------------------------------------------------------


        // fit full float
        nsig->setConstant(kFALSE); nbkg->setConstant(kFALSE);
        RooFitResult* r = totalPdf->fitTo(*data,Extended(kTRUE),Minos(kFALSE),Save(kTRUE),Verbose(kFALSE),PrintLevel(-1));
        /////r->Print();
        double nSigFit = nsig->getVal();
        double nBkgFit = nbkg->getVal();

        // fit fix signal to zero
        nsig->setVal(0.); nsig->setConstant(kTRUE); nbkg->setConstant(kFALSE);
        RooFitResult* r0 = totalPdf->fitTo(*data,Extended(kTRUE),Minos(kFALSE),Save(kTRUE),Verbose(kFALSE), PrintLevel(-1));
        
        std::cout << "FCN r: " << r->minNll() << std::endl;
        std::cout << "FCN r0: " << r0->minNll() << std::endl;
        
        Double_t significance = sqrt(2*fabs(r->minNll() - r0->minNll()));
        std::cout << "significance: " << significance << std::endl;

        signifTuple->Fill( significance,r->minNll(),r0->minNll(), nPoissSig+nPoissBkg, nSigFit, nBkgFit );
        
        delete data;
        delete r;  
        delete r0;  
    }
    
    fout->cd();
    signifTuple->Write();
    
}

// SIGNIFICANCE CALCULATION
void statsFactory::runSignificance(double nSig, double nBkg, int nToys){
    
   //Extended Likelihood Formalism
    double nsignal=nSig;
    double nbackground=nBkg;
    double nPoiss;

    nsig->setVal(nsignal);
    nbkg->setVal(nbackground);

    // --- ntuple
    TNtuple* signifTuple = new TNtuple("signifTuple","signifTuple", "sig:nll0:nll1:nPoiss:nSigFit:nBkgFit"); 
    TRandom rng;
    
    std::cout << "Performing " << nToys << " toys..." << std::endl;
    for (int i = 0; i < nToys; i++){

        if(i%100==0) cout << "toy number " << i << endl;

        nPoiss = rng.Poisson(nsignal+nbackground);
        nsig->setVal(nsignal);
        nbkg->setVal(nbackground);
        //--------------------------------------------------------------------------------------------
        // generating dataset
        RooDataSet* data = totalPdf->generate(*observables, nPoiss);
        
        // fit full float
        nsig->setConstant(kFALSE); nbkg->setConstant(kFALSE);
        RooFitResult* r = totalPdf->fitTo(*data,Extended(kTRUE),Minos(kFALSE),Save(kTRUE),Verbose(kFALSE),PrintLevel(-1));
        /////r->Print();
        double nSigFit = nsig->getVal();
        double nBkgFit = nbkg->getVal();

        // fit fix signal to zero
        nsig->setVal(0.); nsig->setConstant(kTRUE); nbkg->setConstant(kFALSE);
        RooFitResult* r0 = totalPdf->fitTo(*data,Extended(kTRUE),Minos(kFALSE),Save(kTRUE),Verbose(kFALSE), PrintLevel(-1));
        
        //std::cout << "FCN r: " << r->minNll() << std::endl;
        //std::cout << "FCN r0: " << r0->minNll() << std::endl;
        
        Double_t significance = sqrt(2*fabs(r->minNll() - r0->minNll()));
        //std::cout << "significance: " << significance << std::endl;

        signifTuple->Fill( significance,r->minNll(),r0->minNll(), nPoiss, nSigFit, nBkgFit );
        
        delete data;
        delete r;  
        delete r0;  
    }
    
    fout->cd();
    signifTuple->Write();
    
}

// UPPER LIMIT CALCULATION
void statsFactory::runUpperLimit(double nSig, double nBkg, int nToys){
    
    // internal parameters
    int iLoopScans = 200;  // number of steps to take to scan in nSig
    double sclFactor = nBkg*0.3; // how far w.r.t. the input nBkg to scan in 
    
    
    double nsignal=nSig;
    double nbackground=nBkg;
    double nPoiss;

    /*
      // moved to constructor ...
      RooRealVar nsig("nsig","number of signal events",nsignal,0.,300) ;
      RooRealVar nbkg("nbkg","number of background events",nbackground,0.,300) ;
      //Construct composite PDF
      RooAddPdf totalPdf("totalPdf","totalPdf",RooArgList(*H0pdf,*H1pdf),RooArgList(nsig,nbkg));
    */

    nsig->setVal(nsignal);
    nbkg->setVal(nbackground);

    double stepSize = nbackground*sclFactor/( (double) iLoopScans);
    TNtuple* ulTuple = new TNtuple("ulTuple","ulTuple", "UL:nPoiss"); 
    TRandom rng;

    // start running toys
    for (int i = 0; i < nToys; i++) {

        nPoiss = rng.Poisson( 0. + nbackground );  // fix number of signal events to 0
        
        nsig->setVal(0.);
        nbkg->setVal(nbackground);
        RooDataSet* data = totalPdf->generate(*observables, nPoiss) ;

        TH1F* h_chi2Scan = new TH1F( "h_chi2Scan","h_chi2Scan", iLoopScans, 0., nbackground*sclFactor );
        TH1F* h_chi2Scan_reScl = new TH1F( "h_chi2Scan_reScl","h_chi2Scan_reScl", iLoopScans, 0., nbackground*sclFactor );
        TH1F* h_likeliScan_reScl = new TH1F( "h_likeliScan_reScl","h_likeliScan_reScl", iLoopScans, 0., nbackground*sclFactor );

        for (int j = 0; j < iLoopScans; j++){
            
            std::cout << "Experiment: " << i << ", Loop: " << j << std::endl;
            
            nsig->setConstant(kTRUE); 
            nbkg->setConstant(kFALSE);
            
            const Double_t vall = (Double_t) j*stepSize;
            //std::cout << vall << std::endl;
            
            nsig->setVal( vall );
            
            RooFitResult* r = totalPdf->fitTo(*data,Extended(kTRUE),Minos(kTRUE),Save(kTRUE),Verbose(kFALSE), PrintLevel(-1), Warnings(kFALSE), PrintEvalErrors(-1));
            
            // fill histo
            //h_likeliScan->SetBinContent( j+1, likeli );
            h_chi2Scan->SetBinContent( j+1, (r->minNll()) );
            
            delete r; 
            
        }
        
        std::cout << "min Val of chi2 Scan: " << h_chi2Scan->GetMinimum() << std::endl;
        Double_t minR = h_chi2Scan->GetMinimum();
        for (int aa = 1; aa <= iLoopScans; aa++){
            Double_t curR = h_chi2Scan->GetBinContent( aa );
            h_chi2Scan_reScl->SetBinContent( aa, curR - minR ); 
            h_likeliScan_reScl->SetBinContent( aa, exp(minR - curR) );
        }
        
        Double_t nUL = getNUL95( h_likeliScan_reScl );
        ulTuple->Fill( nUL, nPoiss );
        
        delete data;
        delete h_likeliScan_reScl;
        delete h_chi2Scan;
        delete h_chi2Scan_reScl;
    }
   
    fout->cd();
    ulTuple->Write();

}
 
// ------------------------------
// ------------------------------
// UTILITIES
double statsFactory::getNUL95( TH1F* histo ){
    
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
    //std::cout << "ctrForLimit: " << ctrForLimit << ", stopping Bin: " << stoppingBin << std::endl;
    
    return histo->GetBinCenter( stoppingBin );
    
}
    
    
    
    
    
