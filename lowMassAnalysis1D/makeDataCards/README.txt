############################################
####   Rudimentary instructions 
####   for generating datacards 
####   for low mass HZZ2l2q analysis
############################################   

Installation:

#=========================

cmsrel CMSSW_5_2_4
cd CMSSW_5_2_4/src
cmsenv
cvs co -r V02-02-03 HiggsAnalysis/CombinedLimit
scram b -j4

cd HiggsAnalysis/CombinedLimit/test

cvs co -d makeDataCards UserCode/Whitbeck/lowMassAnalysis1D/makeDataCards

#=========================

This code is a modification of the high mass anlaysis scripts.  
The mechanics are very much the same.  To run you will need to 
first set up a working directory:

cp -r datacards_TEMPLATE/ MYdirNAME

Then you'll need to edit one line in both make_datacards.C and 
make_roofitfiles.C:

line 91 in make_datacards.C:
  string outDir="MYdirNAME/";	

line 247 in make_roofitfile.C
  string outFileName="MYdirNAME/"+str_massH+"/hzz2l2q_"+str_chan+str_btag+".input.root";

You'll also need to edit line 249 and 250 to point to your local PDF 
directory and the corresponding include directory for the roofit version
you would like to use.  

There are several configurations variables which you can use to make your
datacards.  All of them can be found at the top of make_datacards.C
The most relevant ones are:

const float lumiee=4.86; //fb^-1, NEW!
const float lumimm=4.88; //fb^-1, NEW!

const bool isSM4=true;  // if true add .15 to CSgg and CSvbf errors 
const bool isXSBr=false; // if true don't write signal yield errors from theory/pdf's 
const bool isFF=false;    // if true use fermiophobic text file, don't use gg->H channel, add 5% to scale errors.  

From here you should be all set.  Just do the following to run the code:

root -l -n -b -q generateDataCards.C

======================
Refer questions to either
----------------------
whitbeck.andrew@gmail.com
sara.bolognesi@cern.ch
----------------------

