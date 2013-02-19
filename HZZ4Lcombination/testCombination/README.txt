scram p CMSSW_6_1_1
cd CMSSW_6_1_1/src
cmsenv

cvs co HiggsAnalysis/CombinedLimit
cvs co -d HiggsAnalysis/HZZ4lCombination UserCode/Whitbeck/HZZ4Lcombination/testCombination

scram b -j4