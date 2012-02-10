#include <algorithm>
#include <vector>

double gaussianSeparation(TH1F h0, TH1F h1);
double gaussianSeparation(double mean1, double sigma1, double mean2, double sigma2);

void processResults(char* inputname){
    
    gROOT->ProcessLine(".L ~ntran/tdrstyle.C");
    setTDRStyle();
    gStyle->SetPadLeftMargin(0.16);
    gStyle->SetOptFit(0);
    
    // stuff to extract
    bool b_getSeparation = true;
    bool b_getSignificance = false;
    bool b_getSignificance_em = false;
    bool b_upperLimit_em = false;


    TFile* file = new TFile(inputname);
    
  
    // -----------------------------------------------------------------------
    if (b_getSignificance){
     
        float sig;
        TTree* t = (TTree*) file->Get("signifTuple");
        t->SetBranchAddress("sig", &sig);

        // get range
        double hlo = 99999.;
        double hhi = -99999.;
        
        for (int i = 0; i < t->GetEntries(); i++){
            t->GetEntry(i);
            if (sig < 100){
                if (sig < hlo) hlo = sig; 
                if (sig > hhi) hhi = sig; 
            }
        }
        
        // fill histo
        TH1F* hsig = new TH1F("hsig","; significance; toys", 100, hlo, hhi);
        for (int i = 0; i < t->GetEntries(); i++){
            t->GetEntry(i);
            if (sig < 100){
                hsig->Fill( sig );
            }
        }
        TCanvas* c = new TCanvas("c","c",800,600);
        hsig->Fit("gaus");
        hsig->Draw();
        TF1 *fit0 = (TF1*)hsig->GetFunction("gaus");
        
        std::cout << "-----------------------" << std::endl;
        std::cout << "significance is " << fit0->GetParameter(1) << " sigma" << std::endl;
        std::cout << "-----------------------" << std::endl;
    }
    // -----------------------------------------------------------------------
    if (b_getSignificance_em){
        
        float sig;
        TTree* t = (TTree*) file->Get("signifTuple_em");
        t->SetBranchAddress("sig", &sig);
        
        // get range
        double hlo = 99999.;
        double hhi = -99999.;
        
        for (int i = 0; i < t->GetEntries(); i++){
            t->GetEntry(i);
            if (sig < 100){
                if (sig < hlo) hlo = sig; 
                if (sig > hhi) hhi = sig; 
            } 
        }
        
        // fill histo
        TH1F* hsig = new TH1F("hsig","; significance; toys", 100, hlo, hhi);
        for (int i = 0; i < t->GetEntries(); i++){
            t->GetEntry(i);
            if (sig < 100){
                hsig->Fill( sig );
            }
        }
        TCanvas* c = new TCanvas("c","c",800,600);
        hsig->Fit("gaus");
        hsig->Draw();
        TF1 *fit0 = (TF1*)hsig->GetFunction("gaus");
        
        std::cout << "-----------------------" << std::endl;
        std::cout << "significance is " << fit0->GetParameter(1) << " sigma for " << hsig->GetEntries() << " toys" << std::endl;
        std::cout << "-----------------------" << std::endl;
    }
    // -----------------------------------------------------------------------
    if (b_upperLimit_em){
        
        float ul;
        TTree* t = (TTree*) file->Get("ulTuple_em");
        t->SetBranchAddress("UL", &ul);
        
        // get range
        double hlo = 99999.;
        double hhi = -99999.;
        
        for (int i = 0; i < t->GetEntries(); i++){
            t->GetEntry(i);
            if (ul < hlo) hlo = ul; 
            if (ul > hhi) hhi = ul; 
        }
        hlo = 0.;
        
        // fill histo
        TH1F* hul = new TH1F("hul","; upper limit (signal events); toys", 100, hlo, hhi);
        std::vector< double > ul_vec;
        for (int i = 0; i < t->GetEntries(); i++){
            t->GetEntry(i);
            if (ul > 1){
                hul->Fill( ul );
                ul_vec.push_back( ul );
            }
        }
        TCanvas* c = new TCanvas("c","c",800,600);
        hul->Draw();
        
        sort(ul_vec.begin(), ul_vec.end());
        int nLimits = ul_vec.size();
        double medianLimit = (nLimits % 2 == 0 ? 0.5*(ul_vec[nLimits/2-1]+ul_vec[nLimits/2]) : ul_vec[nLimits/2]);

        std::cout << "-----------------------" << std::endl;
        std::cout << "median upper limit is " << medianLimit << " sigma for " << hul->GetEntries() << " toys" << std::endl;
        std::cout << "-----------------------" << std::endl;
    }
    // -----------------------------------------------------------------------
    if (b_getSeparation){
        
        float S_H0,S_H1;
        TTree* t = (TTree*) file->Get("hypTuple_em_wBkg");
        t->SetBranchAddress("S_H0", &S_H0);
        t->SetBranchAddress("S_H1", &S_H1);
        
        // get range
        double hlo = 99999.;
        double hhi = -99999.;

        for (int i = 0; i < t->GetEntries(); i++){
            t->GetEntry(i);
            if (S_H0 < hlo) hlo = S_H0; 
            if (S_H1 < hlo) hlo = S_H1; 
            if (S_H0 > hhi) hhi = S_H0; 
            if (S_H1 > hhi) hhi = S_H1; 
        }
        
        //RooRealVar S_val = new RooRealVar("S_val","S_val",hlo,hhi);
        //RooPlot* *plot_S = S_val->frame(55);
        int nBins = 200;
        TH1F* hS0 = new TH1F("hS0","; S = 2ln(L_{0}/L_{1});", nBins, hlo, hhi);
        TH1F* hS1 = new TH1F("hS1","hS1", nBins, hlo, hhi);
        for (int i = 0; i < t->GetEntries(); i++){
            t->GetEntry(i);
            hS0->Fill( S_H0 );
            hS1->Fill( S_H1 );
        }
        TCanvas* c = new TCanvas("c","c",800,600);
        //hS0->Fit("gaus");
        //hS1->Fit("gaus");
        hS0->Draw();
        hS1->SetLineColor(kBlue); hS1->Draw("same");
        //TF1 *fit0 = (TF1*)hS0->GetFunction("gaus");
        //TF1 *fit1 = (TF1*)hS1->GetFunction("gaus");
        c->SaveAs("test.eps");
        
        //double sep = gaussianSeparation(fit0->GetParameter(0),fit0->GetParameter(1),fit0->GetParameter(2),
        //                                fit1->GetParameter(0),fit1->GetParameter(1),fit1->GetParameter(2));
        
        /////////
        int norm0 = hS0->Integral( 1, hS0->GetEntries() );
        int norm1 = hS1->Integral( 1, hS1->GetEntries() );
        
        double diff = 10.;
        double coverage = 0.;
        for (int i = 1; i <= nBins; i++){
            
            double int0 = hS0->Integral(1,i)/norm0;
            double int1 = hS1->Integral(i,nBins)/norm1;
            
            if (fabs(int0-int1) < diff){
                diff = fabs(int0-int1);
                coverage = 0.5*(int0+int1);
            }
            
            std::cout << "int0: " << int0 << ", int1: " << int1 << ", coverage: " << coverage << std::endl;
        }
        double sepH = 2*ROOT::Math::normal_quantile_c(1.0 - coverage, 1.0);
        /////////
        
        //std::cout << "-----------------------" << std::endl;
        //std::cout << "gaussian separation is " << sep << " sigma " << std::endl;
        //std::cout << "-----------------------" << std::endl;
        std::cout << "-----------------------" << std::endl;
        std::cout << "histogram separation is " << sepH << " sigma " << std::endl;
        std::cout << "-----------------------" << std::endl;

    }
    // -----------------------------------------------------------------------
}

double gaussianSeparation(TH1F h0, TH1F h1){
    
    int norm0 = h0->Integral( 1, h0->GetEntries() );
    int norm1 = h1->Integral( 1, h1->GetEntries() );

    for (int i = 1; i <= 100; i++){
        
        double int0 = h0->(1,i)/norm0;
        double int1 = h1->(i,100)/norm1;
        
        std::cout << "int0: " << int0 << ", int1: " << int1 << std::endl;
    }
    
}
double gaussianSeparation(double coff1, double mean1, double sigma1, double coff2, double mean2, double sigma2){
    
    // given two gaussians, find their separation
    //double mean1 = 1;
    //double sigma1 = 1;
    //double mean2 = -1;
    //double sigma2 = 2;
    
    if (mean1 < mean2){
        double tmpm = mean1; double tmps = sigma1; double tmpc = coff1;
        mean1 = mean2; sigma1 = sigma2; coff1 = coff2;
        mean2 = tmpm; sigma2 = tmps; coff2 = tmpc;
        
    }
    
    TF1 *fg1 = new TF1("fg1","gaus(0)",0,10);
    fg1->SetParameter(0,coff1);
    fg1->SetParameter(1,mean1);
    fg1->SetParameter(2,sigma1);

    TF1 *fg2 = new TF1("fg2","gaus(0)",0,10);
    fg2->SetParameter(0,coff2);
    fg2->SetParameter(1,mean2);
    fg2->SetParameter(2,sigma2);
    
    double tolerance = 0.005;
    int ctr = 0;
    
    double start = mean2 - 2*sigma2;
    double loInfty = (mean2 - 10*sigma2);
    double hiInfty = (mean1 + 10*sigma1);

    std::cout << "start: " << start << ", loInfty: " << loInfty << ", hiInfty: " << hiInfty << std::endl;
    
    double stepSize = (sigma1+sigma2)*0.5;
    double norm1 = fg1->Integral(loInfty, hiInfty);
    double norm2 = fg2->Integral(loInfty, hiInfty);
    double gausDiff = 999., lastGausDiff = 999.;
    double curStep = start;
    double coverage = 0;
    while (fabs(gausDiff) > tolerance){
        
        double int1 = fg1->Integral(curStep, hiInfty)/norm1;
        double int2 = fg2->Integral(loInfty, curStep)/norm2;
        
        gausDiff = int1 - int2;
        coverage = int1;
        if (gausDiff*lastGausDiff < 0){
            stepSize *= (-0.5);
        }
        curStep += stepSize;
        
        std::cout << "int1: " << int1 << ", int2: " << int2 << ", gaussDiff: " << gausDiff << ", lastGausDiff: " << lastGausDiff << std::endl;
        std::cout << "curStep: " << curStep << ", stepSize: " << stepSize << ", tolerance: " << tolerance << std::endl;

        // end of loop stuff
        lastGausDiff = int1 - int2;        
        if (ctr > 100) break; // safety!
        ctr++;
    }
    
    double separation = 2*ROOT::Math::normal_quantile_c(1.0 - coverage, 1.0);
    std::cout << "coverage: " << coverage << ", separation: " << separation << std::endl;
    
    return separation;
}