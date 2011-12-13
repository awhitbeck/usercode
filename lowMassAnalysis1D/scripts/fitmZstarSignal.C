#include <cstdlib>
#include <iostream>
#include "RooRealVar.h"
#include "TTree.h"
#include "TFile.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "TCanvas.h"
#include "RooFitResult.h"
#include "../PDFs/Roo2l2jMasses2D.h"
#include "../PDFs/Roo2l2jMasses2D_DoubleCB.h"
#include "RooAddPdf.h"
#include "TLegend.h"
#include "RooWorkspace.h"

using namespace RooFit;

RooFitResult* fitmZstarSignal(
			      int mH=150, 
			      int btag=1,
			      bool returnMatched=true,
			      bool draw=true,
			      bool fix=false,
			      double low_M=120,
			      double high_M=170			      
			      ){

  //gSystem->Load("libRooFit");  
  //gROOT->ProcessLine(".L ../PDFs/Roo2l2jMasses2D.cc+");
  //gROOT->ProcessLine(".L ../PDFs/Roo2l2jMasses2D_DoubleCB.cc+");

  //================ Parameters for Signal PDF ===============
            //=====================================

 char* name="";

  //================== measurables

  cout << "initializing measurables..." <<endl;

  RooRealVar mLL("mLL","mZ*",20,90);
  RooRealVar mZZ("mZZ","m_{ZZ}",low_M,high_M);
  RooRealVar mJJ("mJJ","m_{jj}",75,105);
  RooRealVar nBTags("nBTags","nBTags",-1,2);  
  RooRealVar weight("weight","weight",0,10);
  RooRealVar MCmatch("MCmatch","MCmatch",0,10);
  RooRealVar met("met","met",0,100);
  //================== P_1(m'Z*,mZZ) ==================
	//=============================

  cout << "initializing parameters... " << endl;

  RooRealVar mZ("mZ","m_{Z}",60,120);
  RooRealVar Gamma("Gamma","#Gamma",0,100);
  
  mZ.setVal(91.2);   mZ.setConstant(kTRUE);
  Gamma.setVal(0.); Gamma.setConstant(kTRUE);

  //================= G(m'Z*) ========================
 	//=============================
  //p1_matched -34.1741
  //p1_matched 11.2426
  //p1_matched 0.0937486
  //CMS_zz4l_mean_sig 150
  //CMS_zz4l_sigma_sig 1.95714
  //CMS_zz4l_alpha1 0.591475
  //CMS_zz4l_n1 9.97456
  //CMS_zz4l_alpha2 0.981696
  //CMS_zz4l_n 6.69743
  //p1_unmatched 1.80961
  //p2_unmatche 0.0039612
  //p3_unmatches -0.000287367
  //unmatched_mean 141.037
  //unmatched_width 14.7561

  //============== P(m'Z*,mZZ) ======================
	//============================
  vector<string> paramName;
  paramName.push_back("CMS_zz2l2q_mean_sig");
  paramName.push_back("CMS_zz2l2q_sigma_sig");
  paramName.push_back("CMS_zz2l2q_alpha1");
  paramName.push_back("CMS_zz2l2q_n1");
  paramName.push_back("CMS_zz2l2q_alpha2");
  paramName.push_back("CMS_zz2l2q_n2");
  paramName.push_back("p1_matched");
  paramName.push_back("p2_matched");
  paramName.push_back("p3_matched");
  paramName.push_back("unmatched_mean");
  paramName.push_back("unmatched_width");
  paramName.push_back("p1_unmatched");
  paramName.push_back("p2_unmatched");
  paramName.push_back("p3_unmatched");
  paramName.push_back("p4_unmatched");
  paramName.push_back("frac");
  vector<double> paramVal;
  if(btag==0){
    // ========================= v0 ========================
    /*
    paramVal.push_back(mH 						    );
    paramVal.push_back(-34.2747+.459182*mH-.00142578*mH*mH		    );   //sigma     
    paramVal.push_back(1.99625-.0057799*mH				    );   //alpha1    
    paramVal.push_back(31.47-.160475*mH				            );   //n1	     
    paramVal.push_back(-23.6484+.317242*mH-.00102403*mH*mH		    );   //alpha2    
    paramVal.push_back(18.4988-.100371*mH				    );   //n2	     
    paramVal.push_back(.13011                                               );   //p1_matched
    paramVal.push_back(2.                                                   );   //p2_matched
    paramVal.push_back(2.2                                                  );   //p3_matched
    paramVal.push_back(288.8-1.8888*mH+.00496554*mH*mH+1.2149e-5*mH*mH*mH   );
    paramVal.push_back(568.179-6.91623*mH+.0214025*mH*mH                    );
    paramVal.push_back(.13011                                               );   
    paramVal.push_back(2.                                                   );
    paramVal.push_back(2.2                                                  );
    paramVal.push_back(2.2                                                  );
    paramVal.push_back(.75-.00316*mH                                        );
    */
    // ========================= v1 ========================
    paramVal.push_back(mH 						                );
    paramVal.push_back(34.1639-0.46412*mH + 0.00166325*mH*mH				);   //sigma     	
    paramVal.push_back(18.2297-0.224861*mH + 0.000712743*mH*mH			        );   //alpha1    	
    paramVal.push_back(35.0929-0.18584*mH						);   //n1	     	
    paramVal.push_back(298.762-6.28415*mH + 0.0439633*mH*mH - 0.000101968*mH*mH*mH	);   //alpha2    	
    paramVal.push_back(30.1615-0.167257*mH						);   //n2	     	
    paramVal.push_back(9.32436-0.052872*mH						);   //p1_matched	
    paramVal.push_back(54.3158-0.318029*mH		 				);   //p2_matched	
    paramVal.push_back(94.1471-0.542694*mH                                              );   //p3_matched    	
    paramVal.push_back(-879.044+11.8295*mH -0.03341*mH*mH                               );   //unmatched_mean	
    paramVal.push_back(324.37-3.32413*mH + 0.00837186*mH*mH                             );   //unmatched_width	
    paramVal.push_back(-1.11596+0.00876525*mH                                           );   //p1_unmatched	
    paramVal.push_back(20.6588-0.103222*mH                                              );   //p2_unmatched	
    paramVal.push_back(-40.9796+0.315257*mH                                             );   //p3_unmatched	
    paramVal.push_back(22.4361-0.0862908*mH                                             );   //p4_unmatched     
    paramVal.push_back(-3.21142+0.0526514*mH-0.000201072*mH*mH<0?
		       0.:
		       -3.21142+0.0526514*mH-0.000201072*mH*mH                          );   //frac

  }
  if(btag==1){
    //========================== v0 ============================
    /*
    paramVal.push_back(mH				     );
    paramVal.push_back(20.5495-.285063*mH+.00108348*mH*mH    );   //sigma     	      
    paramVal.push_back(20.7158-.259139*mH+.000855349*mH*mH   );	  //alpha1    	      
    paramVal.push_back( 9				     );	  //n1	     	      
    paramVal.push_back(-1.75895+.0170921*mH		     );	  //alpha2    	      
    paramVal.push_back(16.6304-0.0902441*mH		     );	  //n2	     	      
    paramVal.push_back(.13011                                );	  //p1_matched	      
    paramVal.push_back(2.                                    );	  //p2_matched	      
    paramVal.push_back(2.2                                   );	  //p3_matched        
    paramVal.push_back(125.536+.241826*mH		     );	  //unmatched_mean    
    paramVal.push_back(70.7661-.377219*mH		     );	  //unmatched_width   
    paramVal.push_back(.13011                                );	  //p1_unmatched      
    paramVal.push_back(2.                                    );	  //p2_unmatched      
    paramVal.push_back(2.2                                   );	  //p3_unmatched      
    paramVal.push_back(2.2                                   );	  //p4_unmatched      
    paramVal.push_back(1.26634-.00625*mH                     );
    */






    paramVal.push_back(mH				                                );   
    paramVal.push_back(25.0374-0.336364*mH + 0.00121627*mH*mH	);	  //sigma     	      
    paramVal.push_back(3.59286-0.0289593*mH + 6.12756e-05*mH*mH );	  //alpha1    	      
    paramVal.push_back(-53.9862+1.00772*mH  -0.0039074*mH*mH	);	  //n1	     	      
    paramVal.push_back(33.0186-0.464978*mH + 0.00167324*mH*mH	);	  //alpha2    	      
    paramVal.push_back(-81.6832+1.39671*mH  -0.00538508*mH*mH   );	  //n2	     	      
    paramVal.push_back(51.7557-0.289652*mH                      );	  //p1_matched	      
    paramVal.push_back(404.024-2.32218*mH                       );	  //p2_matched	      	
    paramVal.push_back(217.577-1.21259*mH                       );	  //p3_matched        	
    paramVal.push_back(-244.446+2.49422*mH                      );	  //unmatched_mean    	
    paramVal.push_back(142.717-0.784516*mH                      );	  //unmatched_width   	
    paramVal.push_back(1.12941-0.00653293*mH                    );	  //p1_unmatched      	
    paramVal.push_back(13.4631-0.0814795*mH<0?
		       0:
		       13.4631-0.0814795*mH                     );	  //p2_unmatched      	
    paramVal.push_back(-4.46277+0.0499132*mH                    );	  //p3_unmatched      
    paramVal.push_back(28.6986-0.148659*mH                      );	  //p4_unmatched      
    paramVal.push_back(1.44023-0.00796164*mH                    );        //frac

  }
  if(btag==2){
    /*
    //================================ v0 ============================
    paramVal.push_back(mH				    );
    paramVal.push_back(25.0374-0.336364*mH + 0.00121627*mH*mH	);	  //sigma     	      
    paramVal.push_back(3.59286-0.0289593*mH + 6.12756e-05*mH*mH );	  //alpha1    	      
    paramVal.push_back(-53.9862+1.00772*mH  -0.0039074*mH*mH	);	  //n1	     	      
    paramVal.push_back(33.0186-0.464978*mH + 0.00167324*mH*mH	);	  //alpha2    	      
    paramVal.push_back(-81.6832+1.39671*mH  -0.00538508*mH*mH   );	  //n2	     	      
    paramVal.push_back(51.7557-0.289652*mH                      );	  //p1_matched	      
    paramVal.push_back(404.024-2.32218*mH                       );	  //p2_matched	      	
    paramVal.push_back(217.577-1.21259*mH                       );	  //p3_matched        	
    paramVal.push_back(-244.446+2.49422*mH                      );	  //unmatched_mean    	
    paramVal.push_back(142.717-0.784516*mH                      );	  //unmatched_width   	
    paramVal.push_back(1.12941-0.00653293*mH                    );	  //p1_unmatched      	
    paramVal.push_back(13.4631-0.0814795*mH<0?
		       0:
		       13.4631-0.0814795*mH                     );	  //p2_unmatched      	
    paramVal.push_back(-4.46277+0.0499132*mH                    );	  //p3_unmatched      
    paramVal.push_back(28.6986-0.148659*mH                      );	  //p4_unmatched      
    paramVal.push_back(1.44023-0.00796164*mH                    );        //frac
    */






    //================================ v1 ============================
    paramVal.push_back(mH				    );
    paramVal.push_back(-0.37747+0.0211872*mH );	  //sigma     	      
    paramVal.push_back(-0.56045+0.010567*mH  );	  //alpha1    	      
    paramVal.push_back(1.54696e+01-3.51871e-02*mH   );	  //n1	     	      
    paramVal.push_back(-2.40699+0.0258781*mH );	  //alpha2    	      
    paramVal.push_back(29.3521-0.147738*mH   );	  //n2	     	      
    paramVal.push_back(58.9593-0.268873*mH );	  //p1_matched	      
    paramVal.push_back(669.891-3.72548*mH  );	  //p2_matched	      	
    paramVal.push_back(811.384-4.68439*mH  );	  //p3_matched        	
    paramVal.push_back(-28.9431+1.11513*mH                 );	  //unmatched_mean    	
    paramVal.push_back(104.529-0.574685*mH                 );	  //unmatched_width   	
    paramVal.push_back(1.12941-0.00653293*mH                    );	  //p1_unmatched      	
    paramVal.push_back(13.4631-0.0814795*mH<0?
		       0:
		       13.4631-0.0814795*mH                     );	  //p2_unmatched      	
    paramVal.push_back(-4.46277+0.0499132*mH                    );	  //p3_unmatched      
    paramVal.push_back(28.6986-0.148659*mH                      );	  //p4_unmatched      
    paramVal.push_back(0.0                   );        //frac


  }

  RooRealVar CMS_zz2l2q_mean_sig(paramName.at(0).c_str(),paramName.at(0).c_str(), paramVal[0],0,1000);
  CMS_zz2l2q_mean_sig.setConstant(kTRUE);
  RooRealVar CMS_zz2l2q_sigma_sig(paramName.at(1).c_str(),paramName.at(1).c_str(),paramVal[1],0.,100);
  CMS_zz2l2q_sigma_sig.setConstant(kTRUE); 
  RooRealVar CMS_zz2l2q_alpha1(paramName.at(2).c_str(),paramName.at(2).c_str(),   paramVal[2],0.,10.);
  CMS_zz2l2q_alpha1.setConstant(kTRUE);
  RooRealVar CMS_zz2l2q_n1(paramName.at(3).c_str(),paramName.at(3).c_str(),       paramVal[3],0.,10.);
  CMS_zz2l2q_n1.setConstant(kTRUE);
  RooRealVar CMS_zz2l2q_alpha2(paramName.at(4).c_str(),paramName.at(4).c_str(),   paramVal[4],0.,10.);
  CMS_zz2l2q_alpha2.setConstant(kTRUE);
  RooRealVar CMS_zz2l2q_n2(paramName.at(5).c_str(),paramName.at(5).c_str(),       paramVal[5],0.,10.);
  CMS_zz2l2q_n2.setConstant(kTRUE);

  RooRealVar p1_matched(paramName.at(6).c_str(),paramName.at(6).c_str(),          paramVal[6],0.,1000.);
  p1_matched.setConstant(kTRUE);
  RooRealVar p2_matched(paramName.at(7).c_str(),paramName.at(7).c_str(),          paramVal[7],0.,1000.);
  p2_matched.setConstant(kTRUE);
  RooRealVar p3_matched(paramName.at(8).c_str(),paramName.at(8).c_str(),          paramVal[8],0.,1000.);
  p3_matched.setConstant(kTRUE);

  //============== unmatched PDF ====================
        // ===========================
  RooRealVar unmatched_mean(paramName.at(9).c_str(),paramName.at(9).c_str(),    paramVal[9],0.,300.);
  //unmatched_mean.setConstant(kTRUE);
  RooRealVar unmatched_width(paramName.at(10).c_str(),paramName.at(10).c_str(),   paramVal[10],0.,1000.);  
  //unmatched_width.setConstant(kTRUE);
  RooRealVar p1_unmatched(paramName.at(11).c_str(),paramName.at(11).c_str(),        paramVal[11],0.,1000.);
  //p1_unmatched.setConstant(kTRUE);
  RooRealVar p2_unmatched(paramName.at(12).c_str(),paramName.at(12).c_str(),      paramVal[12],0.,1000.);
  //p2_unmatched.setConstant(kTRUE);
  RooRealVar p3_unmatched(paramName.at(13).c_str(),paramName.at(13).c_str(),      paramVal[13],0.,1000.);
  //p3_unmatched.setConstant(kTRUE);
  RooRealVar p4_unmatched(paramName.at(14).c_str(),paramName.at(14).c_str(),      paramVal[14],0.,1000.);
  //p4_unmatched.setConstant(kTRUE);

  RooRealVar frac(paramName.at(15).c_str(),paramName.at(15).c_str(),                 paramVal[15],0.,1.);
  //frac.setConstant(kTRUE);

  cout << "defining PDF..." << endl;
  
  Roo2l2jMasses2D_DoubleCB matchedPDF("matchedPDF","matchedPDF",
  				      mLL,
				      mZ,
				      mZZ,
				      Gamma,
				      p1_matched,
				      p2_matched,
				      p3_matched,
				      CMS_zz2l2q_mean_sig,
				      CMS_zz2l2q_sigma_sig,
				      CMS_zz2l2q_alpha1,
				      CMS_zz2l2q_n1,
				      CMS_zz2l2q_alpha2,
				      CMS_zz2l2q_n2
				      );

  Roo2l2jMasses2D unMatchedPDF("unMatchedPDF","unMatchedPDF",
			       mLL,
			       mZ,
			       mZZ,
			       Gamma,
			       p1_unmatched,
			       p2_unmatched,
			       p3_unmatched,
			       p4_unmatched,
			       unmatched_mean,
			       unmatched_width
			       );

  RooAddPdf totalPDF_sig("totalPDF_sig","totalPDF_sig",unMatchedPDF,matchedPDF,frac);

  //============== load data ========================
	//============================
  
  cout << "loading data..." << endl;

  char fileName[100];
  if(mH==130||mH==150)
    sprintf(fileName,"../dataFiles/withHLTweights/summer11_SMHiggs_%iGeV_lowmass.root",mH);
  if(mH==140||mH==160 || mH==170 || mH==180)
    sprintf(fileName,"../dataFiles/withHLTweights/spring11_SMHiggs_%iGeV_SPRING11_lowmass.root",mH);

  TFile *file = new TFile(fileName);
  char cutString_matched[100];
  char cutString_unmatched[100];
  char cutString[100];

  if(btag<2){
    sprintf(cutString_matched,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<90&&mJJ>75&&mJJ<105&&nBTags==%i&&MCmatch==1",high_M,low_M,btag);
    sprintf(cutString_unmatched,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<90&&mJJ>75&&mJJ<105&&nBTags==%i&&MCmatch==0",high_M,low_M,btag);
    sprintf(cutString,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<90&&mJJ>75&&mJJ<105&&nBTags==%i",high_M,low_M,btag);
  }
  else{
    sprintf(cutString_matched,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<90&&mJJ>75&&mJJ<105&&nBTags==%i&&MCmatch==1&&met<50",high_M,low_M,btag);
    sprintf(cutString_unmatched,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<90&&mJJ>75&&mJJ<105&&nBTags==%i&&MCmatch==0&&met<50",high_M,low_M,btag);
    sprintf(cutString,"mZZ<%f&&mZZ>%f&&mLL>20&&mLL<90&&mJJ>75&&mJJ<105&&nBTags==%i&&met<50",high_M,low_M,btag);
  }

  cout << "check1" << endl;

  TTree* t;
  if(file)
    t = (TTree*) file->Get("AngularInfo");
  else{
    cout << "ERROR: could not load file..." << endl;
    return 0;
  }

  cout << "check2" << endl;

  if(!t){
    cout << "ERROR: could not load tree..." << endl;
    return 0;
  }
  
  RooDataSet data_matched("data_matched","data_matched",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags,weight,MCmatch,met),cutString_matched,"weight");
  double NumMatched = data_matched.sumEntries();
  RooDataSet data_unmatched("data_unmatched","data_unmatched",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags,weight,MCmatch,met),cutString_unmatched,"weight");
  double NumUnmatched = data_unmatched.sumEntries();
  RooDataSet data("data","data",(TTree*)file->Get("AngularInfo"),RooArgSet(mZZ,mLL,mJJ,nBTags,weight,MCmatch,met),cutString,"weight");
  double NumTotal = data.sumEntries();

  //frac.setVal(NumUnmatched/NumTotal);
  //frac.setConstant(kTRUE);

  cout << "fitting data..." << endl;

  RooFitResult *r1,*r2;

  if(!fix){
  //r1 = matchedPDF.fitTo(data_matched,SumW2Error(kTRUE),Save(kTRUE));
  //r2 = unMatchedPDF.fitTo(data_unmatched,SumW2Error(kTRUE),Save(kTRUE));
  r2 = totalPDF_sig.fitTo(data,SumW2Error(kTRUE),Save(kTRUE));
  }

  p1_matched.setConstant(kTRUE);
  p2_matched.setConstant(kTRUE);
  p3_matched.setConstant(kTRUE);
  CMS_zz2l2q_mean_sig.setConstant(kTRUE);
  CMS_zz2l2q_sigma_sig.setConstant(kTRUE);
  CMS_zz2l2q_alpha1.setConstant(kTRUE);
  CMS_zz2l2q_n1.setConstant(kTRUE);
  CMS_zz2l2q_alpha2.setConstant(kTRUE);
  CMS_zz2l2q_n2.setConstant(kTRUE);
  p1_unmatched.setConstant(kTRUE);
  p2_unmatched.setConstant(kTRUE);
  p3_unmatched.setConstant(kTRUE);
  unmatched_mean.setConstant(kTRUE);
  unmatched_width.setConstant(kTRUE);
  frac.setConstant(kTRUE);

  TLegend *leg = new TLegend(.2,.8,.6,.6);
  RooPlot *plotmLL=mLL.frame(20,90,20);
  RooPlot *plotmZZ=mZZ.frame(low_M,high_M,20);
  RooPlot *plotmLL2=mLL.frame(20,90,20);
  RooPlot *plotmZZ2=mZZ.frame(low_M,high_M,20);
  RooPlot *plotmLL3=mLL.frame(20,90,20);
  RooPlot *plotmZZ3=mZZ.frame(low_M,high_M,20);
  TCanvas*c = new TCanvas("c","c",800,1200);
  
  char imageName[50];
  RooWorkspace* w = new RooWorkspace("w","w");
  char outWorkspace[100];
  TFile *workspaceFile = new TFile(outWorkspace,"RECREATE");

  if(draw){

    cout << "plotting data/fits..." << endl;
    
    data.plotOn(plotmZZ,DataError(RooAbsData::SumW2));
    totalPDF_sig.plotOn(plotmZZ);//,FillColor(3),DrawOption("F"));
    unMatchedPDF.plotOn(plotmZZ,LineStyle(2),Normalization(frac.getVal()));//FillColor(6),DrawOption("F"),Normalization(NumUnmatched/NumTotal));
    data.plotOn(plotmLL,DataError(RooAbsData::SumW2));
    totalPDF_sig.plotOn(plotmLL);//,FillColor(3),DrawOption("F"));
    unMatchedPDF.plotOn(plotmLL,LineStyle(2),Normalization(frac.getVal()));//FillColor(6),DrawOption("F"),Normalization(NumUnmatched/NumTotal));

    data_matched.plotOn(plotmZZ2,DataError(RooAbsData::SumW2));
    matchedPDF.plotOn(plotmZZ2);//,FillColor(3),DrawOption("F"));
    data_matched.plotOn(plotmLL2,DataError(RooAbsData::SumW2));
    matchedPDF.plotOn(plotmLL2);//,FillColor(3),DrawOption("F"));

    data_unmatched.plotOn(plotmZZ3,DataError(RooAbsData::SumW2));
    unMatchedPDF.plotOn(plotmZZ3,Normalization(frac.getVal()*NumTotal/NumUnmatched));//,FillColor(3),DrawOption("F"));
    data_unmatched.plotOn(plotmLL3,DataError(RooAbsData::SumW2));
    unMatchedPDF.plotOn(plotmLL3,Normalization(frac.getVal()*NumTotal/NumUnmatched));//,FillColor(3),DrawOption("F"));

    cout << "drawing..." << endl;
    
    c->Divide(2,3);
    c->cd(1);
    plotmLL->Draw();
    c->cd(2);  
    plotmZZ->Draw();	
    c->cd(3);
    plotmLL2->Draw();
    c->cd(4);  
    plotmZZ2->Draw();	
    c->cd(5);
    plotmLL3->Draw();
    c->cd(6);  
    plotmZZ3->Draw();	

    /*
    for (int i=0; i<plotmZZ->numItems(); i++) {
      TString obj_name=plotmZZ->nameOf(i); if (obj_name=="") continue;
      cout << Form("%d. '%s'\n",i,obj_name.Data());
    }
    */

    leg->SetTextSize(0.036);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->AddEntry("totalPDF_sig_Int[mLL]_Norm[mLL,mZZ]","Signal Fit","l");
    leg->AddEntry("unMatchedPDF_Int[mLL]_Norm[mLL,mZZ]","Unmatched Signal Fit","l");
    leg->AddEntry("h_data","0 b-tag","p");


    cout << "saving canvas..." << endl;
    if(fix){
      sprintf(imageName,"2D_mZSignalFix_%iGeV_%ibtag.eps",mH,btag);
      c->SaveAs(imageName);
    }
      //else
      //  sprintf(imageName,"2D_mZSignalFit_%iGeV_%ibtag.eps",mH,btag);
      //c->SaveAs(imageName);

    /*

  w->addClassDeclImportDir("/afs/cern.ch/cms/slc5_amd64_gcc434/lcg/roofit/5.28.00a-cms12/include/");
  w->addClassDeclImportDir("../PDFs/");

  w->importClassCode(Roo2l2jMasses2D::Class(),kTRUE);
  w->importClassCode("Roo2l2jMasses2D",kTRUE);
  w->importClassCode(Roo2l2jMasses2D_DoubleCB::Class(),kTRUE);
  w->importClassCode("Roo2l2jMasses2D_DoubleCB",kTRUE);
  w->import(totalPDF_sig);

  sprintf(outWorkspace,"2DWorkspace_%i.root",mH);

  w->Write("w");
    */
  }


  if(returnMatched)
    return r1;
  else
    return r2;

}
