scram p CMSSW CMSSW_5_2_5
cd CMSSW_5_2_5/src/
cmsenv
cvs co -r V00-03-01 -d Higgs/Higgs_CS_and_Width UserCode/Snowball/Higgs/Higgs_CS_and_Width 
cvs co -r bonato_supermela_20121107 -d HZZ4L_Combination/CombinationPy UserCode/HZZ4L_Combination/CombinationPy
cvs co -r V00-01-17 -d ZZMatrixElement/MELA UserCode/CJLST/ZZMatrixElement/MELA
cvs co -r V02-06-00 HiggsAnalysis/CombinedLimit
cvs co -d ZZMatrixElement/MEKD UserCode/UFL/ZZMatrixElementMEKD
cvs co -d ZZMatrixElement/MEMerger UserCode/Whitbeck/MEMerger

scram b  

#currently doesn't compile because of problem with MEKD code