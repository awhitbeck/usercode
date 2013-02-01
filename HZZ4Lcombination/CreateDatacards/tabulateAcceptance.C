
TString inputDir="/scratch0/hep/whitbeck/4lHelicity/datafiles/130130/";

enum model {k0mplus,k0minus,k0hplus,k1minus,k1plus,k2mplus_gg,k2mplus_qqb,kNUM_MODEL};
enum channel {k4e,k4mu,k2e2mu,kNUM_CHAN};

TString chanDir[3]={"4e","4mu","2mu2e"};

double eventsGen[7]={300000,300000,300000,200000,200000,300000,200000};

//2e2mu branching ratio
double branchRatio[3][7]={ {.26,.24,.245,.24,.245,.235,.235} , 
			{.26,.24,.245,.24,.245,.235,.235} , 
			{.48,.52,.51,.52,.51,.53,.53} };

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

double calcAcceptance(model myModel, channel myChan){

  TChain* t = new TChain("SelectedTree");
  t->Add(inputDir+"/JHU_8TeV/"+chanDir[myChan]+"/ZZ4lAnalysis_"+sampleName[myModel]+".root");


  return (double)t->Draw("ZZMass","ZZMass>111&&ZZMass<141");

}

void tabulateAcceptance(){

  double corr_total, corr_norm;

  for(model j=0; j<kNUM_MODEL; j++){
    cout << " \\multicolumn{7}{|c|}{" << sampleLabel[j] << "} \\\\ \\hline " << endl;
    cout << "channel &  BR & $N_{reco}$ & $\\alpha$ & $corr_{BR}$ & $corr_{total} $ & $corr_{norm}$ \\\\ \\hline " << endl; 
    for(channel i=0; i<kNUM_CHAN; i++){
      cout << chanDir[i]; 
      cout << " & " << branchRatio[i][j] ;
      cout << " & " << calcAcceptance(j,i) ;
      cout << " & " << eventsGen[k0mplus]/eventsGen[j] ;
      if(i==2){
	corr_total  = eventsGen[k0mplus]/eventsGen[j]*calcAcceptance(j,i)/calcAcceptance(k0mplus,i);
	cout << " & 1.0 & " << corr_total ;
	corr_norm = corr_total*(calcAcceptance(k0mplus,k4e)+calcAcceptance(k0mplus,k4mu)+calcAcceptance(k0mplus,k2e2mu))/
	  (calcAcceptance(k0mplus,k4e)*eventsGen[k0mplus]/eventsGen[j]*calcAcceptance(j,k4e)/calcAcceptance(k0mplus,k4e)*branchRatio[k2e2mu][k0mplus]/branchRatio[k2e2mu][j]+calcAcceptance(k0mplus,k4mu)*eventsGen[k0mplus]/eventsGen[j]*calcAcceptance(j,k4mu)/calcAcceptance(k0mplus,k4mu)*branchRatio[k2e2mu][k0mplus]/branchRatio[k2e2mu][j] + calcAcceptance(k0mplus,k2e2mu)*eventsGen[k0mplus]/eventsGen[j]*calcAcceptance(j,k2e2mu)/calcAcceptance(k0mplus,k2e2mu)) ;
	cout << " & " << corr_norm; 
      }else{
	corr_total  = eventsGen[k0mplus]/eventsGen[j]*calcAcceptance(j,i)/calcAcceptance(k0mplus,i)*branchRatio[k2e2mu][k0mplus]/branchRatio[k2e2mu][j] ;
	cout << " & " << branchRatio[k2e2mu][k0mplus]*(1-branchRatio[k2e2mu][j])/(1-branchRatio[k2e2mu][k0mplus])/branchRatio[k2e2mu][j] ;
	cout << " & " << corr_total ; 
	corr_norm = corr_total*(calcAcceptance(k0mplus,k4e)+calcAcceptance(k0mplus,k4mu)+calcAcceptance(k0mplus,k2e2mu))/
	  (calcAcceptance(k0mplus,k4e)*eventsGen[k0mplus]/eventsGen[j]*calcAcceptance(j,k4e)/calcAcceptance(k0mplus,k4e)*branchRatio[k2e2mu][k0mplus]/branchRatio[k2e2mu][j]+calcAcceptance(k0mplus,k4mu)*eventsGen[k0mplus]/eventsGen[j]*calcAcceptance(j,k4mu)/calcAcceptance(k0mplus,k4mu)*branchRatio[k2e2mu][k0mplus]/branchRatio[k2e2mu][j] + calcAcceptance(k0mplus,k2e2mu)*eventsGen[k0mplus]/eventsGen[j]*calcAcceptance(j,k2e2mu)/calcAcceptance(k0mplus,k2e2mu)) ;
	cout << " & " << corr_norm;
      }
      cout << " \\\\ \\hline " << endl;
    }
  }
}
