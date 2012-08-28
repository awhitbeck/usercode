#include "TH1F.h"
#include "TCanvas.h"
#include <Riostream.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>


#include "RooRealVar.h"
#include "RooGenericPdf.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooProdPdf.h"
#include "RooFFTConvPdf.h"
#include "RooPolynomial.h"
#include "RooWorkspace.h"
#include "RooCBShape.h"
#include "RooExponential.h"
#include "RooBernstein.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"

//#include "../../interface/HZZ2L2QRooPdfs.h"
#include "PDFs/RooDoubleCB.h"

#include "TH1F.h"
#include "TFile.h"
#include "TSystem.h"
#include "TROOT.h"

using namespace std;
//using namespace ROOT::Math;
using namespace RooFit;

//inputs: btag category, observed bkg yield (-> expected one for MC limit calc)
//        mass of Higgs, sigma of Higgs, 4 parameters of CB (depend on mass)

void make_roofitfiles(int btag, int chan, double massH, double sigmaH, double &obs_yield,double &exp_yield, double &alpha, vector<double> cb_pars){

  
  gSystem->Load("libRooFit");
  cout<<"Trying to load custom PDFS ..."<<flush<<endl;
  gSystem->Load("libFFTW");

  string str_btag="dummyb";
  if(btag==0)str_btag="0b";
  else if(btag==1)str_btag="1b";
  else if(btag==2)str_btag="2b";
  else cout<<"Unrecognized number of btags: "<<btag<<endl;

  string str_chan="dummychan";
  if(chan==0)str_chan="ee";
  else if(chan==1)str_chan="mm";
  else cout<<"Unrecognized number of channels: "<<chan<<endl;


  //integration window
  double fitRangeLow=120.;
  double fitRangeHigh=170.0; 
  double lowMassCut=massH-5.;
  double highMassCut=massH+5.;

  // --------------------- measureables -----------------
  RooRealVar CMS_hzz2l2q_mZZ("CMS_hzz2l2q_mZZ", "zz inv mass",fitRangeLow,fitRangeHigh);

  // -------------------- background ---------------------------

  string bkgp1name="CMS_hzz2l2q_bkgp1"; 
  string bkgp2name="CMS_hzz2l2q_bkgp2"; 
  string bkgp3name="CMS_hzz2l2q_bkgp3"; 
  string bkgp4name="CMS_hzz2l2q_bkgp4"; 

  //------ fit to inclusive sidebands ------
  //p1 - 0.0
  //p2 - .434045
  //p3 - 3.44987 +/- 1.04942
  //p4 - 3.07162 +/- .73923
  //------ fit to inclusive signal region ----
  //p1 - 0.0 
  //p2 - .604933
  //p3 - 3.56323 +/- .9574
  //p4 - 3.23295 +/- .6316
  //==========================================

  RooRealVar p1(bkgp1name.c_str(),bkgp1name.c_str(),0.0,0.,1000.);
  p1.setConstant(kTRUE);
  RooRealVar p2(bkgp2name.c_str(),bkgp2name.c_str(),.434045,0.,1000);
  p2.setConstant(kTRUE);
  RooRealVar p3(bkgp3name.c_str(),bkgp3name.c_str(),3.44987,0.,1000.);
  p3.setConstant(kTRUE);
  RooRealVar p4(bkgp4name.c_str(),bkgp4name.c_str(),3.07162,0.,1000.);
  p4.setConstant(kTRUE);

  //  char backgroundPDFname[50];
  //  sprintf(backgroundPDFname,"CMS_hzz2l2q_%s%s",str_chan.c_str(),str_btag.c_str());

  //RooBernstein background(backgroundPDFname,backgroundPDFname,CMS_hzz2l2q_mZZ,RooArgList(p1,p2,p3,p4));
  RooBernstein background("background","background",CMS_hzz2l2q_mZZ,RooArgList(p1,p2,p3,p4));

  ///////////////////////////////////////////////////////////////////////////////////

  ////Fill dataset with REAL DATA 
  
  RooRealVar nBTags("nBTags","nBTags",-1.,3.);
  RooRealVar eventWeight("eventWeight","eventWeight",0,100.);
  RooRealVar mZjj("mZjj","mZjj",0,150.);
  RooRealVar leptType("leptType","lepton type",-1,2);
  RooRealVar met("met","met",0,1000);

  string btag_sel="dummy";
  if(btag==0)btag_sel="nBTags==0.0";
  else if(btag==1)btag_sel="nBTags==1.0";
  else if(btag==2)btag_sel="nBTags==2.0&&met<50.0";
  else btag_sel="DUMMYnBTags==99.0";
  string lept_sel= chan==0 ? "leptType==0.0" :"leptType==1.0" ;
  string tree_sel= btag_sel+"&&mZjj>75.0&&mZjj<105.0&&"+lept_sel;
  stringstream ossmzz1;
  ossmzz1 << float(125.);   // this was changed from fitRangeLow to 125...
  string mzzcut="CMS_hzz2l2q_mZZ>"+ossmzz1.str(); 
  stringstream ossmzz2;
  ossmzz2 << float(fitRangeHigh);
  mzzcut+="&&CMS_hzz2l2q_mZZ<"+ossmzz2.str();
  cout<<"$$$$$$ TEMP SEL:  "<<mzzcut.c_str()<<"  $$$$$$$$$$$$$$$$$$$$$$ 125. - "<< fitRangeHigh<<endl;
  tree_sel+="&&"+mzzcut;

  cout << "TREE SELECTIONS: " << tree_sel << endl;
 
  /* // comment from here to ...
  TFile *dfile = new TFile("fileout-999invpb.root");
  //RooArgList arg1(mZZ);
  RooFormulaVar cut1("mycut1",tree_sel.c_str(),RooArgList(mZZ,nBTags,mZjj,leptType));

  RooDataSet *data_b=new RooDataSet("data_bkg","data_bkg",(TTree*)dfile->Get("tree_passedEvents"),
				    RooArgSet(mZZ,nBTags,mZjj,leptType),cut1,"eventWeight");
  obs_yield=double(data_b->numEntries());
  //RooDataSet *data_b = background.generate(x,int(obs_yield));
  cout<<"\nBTAG "<<btag<<"   OBS_YIELDS: "<<obs_yield<<" ->   "<<int(obs_yield)<<endl<<endl; 
  // ... here when done */

  TFile* file = new TFile("./convertedTree.root");
  
  RooFormulaVar cut1("mycut1",tree_sel.c_str(),RooArgList(CMS_hzz2l2q_mZZ,nBTags,mZjj,leptType,met));
  RooDataSet *dataset_obs_orig=new RooDataSet("dataset_obs_orig","dataset_obs_orig",(TTree*)file->Get("AngularInfo"),
					      RooArgSet(CMS_hzz2l2q_mZZ,nBTags,mZjj,leptType,met),
					      cut1,"eventWeight");
  obs_yield=double(dataset_obs_orig->numEntries());

  RooArgSet *newMZZargset= new RooArgSet(CMS_hzz2l2q_mZZ);
  RooDataSet *dataset_obs=(RooDataSet*) dataset_obs_orig->reduce(*newMZZargset);
  dataset_obs->SetName("dataset_obs");
  cout<<"Dataset entries: ORIG "<< dataset_obs_orig->sumEntries()<< "   NEW "<<dataset_obs->sumEntries()<<endl;
 
  // ----------------------------------------------

  // ====================== defining signal PDF =========================

  vector<double> param;
 
  // ------------------- Crystal Ball --------------------------
  // ================ Matched events PDF =======================

  string sigp1name="CMS_hzz2l2q_sig_"+str_btag+"_p1"; //m
  string sigp2name="CMS_hzz2l2q_sig_"+str_btag+"_p2"; //width
  string sigp3name="CMS_hzz2l2q_sig_"+str_btag+"_p3"; //alpha1
  string sigp4name="CMS_hzz2l2q_sig_"+str_btag+"_p4"; //n1
  string sigp5name="CMS_hzz2l2q_sig_"+str_btag+"_p5"; //alpha2
  string sigp6name="CMS_hzz2l2q_sig_"+str_btag+"_p6"; //n2
  if(cb_pars[0]>1000.||cb_pars[0]<0.) assert(0);
  RooRealVar CB_mean(sigp1name.c_str(),sigp1name.c_str(),cb_pars[0],0.,1000.);
  CB_mean.setConstant(kTRUE);
  if(cb_pars[1]>1000.||cb_pars[1]<0.) assert(0);
  RooRealVar CB_sigma(sigp2name.c_str(),sigp2name.c_str(),cb_pars[1],0.,1000.);
  CB_sigma.setConstant(kTRUE);
  if(cb_pars[2]>1000.||cb_pars[2]<0.) assert(0);
  RooRealVar CB_alpha1(sigp3name.c_str(),sigp3name.c_str(),cb_pars[2],0.,1000.);
  CB_alpha1.setConstant(kTRUE);
  if(cb_pars[3]>1000.||cb_pars[3]<0.) assert(0);
  RooRealVar CB_n1(sigp4name.c_str(),sigp4name.c_str(),cb_pars[3],0.,1000.);
  CB_n1.setConstant(kTRUE);
  if(cb_pars[4]>1000.||cb_pars[4]<0.) assert(0);
  RooRealVar CB_alpha2(sigp5name.c_str(),sigp5name.c_str(),cb_pars[4],0.,1000.);
  CB_alpha2.setConstant(kTRUE);
  if(cb_pars[5]>1000.||cb_pars[5]<0.) assert(0);
  RooRealVar CB_n2(sigp6name.c_str(),sigp6name.c_str(),cb_pars[5],0.,1000.);
  CB_n2.setConstant(kTRUE);

  RooDoubleCB CB_SIG("CB_SIG","Crystal Ball",CMS_hzz2l2q_mZZ,CB_mean,CB_sigma,CB_alpha1,CB_n1,CB_alpha2,CB_n2);

  // ------------------- Crystal Ball --------------------------
  // ================ Matched events PDF =======================
  
  string sigp7name="CMS_hzz2l2q_sig_"+str_btag+"_p7"; //unmatched mean
  string sigp8name="CMS_hzz2l2q_sig_"+str_btag+"_p8"; //unmatched width

  if(cb_pars[6]>1000.||cb_pars[6]<0.) assert(0);
  RooRealVar Gauss_mean(sigp7name.c_str(),sigp7name.c_str(),cb_pars[6],0,1000.);
  Gauss_mean.setConstant(kTRUE);
  if(cb_pars[7]>1000.||cb_pars[7]<0.) assert(0);
  RooRealVar Gauss_width(sigp8name.c_str(),sigp8name.c_str(),cb_pars[7],0,1000.);
  Gauss_width.setConstant(kTRUE);



  RooGaussian Gauss_SIG("Gauss_SIG","Gauss_SIG",CMS_hzz2l2q_mZZ,Gauss_mean,Gauss_width);
  
  //------------------------ total PDF: sum -------------------------

  string sigp9name="CMS_hzz2l2q_sig_"+str_btag+"_p9"; //unmatched frac

  RooRealVar frac(sigp9name.c_str(),sigp9name.c_str(),cb_pars[8],0,1);
  frac.setConstant(kTRUE);

  //  char signalPDFname[50];
  //  sprintf(signalPDFname,"CMS_hzz2l2q_%s%s",str_chan.c_str(),str_btag.c_str());

  RooAddPdf signal("signal","signal",RooArgSet(Gauss_SIG,CB_SIG),frac);
  //RooAddPdf signal(signalPDFname,signalPDFname,RooArgSet(Gauss_SIG,CB_SIG),frac);

  cout << "====================== mZZ sidebands scaling ================== " << endl;
  cout << "Generated events based on background shape according to exact distribution...." << endl;

  CMS_hzz2l2q_mZZ.setRange("lowSideband",125,lowMassCut);
  CMS_hzz2l2q_mZZ.setRange("highSideband",highMassCut,170);
  RooAbsReal* alpha_numerator=0,*alpha_denominator=0,*alpha_denominator2=0;
  alpha_numerator =  background.createIntegral(CMS_hzz2l2q_mZZ);
  alpha_denominator = background.createIntegral(CMS_hzz2l2q_mZZ,Range("lowSideband"));
  alpha_denominator2 = background.createIntegral(CMS_hzz2l2q_mZZ,Range("highSideband"));
  alpha = alpha_numerator->getVal()/(alpha_denominator->getVal()+alpha_denominator2->getVal());
  cout << "alpha: " << alpha << endl;

    /* old code for calculating alpha ... was buggy but I don't know why
  RooBernstein normalizer(background,"normalizer");
  RooDataHist* fauxData = normalizer.generateBinned(RooArgSet(CMS_hzz2l2q_mZZ),1000,kTRUE);
  cout << "Generated Number of events: " << fauxData->sumEntries() << endl;
  sprintf(sidebandCutString,"(CMS_hzz2l2q_mZZ>125&&CMS_hzz2l2q_mZZ<%f)||(CMS_hzz2l2q_mZZ>%f&&CMS_hzz2l2q_mZZ<170)",lowMassCut,highMassCut);
  cout << "Sideband selection: " << sidebandCutString << endl;
  alpha = fauxData->sumEntries()/fauxData->sumEntries(sidebandCutString);
  cout << "alpha: " << alpha << endl;
    */

  char sidebandCutString[350];


  if(btag==2)
    sprintf(sidebandCutString,"((CMS_hzz2l2q_mZZ>125&&CMS_hzz2l2q_mZZ<%f)||(CMS_hzz2l2q_mZZ>%f&&CMS_hzz2l2q_mZZ<170))&&mZjj<105&&mZjj>75&&nBTags==2&&met<50",lowMassCut,highMassCut);
  else
    sprintf(sidebandCutString,"((CMS_hzz2l2q_mZZ>125&&CMS_hzz2l2q_mZZ<%f)||(CMS_hzz2l2q_mZZ>%f&&CMS_hzz2l2q_mZZ<170))&&mZjj<105&&mZjj>75&&nBTags==%i&&leptType==%i",lowMassCut,highMassCut,btag,chan);

  cout << sidebandCutString << endl;

  RooDataSet *mZZSidebandData=new RooDataSet("mZZSidebandData","mZZSidebandData",(TTree*)file->Get("AngularInfo"),
					     RooArgSet(CMS_hzz2l2q_mZZ,nBTags,mZjj,leptType,met),sidebandCutString);
  
  cout << "Number of Events in Sideband: " <<  mZZSidebandData->sumEntries() << endl;
  
  if(btag<2)
    exp_yield = mZZSidebandData->sumEntries()*alpha;
  else{
    if(chan==0)
      exp_yield = mZZSidebandData->sumEntries()*alpha*.406;
    else
      exp_yield = mZZSidebandData->sumEntries()*alpha*.594;
  }
  
  cout << "Number of expected events: " << exp_yield << endl;
  
  delete mZZSidebandData;

  //  - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Note that in the above the events in the mZZ sideband
  // region are counted in each of the six channels separately
  // except in 2 b-tags where the total is counted and multiplied
  // by the expected fraction of ee/mm events
  //  - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  cout << "================================================================ " << endl;

  //--- write everything into the workspace -------  

  RooWorkspace* w = new RooWorkspace("w","w");
  w->addClassDeclImportDir("/afs/cern.ch/cms/slc5_amd64_gcc462/lcg/roofit/5.34.00-cms5/");
  w->addClassDeclImportDir("/afs/cern.ch/work/w/whitbeck/private/2l2qCombination/CMSSW_5_2_4/src/HiggsAnalysis/CombinedLimit/test/makeDataCards/PDFs/");

  w->importClassCode(RooDoubleCB::Class(),kTRUE);
  w->importClassCode("RooDoubleCB",kTRUE);
  cout << "importing background" << endl;
  w->import(background);
  cout << "importing signal" << endl;
  w->import(signal);
  cout << "importing data" << endl;
  w->import(*dataset_obs);

  cout << "done with imports" << endl;

  char char_massH[5];
  sprintf(char_massH,"%i",(int)massH);

  char* char_btag="dummyb";
  if(btag==0)char_btag="0b";
  else if(btag==1)char_btag="1b";
  else if(btag==2)char_btag="2b";
  else cout<<"Unrecognized number of btags: "<<btag<<endl;

  char* char_chan="dummychan";
  if(chan==0)char_chan="ee";
  else if(chan==1)char_chan="mm";
  else cout<<"Unrecognized number of channels: "<<chan<<endl;

  cout << "char_massH: " << char_massH << endl;
  cout << "char_chan: " << char_chan << endl;
  cout << "char_btag: " << char_btag << endl;

  char outFileName[150];
  sprintf(outFileName,"datacards_testJHEPresults/%s/hzz2l2q_%s%s.input.root",char_massH,char_chan,char_btag);
  
  cout<<"OUTFILENAME "<<outFileName<<endl;

  TFile* outFile = new TFile(outFileName,"RECREATE");
  cout << "file open " << endl;
  w->Write();
  cout << "workspace written" << endl;
  outFile->Close();
  cout << "file closed" << endl;
  
  delete file;
  
  cout << "done" << endl;
}


