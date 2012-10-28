## instructions for checking out and install code:
##################################################

scram p CMSSW CMSSW_5_2_5
cd CMSSW_5_2_5/src
cmsenv

## Higgs combination
cvs co HiggsAnalysis/CombinedLimit
scram b -j4

cvs co -d HZZ4Lcombination/CreateDatacards UserCode/Whitbeck/HZZ4Lcombination/CreateDatacards
cd HZZ4Lcombination/CreateDatacards
cvs co -d SM_inputs_7TeV UserCode/HZZ4L_Combination/CombinationPy/CreateDatacards/SM_inputs_7TeV
cvs co -d SM_inputs_8TeV UserCode/HZZ4L_Combination/CombinationPy/CreateDatacards/SM_inputs_8TeV
cvs co -d include UserCode/HZZ4L_Combination/CombinationPy/CreateDatacards/include
cvs co -d templates2D UserCode/HZZ4L_Combination/CombinationPy/CreateDatacards/templates2D
cvs co -d CMSdata UserCode/HZZ4L_Combination/CombinationPy/CreateDatacards/CMSdata

./buildPackage.sh

## to run code:

python makeDCsandWSs.py -i <input directory> -a <output name> -b > output.txt