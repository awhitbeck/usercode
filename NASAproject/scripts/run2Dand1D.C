

void run2Dand1D(double intLumi=20.0){
    
    using namespace RooFit;
    
    gSystem->Load("../PDFs/RooMzzModelBkg_cc.so");
    gSystem->Load("../PDFs/RooNASAModelSig_cc.so");
    gSystem->Load("../PDFs/RooNASAModelBkg_cc.so");
    gSystem->Load("../src/NASA2dPdfFactory_cc.so");
    gSystem->Load("../src/NASA1dPdfFactory_cc.so");
    gSystem->Load("../src/statsFactory_cc.so");
    
    gROOT->ProcessLine(".L ~ntran/tdrstyle.C");
    setTDRStyle();
    gStyle->SetPadLeftMargin(0.16);
    
    RooRealVar* zzmass = new RooRealVar("zzmass","m_{ZZ}",110,140);
    RooRealVar* nasaLD = new RooRealVar("nasaLD","LD",0.,1.);
    RooArgSet* obs2d = new RooArgSet(*zzmass,*nasaLD);
    RooArgSet* obs1d = new RooArgSet(*zzmass);
    //TFile* sigFile = new TFile("../datafiles/SMHiggs_125_JHU_v2_wResolution_withDiscriminants.root");
    TChain *t = new TChain("angles");
    t->Add("/scratch/hep/whitbeck/4lHelicity/datafiles/SMHiggs_125_JHU_v2_wResolution_withDiscriminants_withCuts.root");
    t->Add("/scratch/hep/whitbeck/4lHelicity/datafiles/SMHiggs_125_JHU_wResolution_withDiscriminants_withCuts.root");
    /////t->Add("/scratch/hep/sbologne/filesForNhan/SMHiggs_125_JHU_wResolution_withDiscriminants_withCuts.root");
    TFile* bkgFile = new TFile("/scratch/hep/whitbeck/4lHelicity/datafiles/EWKZZ4l_Powheg_wResolution_withDiscriminants_withCuts.root");
    /////TFile* bkgFile = new TFile("/scratch/hep/sbologne/filesForNhan/EWKZZ4l_Powheg_total_v1_wResolution_withDiscriminants_withCuts.root");
    RooDataSet *bkgData2D = new RooDataSet("bkgData2D","bkgData2D",(TTree*)bkgFile->Get("angles"),*obs2d);
    RooDataSet *sigData2D = new RooDataSet("sigData2D","sigData2D",t,*obs2d);
    RooDataSet *bkgData1D = new RooDataSet("bkgData1D","bkgData1D",(TTree*)bkgFile->Get("angles"),*obs1d);
    RooDataSet *sigData1D = new RooDataSet("sigData1D","sigData1D",t,*obs1d);
    NASA2dPdfFactory my2dModel(zzmass,nasaLD);
    NASA1dPdfFactory my1dModel(zzmass);
    
    // - - - - - - - - - - - - - - - - - - - - - 
    
    my1dModel.sigPDF->fitTo(*sigData1D);
    my1dModel.bkgPDF->fitTo(*bkgData1D);
    
    /*
    RooPlot* plot1 = zzmass->frame(20);
    //RooPlot* plot2 = zzmass->frame(20);
    RooPlot* plot3 = nasaLD.frame(20);
    //RooPlot* plot4 = nasaLD.frame(20);
    
    sigData2D->plotOn(plot1,MarkerColor(2));
    my2dModel.sigPDF->plotOn(plot1,LineColor(2));
    sigData2D->plotOn(plot3,MarkerColor(2));
    my2dModel.sigPDF->plotOn(plot3,LineColor(2));
    
    bkgData2D->plotOn(plot1,MarkerColor(4));
    my2dModel.bkgPDF->plotOn(plot1,LineColor(4));
    bkgData2D->plotOn(plot3,MarkerColor(4));
    my2dModel.bkgPDF->plotOn(plot3,LineColor(4));
    
    TCanvas* fitCan = new TCanvas("fitCan","fitCan",800,400);
    fitCan->Divide(2,1);
    fitCan->cd(1);
    plot1->Draw();
    //TCanvas* fitCan2 = new TCanvas("fitCan2","fitCan2",400,400);
    fitCan->cd(2);
    plot3->Draw();
    fitCan->SaveAs("test.eps");
    //fitCan->cd(3);
    //plot2->Draw();
    //fitCan->cd(4);
    //plot4->Draw();
    //*/
    // - - - - - - - - - - - - - - - - - - - - - 
    
    ///*
    my2dModel.makeParamsConst(true);
    my1dModel.makeParamsConst(true);
    statsFactory *my2dStats = new statsFactory(obs2d,my2dModel.sigPDF,my2dModel.bkgPDF,"2DStats_Feb7.root");
    statsFactory* my1dStats = new statsFactory(obs1d,my1dModel.sigPDF,my1dModel.bkgPDF,"1DStats_Feb7.root");
    
    int nToys = 500;
    
    //my1dStats->runSignificance(10.75*intLumi/20.0,15.61*intLumi/20.,nToys,sigData1D,bkgData1D);
    //my1dStats->runSignificance(10.75*intLumi/20.0,15.61*intLumi/20.,nToys);
    my1dStats->runUpperLimit(10.75*intLumi/20.0,15.61*intLumi/20.0,nToys,sigData1D,bkgData1D);
    //my1dStats->runUpperLimit(10.75*intLumi/20.0,15.61*intLumi/20.0,nToys);
    //my1dStats->hypothesisSeparation(10.75*intLumi/20.,10.75*intLumi/20.,nToys);

    //my2dStats->runSignificance(10.75*intLumi/20.,15.61*intLumi/20.,nToys,sigData2D,bkgData2D);
    //my2dStats->runSignificance(10.75*intLumi/20.,15.61*intLumi/20.,nToys);
    my2dStats->runUpperLimit(10.75*intLumi/20.0,15.61*intLumi/20.0,nToys,sigData2D,bkgData2D);
    //my2dStats->runUpperLimit(10.75*intLumi/20.,15.61*intLumi/20.,nToys);
    //my2dStats->hypothesisSeparation(10.75*intLumi/20.,10.75*intLumi/20.,nToys);
    
    delete my2dStats;
    delete my1dStats;
    //*/
}
