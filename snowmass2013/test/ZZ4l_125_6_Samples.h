#include <iostream>
#include <string>
#include "TMath.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TCanvas.h"

const float PI_VAL = TMath::Pi();

enum sample {
	kfg2_0_fg4_0,
	kfg2_1_fg4_0,
	kfg2_0_fg4_1,
	kfg2_05_fg4_0,
	kfg2_0_fg4_05,
	kfg2_05_fg4_05,
	kfg2_0333_fg4_0333,

	kfg2_01_fg4_0,
	kfg2_0_fg4_01,
	kfg2_01_fg4_01,

	kfg2_05_fg4_0_p290,
	kfg2_0_fg4_05_p390,
	kfg2_05_fg4_05_p390,

	kfg2_01_fg4_0_p290,
	kfg2_0_fg4_01_p390,
	kfg2_01_fg4_01_p390,

	kfg2_0333_fg4_0333_p290,
	kfg2_0333_fg4_0333_p390,

	kfg2_018_fg4_0,
	kfg2_0_fg4_018,
	kfg2_018_fg4_018,

	kfepspr_1,
	kfepspr_m05,
	kfepspr_05,
	kfg3_1,

	kNumSamples
};
string sampleName[kNumSamples] = {
	"fg200_fg400",
	"fg210_fg400",
	"fg200_fg410",
	"fg205_fg400",
	"fg200_fg405",
	"fg205_fg405",
	"fg20333_fg40333",

	"fg201_fg400","fg200_fg401","fg201_fg401",

	"fg205_fg400_p290","fg200_fg405_p390","fg205_fg405_p390",

	"fg201_fg400_p290","fg200_fg401_p390","fg201_fg401_p390",
	"fg233_fg433_p290","fg233_fg433_p390",

	"fg218_fg400","fg200_fg418","fg218_fg418",

	"fepspr10",
	"fepspr-05",
	"fepspr05",
	"fg310"
};
char* sample_suffix[kNumSamples]={
	"0+m","0+h","0-","0+m0+h","0+m0-","0+h0-","0+m0+h0-",

	"fg201_fg400","fg200_fg401","fg201_fg401",

	"fg205_fg400_p290","fg200_fg405_p390","fg205_fg405_p390",

	"fg201_fg400_p290","fg200_fg401_p390","fg201_fg401_p390",
	"fg233_fg433_p290","fg233_fg433_p390",

	"fg218_fg400","fg200_fg418","fg218_fg418",

	"fepspr10",
	"fepspr-05",
	"fepspr05",
	"fg310"
};

double sample_xsec_ratio[kNumSamples]={
	1.0,	0.3726,	0.1573,	3.660,	2.0,	0.3146,	4.659,

	1.664,	1.111,	1.837,

	3.660,	2.0,	0.3146, // restart templates, 0 is unknown

	1.664,	1.111,	1.837,
	4.659,	4.659,

	1.997,	1.219,	2.443,

	6.89150e-9, // g1=1, g1'=-1, lambda=10e4
	3.966863, // g1=12047.01, g1'=-12046.01, lambda=10e4.
	3.966863, // g1=-12045.01, g1'=12046.01, lambda=10e4.

	0.3726 // Needs revision
};
double epspr_mixedSample_g1pr=12046.01; // absolute value is enough.
double epspr_mixedSample_lambda1pr=10000.0;

const float gi_phi2_phi4[kNumSamples][9]={ // g1-4; phia2,3; fa2, 3; fepspr
	{	1.0,	0,	0,	0,		0,	0,	0,	0,	0},
	{	0,	1.0,	0,	0,		0,	0,	1.0,	0,	0},
	{	0,	0,	0,	1.0,		0,	0,	0,	1.0,	0},
	{	1.0,	1.638,	0,	0,		0,	0,	0.5,	0,	0},
	{	1.0,	0,	0,	2.521,		0,	0,	0,	0.5,	0},
	{	0,	0.650,	0,	1.0,		0,	0,	0.5,	0.5,	0},
	{	1.0,	1.638,	0,	2.521,		0,	0,	1.0/3.0,	1.0/3.0,	0},

	{	1.0,	0.546,	0,	0,		0,	0,	0.1,	0,	0},
	{	1.0,	0,	0,	0.840,		0,	0,	0,	0.1,	0},
	{	1.0,	0.579,	0,	0.891,		0,	0,	0.1,	0.1,	0},

	{	1.0,	1.638,	0,	0,		PI_VAL/2.0,	0,	0.5,	0,	0},
	{	1.0,	0,	0,	2.521,		0,	PI_VAL/2.0,	0,	0.5,	0},
	{	0,	0.650,	0,	1.0,		0,	PI_VAL/2.0,	0.5,	0.5,	0},
	{	1.0,	0.546,	0,	0,		PI_VAL/2.0,	0,	0.1,	0,	0},
	{	1.0,	0,	0,	0.840,		0,	PI_VAL/2.0,	0,	0.1,	0},
	{	1.0,	0.579,	0,	0.891,		0,	PI_VAL/2.0,	0.1,	0.1,	0},

	{	1.0,	1.638,	0,	2.521,		PI_VAL/2.0,	0,	1.0/3.0,	1.0/3.0,	0},
	{	1.0,	1.638,	0,	2.521,		0,	PI_VAL/2.0,	1.0/3.0,	1.0/3.0,	0},

	{	1.0,	0.7674,	0,	0,		0,	0,	0.18,	0,	0},
	{	1.0,	0,	0,	1.1812,		0,	0,	0,	0.18,	0},
	{	1.0,	0.8687,	0,	1.337,		0,	0,	0.18,	0.18,	0},

	{	1.0,	0,	0,	0,		0,	0,	0,	0,	1.0}, // g1pr=-1, g1=1
	{	1.0,	0,	0,	0,		0,	0,	0,	0,	-1.5999689689656635e-4}, // g1pr=-1, g1=2
	{	1.0,	0,	0,	0,		0,	0,	0,	0,	1.5999689689656635e-4}, // g1pr=1, g1=-2
	{	0,	0,	1.0,	0,		0,	0,	0,	0,	0}
};

double efficiency_Nevents[2][kNumSamples][4]={ // Energy, first 6 hypothesis, 2e2mu4e4mu all+selected
	{
		{3574920,	3925060,	2092440,	2312200},
		{3737290,	3662690,	2211210,	2164950},
		{3908570,	3591420,	2140210,	1944559},
		{3657490,	3842500,	2159430,	2277890},
		{3731980,	3768000,	2113580,	2138860},
		{3846400,	3653580,	2190580,	2072150},
		{3707920,	3792070,	2154750,	2211150},
		{1,	1,	1,	1},
		{1,	1,	1,	1},
		{1,	1,	1,	1},
		{3647970,	3752020,	2146950,	2213080},
		{3736110,	3763870,	2115170,	2134920},
		{3848520,	3651460,	2191430,	2071090},
		{1,	1,	1,	1},
		{1,	1,	1,	1},
		{1,	1,	1,	1},
		{3763490,	3736500,	2163840,	2149160},
		{3707740,	3792250,	2155830,	2209200},
		{3616760,	3883220,	2129570,	2298710},
		{3533780,	3766200,	2044220,	2190890},
		{3651240,	3848750,	2134720,	2259060},
		{3301330,	3398660,	1924630,	1983541},
		{3301330,	3398660,	1924630,	1983541},//to be checked
		{3301330,	3398660,	1924630,	1983541},//to be checked
		{3801230,	3698750,	2254690,	2191500}
	},
	{
		{3478870,	3821120,	1993250,	2201700},
		{3687230,	3612750,	2134160,	2090380},
		{3755560,	3444430,	2012070,	1826343},
		{3508090,	3691900,	2027650,	2138790},
		{3683560,	3716430,	2041890,	2063060},
		{3795760,	3604220,	2114640,	2000517},
		{3709950,	3790030,	2111350,	2161110},
		{1,	1,	1,	1},
		{1,	1,	1,	1},
		{1,	1,	1,	1},
		{3698430,	3801560,	2129210,	2194080},
		{3733600,	3766380,	2069610,	2091450},
		{3847170,	3652810,	2145970,	2029320},
		{1,	1,	1,	1},
		{1,	1,	1,	1},
		{1,	1,	1,	1},
		{3766150,	3733840,	2118220,	2101970},
		{3705580,	3794410,	2109850,	2162490},
		{3617760,	3882220,	2083450,	2248320},
		{3533460,	3766530,	1999170,	2143820},
		{3652950,	3847030,	2089870,	2207050},
		{3051490,	3148490,	1739230,	1799199},
		{3051490,	3148490,	1739230,	1799199},//to be checked
		{3051490,	3148490,	1739230,	1799199},//to be checked
		{3803410,	3696580,	2207190,	2145410}
	}
};


string user_dir="/afs/cern.ch/work/u/usarica/ggtoH-PWGSamples-125_6/";
