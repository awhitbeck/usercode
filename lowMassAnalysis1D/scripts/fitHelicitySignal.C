


void fitHelicitySignal(){

  gROOT->ProcessLine(".L ../PDFs/RooSpin0ZZstar.cxx+");

  // ============= Initialize Variables ===============

  // ------------ measurables ----------------

  cout << "setting measurables" << endl;

  RooRealVar mLL("mLL","mLL",20,80);
  RooRealVar cosTheta1("cosTheta1","cosTheta1",-1,1);
  RooRealVar cosTheta2("cosTheta2","cosTheta2",0,1);
  RooRealVar phi("phi","phi",-3.1415,3.1415);
  RooRealVar mZZ("mZZ","mZZ",130,170);
  RooRealVar weight("weight","weight",0,100);
  RooRealVar mJJ("mJJ","mJJ",75,105);

  // ------------ parameters ---------------

  cout << "setting parameters" << endl;

  RooRealVar a1Val("a1Val","a1Val",1,0,2);
  a1Val.setConstant(kTRUE);
  RooRealVar a2Val("a2Val","a2Val",0,0,2);
  a2Val.setConstant(kTRUE);
  RooRealVar a3Val("a3Val","a3Val",0,0,2);  
  a3Val.setConstant(kTRUE);
  RooRealVar phi1Val("phi1Val","phi1Val",0,0,2*3.1415);
  phi1Val.setConstant(kTRUE);
  RooRealVar phi2Val("phi2Val","phi2Val",0,0,2*3.1415);
  phi2Val.setConstant(kTRUE);
  RooRealVar phi3Val("phi3Val","phi3Val",0,0,2*3.1415);
  phi3Val.setConstant(kTRUE);
  RooRealVar MZVal("MZVal","MZVal",91.2);
  MZVal.setConstant(kTRUE);
  RooRealVar R1Val("R1Val","R1Val",.15,0,10);
  R1Val.setConstant(kTRUE);
  RooRealVar R2Val("R2Val","R2Val",.15,0,10);
  R2Val.setConstant(kTRUE);

  // =========== define PDF ============

  cout << "defining PDF" << endl;

  RooSpin0ZZstar totalPDF("totalPDF","totalPDF",
			  mLL,
			  cosTheta1,
			  cosTheta2,
			  phi,
			  a1Val,
			  phi1Val,
			  a2Val,
			  phi2Val,
			  a3Val,
			  phi3Val,
			  MZVal,
			  mZZ,
			  R1Val,
			  R2Val
			  );
			  
  // =========== Load Data =============

  cout << "loading data" << endl;

  TFile *f = new TFile("../dataFiles/summer11_SMHiggs_150GeV_lowmass.root");
  TTree *t = (TTree*) f->Get("AngularInfo");
  if(!t){
    cout << "ERROR: tree didn't load" << endl;
    return;
  }
  RooDataSet data("data","data",t,RooArgSet(cosTheta1,cosTheta2,phi));
  
  //totalPDF.fitTo(data);

  cout << "plotting" << endl;

  RooPlot *plot_h1 = cosTheta1.frame();
  RooPlot *plot_h2 = cosTheta2.frame();
  RooPlot *plot_phi = phi.frame();

  data.plotOn(plot_h1);
  totalPDF.plotOn(plot_h1);
  data.plotOn(plot_h2);
  totalPDF.plotOn(plot_h2);
  data.plotOn(plot_phi);
  totalPDF.plotOn(plot_phi);
  
  cout << "drawing" << endl;

  TCanvas *cHelicity =new TCanvas("cHelicity","cHelicity",1200,400);
  cHelicity->Divide(3,1);

  cHelicity->cd(1);
  plot_h1->Draw();
  cHelicity->cd(2);
  plot_h2->Draw();
  cHelicity->cd(3);
  plot_phi->Draw();
  

}
