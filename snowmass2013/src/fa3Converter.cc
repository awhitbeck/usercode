#include <vector>
#include <cmath>

using namespace std;

enum process {kLHC_HZZs_mH125,
	      kLHC_HZZs_mH126,
	      kLHC_VBF_14TeV_mH125,
	      kLHC_H2J_14TeV_mH125,
	      kILC_250_mH125,
	      kILC_250_mH126,
	      kILC_350,
	      kILC_500,
	      kILC_1000,
	      kNUMconvert};

class fa3Converter {

public:
  
  fa3Converter(process _from, process _to);
  void convert(double &fa3, double &fa2);
  void convert(double &fa3);
  double getConverted(double fa3);
  void changeFrom(process _from){ from=_from;};
  void changeTo(process _to){ to=_to;};

private:

  vector<double> sigma1;
  vector<double> sigma2;
  vector<double> sigma4;

  process from;
  process to;
};

fa3Converter::fa3Converter(process _from, process _to ){
  
  from = _from;
  to   = _to;

  sigma1.push_back(1.860351);  // kLHC_HZZs_mH125 
  sigma1.push_back(2.03971);   // kLHC_HZZs_mH126 
  sigma1.push_back(4687.9);    // kLHC_VBF_14TeV_mH125
  sigma1.push_back(6783.68);   // kLHC_H2J_14TeV_mH125
  sigma1.push_back(0.981396);  // kILC_250_mH125  
  sigma1.push_back(0.94692);   // kILC_250_mH126  
  sigma1.push_back(1.48872);   // kILC_350	  
  sigma1.push_back(2.57246);   // kILC_500        
  sigma1.push_back(8.95721);   // kILC_1000        

  sigma2.push_back(0.672859);  // kLHC_HZZs_mH125 
  sigma2.push_back(0.77517);   // kLHC_HZZs_mH126 
  sigma2.push_back(1.0e-30);   // kLHC_VBF_14TeV_mH125
  sigma2.push_back(1.0e-30);   // kLHC_H2J_14TeV_mH125
  sigma2.push_back(33.4674);   // kILC_250_mH125  
  sigma2.push_back(32.1981);   // kILC_250_mH126  
  sigma2.push_back(125.387);   // kILC_350	  
  sigma2.push_back(516.556);   // kILC_500        
  sigma2.push_back(8208.91);   // kILC_1000        

  sigma4.push_back(0.284353);  // kLHC_HZZs_mH125 
  sigma4.push_back(0.32689);   // kLHC_HZZs_mH126 
  sigma4.push_back(84932.1);   // kLHC_VBF_14TeV_mH125
  sigma4.push_back(6583.72);   // kLHC_H2J_14TeV_mH125
  sigma4.push_back(7.9229);    // kILC_250_mH125  
  sigma4.push_back(7.45502);   // kILC_250_mH126  
  sigma4.push_back(75.3199);   // kILC_350	  
  sigma4.push_back(414.378);   // kILC_500        
  sigma4.push_back(7800.2);    // kILC_1000        

};

double fa3Converter::getConverted(double fa3){

  double fa2_ = 0.;
  double fa3_ = fa3;
  convert(fa3_,fa2_);

  return fa3_;

};

void fa3Converter::convert(double &fa3){

  double fa2 = 0.;
  convert(fa3,fa2);

};

void fa3Converter::convert(double &fa3, double &fa2){
  
  double fg2 = fa2;
  double fg4 = fa3;
  //-----------
  // convert f's back to g's using 
  // cross sections from sigma*[from]
  //-----------
  double g1 = 1.;
  double g2 = sqrt(fg2/(1-fg2-fg4))*sqrt(sigma1[from]/sigma2[from]);
  double g4 = sqrt(fg4/(1-fg2-fg4))*sqrt(sigma1[from]/sigma4[from]);
  //-----------
  // convert g's back to f's using 
  // cross sections from sigma*[to]
  //------------
  double fg2_p = sigma2[to]*g2*g2 / (  sigma1[to]*g1*g1 + sigma2[to]*g2*g2 + sigma4[to]*g4*g4 );
  double fg4_p = sigma4[to]*g4*g4 / (  sigma1[to]*g1*g1 + sigma2[to]*g2*g2 + sigma4[to]*g4*g4 );
  //------------
  
  fa3 = fg4_p;
  fa2 = fg2_p;

};
