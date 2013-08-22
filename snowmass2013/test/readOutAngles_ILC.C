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
#include "TLorentzRotation.h"
#include "Math/VectorUtil.h"

using namespace std;

void calculateAngles(TLorentzVector p4H, TLorentzVector p4Z1, TLorentzVector p4M11, TLorentzVector p4M12, TLorentzVector p4Z2, TLorentzVector p4M21, TLorentzVector p4M22, double& costheta1, double& costheta2, double& phi, double& costhetastar, double& phistar1, double& phistar2, double& phistar12, double& phi1, double& phi2);

void computeAngles(TLorentzVector thep4H, TLorentzVector thep4Z1, TLorentzVector thep4M11, TLorentzVector thep4M12, TLorentzVector thep4Z2, TLorentzVector thep4M21, TLorentzVector thep4M22, double& costheta1, double& costheta2, double& Phi, double& costhetastar, double& Phi1);

vector<TLorentzVector> Calculate4Momentum(float Mx,float M1,float M2,float theta,float theta1,float theta2,float _Phi1_,float _Phi_); 

void readOutAngles_ILC(std::string filename, bool applyAcc=false, bool debug = false) {
    
    ifstream fin;
    std::string filenameT = filename + ".txt";
    std::cout << "Processing " << filenameT << std::endl;
    fin.open(filenameT.c_str());
    
    int maxEvents = 100000000;
    
    char oname[250];
    sprintf(oname,"%s.root",filename.c_str());
    
    TFile fout(oname, "RECREATE");
    TTree* tree = new TTree("SelectedTree", "SelectedTree");
    
    Float_t m_costheta1, m_costheta2, m_phi, m_costhetastar, m_phistar1;
    Float_t m_m, m_Y, m_zmass, m_hmass;
    Float_t m_ptlplus, m_etalplus, m_philplus;
    Float_t m_ptlminus, m_etalminus, m_philminus;
    Float_t m_ptb, m_etab, m_phib;
    Float_t m_ptbbar, m_etabbar, m_phibbar;
    Float_t m_ptH; 
    Float_t m_wt; 
    
    Float_t ptlminus_ALT,ptlplus_ALT;
    Float_t etalminus_ALT,etalplus_ALT;
    
    tree->Branch("m", &m_m, "m/F");
    tree->Branch("Y", &m_Y, "Y/F");    
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
    
    tree->Branch("ptlplus_ALT"     , &ptlplus_ALT, "ptlplus_ALT/F");
    tree->Branch("ptlminus_ALT"     , &ptlminus_ALT, "ptlminus_ALT/F");
    tree->Branch("etalplus_ALT"     , &etalplus_ALT, "etalplus_ALT/F");
    tree->Branch("etalminus_ALT"     , &etalminus_ALT, "etalminus_ALT/F");
    
    tree->Branch("ptb"     , &m_ptb, "ptb/F");
    tree->Branch("etab"    , &m_etab,"etab/F");
    tree->Branch("phib"    , &m_phib,"phib/F");
    
    tree->Branch("ptbbar"     , &m_ptbbar, "ptbbar/F");
    tree->Branch("etabbar"    , &m_etabbar,"etabbar/F");
    tree->Branch("phibbar"    , &m_phibbar,"phibbar/F");
    
    tree->Branch("ptH", &m_ptH, "ptH/F");
    
    tree->Branch("wt"           , &m_wt, "wt/F");
    
    
    int ctr = 0;
    int FourlCount = 0;
    std::vector <float> listOfMom;
    int idup[4], istup[4], mothup[4][2], icolup[4][2];
    float pup[4][5], vtimup[4], spinup[4];
    int nparticle, para;
    double weight, m_V, alpha_qed, alpha_s; 
    // has to use double as some weight is E-38, which exceeds the float
    
    //    while (!fin.eof() && fin.good()){
    while (true) {
        
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
        if(fin.eof()) break;
        
        // assign the p4 from the text file
        TLorentzVector p_lplus;
        TLorentzVector p_lminus;
        TLorentzVector p_b;    
        TLorentzVector p_bbar; 
        
        for ( int a = 0; a < 4; a++ ) {
            
            if ( idup[a] == -11 || idup[a] == -13  || idup[a] == -15 ) 		
                p_lplus.SetPxPyPzE(pup[a][0], pup[a][1], pup[a][2], pup[a][3]);
            
            if ( idup[a] == 11 || idup[a] == 13  || idup[a] == 15 ) 		
                p_lminus.SetPxPyPzE(pup[a][0], pup[a][1], pup[a][2], pup[a][3]);
            
            if ( idup[a] == 5 ) 
                p_b.SetPxPyPzE(pup[a][0], pup[a][1], pup[a][2], pup[a][3]);
            
            if ( idup[a] == -5 ) 
                p_bbar.SetPxPyPzE(pup[a][0], pup[a][1], pup[a][2], pup[a][3]);
        }
        
        TLorentzVector pZ = p_lplus + p_lminus;
        TLorentzVector pH = p_b + p_bbar; 
        TLorentzVector pZstar = pZ + pH; 
        
        if ( debug ) {
	     std::cout << "=====================p4 Inputs =====================\n";
	     std::cout << "p4(l+): "   << p_lplus.Px()  << ", " <<  p_lplus.Py()  << ", " << p_lplus.Pz()  << ", " << p_lplus.E() 
		       << "\t (pt, eta)" << p_lplus.Pt() << ", " << p_lplus.Eta() << "\n";
	     std::cout << "p4(l-): "   << p_lminus.Px() << ", " <<  p_lminus.Py() << ", " << p_lminus.Pz() << ", " << p_lminus.E() 
		       << "\t (pt, eta)" << p_lminus.Pt() << ", " << p_lminus.Eta() << "\n";
	     std::cout << "p4(b): "    << p_b.Px()      << ", " <<  p_b.Py()      << ", " << p_b.Pz()      << ", " << p_b.E() 
		       << "\t (pt, eta)" << p_b.Pt() << ", " << p_b.Eta() << "\n";
	     std::cout << "p4(bbar): " << p_bbar.Px()   << ", " <<  p_bbar.Py()   << ", " << p_bbar.Pz()   << ", " << p_bbar.E()
		      << "\t (pt, eta)" << p_bbar.Pt() << ", " << p_bbar.Eta() << "\n";
	     std::cout << "====================================================\n";
	     std::cout << "pZ: "       << pZ.Px()       << ", " <<  pZ.Py()       << ", " << pZ.Pz()       << ", " << pZ.E() << "\n";
	     std::cout << "pH: "       << pH.Px()       << ", " <<  pH.Py()       << ", " << pH.Pz()       << ", " << pH.E() << "\n";
	     std::cout << "p(H+Z): "   << (pH+pZ).Px()       << ", " <<  (pH+pZ).Py()       << ", " << (pH+pZ).Pz()       << ", " << (pH+pZ).E() << "\n";
	     
        }
        
        double angle_costheta1, angle_costheta2, angle_phi, angle_costhetastar, angle_phistar1, angle_phistar2, angle_phistar12, angle_phi1, angle_phi2;
	//calculateAngles( pZstar, pZ, p_lminus, p_lplus, pH, p_b, p_bbar, angle_costheta1, angle_costheta2, angle_phi, angle_costhetastar, angle_phistar1, angle_phistar2, angle_phistar12, angle_phi1, angle_phi2);
	// use the new calcualtions..
	computeAngles( pZstar, pZ, p_lminus, p_lplus, pH, p_b, p_bbar, angle_costheta1, angle_costheta2, angle_phi, angle_costhetastar, angle_phistar1);
	
        // replace the angles 
        m_costheta1 = float(angle_costhetastar);
        m_costheta2 = float(angle_costheta1);
        m_phi = float(angle_phistar1);
        m_costhetastar = float(angle_costheta2);
        m_phistar1 = float(angle_phi);
        
        m_m = float(pZstar.M());
        m_Y = float(pZstar.Rapidity());
        m_zmass = float(pZ.M());
        m_hmass = float(pH.M());
        
        m_ptlplus = p_lplus.Pt();
        m_etalplus = p_lplus.Eta();
        m_philplus = p_lplus.Phi();
        
        m_ptlminus = p_lminus.Pt();
        m_etalminus = p_lminus.Eta();
        m_philminus = p_lminus.Phi();
        
        m_ptb = p_b.Pt();
        m_etab = p_b.Eta();
        m_phib = p_b.Phi();
        
        m_ptbbar = p_bbar.Pt();
        m_etabbar = p_bbar.Eta();
        m_phibbar = p_bbar.Phi();
        
        m_ptH = pH.Pt();
        
        m_wt = weight;
        
	vector<TLorentzVector> lep_4vec = Calculate4Momentum(m_m,91.188,125.,acos(m_costheta1),acos(m_costheta2),acos(0),m_phi,0);
	//vector<TLorentzVector> lep_4vec = Calculate4Momentum(m_m,m_zmass,m_hmass,acos(m_costheta1),acos(m_costheta2),acos(m_costhetastar),m_phi,m_phistar1);
        
	// now boost the 4leptons to the original frame
	TLorentzVector pZstar_new;
	// calculate pz and E based on m and Y
	double pz_Zstar_new = m_m*sqrt((pow(exp(2*m_Y),2) -1)/(4*exp(2*m_Y)));
	pZstar_new.SetPxPyPzE(0, 0, pz_Zstar_new, sqrt(pz_Zstar_new*pz_Zstar_new+m_m*m_m));
	TVector3 boost_pZstar = pZstar_new.BoostVector();
	
	for (int i = 0 ; i < 4 ; i++ )
	  lep_4vec[i].Boost(boost_pZstar); 
	
	if ( debug ) 
	  std::cout << "p(H+Z) after recalculation: " 
		    << pZstar_new.Px() << ", " 
		    << pZstar_new.Py() << ", " 
		    << pZstar_new.Pz() << ", " 
		    << pZstar_new.E() << "\n";
	
        ptlminus_ALT = lep_4vec[0].Pt();
        ptlplus_ALT = lep_4vec[1].Pt();
        
        if ( ptlminus_ALT > 0.  && ptlplus_ALT > 0.) {
            etalminus_ALT = lep_4vec[0].Eta();
            etalplus_ALT = lep_4vec[1].Eta();
        }
        
        if ( m_hmass  < 115. ) continue;
        if ( m_hmass  > 140. ) continue;
        
        if ( applyAcc ) {
            if ( ptlplus_ALT < 5. ) continue;
            if ( TMath::Abs(etalplus_ALT) > 2.4 ) continue;
            if ( ptlminus_ALT < 5. ) continue;
            if ( TMath::Abs(etalminus_ALT) > 2.4 ) continue;
        }

	if ( debug ) {
	  std::cout << "=======After recalculation based on the angles=========\n"; 
	  std::cout << "p4(l+): "   << lep_4vec[1].Px()  << ", " <<  lep_4vec[1].Py()  << ", " << lep_4vec[1].Pz()  << ", " << lep_4vec[1].E() 
		    << "\t (pt, eta)" << lep_4vec[1].Pt() << ", " << lep_4vec[1].Eta() << "\n";
	  std::cout << "p4(l-): "   << lep_4vec[0].Px() << ", " <<  lep_4vec[0].Py() << ", " << lep_4vec[0].Pz() << ", " << lep_4vec[0].E() 
		    << "\t (pt, eta)" << lep_4vec[0].Pt() << ", " << lep_4vec[0].Eta() << "\n";
	  std::cout << "p4(b): "    << lep_4vec[2].Px()      << ", " <<  lep_4vec[2].Py()      << ", " << lep_4vec[2].Pz()      << ", " << lep_4vec[2].E() 
		    << "\t (pt, eta)" << lep_4vec[2].Pt() << ", " << lep_4vec[2].Eta() << "\n";
	  std::cout << "p4(bbar): " << lep_4vec[3].Px()   << ", " <<  lep_4vec[3].Py()   << ", " << lep_4vec[3].Pz()   << ", " << lep_4vec[3].E() 
		    << "\t (pt, eta)" << lep_4vec[3].Pt() << ", " << lep_4vec[3].Eta() << "\n";
	  std::cout << "deltaR(l+,l-): " << ROOT::Math::VectorUtil::DeltaR(p_lminus, p_lplus) << ", after calculation " << ROOT::Math::VectorUtil::DeltaR(lep_4vec[0], lep_4vec[1]) << "\n";
	  std::cout << "======================================================\n";
	}
        
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

vector<TLorentzVector> Calculate4Momentum(float Mx,float M1,float M2,float theta,float theta1,float theta2,float _Phi1_,float _Phi_)
{
    
    float phi1,phi2;
    phi1=TMath::Pi()-_Phi1_;
    phi2=_Phi1_+_Phi_;
    
    
    float gamma1,gamma2,beta1,beta2;
    
    gamma1=(Mx*Mx+M1*M1-M2*M2)/(2*Mx*M1);
    gamma2=(Mx*Mx-M1*M1+M2*M2)/(2*Mx*M2);
    beta1=sqrt(1-1/(gamma1*gamma1));
    beta2=sqrt(1-1/(gamma2*gamma2));
    
    //gluon 4 vectors
    TLorentzVector p1CM(0,0,Mx/2,Mx/2);
    TLorentzVector p2CM(0,0,-Mx/2,Mx/2);
    
    //vector boson 4 vectors
    TLorentzVector kZ1(gamma1*M1*sin(theta)*beta1,0, gamma1*M1*cos(theta)*beta1,gamma1*M1*1);   
    TLorentzVector kZ2(-gamma2*M2*sin(theta)*beta2,0, -gamma2*M2*cos(theta)*beta2,gamma2*M2*1);
    
    //Rotation and Boost matrices. Note gamma1*beta1*M1=gamma2*beta2*M2.
    
    TLorentzRotation Z1ToZ,Z2ToZ;
    
    Z1ToZ.Boost(0,0,beta1);
    Z2ToZ.Boost(0,0,beta2);
    Z1ToZ.RotateY(theta);
    Z2ToZ.RotateY(TMath::Pi()+theta);
    
    //fermons 4 vectors in vector boson rest frame
    
    TLorentzVector p3Z1((M1/2)*sin(theta1)*cos(phi1),(M1/2)*sin(theta1)*sin(phi1),(M1/2)*cos(theta1),(M1/2)*1);
    
    TLorentzVector p4Z1(-(M1/2)*sin(theta1)*cos(phi1),-(M1/2)*sin(theta1)*sin(phi1),-(M1/2)*cos(theta1),(M1/2)*1);
    
    TLorentzVector p5Z2((M2/2)*sin(theta2)*cos(phi2),(M2/2)*sin(theta2)*sin(phi2),(M2/2)*cos(theta2),(M2/2)*1);
    
    TLorentzVector p6Z2(-(M2/2)*sin(theta2)*cos(phi2),-(M2/2)*sin(theta2)*sin(phi2),-(M2/2)*cos(theta2),(M2/2)*1);
    
    
    // fermions 4 vectors in CM frame
    
    TLorentzVector p3CM,p4CM,p5CM,p6CM;
    
    p3CM=Z1ToZ*p3Z1;
    p4CM=Z1ToZ*p4Z1;
    p5CM=Z2ToZ*p5Z2;
    p6CM=Z2ToZ*p6Z2;
    
    vector<TLorentzVector> p;
    
    p.push_back(p3CM);
    p.push_back(p4CM);
    p.push_back(p5CM);
    p.push_back(p6CM);
    
    return p;
}


//////////////////////////////////
//// P A P E R   4 - V E C T O R   D E F I N I T I O N   O F   P H I   A N D   P H I 1
//////////////////////////////////
void computeAngles(TLorentzVector thep4H, TLorentzVector thep4Z1, TLorentzVector thep4M11, TLorentzVector thep4M12, TLorentzVector thep4Z2, TLorentzVector thep4M21, TLorentzVector thep4M22, double& costheta1, double& costheta2, double& Phi, double& costhetastar, double& Phi1){
  
        ///////////////////////////////////////////////
        // check for z1/z2 convention, redefine all 4 vectors with convention
        ///////////////////////////////////////////////	
    TLorentzVector p4H, p4Z1, p4M11, p4M12, p4Z2, p4M21, p4M22;
    p4H = thep4H;
        
    p4Z1 = thep4Z1; p4M11 = thep4M11; p4M12 = thep4M12;
    p4Z2 = thep4Z2; p4M21 = thep4M21; p4M22 = thep4M22;
        //// costhetastar
	TVector3 boostX = -(thep4H.BoostVector());
	TLorentzVector thep4Z1inXFrame( p4Z1 );
	TLorentzVector thep4Z2inXFrame( p4Z2 );	
	thep4Z1inXFrame.Boost( boostX );
	thep4Z2inXFrame.Boost( boostX );
	TVector3 theZ1X_p3 = TVector3( thep4Z1inXFrame.X(), thep4Z1inXFrame.Y(), thep4Z1inXFrame.Z() );
	TVector3 theZ2X_p3 = TVector3( thep4Z2inXFrame.X(), thep4Z2inXFrame.Y(), thep4Z2inXFrame.Z() );    
    costhetastar = theZ1X_p3.CosTheta();

        //// --------------------------- costheta1
    TVector3 boostV1 = -(thep4Z1.BoostVector());
    TLorentzVector p4M11_BV1( p4M11 );
	TLorentzVector p4M12_BV1( p4M12 );	
    TLorentzVector p4M21_BV1( p4M21 );
	TLorentzVector p4M22_BV1( p4M22 );
    p4M11_BV1.Boost( boostV1 );
	p4M12_BV1.Boost( boostV1 );
	p4M21_BV1.Boost( boostV1 );
	p4M22_BV1.Boost( boostV1 );
    
    TLorentzVector p4V2_BV1 = p4M21_BV1 + p4M22_BV1;
        //// costheta1
    costheta1 = -p4V2_BV1.Vect().Dot( p4M11_BV1.Vect() )/p4V2_BV1.Vect().Mag()/p4M11_BV1.Vect().Mag();

        //// --------------------------- costheta2
    TVector3 boostV2 = -(thep4Z2.BoostVector());
    TLorentzVector p4M11_BV2( p4M11 );
	TLorentzVector p4M12_BV2( p4M12 );	
    TLorentzVector p4M21_BV2( p4M21 );
	TLorentzVector p4M22_BV2( p4M22 );
    p4M11_BV2.Boost( boostV2 );
	p4M12_BV2.Boost( boostV2 );
	p4M21_BV2.Boost( boostV2 );
	p4M22_BV2.Boost( boostV2 );
    
    TLorentzVector p4V1_BV2 = p4M11_BV2 + p4M12_BV2;
        //// costheta2
    costheta2 = -p4V1_BV2.Vect().Dot( p4M21_BV2.Vect() )/p4V1_BV2.Vect().Mag()/p4M21_BV2.Vect().Mag();
    
        //// --------------------------- Phi and Phi1
//    TVector3 boostX = -(thep4H.BoostVector());
    TLorentzVector p4M11_BX( p4M11 );
	TLorentzVector p4M12_BX( p4M12 );	
    TLorentzVector p4M21_BX( p4M21 );
	TLorentzVector p4M22_BX( p4M22 );	
    
	p4M11_BX.Boost( boostX );
	p4M12_BX.Boost( boostX );
	p4M21_BX.Boost( boostX );
	p4M22_BX.Boost( boostX );
    
    TVector3 tmp1 = p4M11_BX.Vect().Cross( p4M12_BX.Vect() );
    TVector3 tmp2 = p4M21_BX.Vect().Cross( p4M22_BX.Vect() );    
    
    TVector3 normal1_BX( tmp1.X()/tmp1.Mag(), tmp1.Y()/tmp1.Mag(), tmp1.Z()/tmp1.Mag() ); 
    TVector3 normal2_BX( tmp2.X()/tmp2.Mag(), tmp2.Y()/tmp2.Mag(), tmp2.Z()/tmp2.Mag() ); 

        //// Phi
    TLorentzVector p4Z1_BX = p4M11_BX + p4M12_BX;    
    double tmpSgnPhi = p4Z1_BX.Vect().Dot( normal1_BX.Cross( normal2_BX) );
    double sgnPhi = tmpSgnPhi/fabs(tmpSgnPhi);
    Phi = sgnPhi * acos( -1.*normal1_BX.Dot( normal2_BX) );
    
    
        //////////////
    
    TVector3 beamAxis(0,0,1);
    TVector3 tmp3 = (p4M11_BX + p4M12_BX).Vect();
    
    TVector3 p3V1_BX( tmp3.X()/tmp3.Mag(), tmp3.Y()/tmp3.Mag(), tmp3.Z()/tmp3.Mag() );
    TVector3 tmp4 = beamAxis.Cross( p3V1_BX );
    TVector3 normalSC_BX( tmp4.X()/tmp4.Mag(), tmp4.Y()/tmp4.Mag(), tmp4.Z()/tmp4.Mag() );
        
        //// Phi1
    double tmpSgnPhi1 = p4Z1_BX.Vect().Dot( normal1_BX.Cross( normalSC_BX) );
    double sgnPhi1 = tmpSgnPhi1/fabs(tmpSgnPhi1);    
    Phi1 = sgnPhi1 * acos( normal1_BX.Dot( normalSC_BX) );    
    
//    std::cout << "extractAngles: " << std::endl;
//    std::cout << "costhetastar = " << costhetastar << ", costheta1 = " << costheta1 << ", costheta2 = " << costheta2 << std::endl;
//    std::cout << "Phi = " << Phi << ", Phi1 = " << Phi1 << std::endl;    

}
