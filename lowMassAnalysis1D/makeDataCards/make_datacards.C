#include <Riostream.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

#include "TGraph.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TF1.h"
#include "TH1.h"
#include "TAxis.h"

#include "make_roofitfiles.C"
//#include "HiggsCSandWidth.cc"

const int nbtag=3; //btag0, btag1 and btag2
const int nprod=2; //VBF and gg
const int nchan=2;//2e2j and 2m2j
const float lumiee=4.6;  //4.86; //fb^-1, NEW!
const float lumimm=4.6;  //4.88; //fb^-1, NEW!
const int nmass=5;
const bool isSM4=false;  // if true add .15 to CSgg and CSvbf errors 
const bool isXSBr=false; // if true don't write signal yield errors from theory/pdf's 
const bool isFF=false;    // if true use fermiophobic text file, don't use gg->H channel, add 5% to scale errors.  
const float mass[nmass]={130,140,150,160,170};

bool ZeroBias=false; // set to true if you want the expected background yield to be exactly the same as observed yield 

float exp_sig_yields[nbtag][nchan][nmass];

float gen_sig_yields[nmass];

const int maxNbkg[nbtag]={1,1,1};//for every btag, there are MAX==2 different types of bkg (only one for btag0 and 1, ttbar +all the rest for btag2)

float exp_bkg[nbtag][nchan];
 
//const float NLOxsect[nmass]={ 144.1,106.3, 80.2, 76.9, 55.3, 35.3, 22.4, 8.4};//in fb
//const float meas_lumi[nmass]={ 757.2,1034.6, 1360.4, 1418.4, 1987.8, 3113.5, 4591.8,109977/ 8.4};//in fb^-1
vector<float> btag0_ee_eff_pars,  btag1_ee_eff_pars, btag2_ee_eff_pars; //we store the parameters of the dependence of the eff on the Mzz
vector<float> btag0_mm_eff_pars,  btag1_mm_eff_pars, btag2_mm_eff_pars; //we store the parameters of the dependence of the eff on the Mzz
const float maxDiffSyst=0.01;
//HiggsCSandWidth *myCSW;

//vector<float>aux;


string make_ratestring(float mymass, vector<float> myxsect, int ibtag,int chan, float expbkg=-99.9);
string make_theorunc(float mymass,float myerrp, float myerrm,bool is_ggH=true, int ibtag=0);  
string make_theorunc(float mymass,float myerrCSp, float myerrCSm, float myerrVBFp, float myerrVBFm);
string make_pdftheorunc(float mymass,float myerrp, float myerrm,bool is_ggH=true, int ibtag=0);  
string make_theorgammaunc(float mymass);
string make_btagunc(float mymass,int ibtag, int ich); 
string make_JESunc(float mymass);
string make_bakcgrNormErrLine(double expyields,double alpha, int ibtag, int ich); 
string make_obsstring(int btag);
string make_obsstring(int btag, int obs);
vector<float> eff_fit(int mybtag,int chan);
vector<double> calculate_CBpars(int btag,double mH);
int get_mass_index(float mymass);
void extract_exp_sig_yields(float mymass,float mywidth);//,vector<float> ee_y , vector<float> mm_y);
void get_gen_yields();
int make_datacards();
int get_signal_gen(float mymass);
double countSideBandEvents(int btag,int LEP,bool returnExpected,double obs_yield);
int make_datacards(){

  // gROOT->ProcessLine(".L make_roofitfiles.C++");

  //myCSW = new HiggsCSandWidth();

 // return 1;
 
  string nameXsecFile;
  if(isSM4)
    nameXsecFile="./xsect_higgs_173points_4generation_new.txt";
  else if(isFF)
    nameXsecFile="./xsect_higgs_173points_FF.txt";
  else
    nameXsecFile="./xsect_higgs_173points_new.txt";
  ifstream xsect_file(nameXsecFile.c_str(),ios::in);

  if (! xsect_file.is_open()){ cout<<"Failed to open file with xsections"<<endl;}
  float mH, CSgg, CSgg_p, CSgg_m, CSpdfgg_p,CSpdfgg_m,CSvbf, CSvbf_p, CSvbf_m,CSpdfvbf_p,CSpdfvbf_m, 
    Gamma, BRHZZ, gamma_Hff, gamma_HVV, gamma_Hgg, BRZZ2l2q;

  string outDir="datacards_testJHEPresults/";

  //calculate expected yields
  get_gen_yields();
  while(xsect_file.good()){
    if(isSM4)
      xsect_file >> mH>> CSgg>> CSgg_p >> CSgg_m >>  CSpdfgg_p>>CSpdfgg_m>>CSvbf >> CSvbf_p >> CSvbf_m >>  CSpdfvbf_p>>CSpdfvbf_m>>Gamma >> BRHZZ >> BRZZ2l2q >> gamma_Hff >> gamma_HVV >> gamma_Hgg;
    else
      xsect_file >> mH>> CSgg>> CSgg_p >> CSgg_m >>  CSpdfgg_p>>CSpdfgg_m>>CSvbf >> CSvbf_p >> CSvbf_m >>  CSpdfvbf_p>>CSpdfvbf_m>>Gamma >> BRHZZ >> BRZZ2l2q;

    extract_exp_sig_yields(mH,Gamma);//exp_sig_yields_ee,exp_sig_yields_mm);
    extract_exp_sig_yields(mH,Gamma);//exp_sig_yields_ee,exp_sig_yields_mm);
 }

  //parametrize efficiencies
  btag0_ee_eff_pars=eff_fit(0,0);
  //  cout<<"In main: "<<btag0_eff_pars.at(0)<<"    "<<btag0_eff_pars.at(1)<<endl;
  btag1_ee_eff_pars=eff_fit(1,0);
  btag2_ee_eff_pars=eff_fit(2,0);
  btag0_mm_eff_pars=eff_fit(0,1);
  btag1_mm_eff_pars=eff_fit(1,1);
  btag2_mm_eff_pars=eff_fit(2,1);

  xsect_file.clear();
  xsect_file.seekg (0);
 
  if(!xsect_file.good())
    cout << "oops... having trouble reading text file" << endl;

  while(xsect_file.good()){

    if(isSM4)
      xsect_file >> mH>> CSgg>> CSgg_p >> CSgg_m >>  CSpdfgg_p>>CSpdfgg_m>>CSvbf >> CSvbf_p >> CSvbf_m >>  CSpdfvbf_p>>CSpdfvbf_m>>Gamma >> BRHZZ >> BRZZ2l2q >> gamma_Hff >> gamma_HVV >> gamma_Hgg;
    else
      xsect_file >> mH>> CSgg>> CSgg_p >> CSgg_m >>  CSpdfgg_p>>CSpdfgg_m>>CSvbf >> CSvbf_p >> CSvbf_m >>  CSpdfvbf_p>>CSpdfvbf_m>>Gamma >> BRHZZ >> BRZZ2l2q;
  
    cout<<mH<<"  "<<CSgg<<endl;

    if(mH>170.0) continue;
    if(mH<130.0) continue;
    //if(!(mH==130.0||mH==135.0||mH==140.0||mH==145.0||mH==150.0||mH==155.0||mH==160.0||mH==164.0||mH==170.0)) continue;

    std::ostringstream ossDir;
    ossDir<<mH;
    //if(mkdir((outDir+ossDir.str()).c_str(),0777)==-1){
    //if(mkdir((outDir+ossDir.str()).c_str())==-1){
    //  cout<<"Failed to create directory "<<ossDir.str()<<endl;
    //  cout<<"Failed to create directory "<<outDir+ossDir.str().c_str()<<endl;
    //  break;
    // }

    float CSggxs =1000.0*CSgg  *BRHZZ *BRZZ2l2q;
    float CSvbfxs=1000.0*CSvbf*BRHZZ  *BRZZ2l2q;

    /**
    cout << "= = = = = = = = = = = = = = = = = = = =  = = = = = = = " << endl;
    cout<<"THEOR xsect: "<<CSgg<<", Vbf= "<<CSvbf<<"  BRHZZ="<<BRHZZ<< " BRHZZerr=" << BRHZZerr << "  BRZZ2l2q="<<BRZZ2l2q<<endl;
    cout << "CSgg+: " << CSgg_p << " CSgg-: " << CSgg_m << " CSpdfgg+: " << CSpdfgg_p << " CSpdfgg-:" << CSpdfgg_m << endl;
    cout << "CSvbf+: " << CSvbf_p << " CSvbf-: " << CSvbf_m << " CSpdfvbf+: " << CSpdfvbf_p << " CSpdfvbf-:" << CSpdfvbf_m << endl;
    cout << "= = = = = = = = = = = = = = = = = = = =  = = = = = = = " << endl;
    */

    vector<float> myxsect;
    myxsect.push_back(CSggxs);
    myxsect.push_back(CSvbfxs);

    float ggRelErrp=CSgg_p;
    float ggRelErrm=CSgg_m;//the error quoted in the txt file has already a minus
    float vbfRelErrp=CSvbf_p;
    float vbfRelErrm=CSvbf_m;
    float ggPDFErrp=CSpdfgg_p;
    float ggPDFErrm=CSpdfgg_m;//the error quoted in the txt file has already a minus
    float vbfPDFErrp=CSpdfvbf_p;
    float vbfPDFErrm=CSpdfvbf_m;
    if(isFF){
      vbfRelErrp+=.05;
      vbfRelErrm-=.05;
    }

  //cout<<"MH = "<<mH<<"    "<<test1.c_str()<<endl;  

    string str_ch[2]={"ee","mm"};
    string str_btag[3]={"0b","1b","2b"};
      //loop over btag categories
    for(int ibtag=0;ibtag<nbtag;ibtag++){
   
      string theorErrLine_ggH=make_theorunc(mH, ggRelErrp, ggRelErrm, true,ibtag);  
      string theorErrLine_VBF=make_theorunc(mH, vbfRelErrp, vbfRelErrm, false,ibtag);  
      string pdfErrLine_ggH=make_pdftheorunc(mH, ggPDFErrp, ggPDFErrm, true,ibtag); 
      string pdfErrLine_VBF=make_pdftheorunc(mH, vbfPDFErrp, vbfPDFErrm, false,ibtag);  
      string gammaErrLine=make_theorgammaunc(mH);

      string jesLine=make_JESunc(mH);
      //loop over decay channels
      for(int ich=0;ich<nchan;ich++){
 
	string beffLine=make_btagunc(mH,ibtag,ich);
  	string str_id=str_ch[ich]+str_btag[ibtag];
	string limit_tag;
	if(isSM4)
	  limit_tag="_SM4";
	else if(isXSBr)
	  limit_tag="_XSBr";
	else if(isFF)
	  limit_tag="_FF";
	else 
	  limit_tag="";

	ifstream tpl_file(("./hzz2l2q_"+ str_id+".tpl"+limit_tag).c_str(),ios::in);
	std::ostringstream mass_str;
	mass_str<<mH;

	//ok, now produce the root file wit hthe workspace  
	vector<double> cbpars =calculate_CBpars(ibtag,mH);
	double obsyields=0.0;
	double expyields=0.0;
	double alpha=0.0;
	
	cout << " generate workspaces " << endl;
	make_roofitfiles(ibtag,ich,mH,Gamma, obsyields, expyields, alpha, cbpars);
	cout << " back! " << endl;
	// =====================================================================
	// ============== calculate exp_bkg from mJJ alpha sideband ============
	//exp_bkg[ibtag][ich] = countSideBandEvents(ibtag,ich,true,obsyields);

	// =====================================================================
	exp_bkg[ibtag][ich] = expyields;

	cout<<"\n\n~~~~~~~~~~Btag "<< ibtag<<"   Expected MAKEDATACARDS "<<exp_bkg[ibtag][ich]<<endl;
	string expRateLine=make_ratestring(mH,myxsect,ibtag,ich,exp_bkg[ibtag][ich]);
	string obsLine=make_obsstring(ibtag, int(obsyields));
	string bakcgrNormErrLine = make_bakcgrNormErrLine(exp_bkg[ibtag][ich],alpha,ibtag,ich);
	cout << "CHECKING::::" << endl;
	cout << bakcgrNormErrLine << endl;
	//string datacard_name=outDir+ossDir.str()+"/hzz2l2q_"+str_id+"."+mass_str.str() +".txt";
	string datacard_name=outDir+ossDir.str()+"/hzz2l2q_"+str_id+"_7TeV.txt";
	ofstream datacard_new(datacard_name.c_str(),ios::out);
	string tpl_line;
	string str_ratetag("<dummy1>");
	string str_obstag("<dummyobs>");
	string str_befftag("<dummybeff>");
	string str_gammatag("<dummygammaBW>");
	string str_jestag("<dummyJES>");
	string str_theorggHtag("<dummyggH>");
	string str_theorVBFtag("<dummyVBF>");
	string str_pdftheorggHtag("<dummypdfggH>");
	string str_pdftheorVBFtag("<dummypdfqqH>");	
	string str_HVVtag("<dummyHVV>");
	string str_Hfftag("<dummyHff>");
	string str_Hggtag("<dummyHgg>");
	string str_bakcgrNorm("<dummyBkgNorm>");
	string str_bckgShape_0b_1("CMS_hzz2l2q_bkg_0b_p1");
	string str_bckgShape_0b_2("CMS_hzz2l2q_bkg_0b_p2");
	string str_bckgShape_0b_3("CMS_hzz2l2q_bkg_0b_p3");
	string str_bckgShape_0b_4("CMS_hzz2l2q_bkg_0b_p4");
	string str_bckgShape_1b_1("CMS_hzz2l2q_bkg_1b_p1");
	string str_bckgShape_1b_2("CMS_hzz2l2q_bkg_1b_p2");
	string str_bckgShape_1b_3("CMS_hzz2l2q_bkg_1b_p3");
	string str_bckgShape_1b_4("CMS_hzz2l2q_bkg_1b_p4");
	string str_bckgShape_2b_1("CMS_hzz2l2q_bkg_2b_p1");
	string str_bckgShape_2b_2("CMS_hzz2l2q_bkg_2b_p2");
	string str_bckgShape_2b_3("CMS_hzz2l2q_bkg_2b_p3");
	string str_bckgShape_2b_4("CMS_hzz2l2q_bkg_2b_p4");
	bool found=false;
	while(tpl_file.good()){
	  getline (tpl_file,tpl_line);
	  size_t posrate_found=tpl_line.find(str_ratetag);
	  size_t posobs_found=tpl_line.find(str_obstag);
	  size_t posbeff_found=tpl_line.find(str_befftag);
	  size_t posjes_found=tpl_line.find(str_jestag);
	  size_t postheorgamma_found=tpl_line.find(str_gammatag);
	  size_t postheorggH_found=tpl_line.find(str_theorggHtag);
	  size_t postheorVBF_found=tpl_line.find(str_theorVBFtag);
	  size_t pospdfggH_found=tpl_line.find(str_pdftheorggHtag);
	  size_t pospdfVBF_found=tpl_line.find(str_pdftheorVBFtag);
	  size_t posHVV_found=tpl_line.find(str_HVVtag);
	  size_t posHff_found=tpl_line.find(str_Hfftag);
	  size_t posHgg_found=tpl_line.find(str_Hggtag);
	  size_t posbnorm_found=tpl_line.find(str_bakcgrNorm);
	  size_t pos_bckgShape_0b_1_found=tpl_line.find(str_bckgShape_0b_1);
	  size_t pos_bckgShape_0b_2_found=tpl_line.find(str_bckgShape_0b_2);
	  size_t pos_bckgShape_0b_3_found=tpl_line.find(str_bckgShape_0b_3);
	  size_t pos_bckgShape_0b_4_found=tpl_line.find(str_bckgShape_0b_4);
	  size_t pos_bckgShape_1b_1_found=tpl_line.find(str_bckgShape_1b_1);
	  size_t pos_bckgShape_1b_2_found=tpl_line.find(str_bckgShape_1b_2);
	  size_t pos_bckgShape_1b_3_found=tpl_line.find(str_bckgShape_1b_3);
	  size_t pos_bckgShape_1b_4_found=tpl_line.find(str_bckgShape_1b_4);
	  size_t pos_bckgShape_2b_1_found=tpl_line.find(str_bckgShape_2b_1);
	  size_t pos_bckgShape_2b_2_found=tpl_line.find(str_bckgShape_2b_2);
	  size_t pos_bckgShape_2b_3_found=tpl_line.find(str_bckgShape_2b_3);
	  size_t pos_bckgShape_2b_4_found=tpl_line.find(str_bckgShape_2b_4);
	  if(posrate_found!=string::npos){
	    //	 cout<<tpl_line.c_str()<<endl;
	    found=true;
	    datacard_new<<expRateLine<<endl;
	  }
	  else if(posobs_found!=string::npos){
	    datacard_new<<obsLine<<endl;
	  }
	  else if(posbeff_found!=string::npos){
	    datacard_new<<beffLine<<endl;
	  }
	  else if(posjes_found!=string::npos){
	    datacard_new<<jesLine<<endl;
	  }
	  else if(postheorggH_found!=string::npos && !isXSBr){
	    datacard_new<<theorErrLine_ggH<<endl;
	  }
	  else if(postheorVBF_found!=string::npos && !isXSBr){
	    datacard_new<<theorErrLine_VBF<<endl;
	  }
	  else if(pospdfggH_found!=string::npos && !isXSBr){
	    datacard_new<<pdfErrLine_ggH<<endl;
	  }
	  else if(pospdfVBF_found!=string::npos && !isXSBr){
	    datacard_new<<pdfErrLine_VBF<<endl;
	  }
	  else if(posHVV_found!=string::npos && !isXSBr){
	    char HVVline[50];
	    sprintf(HVVline,"gamma_HVV lnN %f 1.0 1.0 ",gamma_HVV);
	    datacard_new<< HVVline <<endl;
	  }
	  else if(posHff_found!=string::npos && !isXSBr){
	    char Hffline[50];
	    sprintf(Hffline,"gamma_Hff lnN %f 1.0 1.0",gamma_Hff);
	    datacard_new<< Hffline <<endl;
	  }
	  else if(posHgg_found!=string::npos && !isXSBr){
	    char Hggline[50];
	    sprintf(Hggline,"gamma_Hgg lnN %f 1.0 1.0",gamma_Hgg);
	    datacard_new<< Hggline <<endl;
	  }
	  else if(posbnorm_found!=string::npos){
	    datacard_new<<bakcgrNormErrLine<<endl;
	   }
	  //else if(pos_bckgShape_0b_1_found!=string::npos  || pos_bckgShape_0b_2_found!=string::npos ||
		//  pos_bckgShape_0b_3_found!=string::npos  || pos_bckgShape_0b_4_found!=string::npos ||
		 // pos_bckgShape_1b_1_found!=string::npos  || pos_bckgShape_1b_2_found!=string::npos ||
		  //pos_bckgShape_1b_3_found!=string::npos  || pos_bckgShape_1b_4_found!=string::npos ||
		  //pos_bckgShape_2b_1_found!=string::npos  || pos_bckgShape_2b_2_found!=string::npos ||
		  //pos_bckgShape_2b_3_found!=string::npos  || pos_bckgShape_2b_4_found!=string::npos ){
	    //datacard_new<<"# no shape errors for now"<<endl;
	    //}
	  //else if(postheorgamma_found!=string::npos){
	  //   datacard_new<<gammaErrLine<<endl;
	  //  }
	  else datacard_new<<tpl_line<<endl;
	}//ed loop over lines of template file
	if(!found)cout<<"it did not find the tag string "<<str_ratetag.c_str()<<endl;
	
	datacard_new.close();

      }//end loop on channels
    }//end loop on btag
  }//end loop over xsect file (-> loop over mass points)
  

  
  return 0;
}//end main
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

string make_ratestring(float mymass,vector<float> myxsect, int ibtag, int chan,float expbkg){

  //vector of signal eff: btag0_2e2j_VBF,  btag0_2e2j_gg,  btag0_2m2j_VBF,  btag0_2m2j_gg,  btag1_2e2j_VBF, ...  btag2_2m2j_gg = 12 entries
  vector<float> sig_eff;
  string rate_str;
  rate_str="rate     ";
  //signal efficiency parametrization
  float lumi=0.0;
  float eff=0.0;
  float mya=0.0, myb=0.0, myc=0.0,myd=0.0;
  if(chan==0){
    if(ibtag==0){
      mya=btag0_ee_eff_pars.at(0);
      myb=btag0_ee_eff_pars.at(1);
      myc=btag0_ee_eff_pars.at(2);
      myd=btag0_ee_eff_pars.at(3);
    }
    else if(ibtag==1){
      mya=btag1_ee_eff_pars.at(0);
      myb=btag1_ee_eff_pars.at(1);
      myc=btag1_ee_eff_pars.at(2);
      myd=btag1_ee_eff_pars.at(3);
    }
    else if(ibtag==2){
      mya=btag2_ee_eff_pars.at(0);
      myb=btag2_ee_eff_pars.at(1);
      myc=btag2_ee_eff_pars.at(2);
      myd=btag2_ee_eff_pars.at(3);
    }
    else{
      cout<<"\n\nERROR !! Btag index out of range!"<<endl<<endl<<endl;
      return "dummy";
    }
    lumi=lumiee;
  }
  else{//chan==1 -> muons
    if(ibtag==0){
      mya=btag0_mm_eff_pars.at(0);
      myb=btag0_mm_eff_pars.at(1);
      myc=btag0_mm_eff_pars.at(2);
      myd=btag0_mm_eff_pars.at(3);
    }
    else if(ibtag==1){
      mya=btag1_mm_eff_pars.at(0);
      myb=btag1_mm_eff_pars.at(1);
      myc=btag1_mm_eff_pars.at(2);
      myd=btag1_mm_eff_pars.at(3);
    }
    else if(ibtag==2){
      mya=btag2_mm_eff_pars.at(0);
      myb=btag2_mm_eff_pars.at(1);
      myc=btag2_mm_eff_pars.at(2);
      myd=btag2_mm_eff_pars.at(3);
    }
    else{
      cout<<"\n\nERROR !! Btag index out of range!"<<endl<<endl<<endl;
      return "dummy";
    }
    lumi=lumimm;
  }
  //  eff=mya + myb*mymass;
  //cout<<"BTAG = "<<ibtag<<" CHAN = "<<chan<<" using these params: "<<mya<<" "<<myb<<"  "<<myc<<"  "<<myd<<"  Mass: "<<mymass<<endl;
  eff=mya + myb*mymass + myc *mymass*mymass + myd *mymass*mymass*mymass;
  //  eff = mya+myb*log(mymass);
  //read from text file the right Higgs xsect
  //loop over prod mechanism

  //================
  // fix added by andrew so that gg-H is not include for fermiophobic higgs
  //----------------
  int startProd=0;
  if(isFF)
    startProd++;

  //----------------

  for(int iprod=startProd;iprod<nprod;iprod++){
    //    cout<<"\n\nBTAG "<<ibtag<<"  CHAN "<<chan<<"  PROD "<<iprod<<"   :  Eff= "<<eff<<endl;
    float xsect=myxsect.at(iprod);
   

    // cout<<"\n\n~~~M = "<<mymass<<"BTAG = "<<ibtag<<" CHAN = "<<chan<<"  eff="<<eff<<"  xsect="<<xsect<<"  lumi="<<lumi<<"  BrZtoL="<<brZtoL<<flush;
    float nexpected=eff*xsect*lumi*0.5;//divide by two because the xsect is the sum of 2e2j and 2m2j
    cout << "--------------expected signal---------------------" << endl;
    cout << "eff: " << eff << endl;
    cout << "xsect: " << xsect << endl;
    cout << "lumi: " << lumi << endl;
    cout<<"  ---> EXP: "<<nexpected<<endl;
    cout << "--------------------------------------------------" << endl;
    sig_eff.push_back(nexpected);
    std::ostringstream ossm;
    ossm<<nexpected;
    string nexp_str=ossm.str() ;
    rate_str+=("    "+nexp_str);
    if(iprod==nprod-1){//finished to fill signal yields, add bakgd
      
      // for(int ibkg=0;ibkg<maxNbkg[ibtag];ibkg++){//not necessary any more, all bkg lumped into one also for 2btag
	std::ostringstream ossbkg;
	if(expbkg<0.0)ossbkg<<exp_bkg[ibtag][chan];
	else ossbkg<<expbkg;
	string bkg_str=ossbkg.str() ;
	rate_str+=("    "+bkg_str);
	//	  }//end loop on diferent types of bkgd
    }//end if iprod==nprod-1
    
    //close three loops
      }//end loop on prod mechanisms
    
  //background yields are fixed (because we fit the shape over the whole mass range)
  
  return rate_str;
}//END string make_ratestring(float mass)



string make_theorunc(float mymass, float myerrp, float myerrm, bool is_ggH, int ibtag){
  //ERRORS SHOULD BE RELATIVE AND NETTO, I.E.: for a 6% syst, pass 0.06 AND NOT 1.06 
  string theor_str;
  if(is_ggH)  theor_str="QCDscale_ggH    lnN       ";
  else theor_str="QCDscale_qqH    lnN       "; //else is VBF
  //float therrp=0., therrm=0.0;
  string nthp="-999";
  string nthm="+999";
  std::ostringstream ossp;
  myerrp+=1.0;
  myerrm+=1.0;
  ossp<<(myerrp);
  nthp=ossp.str() ;
  std::ostringstream ossm;
  ossm<<(myerrm);//negative errors are passed already with the minus sign
  nthm=ossm.str() ;
  string therr_str=nthm+"/"+nthp;
  bool asymm=true;
  float erravg=(fabs(myerrp-1.0)+fabs(myerrm-1.0))/2.0 + 1.0;
  if(fabs(erravg-myerrp)/erravg <maxDiffSyst ){
    asymm=false;//symmetric errors
    std::ostringstream ossavg;
    ossavg<<erravg;
    therr_str=ossavg.str();
  }

  if(is_ggH){
    theor_str+=("    "+therr_str);
    //for the VBF part, no error
    theor_str+="    1.0";
    //for the background, no err
    theor_str+="    1.0"; 
    //  if(ibtag==2){//two bakg sources for the btag2 category
    //    theor_str+="    1.0";
    //  }
  }
  else{
    //for the ggH part, no error
    if(!isFF)
      theor_str+="    1.0";
    theor_str+=("    "+therr_str);
    //for the background, no err
    theor_str+="    1.0"; 
    //  if(ibtag==2){//two bakg sources for the btag2 category
      //    theor_str+="    1.0";
    //  }
  }
  //  if(int(mymass)%50==0) cout<<"*** MASS "<<mymass<<" "<<theor_str.c_str()<<endl;
  return theor_str;
}//end make_theorunc
  

string make_theorunc(float mymass,float myerrCSp, float myerrCSm, float myerrVBFp, float myerrVBFm){
 //ERRORS SHOULD BE RELATIVE AND NETTO, I.E.: for a 6% syst, pass 0.06 AND NOT 1.06 
  string theor_str;
  theor_str="QCDscale_ggH    lnN       ";
 //loop over btag categories
  for(int ibtag=0;ibtag<nbtag;ibtag++){
    //loop over decay channels
    for(int ich=0;ich<nchan;ich++){
      //float therrp=0., therrm=0.0;
     //loop over prod mechanism
      for(int iprod=0;iprod<nprod;iprod++){// same err for all chans and btags
	string nthp="-999";
	string nthm="+999";
	if(iprod==0){//gg	  
	  std::ostringstream ossp;
	  myerrCSp+=1.0;
	  myerrCSm+=1.0;
	  ossp<<(myerrCSp);
	  nthp=ossp.str() ;
	  std::ostringstream ossm;
	  ossm<<(myerrCSm);//negative errors are passed already with the minus sign
	  nthm=ossm.str() ;
	}
	else if(iprod==1){//vbf	  
	  std::ostringstream ossp;
	  myerrVBFp+=1.0;
	  myerrVBFm+=1.0;
	  ossp<<(myerrVBFp);
	  nthp=ossp.str() ;
	  std::ostringstream ossm;
	  ossm<<(myerrVBFm);
	  nthm=ossm.str() ;
	}
	else{
	  cout<<"IPROD out of range"<<endl;
	}
	theor_str+=("    "+nthm+"/"+nthp);
	//for the background, no err 
	if(iprod==nprod-1){
	  theor_str+="    1.0";
	  // if(ibtag==2){//two bakg sources for the btag2 category
	  //    theor_str+="    1.0";
	  //  }
	}
      }//end  for(int iprod
    }//end  for(int ich=0;
  }//end  for(int ibtag

  return theor_str;
}//end make_theorunc

string make_pdftheorunc(float mymass, float myerrp, float myerrm, bool is_ggH, int ibtag){
 //ERRORS SHOULD BE RELATIVE AND NETTO, I.E.: for a 6% syst, pass 0.06 AND NOT 1.06 
  string theor_str;
  if(is_ggH)  theor_str="pdf_gg   lnN       ";
  else theor_str="pdf_qqbar    lnN       "; //else is VBF
  //float therrp=0., therrm=0.0;
  string nthp="-999";
  string nthm="+999";
  float signp = (myerrp>0 ? 1.0 : -1.0 );
  float signm = (myerrm>0 ? 1.0 : -1.0 );

  float add_vbf_err=0.0;
  /*if(!is_ggH){//for VBF, add errors due to imperfect knowledge of eff, see Table 23 of AN-2011/100
   
    if(ibtag==0){
      if(mymass<=500.0)add_vbf_err=0.01;
      else add_vbf_err=0.02;
    }
    else if(ibtag==1){
      if(mymass<=500.0)add_vbf_err=0.01;
      else add_vbf_err=0.04;
    }
    else if(ibtag==2){
      if(mymass<=500.0)add_vbf_err=0.01;
      else add_vbf_err=0.02;
    }
    else{
      cout<<"wrong btag passed as argument to make_pdftheorunc"<<endl;
    }
    }*/ //has nothing to do with PDF!!!

  std::ostringstream ossp;
  float errplus=signp*sqrt(myerrp*myerrp+0.015*0.015+add_vbf_err*add_vbf_err);//0.015 is the syst on signal acceptance due to pdf uncertainties
  float errminus=signm*sqrt(myerrm*myerrm+0.04*0.04+add_vbf_err*add_vbf_err);//0.04 is the syst on signal acceptance due to pdf uncertainties
  errplus=errplus+1.0;
  errminus=errminus+1.0;
  ossp<<errplus;
  nthp=ossp.str() ;
  std::ostringstream ossm;
  ossm<<errminus;//negative errors are passed already with the minus sign
  nthm=ossm.str() ;
  string therr_str=nthm+"/"+nthp;
  bool asymm=true;
  float erravg=(fabs(errplus-1.0)+fabs(errminus-1.0))/2.0 +1.0;
   if(fabs(erravg-errplus)/erravg <maxDiffSyst ){
    asymm=false;//symmetric errors
    std::ostringstream ossavg;
    ossavg<<erravg;
    therr_str=ossavg.str();
     }

  if(is_ggH){
    theor_str+=("    "+therr_str);
    //for the VBF part, no error
    theor_str+="    1.0";
    //for the background, no err
    theor_str+="    1.0"; 
    // if(ibtag==2){
    //  theor_str+="    1.0";
    // }
  }
  else{
    //for the ggH part, no error
    if(!isFF)
      theor_str+="    1.0";
    theor_str+=("    "+therr_str);
    //for the background, no err
    theor_str+="    1.0"; 
    // if(ibtag==2){//two bakg sources for the btag2 category
    //   theor_str+="    1.0";
    // }
  }

  //  cout<<"+++ MASS "<<mymass<<" "<<theor_str.c_str()<<endl;
  return theor_str;
}//end make_pdftheorunc

string make_theorgammaunc(float mymass){
  string gamma_str="theory_gamma  lnN  ";

  float mHinTeV=mymass/1000.0;
  float unc=150*(mHinTeV*mHinTeV*mHinTeV);//percentual error on xsect
  std::ostringstream ossp;
  ossp<<(1.0+unc/100.0);
  string tmp_str=ossp.str()+"   "+ossp.str()+"   1.0" ;
  gamma_str+=tmp_str;
  return gamma_str;
}


string make_obsstring(int btag){//superseded !!!!!
  string obs_str="observation   ";

  std::ostringstream ossp;
  ossp<<int(exp_bkg[btag][0]/2.0);
  obs_str+=ossp.str() ;

  return obs_str;
}

string make_obsstring(int btag, int obs){
  string obs_str="observation   ";

  std::ostringstream ossp;
  ossp<<obs;
  obs_str+=ossp.str() ;

  return obs_str;
}

string make_btagunc(float mymass,int ibtag, int ich){
  string b_str="CMS_eff_b	lnN";
  float p0=0.0, p1=0.0;
  float m0=0.0, m1=0.0;
  float errp=999.0, errm=-9999.0;
  if(ich==0){ //electrons
    if(ibtag==0){
      //high mass
      /*p1=-8.83532570978e-05;
      p0=0.983256647923;
      m1=7.13061639147e-05;
      m0=1.02907356239;*/
      /*p1=-8.51004e-05;
      p0=0.981723;
      m1=7.16387e-05;
      m0=1.02888;*/
      p1=-0.000100716;
      p0=0.98644;
      m1=8.12716e-05;
      m0=1.0176;
    }
    else if(ibtag==1){
       //high mass
      /*p1=-1.95508160829e-05;
      p0=1.04446110956;
      m1=7.37044467898e-05;
      m0=0.940063743731;*/
      /*p1=-3.67136e-05;
      p0=1.05265;
      m1=9.1086e-05;
      m0=0.931829;*/
      p1=-1.47685e-05;
      p0=1.04847;
      m1=4.63272e-05;
      m0=0.943369;
     }
    else if(ibtag==2){
      //high mass
      /*p1=5.84572717646e-06;
      p0=1.13365470372;
      m1=-1.61054152592e-05;
      m0=0.82161771535;*/
      /*p1=1.01786e-05;
      p0=1.13163;
      m1=-1.03036e-05;
      m0=0.818871;*/
      p1=1.74066e-05;
      p0=1.1035;
      m1=-4.08034e-07;
      m0=0.885855;

    }
    else{
      cout<<"Wrong # btags to make_btagunc"<<endl;
    }
  }
  else if(ich==1){ //muons
    if(ibtag==0){
     
       //high mass
      /*p1=-8.98705296203e-05;
      p0=0.984636818312;
      m1=7.26807344479e-05;
      m0=1.02836905579;*/
      /*p1=-8.5292e-05;
      p0=0.98247;
      m1=6.89051e-05;
      m0=1.03012;*/
      p1=-0.000108687;
      p0=0.990292;
      m1=7.44788e-05;
      m0=1.02153;

    }
    else if(ibtag==1){
      //high mass
      /*p1=-2.06096278947e-05;
      p0=1.04385580002;
      m1=7.19882385098e-05;
      m0=0.942713582987;*/
      /*p1=-3.51857e-05;
      p0=1.05084;
      m1=9.91515e-05;
      m0=0.929815;*/
      p1=-9.26161e-06;
      p0=1.04438;
      m1=5.95741e-05;
      m0=0.938029;
    }
    else if(ibtag==2){
    //high mass
      /*p1=4.62542786413e-06;
      p0=1.1333366687;
      m1=-2.05840248842e-06;
      m0=0.813316607701;*/
      /*p1=-4.69484e-06;
      p0=1.13777;
      m1=-6.93966e-06;
      m0=0.815609;*/
      p1=6.55678e-07;
      p0=1.11229;
      m1=-2.25839e-05;
      m0=0.897045;
    }
    else{
      cout<<"Wrong # btags to make_btagunc"<<endl;
    }
  }
  else{
      cout<<"Wrong # channel to make_btagunc"<<endl;
    }
  cout<<"p0 "<<p0<<endl;
  cout<<"p1 "<<p1<<endl;
  cout<<"m0 "<<m0<<endl;
  cout<<"m1 "<<m1<<endl;
  cout<<"mymass "<<mymass<<endl;
  errp=p1*mymass+p0;
  errm=m1*mymass+m0;
  cout<<"errp "<<errp<<endl;
  cout<<"errm "<<errm<<endl;
  float erravg=(fabs(errp-1.0)+fabs(errm-1.0))/2.0 +1.0;
  cout<<"erravg "<<erravg<<endl;
  bool asymm=true;
  if(fabs(erravg-errp)/erravg <maxDiffSyst ){
    asymm=false;//symmetric errors
    // cout<<"@@@@@@@ asymm errors for BTAG ("<< maxDiffSyst<<"): "<<errm<<"  "<<errp<<"  "<<erravg<<endl;
  }

  std::ostringstream ossp;
  ossp<<errp;
  std::ostringstream ossm;
  ossm<<errm;
  std::ostringstream ossavg;
  ossavg<<erravg;
  cout<<"errp "<<errp<<endl;
  cout<<"errm "<<errm<<endl;
  cout<<"erravg "<<erravg<<endl;
  string tmp_str;//=ossm.str()+"/"+ossp.str();
  if(asymm)tmp_str=ossm.str()+"/"+ossp.str();
  else tmp_str=ossavg.str();

  if(isFF)
    b_str+=("      "+tmp_str+"      1.0");
  else
    b_str+=("  "+tmp_str+"      "+tmp_str+"      1.0");

  return b_str;
}

string make_JESunc(float mymass){

  string jes_str="CMS_scale_j	lnN";
  //from high mass
  //float p0= 8.3  , p1=-0.0215 ;
  //float m0=-8.6, m1=0.02 ;
  //from low mass
  float p0=41   , p1=-0.25;
  float m0=3.6, m1=-0.06 ;
  float errp=999.0, errm=-9999.0;

  //in percent
  errp=1.0+0.01*(p0+p1*mymass);
  errm=1.0+0.01*(m0+m1*mymass);
  //  cout<<"========= ERR JES:  "<<errp<<"   "<<errm<<endl;
  float erravg=(errp-1.0+fabs(errm-1.0))/2.0 +1.0;
  bool asymm=true;
  if(fabs(erravg-errp)/erravg <maxDiffSyst ){
    asymm=false;//symmetric errors
  }

  std::ostringstream ossp;
  ossp<<errp;
  std::ostringstream ossm;
  ossm<<errm;
  std::ostringstream ossavg;
  ossavg<<erravg;
  string tmp_str;//=ossm.str()+"/"+ossp.str();
  if(asymm)tmp_str=ossm.str()+"/"+ossp.str();
  else tmp_str=ossavg.str();
  
  if(isFF)
    jes_str+=("      "+tmp_str+"      1.0");
  else
    jes_str+=("  "+tmp_str+"      "+tmp_str+"      1.0");
  return jes_str;
}

string make_bakcgrNormErrLine(double expyields, double alpha, int btag, int ich){
  
  stringstream convert;
  
  string str_chann;
  if(ich==0)
    str_chann="ee";
  if(ich==1)
    str_chann="mm";

  //double nSB=countSideBandEvents(btag,ich,false,0.);
  //double alpha=expyields/nSB;
  //the above is for scaling mjj sidebands

  double nSB=expyields/alpha;

  if(btag==2){
    if(ich==0){
      nSB=nSB/.406;
      alpha=alpha*.406;
    }else{
      nSB=nSB/.594;
      alpha=alpha*.594;
    }      
  }

  string bNorm_str="CMS_hzz2l2q_bkg";
  convert << btag;
  bNorm_str+=convert.str();
  if(btag<2){                                // since the normalization is taken from the sum of ee and mm events
    convert.str("");                         // and then multiplied by the fraction expected, the normalization
    convert << str_chann;                    // uncertainties need to then be correlated and thus have the same name
    bNorm_str+=convert.str();                // -- AJW
  }
  convert.str("");
  convert << nSB;
  bNorm_str+="p0    gmN    "+convert.str();
  convert.str(""); 
  convert << alpha;

  if(isFF)
    bNorm_str+=" ----  "+convert.str();
  else
    bNorm_str+=" ---- ----   "+convert.str();
  convert.str("");
 
  //sprintf(bNorm_char,"CMS_hzz2l2q_bkg%i%sp0    gmN    %f ---- ----   %f",btag,str_chann,nSB,alpha);
  cout << bNorm_str << endl;
    
  return bNorm_str;
}

vector<float> eff_fit(int mybtag,int chan){
  cout<<"\n~~~~~~~ Eff fit for btag="<<mybtag<<" chan = "<<chan<<endl;
  std::ostringstream ossm;
  ossm<<mybtag;
  string btag_str="btag"+ossm.str();
  string chan_str= (chan==0)?"ee" : "mm";
  float myeff[nmass];
  for(int im=0;im<nmass;im++){
    //  myeff[im]=exp_eff[mybtag][im];

    double brZtoL=1.0/3.0;
      
    myeff[im]=exp_sig_yields[mybtag][chan][im]/(gen_sig_yields[im]*brZtoL);  //(lumi*NLOxsect[im]);
    cout<<"mh"<<im<<"  my eff "<<myeff[im]<<"  selected events "<<exp_sig_yields[mybtag][chan][im]<< "    generated events "<<gen_sig_yields[im]*brZtoL<<endl;
    //  cout<<"Calc EFF: im="<<im<<"  chan="<<chan<<" "<<mybtag<<"b  EXP="<<exp_sig_yields[mybtag][chan][im]<<"  GEN="<<gen_sig_yields[im]<<"  brZtoL="<<brZtoL<<" ---> EFF: "<<myeff[im]<<endl;
    //if(chan==0)  myeff[im]=exp_sig_yields[mybtag][chan][im]/(gen_sig_yields[im]/2.0);  //(lumi*NLOxsect[im]);
    //else  myeff[im]=exp_sig_yields_mm[mybtag][im]/(gen_sig_yields[im]/2.0);//(lumi*NLOxsect[im]);
    // cout<<"Eff for mass "<<mass[im]<<"  BTag "<<mybtag <<"  "<<myeff[im]<<endl;
  }

  TGraph *gr_eff=new TGraph(nmass,mass,myeff);
  gr_eff->SetName(("effgr_"+btag_str+chan_str).c_str());
  gr_eff->SetTitle(("Efficiency vs Mass ("+chan_str+" , "+btag_str+")").c_str());
  gr_eff->SetMarkerStyle(21);
  gr_eff->SetMinimum(0.0);

  // TF1 *f1 = new TF1("fit_pol1","pol1",1,2);
  // TF1 *f1 = new TF1("fit_sqrt","[0]+[1]*sqrt(x)",1,2);
  // TF1 *f1 = new TF1("fit_log","[0]+[1]*log(x)",150.0,800.0);
  string f1name="fit_poly3_"+btag_str+chan_str;
  TF1 *f1 = new TF1(f1name.c_str(),"pol3",125.0,170.0);
  gr_eff->Fit(f1name.c_str(),"Q");
  vector<float> fit_res;
  fit_res.push_back(f1->GetParameter(0) );
  fit_res.push_back(f1->GetParameter(1) );
  fit_res.push_back(f1->GetParameter(2) );
  fit_res.push_back(f1->GetParameter(3) );
  cout<<"\n\nFit results for BTAG "<<mybtag<<"  (eff= a + b*Mzz + c*Mzz^2 + d*Mzz^3):"<<endl;
  cout<<" a = "<<fit_res.at(0)<<" +/- "<<f1->GetParError(0) <<endl;
  cout<<" b = "<<fit_res.at(1)<<" +/- "<<f1->GetParError(1) <<endl;
  cout<<" c = "<<fit_res.at(2)<<" +/- "<<f1->GetParError(2) <<endl;
  cout<<" d = "<<fit_res.at(3)<<" +/- "<<f1->GetParError(3) <<endl<<endl;

  string canname="c_fitfunc_"+btag_str+chan_str;
  TCanvas *c1=new TCanvas(canname.c_str(),"CANFIT", 900,900);
  c1->SetFillColor(kWhite);
  c1->SetBorderMode(0);
  c1->cd();
  gPad->SetFillColor(kWhite);
  gr_eff->GetXaxis()->SetTitle("M_{H}");
  gr_eff->GetYaxis()->SetTitle("#varepsilon");
  gr_eff->GetYaxis()->SetTitleOffset(1.0);
  gr_eff->Draw("AP");
  c1->SaveAs( ("eff_param_newHLT_"+btag_str +chan_str+".root").c_str()  );

  delete c1;
  canname+=canname+"-ext";
  TCanvas *c1f=new TCanvas(canname.c_str(),"CANFITFUNC", 900,900);
  c1f->SetFillColor(kWhite);
  c1f->SetBorderMode(0);
  c1f->cd();
  f1->GetXaxis()->SetTitle("M_{H}");
  f1->GetYaxis()->SetTitle("#varepsilon");
  f1->GetYaxis()->SetTitleOffset(1.0);
  f1->Draw("L");
  gr_eff->Draw("Psame");
  c1f->SaveAs( ("eff_param_newHLT_"+btag_str +chan_str+"-extrange.root").c_str()  );
  c1f->SaveAs( ("eff_param_newHLT_"+btag_str +chan_str+"-extrange.C").c_str()  );
  // gr_eff->Write();

  delete gr_eff;
  delete c1f;
  delete f1;
  return fit_res;
}//end eff_fit


vector<double> calculate_CBpars(int ibtag,double mH){
  vector<double>outpars;
  double CB_mean,CB_sigma,CB_alpha1,CB_n1,CB_alpha2,CB_n2,unmMean, unmWidth,f;
  if(ibtag==0){
    CB_mean   = mH;						    
    CB_sigma  = -34.2747+.459182*mH-.00142578*mH*mH;		    
    CB_alpha1 = 1.99625-.0057799*mH;				    
    CB_n1     = 31.47-.160475*mH;				    
    CB_alpha2 = -23.6484+.317242*mH-.00102403*mH*mH;		    
    CB_n2     = 18.4988-.100371*mH;				    
    unmMean   = 288.8-1.8888*mH+.00496554*mH*mH+1.2149e-5*mH*mH*mH; 
    unmWidth  = 568.179-6.91623*mH+.0214025*mH*mH;                  
    f         = .75-.00316*mH;
  }
  else if(ibtag==1){
    CB_mean   = mH;				     
    CB_sigma  = 20.5495-.285063*mH+.00108348*mH*mH;  
    CB_alpha1 = 20.7158-.259139*mH+.000855349*mH*mH; 
    CB_n1     =  9;				     
    CB_alpha2 = -1.75895+.0170921*mH;		     
    CB_n2     = 16.6304-0.0902441*mH;		     
    unmMean   = 125.536+.241826*mH;		     
    unmWidth  = 70.7661-.377219*mH;		     
    f         = 1.26634-.00625*mH;                   
  }
  else if(ibtag==2){
    CB_mean   = mH;				    
    CB_sigma  = 25.7944-.331101*mH+.00116889*mH*mH; 
    CB_alpha1 = 4.57891-.0173362*mH;		    
    CB_n1     = 12.2657-.0574034*mH;		    
    CB_alpha2 = 26.8051-.370267*mH+.0013091*mH*mH;  
    CB_n2     = -220+3.012*mH-.00996212*mH*mH;	    
    unmMean   = 100.889+.380056*mH;		    
    unmWidth  = 67.889-.378584*mH;		    
    f         = .167667;                            
  }
  

  outpars.push_back(CB_mean);
  outpars.push_back(CB_sigma);
  outpars.push_back(CB_alpha1);
  outpars.push_back(CB_n1);
  outpars.push_back(CB_alpha2);
  outpars.push_back(CB_n2);

  outpars.push_back(unmMean);
  outpars.push_back(unmWidth);
  outpars.push_back(f);

  return outpars;
}//end vector<double> calculate_CBpars


void extract_exp_sig_yields(float mymass,float mywidth){
  int im=get_mass_index(mymass);
  if(im<0)return;

 ///////////CHANGE THIS DIR NAME ALSO IN get_gen_yields() 
  string myDir="~whitbeck/scratch0/dataFiles/4fbData/";
  //1nvfb_trigfix//";//dir with Francesco's tree //960invpb/

  //signal file
  // fixed by hand.... all files are now summer 11
  int spring11_summer11 = get_signal_gen(mymass);

  string prefix;
  string suffix;

  if(spring11_summer11){
    prefix="spring11_SMHiggs_";
    suffix="GeV_lowmass_last.root";
  }
  else{
  prefix="summer11_SMHiggs_";
  suffix="GeV_lowmass_last.root";
  }
  std::ostringstream ossm;
  ossm<<mass[im];
  string filename=myDir+prefix+ossm.str()+suffix;
  cout << "Opening file: " << filename << " for calculating number of selected signal events" << endl;
  TFile *f=new TFile(filename.c_str(),"READ");
  TTree *t=(TTree*)f->Get("AngularInfo");

  
  string baseline_sel;

  if(spring11_summer11)
    baseline_sel="(mJJ>75 && mJJ<105 && mZZ>125.0 && mZZ<170)";
  else 
    baseline_sel="(mJJ>75 && mJJ<105 && mZZ>125.0 && mZZ<170)";
 
  for(int ib=0;ib<nbtag;ib++){
    string sel="dumm";
    if(ib==0)sel=baseline_sel+"&&nBTags==0";
    else if(ib==1)sel=baseline_sel+"&&nBTags==1";
    else if(ib==2 && spring11_summer11==1)sel=baseline_sel+"&&nBTags==2&&met<50";
    else if(ib==2 && spring11_summer11==0)sel=baseline_sel+"&&nBTags==2&&met<60";
    string sel2="dumsel2";
    for(int ich=0;ich<nchan;ich++){
      cout << "CUTSTRING - " << sel << endl;
      if(ich==0)sel2="("+sel+"&&lep==0.0)*HLTweight*PUweight"; //ele
      else sel2="("+sel+"&&lep==1.0)*HLTweight*PUweight"; //mu
      //      cout<<"M"<<mymass<<", "<<ib<<"b, "<<(ich==0 ? "ee" : "mm") <<"\tSelection string "<<sel2.c_str()<<endl;


      float nexp=float(t->Draw("mZZ>>mzzrescaled(60,125,170)",sel2.c_str(),"goff"));//sel2 contains tighter mzz cut
      TH1F *mzztmp=(TH1F*)gDirectory->Get("mzzrescaled");
      nexp=float( mzztmp->Integral()  );
      exp_sig_yields[ib][ich][im]=nexp;
      cout << "number of selected events for " << mymass << " is: " << nexp << endl;
      delete mzztmp;
      //cout<<"MH-"<<mymass<<" "<<ib<<"b, ee only -> "<<im<<" Selecting : "<<sel2.c_str()<<"  Exp Sig "<<exp_sig_yields[ib][ich][im]<<"  ExpBkg "<<exp_bkg[ib][ich]<<endl;
    }
  }//end loop on btag

  delete t;delete f;


}//extract_exp_sig_yields

void get_gen_yields(){
  gen_sig_yields[0] = 296603; //mh130
  gen_sig_yields[1] = 290000; //mh140
  gen_sig_yields[2] = 286345; //mh150
  gen_sig_yields[3] = 296000; //mh160
  gen_sig_yields[4] = 296000; //mh170
}

int get_signal_gen(float mymass){
  int genID=-1;//0=spring11; 1=summer11

  if(mymass==140||mymass==160.0||mymass==170.0) genID=1;
  else genID=0;
  return genID;

}
int get_mass_index(float mymass){
  int index=-1;
  //  cout<<"Mass "<<mymass<<flush;
  for(int im=0;im<nmass;im++){
    if(mymass==mass[im]){
      index=im;
      break;
    }
  }
  //  if(index>=0)cout<<" found"<<endl;
  // else cout<<" NOT found"<<endl;
  return index;

}
double countSideBandEvents(int btag,int LEP,bool returnExpected,double obs_yield){

  if(ZeroBias && returnExpected)
    return obs_yield;


  double lowMassCut=125;
  double highMassCut=170;
  double lowZcut=0;
  double highZcut=120;
  char* fileNameMC="~/2l2jHelicity/dataFiles/4fbData/summer11_totalMC_lowmass_newPUapprox.root";
  char* fileName="./convertedTree.root";
  //-------------------------------------------                                                                                                                                                      
  // all background samples except low mass DY sample                                                                                                                                                
  //===========================================                                                                                                                                                      

  TFile *fData = new TFile(fileName);
  if(!fData){
    cout << "ERROR: could not load file." << endl;
    return -1.;
  }
  TTree* tData = (TTree*) fData->Get("AngularInfo");
  if(!tData){
    cout << "ERROR: could not load tree." << endl;
    return -1.;
  }
  TFile *fMC = new TFile(fileNameMC);
  if(!fMC){
    cout << "ERROR: could not load file." << endl;
    return -1.;
  }
  TTree* tMC = (TTree*) fMC->Get("AngularInfo");
  if(!tMC){
    cout << "ERROR: could not load tree." << endl;
    return -1.;
  }

  double mZZ,mLL,mJJ,met,weight;
  double nBTags,lep;

  tMC->SetBranchAddress("mZZ",&mZZ);
  tMC->SetBranchAddress("mLL",&mLL);
  tMC->SetBranchAddress("mJJ",&mJJ);
  tMC->SetBranchAddress("nBTags",&nBTags);
  tMC->SetBranchAddress("met",&met);
  tMC->SetBranchAddress("lep",&lep);
  tMC->SetBranchAddress("weight",&weight);

  double totWght_SignalRegion=0, totWght_SideBand=0, totWghtSqrd_SignalRegion=0, totWghtSqrd_SideBand=0;

  for(int iEvt=0; iEvt<tMC->GetEntries(); iEvt++){

    tMC->GetEntry(iEvt);

    if(mZZ<170 && mZZ>120 && mJJ<105 && mJJ>75){
      if(nBTags==(double)btag && btag!=2){
        totWght_SignalRegion+=weight;
        totWghtSqrd_SignalRegion+=weight*weight;
      }
      if(nBTags==btag && nBTags==2 && met<50){
        totWght_SignalRegion+=weight;
        totWghtSqrd_SignalRegion+=weight*weight;
      }
    }

    if(mZZ<170 && mZZ>120 && ((mJJ>105 && mJJ<130)||(mJJ<75&&mJJ>60))){
      if(nBTags==btag && btag!=2){
        totWght_SideBand+=weight;
        totWghtSqrd_SideBand+=weight*weight;
      }
      if(nBTags==btag && btag==2 && met<50){
        totWght_SideBand+=weight;
        totWghtSqrd_SideBand+=weight*weight;
      }
    }
  }
  tData->SetBranchAddress("CMS_hzz2l2q_mZZ",&mZZ);
  tData->SetBranchAddress("mZjj",&mJJ);
  tData->SetBranchAddress("nBTags",&nBTags);
  tData->SetBranchAddress("met",&met);
  tData->SetBranchAddress("leptType",&lep);
  tData->SetBranchAddress("eventWeight",&weight);

  double totalWeight=0;

  for(int iEvt=0; iEvt<tData->GetEntries(); iEvt++){

    tData->GetEntry(iEvt);

    if(mZZ<highMassCut && mZZ>lowMassCut && ((mJJ>105&&mJJ<130)||(mJJ<75&&mJJ>60))&&lep==LEP){// && mLL>lowZcut && mLL<highZcut){                                                                    
      if(nBTags==btag && btag!=2)
        totalWeight+=weight;
      if(nBTags==btag && btag==2 && met<50)
        totalWeight+=weight;
    }

  }
  cout << btag << " b-tags/fb=(#Evt in SideBand)*(alpha)= (" << totalWeight << ")*("
       << totWght_SignalRegion/totWght_SideBand << "+/-"
       << sqrt(totWghtSqrd_SignalRegion/(totWght_SideBand*totWght_SideBand)+totWghtSqrd_SideBand*(totWght_SignalRegion*totWght_SignalRegion/pow(totWght_SideBand,4)))
       << ")= " << totalWeight*totWght_SignalRegion/totWght_SideBand << endl;

  delete fData;
  delete fMC;

  if(returnExpected)
    return totalWeight*totWght_SignalRegion/totWght_SideBand;
  else 
    return totalWeight;
}

