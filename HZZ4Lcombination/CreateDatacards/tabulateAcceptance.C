
TString inputDir="/scratch0/hep/whitbeck/4lHelicity/datafiles/130203/";

enum model {k0mplus,k0minus,k0hplus,k1minus,k1plus,k2mplus_gg,k2mplus_qqb,kNUM_MODEL};
enum channel {k4e,k4mu,k2e2mu,kNUM_CHAN};

TString chanDir[3]={"4e","4mu","2mu2e"};

double eventsGen[7]={300000,300000,300000,200000,200000,300000,200000};

//2e2mu branching ratio
double branchRatio[3][7]={ {.2592,.2382,.2458,.2395,.2466,.2368,.2368} , 
			{.2592,.2382,.2458,.2395,.2466,.2368,.2368} , 
			{.4816,.5236,.5084,.5210,.5068,.5265,.5265} };

TString sampleName[7]={"jhuGenV2H126",
		       "jhuGenV2PseH126",
		       "jhuGenV2ScaHH126",
		       "jhuGenV2Vec1MH126",
		       "jhuGenV2Vec1PH126",
		       "jhuGenV2GravH126",
		       "jhuGenV2qqGravH126"};

TString sampleLabel[7]={"$0^{+}_{m}$",
			"$0^{-}$",
			"$0^{+}_{h}$",
			"$1^{-}$",
			"$1^{+}$",
			"$2^{+}_{m} (gg)$",
			"$2^{+}_{m} (q\\bar{q})$"};

double calcAcceptance(model myModel, channel myChan, bool useWeights=true){

  TChain* t = new TChain("SelectedTree");
  t->Add(inputDir+"/JHU/"+chanDir[myChan]+"/HZZ4lTree_"+sampleName[myModel]+".root");

  if(!useWeights){
    return (double)t->Draw("ZZMass>>temp","MC_weight_noxsec*(ZZMass>111&&ZZMass<141)");
  }else{
    t->Draw("ZZMass>>temp","MC_weight*4/9*(ZZMass>111&&ZZMass<141)");
    return temp->Integral();
  }

}

void tabulateAcceptance(){

  double corr_total, corr_norm;

  double NexpJP[3],NrecoJP[3];
  double NexpH[3],NrecoH[3];

  cout << " \\multicolumn{8}{|c|}{" << sampleLabel[k0mplus] << "} \\\\ \\hline " << endl;
  cout << "channel & BR & $N_{reco}$ & $\\epsilon_{reco}$ & $\\alpha$ & $corr_{BR}$ & $corr_{total} $ & $corr_{norm}$ \\\\ \\hline " << endl; 
  
  for (channel i=0; i<kNUM_CHAN; i++){

    NexpH[i]=5.051*calcAcceptance(k0mplus,i);
    NrecoH[i]=NexpH[i];

    // channel column
    cout << chanDir[i];
    
    // BR column
    cout << " & " << branchRatio[i][k0mplus] ;

    // N_reco column
    cout << " & " << NrecoH[i] ;

    // \epsilon_{reco} column
    cout << " & " << calcAcceptance(k0mplus,i,false)/eventsGen[k0mplus]/branchRatio[i][k0mplus] ; 
    
    // \alpha and corr_{BR} and corr_{total} and corr_{norm}
    cout << " & 1.0  & 1.0 & 1.0 & 1.0 \\\\ \\hline" ;

  }

  /////////////////////////////////////
  // tabulate for other JP models
  /////////////////////////////////////
  for(model j=1; j<kNUM_MODEL; j++){
    cout << " \\multicolumn{8}{|c|}{" << sampleLabel[j] << "} \\\\ \\hline " << endl;
    cout << "channel &  BR & $N_{reco}$ & $\\epsilon_{reco}$ & $\\alpha$ & $corr_{BR}$ & $corr_{total} $ & $corr_{norm}$ \\\\ \\hline " << endl; 

    // get variable for formulas, reduce redundant calculations
    for(channel i=0; i<kNUM_CHAN; i++){
      
      NrecoJP[i] = 5.051*calcAcceptance(j,i);
      //if(i==2)
      //	NexpJP[i] = NexpH[i] * (NrecoJP[i]/eventsGen[j])/(NrecoH[i]/eventsGen[k0mplus]) ;
      //else
	NexpJP[i] = NexpH[i] * (NrecoJP[i]/eventsGen[j])/(NrecoH[i]/eventsGen[k0mplus]) * branchRatio[k2e2mu][k0mplus]/branchRatio[k2e2mu][j];

    }

    // print calculations for table
    for(channel i=0; i<kNUM_CHAN; i++){

      // channel column
      cout << chanDir[i];
      
      // BR column
      cout << " & " << branchRatio[i][j] ;
				
      // N_reco column
      cout << " & " << NrecoJP[i] ;

      // \epsilon_{reco} column
      cout << " & " << calcAcceptance(j,i,false)/eventsGen[j]/branchRatio[i][j] ; 

      // \alpha column
      cout << " & " << eventsGen[k0mplus]/eventsGen[j] ;

      // corr_{BR} column
      if(i==2)
	cout << " & 1.0 " ;
      else
	cout << " & " << branchRatio[k2e2mu][k0mplus]*(1-branchRatio[k2e2mu][j])/(1-branchRatio[k2e2mu][k0mplus])/branchRatio[k2e2mu][j] ;

      
      // corr_{total}
      cout << " & " << NexpJP[i]/NexpH[i] << endl;

      // corr_{norm}
      cout << "% N^{Higg} = " << NexpH[0] + NexpH[1] + NexpH[2] << endl;
      cout << "% N^{JP} = " << NexpJP[0] << " + " << NexpJP[1] << " + " << NexpJP[2] << " = " << NexpJP[0] + NexpJP[1] + NexpJP[2] << endl;
      corr_norm = NexpJP[i]/NexpH[i] * (NexpH[0]+NexpH[1]+NexpH[2])/(NexpJP[0]+NexpJP[1]+NexpJP[2]) ;
    
      cout << " & " << corr_norm;
    
    cout << " \\\\ \\hline " << endl;
    }
  }
}
	    
