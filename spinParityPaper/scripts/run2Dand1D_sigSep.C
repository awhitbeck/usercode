using namespace RooFit;

void run2Dand1D_sigSep(int altModelIndex=0, double intLumi=22.0, int nToys = 1000, bool is2D=true,bool pure=true, bool signif=false){
    
    gROOT->ProcessLine(".L ~ntran/tdrstyle.C");
    setTDRStyle();
    gStyle->SetPadLeftMargin(0.16);

    gROOT->ProcessLine(".L  ../src/statsFactory.cc+");

    // =============== num events ===========

    double lowMZZ;
    double highMZZ;
    double sigRate;
    double bkgRate;

    lowMZZ=110; 
    highMZZ=160;
    sigRate=8.3/10.31;
    bkgRate=19.9/10.31;

    //lowMZZ=105; 
    //highMZZ=140;
    //sigRate=2.68097/5.;
    //bkgRate=5.65016/5.;
         
    // --------------------------------------

    string altModelData[8]={"/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/psScalar_store/psScalar_125GeV_wResolution_withDiscriminants.root",
			    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/minGrav_store/minGrav_125GeV_wResolution_withDiscriminants.root",
			    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/vector_store/vector_125GeV_wResolution_withDiscriminants.root",
			    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/psVector_store/psVector_125GeV_wResolution_withDiscriminants.root",
			    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/2hPlus_store/2hPlus_125GeV_wResolution_withDiscriminants.root",
			    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/2hMinus_store/2hMinus_125GeV_wResolution_withDiscriminants.root",
			    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/0hPlus_store/0hPlus_125GeV_wResolution_withDiscriminants.root",
			    "/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/0hPlus_store/0hPlus_125GeV_wResolution_withDiscriminants.root"
    };

    string discrimName[8]={"pseudomelaLD","gravimelaLD","vectormelaLD","psVectormelaLD","2hPlusmelaLD","2hMinusmelaLD","0hPlusmelaLD","melaLD"};    

    // ============ measureables ===============
    RooRealVar* zzmass = new RooRealVar("zzmass","m_{ZZ}",lowMZZ,highMZZ);
    zzmass->setBins(50);
    RooRealVar* LD = new RooRealVar(discrimName[altModelIndex].c_str(),"MELA",0.,1.);
    LD->setBins(40);
    RooArgSet* obs2d = new RooArgSet(*zzmass,*LD);
    RooArgSet* obs1d = new RooArgSet(*zzmass);
    
    // ==================== load data ==================

    TChain *tModel = new TChain("angles");
    tModel->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/SMHiggs_store/SMHiggs_125GeV_wResolution_withDiscriminants.root");
    
    //Model->Add(altModelData[altModelIndex].c_str());

    RooDataSet* model_dataSet = new RooDataSet("model_dataSet","model_dataSet",tModel,is2D?*obs2d:*obs1d);

    if(model_dataSet->sumEntries()<=0){cout << "oops - model" << endl;  return;}

    TChain *tAltModel = new TChain("angles");
    //tAltModel->Add("/scratch0/hep/whitbeck/OLDHOME/4lHelicity/generatorJHU_V02-01-00/minGrav_store/minGrav_125GeV_wResolution_withDiscriminants.root");
    tAltModel->Add(altModelData[altModelIndex].c_str());
    RooDataSet* altModel_dataSet = new RooDataSet("altModel_dataSet","altModel_dataSet",tAltModel,is2D?*obs2d:*obs1d);

    if(altModel_dataSet->sumEntries()<=0){cout << "oops - altModel" << endl;  return;}

    TChain *bkgTree = new TChain("angles");
    bkgTree->Add("PowhegFiles/pwgevents_*_wResolution_withDiscriminants.root");
    
    RooDataSet* bkg_dataSet = new RooDataSet("bkg_dataSet","bkg_dataSet",bkgTree,is2D?*obs2d:*obs1d);

    if(bkg_dataSet->sumEntries()<=0){cout << "oops - powheg" << endl;  return;}

    // ================================================
    
    RooDataHist* model_dataHist = new RooDataHist("model_dataHist","model_dataHist",is2D?*obs2d:*obs1d,*model_dataSet);
    RooDataHist* altModel_dataHist = new RooDataHist("altModel_dataHist","altModel_dataHist",is2D?*obs2d:*obs1d,*altModel_dataSet);
    RooDataHist* bkg_dataHist = new RooDataHist("bkg_dataHist","bkg_dataHist",is2D?*obs2d:*obs1d,*bkg_dataSet);

    RooHistPdf* model_histPDF = new RooHistPdf("model_histPDF","model_histPDF",is2D?*obs2d:*obs1d,*model_dataHist);
    RooHistPdf* altModel_histPDF = new RooHistPdf("altModel_histPDF","altModel_histPDF",is2D?*obs2d:*obs1d,*altModel_dataHist);
    RooHistPdf* bkg_histPDF = new RooHistPdf("bkg_histPDF","bkg_histPDF",is2D?*obs2d:*obs1d,*bkg_dataHist);

    // ================================================

    RooPlot* mzz_plot = zzmass->frame();

    model_dataSet->plotOn(mzz_plot,LineColor(kRed),LineWidth(2),MarkerColor(kRed));
    altModel_dataSet->plotOn(mzz_plot,LineColor(kBlue),LineWidth(2),MarkerColor(kBlue));
    bkg_dataSet->plotOn(mzz_plot,LineColor(kGreen),LineWidth(2),MarkerColor(kGreen));
    
    if(is2D){

    RooPlot* LD_plot = LD->frame();
    
    model_dataSet->plotOn(LD_plot,LineColor(kRed),LineWidth(2),MarkerColor(kRed));
    altModel_dataSet->plotOn(LD_plot,LineColor(kBlue),LineWidth(2),MarkerColor(kBlue));
    bkg_dataSet->plotOn(LD_plot,LineColor(kGreen),LineWidth(2),MarkerColor(kGreen));
    }


    TCanvas* can = new TCanvas("can","can",is2D?600:300,300);
    if(is2D) can->Divide(2,1);
    can->cd(1);
    mzz_plot->Draw();

    if(is2D){
      can->cd(2);
      LD_plot->Draw();
    }

    
    char saveName[100];

    /*
    sprintf(saveName,"epsfiles/%s_kinematics.eps",discrimName[altModelIndex].c_str());
    can->SaveAs(saveName);
    sprintf(saveName,"pngfiles/%s_kinematics.png",discrimName[altModelIndex].c_str());
    can->SaveAs(saveName);
    */

    // - - - - - - - - - - - - - - - - - - - - - 
    
    TTimeStamp t;
    TRandom rand(t.GetNanoSec());
    int randSeed = rand.Uniform(100000);

    sprintf(saveName,"SMHiggs_vs_bkg_%ifb_%i.root", (int)intLumi, randSeed);
    statsFactory *mySignificance;

    if(signif){

      cout << "mysignificance" << endl;

      mySignificance = new statsFactory(is2D?obs2d:obs1d,model_histPDF,bkg_histPDF,randSeed,saveName);
      mySignificance->runSignificance(intLumi*sigRate,intLumi*bkgRate,nToys);
      delete mySignificance;
    }
    
    sprintf(saveName,"hypSep_22fb_fullStats/SMHiggs_vs_altModel_%s_%ifb_%i.root",discrimName[altModelIndex].c_str(), intLumi, randSeed);
    statsFactory *myHypothSep = new statsFactory(obs2d,model_histPDF,altModel_histPDF,randSeed,saveName);
 
    if(!signif){
      if(!pure)
	myHypothSep->hypothesisSeparationWithBackground(sigRate*intLumi,sigRate*intLumi,nToys, model_dataSet, altModel_dataSet, bkg_histPDF,bkgRate*intLumi, bkg_dataSet);
      else
	myHypothSep->hypothesisSeparationWithBackground(sigRate*intLumi,sigRate*intLumi,nToys,bkg_histPDF,bkgRate*intLumi);
    }

    delete myHypothSep;

    /*
    sprintf(saveName,"SMHiggs_vs_bkg_%s_%ifb_%i.root",discrimName[altModelIndex].c_str(), intLumi, randSeed);
    statsFactory *mySepSig_SMH_vs_bkg = new statsFactory(obs2d,bkg_histPDF,model_histPDF,randSeed,saveName);
    mySepSig_SMH_vs_bkg->hypothesisSeparationWithBackground(sigRate*intLumi,sigRate*intLumi,nToys,bkg_histPDF,bkgRate*intLumi);
    delete mySepSig_SMH_vs_bkg;
    */

    cout << "===========REPORT============" << endl;
    cout << "sigRate: " << intLumi*sigRate << endl;
    cout << "bkgRage: " << intLumi*bkgRate << endl;
    cout << "nToys: " << nToys << endl;

}
