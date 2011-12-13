//#include "fitmZZSignal.C"
#include "fitmZstarSignal.C"
#include "RooRealVar.h"
#include "RooFitResult.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include <vector>
#include <string>
#include "TCanvas.h"
#include "Riostream.h"
#include "TAxis.h"
#include "TF1.h"

void plotFitParams(int btag=1, bool returnMatched=true){//,char* paramName="CMS_zz2l2q_sigma_sig"){

  //gROOT->ProcessLine(".L fitmZZSignal.C");

  const int numMasses=5;
  vector<string> paramName;
  //  paramName.push_back("CMS_zz2l2q_mean_sig");
  if(returnMatched){
    paramName.push_back("CMS_zz2l2q_sigma_sig");
    paramName.push_back("CMS_zz2l2q_alpha1");
    paramName.push_back("CMS_zz2l2q_n1");
    paramName.push_back("CMS_zz2l2q_alpha2");
    paramName.push_back("CMS_zz2l2q_n2");
    paramName.push_back("p1_matched");
    paramName.push_back("p2_matched");
    paramName.push_back("p3_matched");
  }else{
    paramName.push_back("unmatched_mean");
    paramName.push_back("unmatched_width");
    paramName.push_back("p1_unmatched");
    paramName.push_back("p2_unmatched");
    paramName.push_back("p3_unmatched");
    paramName.push_back("p4_unmatched");
    paramName.push_back("frac");
  }

  const int numParams=paramName.size();

  double mass[numMasses]={130.,140.,150.,160.,170.};
  double massError[numMasses]={5.,5.,5.,5.,5.};
  double param[numParams][numMasses];
  double paramError[numParams][numMasses];
  RooFitResult* r;
  RooRealVar *p;

  for(int iMass=0; iMass<numMasses; iMass++){

    r=fitmZstarSignal(mass[iMass],btag,returnMatched,true,false,120,180);

    cout << "done fitting..." << endl;

    for(int iParam=0; iParam<numParams; iParam++){
      cout << "iParam: " << iParam << endl;
      if(!r){
	cout << "no fit result found" << endl;
	param[iParam][iMass]=-9999.;
	paramError[iParam][iMass]=1000.;
	continue;
      } 

      cout << "Getting RooRealVar... " << endl;
      p=(RooRealVar*)r->floatParsFinal().find(paramName[iParam].c_str());
      if(!p){
	cout << "no parameter found" << endl;
	param[iParam][iMass]=-9999;
	paramError[iParam][iMass]=1000;
	continue;
      }

      cout << "saving fit results ... " << endl;
      param[iParam][iMass] = p->getVal();
      paramError[iParam][iMass] = p->getError();
    }
  }

  cout << "Delaring TGraphs ... " << endl;

  TGraph *graph[numParams];  
  //TGraphErrors *graph[numParams];  
  TF1* linear[numParams];
  TF1* quad[numParams];
  TF1* cubic[numParams];
  char linearName[8];
  char quadName[8];
  char cubicName[8];

  cout << "Initializing and plotting TGraphs ... " << endl;

  TCanvas* cParam = new TCanvas("cParam","cParam",900,900);
  cParam->Divide(3,3);

  for(int iParam=0; iParam<numParams; iParam++){

    graph[iParam] = new TGraph(numMasses,mass,param[iParam]);
    //graph[iParam] = new TGraphErrors(numMasses,mass,param[iParam],massError,paramError[iParam]);
    graph[iParam]->GetYaxis()->SetTitle(paramName[iParam].c_str());
    graph[iParam]->GetXaxis()->SetTitle("m_{H} [GeV]");
    graph[iParam]->SetMarkerStyle(2);
    graph[iParam]->SetMarkerColor(2);

    // -------------- Defining fit functions -----------
    sprintf(linearName,"linear_%s",paramName[iParam].c_str());
    linear[iParam] = new TF1(linearName,"[0]+[1]*x",120,180);
    linear[iParam]->SetLineColor(1);
    
    sprintf(quadName,"quad_%s",paramName[iParam].c_str());
    quad[iParam] = new TF1(quadName,"[0]+[1]*x+[2]*x*x",120,180);
    quad[iParam]->SetLineColor(3);

    sprintf(cubicName,"cubic_%s",paramName[iParam].c_str());
    cubic[iParam] = new TF1(cubicName,"[0]+[1]*x+[2]*x*x+[3]*x*x*x",120,180);
    cubic[iParam]->SetLineColor(4);
    
    cout << "performing fits... " << endl;
    // --------------- Fitting functions ---------------
    //if(iParam==1 || iParam==4)
    graph[iParam]->Fit(linear[iParam],"","",120,180);
    //else
    //graph[iParam]->Fit(quad[iParam],"","",120,180);
    //==================================================
    cParam->cd(iParam+1);
    graph[iParam]->Draw("A*");
    //quad[iParam]->Draw("SAME");
    //linear[iParam]->Draw("SAME");
    //cubic[iParam]->Draw("SAME");
  }      


  for(int iParam=0; iParam<numParams; iParam++){
    //if(iParam==1 || iParam==4)
    cout << linear[iParam]->GetParameter(0) << "+" << linear[iParam]->GetParameter(1) << "*mH" << endl;
    //else
    //cout << quad[iParam]->GetParameter(0) << "+" << quad[iParam]->GetParameter(1) << "*mH + " <<  quad[iParam]->GetParameter(2) << "*mH*mH" << endl;
  }


}
