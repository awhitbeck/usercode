#include "Riostream.h" 
#include "RooSpinZero_KD_ZH.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h>
#include "TMath.h"
#include "TH1F.h"

using namespace TMath;

ClassImp(RooSpinZero_KD_ZH) 

  RooSpinZero_KD_ZH::RooSpinZero_KD_ZH(const char *name, const char *title, 
				       RooAbsReal& _kd,
				       RooAbsReal& _fa3,
				       bool      _withAcc):
   RooAbsPdf(name,title), 
   kd("kd","kd",this,_kd),
   fa3("fa3","fa3",this,_fa3),
   withAcc(_withAcc)
 { 
 } 


 RooSpinZero_KD_ZH::RooSpinZero_KD_ZH(const RooSpinZero_KD_ZH& other, const char* name) :  
   RooAbsPdf(other,name), 
   kd("kd",this,other.kd),
   fa3("fa3",this,other.fa3),
   withAcc(other.withAcc)
 { 
 } 



 Double_t RooSpinZero_KD_ZH::evaluate() const 
 { 

   const int nbins = 100;
   float xMin = 0.3;
   float xMax = 1.;
   
   double zeroplus[nbins];
   double zerominus[nbins];

   zeroplus[0] = 0.000000;
   zeroplus[1] = 0.000000;
   zeroplus[2] = 0.000000;
   zeroplus[3] = 0.000000;
   zeroplus[4] = 0.000000;
   zeroplus[5] = 0.000000;
   zeroplus[6] = 0.000000;
   zeroplus[7] = 0.131714;
   zeroplus[8] = 0.640286;
   zeroplus[9] = 0.986143;
   zeroplus[10] = 1.293714;
   zeroplus[11] = 1.585857;
   zeroplus[12] = 1.836286;
   zeroplus[13] = 2.132571;
   zeroplus[14] = 2.452571;
   zeroplus[15] = 3.024714;
   zeroplus[16] = 3.118286;
   zeroplus[17] = 3.150000;
   zeroplus[18] = 3.154429;
   zeroplus[19] = 3.179286;
   zeroplus[20] = 3.221714;
   zeroplus[21] = 3.264714;
   zeroplus[22] = 3.301000;
   zeroplus[23] = 3.340286;
   zeroplus[24] = 3.404714;
   zeroplus[25] = 3.531000;
   zeroplus[26] = 3.539714;
   zeroplus[27] = 3.643000;
   zeroplus[28] = 3.660428;
   zeroplus[29] = 3.190571;
   zeroplus[30] = 2.919429;
   zeroplus[31] = 2.718714;
   zeroplus[32] = 2.623857;
   zeroplus[33] = 2.462571;
   zeroplus[34] = 2.369714;
   zeroplus[35] = 2.294429;
   zeroplus[36] = 2.214000;
   zeroplus[37] = 2.138000;
   zeroplus[38] = 2.053000;
   zeroplus[39] = 1.973000;
   zeroplus[40] = 1.946429;
   zeroplus[41] = 1.876714;
   zeroplus[42] = 1.789714;
   zeroplus[43] = 1.776429;
   zeroplus[44] = 1.704143;
   zeroplus[45] = 1.690000;
   zeroplus[46] = 1.632000;
   zeroplus[47] = 1.572286;
   zeroplus[48] = 1.551429;
   zeroplus[49] = 1.541571;
   zeroplus[50] = 1.494000;
   zeroplus[51] = 1.448429;
   zeroplus[52] = 1.419857;
   zeroplus[53] = 1.375571;
   zeroplus[54] = 1.331429;
   zeroplus[55] = 1.306714;
   zeroplus[56] = 1.260571;
   zeroplus[57] = 1.262714;
   zeroplus[58] = 1.230286;
   zeroplus[59] = 1.182429;
   zeroplus[60] = 1.183714;
   zeroplus[61] = 1.133000;
   zeroplus[62] = 1.134000;
   zeroplus[63] = 1.112714;
   zeroplus[64] = 1.038000;
   zeroplus[65] = 1.036571;
   zeroplus[66] = 1.022000;
   zeroplus[67] = 1.000857;
   zeroplus[68] = 0.957143;
   zeroplus[69] = 0.938714;
   zeroplus[70] = 0.920714;
   zeroplus[71] = 0.895857;
   zeroplus[72] = 0.868000;
   zeroplus[73] = 0.842714;
   zeroplus[74] = 0.824286;
   zeroplus[75] = 0.801571;
   zeroplus[76] = 0.787000;
   zeroplus[77] = 0.758714;
   zeroplus[78] = 0.736286;
   zeroplus[79] = 0.736286;
   zeroplus[80] = 0.696000;
   zeroplus[81] = 0.688286;
   zeroplus[82] = 0.639857;
   zeroplus[83] = 0.641286;
   zeroplus[84] = 0.595714;
   zeroplus[85] = 0.597571;
   zeroplus[86] = 0.574143;
   zeroplus[87] = 0.546571;
   zeroplus[88] = 0.512857;
   zeroplus[89] = 0.492429;
   zeroplus[90] = 0.477429;
   zeroplus[91] = 0.441714;
   zeroplus[92] = 0.420429;
   zeroplus[93] = 0.387571;
   zeroplus[94] = 0.347000;
   zeroplus[95] = 0.318571;
   zeroplus[96] = 0.286143;
   zeroplus[97] = 0.234286;
   zeroplus[98] = 0.191429;
   zeroplus[99] = 0.097286;
   zerominus[0] = 0.000000;
   zerominus[1] = 0.000000;
   zerominus[2] = 0.000000;
   zerominus[3] = 0.000000;
   zerominus[4] = 0.000000;
   zerominus[5] = 0.000000;
   zerominus[6] = 0.000000;
   zerominus[7] = 0.285571;
   zerominus[8] = 1.377571;
   zerominus[9] = 2.055857;
   zerominus[10] = 2.540143;
   zerominus[11] = 3.042000;
   zerominus[12] = 3.434571;
   zerominus[13] = 3.897571;
   zerominus[14] = 4.415571;
   zerominus[15] = 5.209143;
   zerominus[16] = 5.228143;
   zerominus[17] = 5.101143;
   zerominus[18] = 5.034857;
   zerominus[19] = 4.893429;
   zerominus[20] = 4.800857;
   zerominus[21] = 4.743285;
   zerominus[22] = 4.643857;
   zerominus[23] = 4.608714;
   zerominus[24] = 4.571857;
   zerominus[25] = 4.507000;
   zerominus[26] = 4.477286;
   zerominus[27] = 4.457286;
   zerominus[28] = 4.381571;
   zerominus[29] = 3.713143;
   zerominus[30] = 3.338286;
   zerominus[31] = 3.008143;
   zerominus[32] = 2.766286;
   zerominus[33] = 2.546143;
   zerominus[34] = 2.395286;
   zerominus[35] = 2.212429;
   zerominus[36] = 2.083857;
   zerominus[37] = 1.966429;
   zerominus[38] = 1.863857;
   zerominus[39] = 1.764143;
   zerominus[40] = 1.627286;
   zerominus[41] = 1.507000;
   zerominus[42] = 1.484000;
   zerominus[43] = 1.390286;
   zerominus[44] = 1.326429;
   zerominus[45] = 1.206000;
   zerominus[46] = 1.182000;
   zerominus[47] = 1.089714;
   zerominus[48] = 1.035857;
   zerominus[49] = 0.987571;
   zerominus[50] = 0.941286;
   zerominus[51] = 0.875714;
   zerominus[52] = 0.835143;
   zerominus[53] = 0.819429;
   zerominus[54] = 0.750429;
   zerominus[55] = 0.720000;
   zerominus[56] = 0.678857;
   zerominus[57] = 0.632286;
   zerominus[58] = 0.581429;
   zerominus[59] = 0.559857;
   zerominus[60] = 0.531143;
   zerominus[61] = 0.491857;
   zerominus[62] = 0.484286;
   zerominus[63] = 0.445143;
   zerominus[64] = 0.416000;
   zerominus[65] = 0.390000;
   zerominus[66] = 0.363714;
   zerominus[67] = 0.358857;
   zerominus[68] = 0.326857;
   zerominus[69] = 0.300571;
   zerominus[70] = 0.282429;
   zerominus[71] = 0.279286;
   zerominus[72] = 0.254429;
   zerominus[73] = 0.235143;
   zerominus[74] = 0.219857;
   zerominus[75] = 0.197429;
   zerominus[76] = 0.193857;
   zerominus[77] = 0.173857;
   zerominus[78] = 0.163429;
   zerominus[79] = 0.145857;
   zerominus[80] = 0.132714;
   zerominus[81] = 0.118429;
   zerominus[82] = 0.114714;
   zerominus[83] = 0.100000;
   zerominus[84] = 0.079143;
   zerominus[85] = 0.079143;
   zerominus[86] = 0.070143;
   zerominus[87] = 0.059429;
   zerominus[88] = 0.054286;
   zerominus[89] = 0.043857;
   zerominus[90] = 0.037429;
   zerominus[91] = 0.032429;
   zerominus[92] = 0.024571;
   zerominus[93] = 0.019857;
   zerominus[94] = 0.014714;
   zerominus[95] = 0.010429;
   zerominus[96] = 0.007714;
   zerominus[97] = 0.004857;
   zerominus[98] = 0.003143;
   zerominus[99] = 0.000286;


   double value = 0.;
   double bin_width = (xMax - xMin) / nbins;
   double array_index = (kd - xMin) / bin_width; 

   value = (1.-fa3) * zeroplus[int(array_index)] + fa3 * zerominus[int(array_index)];

   return value ; 
   
 } 

Int_t RooSpinZero_KD_ZH::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
{

  if (matchArgs(allVars,analVars,RooArgSet(*kd.absArg()))) return 1 ;
  return 0 ;
}

Double_t RooSpinZero_KD_ZH::analyticalIntegral(Int_t code, const char* rangeName) const
{
   
   switch(code)
     {
     case 1: 
       return 1;
     }
   
   assert(0) ;
   return 0 ;
}

