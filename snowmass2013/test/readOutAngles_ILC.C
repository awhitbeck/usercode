#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include "TFile.h"
#include "TList.h"
#include "TNtuple.h"
#include "TTree.h"
#include "TRandom.h"
#include "TLorentzVector.h"

using namespace std;

void calculateAngles(TLorentzVector p4H, TLorentzVector p4Z1, TLorentzVector p4M11, TLorentzVector p4M12, TLorentzVector p4Z2, TLorentzVector p4M21, TLorentzVector p4M22, double& costheta1, double& costheta2, double& phi, double& costhetastar, double& phistar1, double& phistar2, double& phistar12, double& phi1, double& phi2);

void readOutAngles_ILC(std::string filename, bool debug = false) {
  
    ifstream fin;
    std::string filenameT = filename + ".txt";
    std::cout << "Processing " << filenameT << std::endl;
    fin.open(filenameT.c_str());
    
    int maxEvents = 100000;
	
    char oname[250];
    sprintf(oname,"%s.root",filename.c_str());
    
    TFile fout(oname, "RECREATE");
    TTree* tree = new TTree("SelectedTree", "SelectedTree");
    
    Float_t m_costheta1, m_costheta2, m_phi, m_costhetastar, m_phistar1;
    Float_t m_zhmass, m_zmass, m_hmass;
    Float_t m_ptlplus, m_etalplus, m_philplus;
    Float_t m_ptlminus, m_etalminus, m_philminus;
    Float_t m_wt; 

    
    tree->Branch("ZHMass", &m_zhmass, "ZHMass/F");
    tree->Branch("ZMass",  &m_zmass,  "ZMass/F");
    tree->Branch("HMass",  &m_hmass,  "HMass/F");
    
    tree->Branch("costheta1"   , &m_costheta1, "costheta1/F");
    tree->Branch("costheta2"   , &m_costheta2, "costheta2/F");
    tree->Branch("phi"         , &m_phi, "phi/F");
    tree->Branch("costhetastar", &m_costhetastar, "costhetastar/F");
    tree->Branch("phistar1"    , &m_phistar1, "phistar1/F");

    tree->Branch("ptlplus"     , &m_ptlplus, "ptlplus/F");
    tree->Branch("etalplus"    , &m_etalplus,"etalplus/F");
    tree->Branch("philplus"    , &m_philplus,"philplus/F");

    tree->Branch("ptlminus"     , &m_ptlminus, "ptlminus/F");
    tree->Branch("etalminus"    , &m_etalminus,"etalminus/F");
    tree->Branch("philminus"    , &m_philminus,"philminus/F");
    tree->Branch("wt"           , &m_wt, "wt/F");

        
    int ctr = 0;
    int FourlCount = 0;
    std::vector <float> listOfMom;
    int idup[4], istup[4], mothup[4][2], icolup[4][2];
    float pup[4][5], vtimup[4], spinup[4];
    int nparticle, para;
    double weight, m_V, alpha_qed, alpha_s; 
    // has to use double as some weight is E-38, which exceeds the float

    while (!fin.eof() && fin.good()){

      fin  >> nparticle >> para >> weight >> m_V >> alpha_qed >> alpha_s; 
      if ( debug ) 
	std::cout << nparticle << "\t" << para << "\t" <<  weight << "\t" << m_V << "\t" <<  alpha_qed  << "\t" <<  alpha_s << "\n"; 

      for (int a = 0; a < 4; a++){
	fin >> idup[a] >> istup[a] >> mothup[a][0] >> mothup[a][1] >> icolup[a][0] >> icolup[a][1];
	if ( debug ) 
	  std::cout << idup[a] << "\t" <<  istup[a] << "\t" <<  mothup[a][0] << "\t" <<  mothup[a][1] << "\t" <<  icolup[a][0]  << "\t" <<  icolup[a][1] << "\t"; 
	for (int i = 0; i < 5; i++){
	  fin >> pup[a][i];
	  if ( debug ) 
	    std::cout << pup[a][i] << "  "; 
	}
	fin >> vtimup[a] >> spinup[a];
	if ( debug ) 
	  std::cout << "\n";
      }
      
      // assign the p4 from the text file
      TLorentzVector p_lplus;
      TLorentzVector p_lminus;
      TLorentzVector p_b;    
      TLorentzVector p_bbar; 

      for ( int a = 0; a < 4; a++ ) {
	
	if ( idup[a] == -11 || idup[a] == -13  || idup[a] == -15 ) 		
	  p_lplus = new TLorentzVector(pup[a][0], pup[a][1], pup[a][2], pup[a][3]);

	if ( idup[a] == 11 || idup[a] == 13  || idup[a] == 15 ) 		
	  p_lminus = new TLorentzVector(pup[a][0], pup[a][1], pup[a][2], pup[a][3]);
	
	if ( idup[a] == 5 ) 
	  p_b = new TLorentzVector(pup[a][0], pup[a][1], pup[a][2], pup[a][3]);
	
	if ( idup[a] == -5 ) 
	  p_bbar = new TLorentzVector(pup[a][0], pup[a][1], pup[a][2], pup[a][3]);
      }

      TLorentzVector pZ = p_lplus + p_lminus;
      TLorentzVector pH = p_b + p_bbar; 
      TLorentzVector pZstar = pZ + pH; 

      if ( debug ) {
	std::cout << "p4(l+): "   << p_lplus.Px()  << ", " <<  p_lplus.Py()  << ", " << p_lplus.Pz()  << ", " << p_lplus.E() << "\n";
	std::cout << "p4(l-): "   << p_lminus.Px() << ", " <<  p_lminus.Py() << ", " << p_lminus.Pz() << ", " << p_lminus.E() << "\n";
	std::cout << "p4(b): "    << p_b.Px()      << ", " <<  p_b.Py()      << ", " << p_b.Pz()      << ", " << p_b.E() << "\n";
	std::cout << "p4(bbar): " << p_bbar.Px()   << ", " <<  p_bbar.Py()   << ", " << p_bbar.Pz()   << ", " << p_bbar.E() << "\n";
	std::cout << "pZ: "       << pZ.Px()       << ", " <<  pZ.Py()       << ", " << pZ.Pz()       << ", " << pZ.E() << "\n";
	std::cout << "pH: "       << pH.Px()       << ", " <<  pH.Py()       << ", " << pH.Pz()       << ", " << pH.E() << "\n";
	std::cout << "p(H+Z): "   << (pH+pZ).Px()       << ", " <<  (pH+pZ).Py()       << ", " << (pH+pZ).Pz()       << ", " << (pH+pZ).E() << "\n";
      }
      
      double angle_costheta1, angle_costheta2, angle_phi, angle_costhetastar, angle_phistar1, angle_phistar2, angle_phistar12, angle_phi1, angle_phi2;
      calculateAngles( pZstar, pZ, p_lminus, p_lplus, pH, p_b, p_bbar, angle_costheta1, angle_costheta2, angle_phi, angle_costhetastar, angle_phistar1, angle_phistar2, angle_phistar12, angle_phi1, angle_phi2);
      
      // replace the angles 
      m_costheta1 = float(angle_costhetastar);
      m_costheta2 = float(angle_costheta1);
      m_phi = float(angle_phistar1);
      m_costhetastar = float(angle_costheta2);
      m_phistar1 = float(angle_phi);
      
      m_zhmass = float(pZstar.M());
      m_zmass = float(pZ.M());
      m_hmass = float(pH.M());

      m_ptlplus = p_lplus.Pt();
      m_etalplus = p_lplus.Eta();
      m_philplus = p_lplus.Phi();

      m_ptlminus = p_lminus.Pt();
      m_etalminus = p_lminus.Eta();
      m_philminus = p_lminus.Phi();
      
      m_wt = weight;
      
      tree->Fill();
			  
      // counter
      ctr++;
      if (ctr%1000 == 0) std::cout << "event number: " << ctr << std::endl;
      if (ctr == maxEvents) break;	
    }
    
    fout.cd();
    tree->Write();
    fout.Close();
	
}

void calculateAngles(TLorentzVector thep4H, TLorentzVector thep4Z1, TLorentzVector thep4M11, TLorentzVector thep4M12, TLorentzVector thep4Z2, TLorentzVector thep4M21, TLorentzVector thep4M22, double& costheta1, double& costheta2, double& phi, double& costhetastar, double& phistar1, double& phistar2, double& phistar12, double& phi1, double& phi2){
	
  float norm;
  
  TVector3 boostX = -(thep4H.BoostVector());
  TLorentzVector thep4Z1inXFrame( thep4Z1 );
  TLorentzVector thep4Z2inXFrame( thep4Z2 );	
  thep4Z1inXFrame.Boost( boostX );
  thep4Z2inXFrame.Boost( boostX );
  TVector3 theZ1X_p3 = TVector3( thep4Z1inXFrame.X(), thep4Z1inXFrame.Y(), thep4Z1inXFrame.Z() );
  TVector3 theZ2X_p3 = TVector3( thep4Z2inXFrame.X(), thep4Z2inXFrame.Y(), thep4Z2inXFrame.Z() );
  
  // calculate phi1, phi2, costhetastar
  phi1 = theZ1X_p3.Phi();
  phi2 = theZ2X_p3.Phi();
  
  ///////////////////////////////////////////////
  // check for z1/z2 convention, redefine all 4 vectors with convention
  ///////////////////////////////////////////////	
  TLorentzVector p4H, p4Z1, p4M11, p4M12, p4Z2, p4M21, p4M22;
  p4H = thep4H;
  
  /* ORDER OF Z1 AND Z2 ALREADY CHOSEN IN MAIN FUNCTION!!!!!! - - - - - - 
     if ((phi1 < 0)&&(phi1 >= -TMath::Pi())){   // old convention based on phi
     p4Z1 = thep4Z2; p4M11 = thep4M21; p4M12 = thep4M22;
     p4Z2 = thep4Z1; p4M21 = thep4M11; p4M22 = thep4M12;		
     costhetastar = theZ2X_p3.CosTheta();
     }
     else{
     p4Z1 = thep4Z1; p4M11 = thep4M11; p4M12 = thep4M12;
     p4Z2 = thep4Z2; p4M21 = thep4M21; p4M22 = thep4M22;
     costhetastar = theZ1X_p3.CosTheta();
     }
     - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - - - -*/
  
  p4Z1 = thep4Z1; p4M11 = thep4M11; p4M12 = thep4M12;
  p4Z2 = thep4Z2; p4M21 = thep4M21; p4M22 = thep4M22;
  costhetastar = theZ1X_p3.CosTheta();
	
  // now helicity angles................................
  // ...................................................
  TVector3 boostZ1 = -(p4Z1.BoostVector());
  TLorentzVector p4Z2Z1(p4Z2);
  p4Z2Z1.Boost(boostZ1);
  // find the decay axis
  TVector3 unitx_1( -p4Z2Z1.X(), -p4Z2Z1.Y(), -p4Z2Z1.Z() );
  norm = 1/(unitx_1.Mag());
  unitx_1*=norm;
  // boost daughters of z2
  TLorentzVector p4M21Z1(p4M21);
  TLorentzVector p4M22Z1(p4M22);
  p4M21Z1.Boost(boostZ1);
  p4M22Z1.Boost(boostZ1);
  // create z and y axes
  TVector3 p4M21Z1_p3( p4M21Z1.X(), p4M21Z1.Y(), p4M21Z1.Z() );
  TVector3 p4M22Z1_p3( p4M22Z1.X(), p4M22Z1.Y(), p4M22Z1.Z() );
  TVector3 unitz_1 = p4M21Z1_p3.Cross( p4M22Z1_p3 );
  norm = 1/(unitz_1.Mag());
  unitz_1 *= norm;
  TVector3 unity_1 = unitz_1.Cross(unitx_1);
  
  // calculate theta1
  TLorentzVector p4M11Z1(p4M11);
  p4M11Z1.Boost(boostZ1);
  TVector3 p3M11( p4M11Z1.X(), p4M11Z1.Y(), p4M11Z1.Z() );
  TVector3 unitM11 = p3M11.Unit();
  float x_m11 = unitM11.Dot(unitx_1); float y_m11 = unitM11.Dot(unity_1); float z_m11 = unitM11.Dot(unitz_1);
  TVector3 M11_Z1frame(y_m11, z_m11, x_m11);
  costheta1 = M11_Z1frame.CosTheta();

  //////-----------------------old way of calculating phi---------------/////////
  phi = M11_Z1frame.Phi();
  
  // set axes for other system
  TVector3 boostZ2 = -(p4Z2.BoostVector());
  TLorentzVector p4Z1Z2(p4Z1);
  p4Z1Z2.Boost(boostZ2);
  TVector3 unitx_2( -p4Z1Z2.X(), -p4Z1Z2.Y(), -p4Z1Z2.Z() );
  norm = 1/(unitx_2.Mag());
  unitx_2*=norm;
  // boost daughters of z2
  TLorentzVector p4M11Z2(p4M11);
  TLorentzVector p4M12Z2(p4M12);
  p4M11Z2.Boost(boostZ2);
  p4M12Z2.Boost(boostZ2);
  TVector3 p4M11Z2_p3( p4M11Z2.X(), p4M11Z2.Y(), p4M11Z2.Z() );
  TVector3 p4M12Z2_p3( p4M12Z2.X(), p4M12Z2.Y(), p4M12Z2.Z() );
  TVector3 unitz_2 = p4M11Z2_p3.Cross( p4M12Z2_p3 );
  norm = 1/(unitz_2.Mag());
  unitz_2*=norm;
  TVector3 unity_2 = unitz_2.Cross(unitx_2);
  // calcuate theta2
  TLorentzVector p4M21Z2(p4M21);
  p4M21Z2.Boost(boostZ2);
  TVector3 p3M21( p4M21Z2.X(), p4M21Z2.Y(), p4M21Z2.Z() );
  TVector3 unitM21 = p3M21.Unit();
  float x_m21 = unitM21.Dot(unitx_2); float y_m21 = unitM21.Dot(unity_2); float z_m21 = unitM21.Dot(unitz_2);
  TVector3 M21_Z2frame(y_m21, z_m21, x_m21);
  costheta2 = M21_Z2frame.CosTheta();
  
  // calculate phi
  // calculating phi_n
  TLorentzVector n_p4Z1inXFrame( p4Z1 );
  TLorentzVector n_p4M11inXFrame( p4M11 );
  n_p4Z1inXFrame.Boost( boostX );
  n_p4M11inXFrame.Boost( boostX );        
  TVector3 n_p4Z1inXFrame_unit = n_p4Z1inXFrame.Vect().Unit();
  TVector3 n_p4M11inXFrame_unit = n_p4M11inXFrame.Vect().Unit();  
  TVector3 n_unitz_1( n_p4Z1inXFrame_unit );
  //// y-axis is defined by neg lepton cross z-axis
  //// the subtle part is here...
  TVector3 n_unity_1 = n_unitz_1.Cross( n_p4M11inXFrame_unit );
  TVector3 n_unitx_1 = n_unity_1.Cross( n_unitz_1 );
  
  TLorentzVector n_p4M21inXFrame( p4M21 );
  n_p4M21inXFrame.Boost( boostX );
  TVector3 n_p4M21inXFrame_unit = n_p4M21inXFrame.Vect().Unit();
  //rotate into other plane
  TVector3 n_p4M21inXFrame_unitprime( n_p4M21inXFrame_unit.Dot(n_unitx_1), n_p4M21inXFrame_unit.Dot(n_unity_1), n_p4M21inXFrame_unit.Dot(n_unitz_1) );
  
  ///////-----------------new way of calculating phi-----------------///////
  // float phi_n =  n_p4M21inXFrame_unitprime.Phi();
  /// and then calculate phistar1
  TVector3 n_p4PartoninXFrame_unit( 0.0, 0.0, 1.0 );
  TVector3 n_p4PartoninXFrame_unitprime( n_p4PartoninXFrame_unit.Dot(n_unitx_1), n_p4PartoninXFrame_unit.Dot(n_unity_1), n_p4PartoninXFrame_unit.Dot(n_unitz_1) );
  // negative sign is for arrow convention in paper
  phistar1 = (n_p4PartoninXFrame_unitprime.Phi());
  
  // and the calculate phistar2
  TLorentzVector n_p4Z2inXFrame( p4Z2 );
  n_p4Z2inXFrame.Boost( boostX );
  TVector3 n_p4Z2inXFrame_unit = n_p4Z2inXFrame.Vect().Unit();
  TVector3 n_unitz_2( n_p4Z2inXFrame_unit );
  //// y-axis is defined by neg lepton cross z-axis
  //// the subtle part is here...
  TVector3 n_unity_2 = n_unitz_2.Cross( n_p4M21inXFrame_unit );
  TVector3 n_unitx_2 = n_unity_2.Cross( n_unitz_2 );
  TVector3 n_p4PartoninZ2PlaneFrame_unitprime( n_p4PartoninXFrame_unit.Dot(n_unitx_2), n_p4PartoninXFrame_unit.Dot(n_unity_2), n_p4PartoninXFrame_unit.Dot(n_unitz_2) );
  phistar2 = (n_p4PartoninZ2PlaneFrame_unitprime.Phi());
  
  float phistar12_0 = phistar1 + phistar2;
  if (phistar12_0 > TMath::Pi()) phistar12 = phistar12_0 - 2*TMath::Pi();
  else if (phistar12_0 < (-1.)*TMath::Pi()) phistar12 = phistar12_0 + 2*TMath::Pi();
  else phistar12 = phistar12_0;
	
}







