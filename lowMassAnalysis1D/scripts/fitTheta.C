



using namespace RooFit;

RooFitResult* fitTheta(){

  gROOT->ProcessLine(".L ../PDFs/RooPosPoly.cc+");
  gROOT->ProcessLine(".L ../PDFs/RooTheta2Func.cc+");


  RooRealVar mZZ("mZZ","m_{ZZ}",600,800);
  RooRealVar cosTheta1("cosTheta1","cosTheta1",-1,1);
  RooRealVar cosTheta2("cosTheta2","cosTheta2",0,1);
  RooRealVar mJJ("mJJ","m_{jj}",75,105);
  RooRealVar PUweight("PUweight","PUweight",0,100);
  RooRealVar MCmatch("MCmatch","MCmatch",0,10);
  RooRealVar weight("weight","weight",0,100);
  RooRealVar nBTags("nBTags","nBTags",-1,2);

  //--------- parameters for PDFs -------------

  RooRealVar a1("a1","a1",5,0,100);  
  RooRealVar a2("a2","a2",5,0,100);
  RooRealVar a3("a3","a3",5,0,100);
  RooRealVar m("m","m",1,0,2);
  RooRealVar g("g","g",.2,-100,100);

  RooRealVar b1("b1","b1",5,0,100);  
  RooRealVar b2("b2","b2",5,0,100);
  RooRealVar b3("b3","b3",5,0,100);

  //------------ PDFs -----------------

  RooTheta2Func theta2PDF("theta2PDF","theta2PDF",cosTheta2,a1,a2,a3,m,g);

  RooPosPoly theta1PDF("theta1PDF","theta1PDF",cosTheta1,b1,b2,b3);
  
  RooProdPdf totalPDF("totalPDF","totalPDF",theta1PDF,theta2PDF);

  //------------ Load data --------------


  TFile *f = new TFile("../dataFiles/forMatthew/summer11_ZJets_madgraph_lowmass.root");
  if(!f){
    cout << "ERROR: could not load file..." << endl;
    return 0;
  }
  RooDataSet data("data","data",(TTree*) f->Get("AngularInfo"),
		   RooArgSet(cosTheta1,cosTheta2,mJJ,mZZ,weight),
		   "mJJ>75&&mJJ<105","weight");
  

  //------------ fit data -----------------

  RooFitResult* r = totalPDF.fitTo(data,SumW2Error(kTRUE),Save(kTRUE));

  //------------ plot ---------------

  RooPlot *Ph1 = cosTheta1.frame(-1,1,20);  
  RooPlot *Ph2 = cosTheta2.frame(0,1,20);

  data.plotOn(Ph1,DataError(RooAbsData::SumW2));
  totalPDF.plotOn(Ph1);
  data.plotOn(Ph2,DataError(RooAbsData::SumW2));
  totalPDF.plotOn(Ph2);

  //----------- draw -----------------

  TCanvas *c = new TCanvas("c","c",800,400);
  c->Divide(2,1);

  c->cd(1);
  Ph1->Draw();
  c->cd(2);
  Ph2->Draw();

  return r;
  
}

  

  
  
 
