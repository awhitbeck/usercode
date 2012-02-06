

void run2Dand1D(){

gSystem->Load("../PDFs/RooNASAModelSig_cc.so");
gSystem->Load("../PDFs/RooNASAModelBkg_cc.so");
gSystem->Load("../PDFs/RooMzzModelBkg_cc.so"); 
gSystem->Load("../src/NASA2dPdfFactory_cc.so");
gSystem->Load("../src/NASA1dPdfFactory_cc.so");
gSystem->Load("../src/statsFactory_cc.so");

RooRealVar* zzmass = new RooRealVar("zzmass","m_{ZZ}",110,140);
RooRealVar* nasaLD = new RooRealVar("nasaLD","LD",0.,1.);
RooArgSet* obs2d = new RooArgSet(*zzmass,*nasaLD);
RooArgSet* obs1d = new RooArgSet(*zzmass);
TFile* sigFile = new TFile("../datafiles/SMHiggs_125_JHU_v2_wResolution_withDiscriminants.root");
TFile* bkgFile = new TFile("../datafiles/EWKZZ4l_Powheg_total_v5_wResolution_withDiscriminants.root");
RooDataSet *bkgData = new RooDataSet("bkgData","bkgData",(TTree*)bkgFile->Get("angles"),*obs2d);
RooDataSet *sigData = new RooDataSet("sigData","sigData",(TTree*)sigFile->Get("angles"),*obs2d);
NASA2dPdfFactory my2dModel(zzmass,nasaLD);
NASA1dPdfFactory my1dModel(zzmass);
my2dModel.makeParamsConst(true);
my1dModel.makeParamsConst(true);
statsFactory my2dStats(obs2d,my2dModel.sigPDF,my2dModel.bkgPDF,"test2DStats.root");
statsFactory my1dStats(obs1d,my1dModel.sigPDF,my1dModel.bkgPDF,"test1DStats.root");
my1dStats.runSignificance(10.75,15.61,100,sigData,bkgData);
//my1dStats.runUpperLimit(10.75,15.61,100);
my2dStats.runSignificance(10.75,15.61,100,sigData,bkgData);
//my2dStats.runUpperLimit(10.75,15.61,100);
}
