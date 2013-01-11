/*************************************************************************
 *  Authors:   MEKD & MELA fans
 *************************************************************************/
#ifndef MEMCalc_MEMCalc_cpp
#define MEMCalc_MEMCalc_cpp

/// MEMCalculators header
#include "ZZMatrixElement/MEMerger/interface/MEMCalculators.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
///  MEMCalculators interface class to MELA & MEKD packages.
///
///  Provides interface to the MEKD & MELA packages to
///  computes MEs and KDs for the process specified by the user.
///
//////////////////////////////////////////////////////////////////////////


///----------------------------------------------------------------------------------------------
/// MEMCalculators::MEMCalculators - a default constructor
///----------------------------------------------------------------------------------------------
MEMCalculators::MEMCalculators(double collisionEnergy)
{
  /// Initialise MEKD
  MEKD* m_MEKD = new MEKD(collisionEnergy,"");
  /// Initialise MELA
  Mela* m_MELA = new Mela(false, collisionEnergy);
    
  /// Set some non-physical values for MEs initially
  for(int iMemCalc = 0; iMemCalc < NUM_MEMCALCS; iMemCalc++ )
    for(int iProcess = 0; iProcess < NUM_PROCESSES; iProcess++ )
      computedME[iProcess][iMemCalc] = -999.;

  /// Matrix of supproted processes (to be updated)
  isProcSupported[0][0]=0; isProcSupported[0][1]=1; isProcSupported[0][2]=0; isProcSupported[0][3]=0;
  isProcSupported[1][0]=0; isProcSupported[1][1]=1; isProcSupported[1][2]=0; isProcSupported[1][3]=0;
  isProcSupported[2][0]=0; isProcSupported[2][1]=1; isProcSupported[2][2]=0; isProcSupported[2][3]=0;

  m_processNameMEKD[0] = "SMHiggs";
  m_processNameMEKD[1] = "Higgs0M";
  m_processNameMEKD[2] = "ZZ";

}

///----------------------------------------------------------------------------------------------
/// MEMCalculators::computeME - Compute ME for the specified process.
///----------------------------------------------------------------------------------------------
int MEMCalculators::computeME(Processes process, MEMCalcs calculator, vector<TLorentzVector> partP, vector<int> partId, double& me2process)
{
  /// check if process is supported
  if (!isProcSupported[process][calculator]) return ERR_PROCESS;
    
  /// perform computation according to the specified process and MEM package
  switch ( calculator ) {
  case kMEKD:         /// compute ME with MEKD
    if( (m_MEKD->computeME(m_processNameMEKD[process], partP, partId, me2process)) != 0 ) return ERR_COMPUTE;
    break;
  case kAnalytical:   /// compute ME with MELA
    cacheMELAcalculation(partP,partId);
    // insert logic to grab the right ME calculation
    break;
  case kJHUGen:       /// compute ME with JHUGen
    cacheMELAcalculation(partP,partId);
    // insert logic to grab the right ME calculation
    break;
  case kMCFM:         /// compute ME with MCFM
    cacheMELAcalculation(partP,partId);
    me2process=p0plus_VAMCFM;

    break;
  default:
    return ERR_PROCESS;
    break;
  }
    
  return NO_ERR;
}

///----------------------------------------------------------------------------------------------
/// MEMCalculators::computeKD - Compute KD and MEs for the specified processes and MEM calculator.
///----------------------------------------------------------------------------------------------
int MEMCalculators::computeKD(Processes processA, Processes processB, MEMCalcs calculator, vector<TLorentzVector> partP, vector<int> partId, double& kd, double& me2processA, double& me2processB )
{    
  /// check if processes are supported
  if (!isProcSupported[processA][calculator]) return ERR_PROCESS;
  if (!isProcSupported[processB][calculator]) return ERR_PROCESS;
    
  /// perform computation according to the specified process and MEM package
  switch ( calculator ) {
  case kMEKD:         /// compute KD with MEKD
    if( (m_MEKD->computeME(m_processNameMEKD[processA], m_processNameMEKD[processB], partP, partId, kd, me2processA, me2processB)) != 0 ) return ERR_COMPUTE;
    break;
  case kAnalytical:   /// compute KD with MELA
    cacheMELAcalculation(partP,partId);
    // insert logic to grab the right ME calculation
    break;
  case kJHUGen:       /// compute KD with JHUGen
    cacheMELAcalculation(partP,partId);
    // insert logic to grab the right ME calculation
    break;
  case kMCFM:         /// compute KD with MCFM
    cacheMELAcalculation(partP,partId);
    me2process=p0plus_VAMCFM;

    break;
  default:
    return ERR_PROCESS;
    break;
  }
  return NO_ERR;
}


///----------------------------------------------------------------------------------------------
/// MEMCalculators::computeMEs - Compute MEs for the supported set of processes.
///----------------------------------------------------------------------------------------------
int MEMCalculators::computeMEs(vector<TLorentzVector> partP, vector<int> partId)
{
  //loop over MEMCalcs and loop over Processes
  for(int iMemCalc = 0; iMemCalc < NUM_MEMCALCS; iMemCalc++ ) {
    for(int iProcess = 0; iProcess < NUM_PROCESSES; iProcess++ ) {
      if (!isProcSelected[iMemCalc][iProcess]) continue;
      double me2process;
      if( (computeME(iProcess, iMemCalc, partP, partId, me2process)) != 0 ) return ERR_COMPUTE;
      computedME[iProcess][iMemCalc] = me2process;
    }
  }
  return NO_ERR;
}

///----------------------------------------------------------------------------------------------
/// MEMCalculators::retrieveME - Retrieve ME for specified process and specified calculator tool.
///----------------------------------------------------------------------------------------------
void MEMCalculators::retrieveME(Processes process, MEMCalcs calculator, double& me2process)
{
  me2process = computedME[process][calculator];
}

///----------------------------------------------------------------------------------------------
/// MEMCalculators::computeKD - Compute KD for process A and process B, for specified calculator.
///----------------------------------------------------------------------------------------------
void MEMCalculators::computeKD(Processes processA, MEMCalcs calculatorA, Processes processB, MEMCalcs calculatorB, double (*funcKD)(double, double), double& kd, double& me2processA, double& me2processB );
{
  me2processA = computedME[processA][calculatorA];
  me2processB = computedME[processB][calculatorB];
  kd = (*funcKD)(me2processA, me2processB);
}

///----------------------------------------------------------------------------------------------
/// MEMCalculators::logRatio - KD function which returns ln( me2processA / me2processB )
///----------------------------------------------------------------------------------------------
double MEMCalculators::logRatio(double me2processA, double me2processB){
  if (me2processB == 0) return -999.;
  return ln( me2processA / me2processB );
}

///----------------------------------------------------------------------------------------------
/// MEMCalculators::probRatio - KD function which returns me2processA / ( me2processA + c * me2processB )
///----------------------------------------------------------------------------------------------
double MEMCalculators::probRatio(double me2processA, double me2processB){
  double c = 3000;
  if (me2processA + c * me2processB == 0) return -999.;
  return me2processA/( me2processA + c * me2processB );
}

///----------------------------------------------------------------------------------------------
/// MEMCalculators::cacheMELAcalculation - method to interface with Mela::computeP and cache results
///----------------------------------------------------------------------------------------------
void  MEMCalculators::cacheMELAcalculation(vector<TLorentzVector> partP, vector<int> partId){

  // NEED TO INCLUDE SOME PROTECTION SO THAT USER CANT 
  // PASS FOUR-VECTORS IN WRONG ORDER.  FOR NOW ASSUMING
  // THEY ARE PASSED AS e-,e+,mu-,mu+
  // ------------------ channel ------------------------
  int flavor;

  if(abs(partId[0])==abs(partId[1])&&
     abs(partId[0])==abs(partId[2])&&
     abs(partId[0])==abs(partId[3])){

    if(abs(partId[0])==11) flavor=1;
    else flavor=2;

  }else flavor=3;

  // ---------------------------------------------------
  // ---------- COMPUTE ANGLES and MASSES --------------
  float costheta1, costheta2, costhetastar;
  float phi, phi1;

  mela::computeAngles(partP[0], partId[0], partP[1], partId[1], 
		      partP[2], partId[2], partP[3], partId[3],
		      costhetastar,costheta1,costheta2,phi,phi1);

  float m1=(partP[0] + partP[1]).M();
  float m2=(partP[2] + partP[3]).M();

  TLorentzVector ZZ = (partP[0] + partP[1] + partP[2] + partP[3]);
  float mzz = ZZ.M();

  float pt4l  = ZZ.Pt();
  float Y4l   = ZZ.Rapidity(); // Fixme: should probably protect against NaN?
  // ---------------------------------------------------

  m_MELA->computeP(float mzz, float m1, float m2, 
		   float costhetastar,
		   float costheta1, 
		   float costheta2,
		   float phi,
		   float phi1,
		   //signal probabilities
		   float& p0plus_melaNorm,   // higgs, analytic distribution          
		   float& p0plus_mela,   // higgs, analytic distribution          
		   float& p0minus_mela,  // pseudoscalar, analytic distribution 
		   float& p0plus_VAJHU,  // higgs, vector algebra, JHUgen
		   float& p0minus_VAJHU, // pseudoscalar, vector algebra, JHUgen
		   float& p0plus_VAMCFM,// higgs, vector algebra, MCFM
		   float& p1_mela,  // zprime, analytic distribution 
		   float& p1_VAJHU, // zprime, vector algebra, JHUgen,
		   float& p2_mela , // graviton, analytic distribution 
		   float& p2_VAJHU, // graviton, vector algebra, JHUgen,
		   //backgrounds
		   float& bkg_mela,  // background,  analytic distribution 
		   float& bkg_VAMCFM, // background, vector algebra, MCFM
		   float& ggzz_VAMCFM, // background, vector algebra, MCFM for ggzz
		   float& bkg_VAMCFMNorm, // background, vector algebra, MCFM Normalized
		   //pt/rapidity
		   float& p0_pt, // multiplicative probability for signal pt
		   float& p0_y, // multiplicative probability for signal y
		   float& bkg_pt, // multiplicative probability for bkg pt
		   float& bkg_y, // multiplicative probability for bkg y
		   // supermela
		   float& p0plus_m4l,  // signal m4l probability as in datacards
		   float& bkg_m4l,     // backgroun m4l probability as in datacards
		   //optional input parameters
		   float pt4l,
		   float Y4l,
		   int flavor // 1:4e, 2:4mu, 3:2e2mu (for interference effects)
		   )

}

/////----------------------------------------------------------------------------------------------
///// MEMCalculators::computeMEs - Compute MEs for all the processes.
/////----------------------------------------------------------------------------------------------
//int computeMEs(vector<TLorentzVector> partP, vector<int> partId)
//{    
//    /// compute all MEs with MEKD package
//    if( (m_MEKD->computeMEs(partP, partId)) != 0 ) return ERR_COMPUTE;
//    // ... then retrieve all MEs and store in computedME[..][..]
//
//    /// compute all MEs with MELA package
//    /// ...
//}


//////////////////////////////////////////////////////////////////////////

#endif
