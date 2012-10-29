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

cvs co -d SignalSeparation UserCode/HZZ4L_Combination/CombinationPy/SignalSeparation

./buildPackage.sh

## to test results from Alessio, copy his SM inputs and templates:

scp -r lxplus.cern.ch:~bonato/public/cards_HypTest_Combine_HCPlumi_SMD_20121023b_GOOD/templates2D_smd_7TeV_20121023b_GOODvarbin .
scp -r lxplus.cern.ch:~bonato/public/cards_HypTest_Combine_HCPlumi_SMD_20121023b_GOOD/templates2D_smd_7TeV_20121023b_GOODvarbin .

scp -r lxplus.cern.ch:~bonato/public/SM_inputs_7TeV/ .
scp -r lxplus.cern.ch:~bonato/public/SM_inputs_8TeV/ .


## running standard limits and significance
## to run code:

python makeDCsandWSs.py -i <input directory> -a <output name> -b > output.txt

##############################
## hypothesis separation    ##
##############################

##############################
## modify input files so that 
##############################

channels all qqZZ ggZZ zjets

doHypTest True

#####################################
## this can be done automatically by:
#####################################

sed -i 's|ggH qqH WH ZH ttH|all|' SM_inputs_8TeV/inputs_4mu.txt
sed -i 's|ggH qqH WH ZH ttH|all|' SM_inputs_8TeV/inputs_4e.txt
sed -i 's|ggH qqH WH ZH ttH|all|' SM_inputs_8TeV/inputs_2e2mu.txt
sed -i 's|ggH qqH WH ZH ttH|all|' SM_inputs_7TeV/inputs_4mu.txt
sed -i 's|ggH qqH WH ZH ttH|all|' SM_inputs_7TeV/inputs_4e.txt
sed -i 's|ggH qqH WH ZH ttH|all|' SM_inputs_7TeV/inputs_2e2mu.txt

sed -i 's|doHypTest False|doHypTest True|' SM_inputs_8TeV/inputs_4mu.txt
sed -i 's|doHypTest False|doHypTest True|' SM_inputs_8TeV/inputs_4e.txt
sed -i 's|doHypTest False|doHypTest True|' SM_inputs_8TeV/inputs_2e2mu.txt
sed -i 's|doHypTest False|doHypTest True|' SM_inputs_7TeV/inputs_4mu.txt
sed -i 's|doHypTest False|doHypTest True|' SM_inputs_7TeV/inputs_4e.txt
sed -i 's|doHypTest False|doHypTest True|' SM_inputs_7TeV/inputs_2e2mu.txt

############################
## generate cards
############################

python makeDCsandWSs.py -i SM_inputs_8TeV/ -a test_alessio_8TeV -b -t templates2D_smd_8TeV_20121023b_GOODvarbin/ -d 3 > test_alessio_8TeV.txt

python makeDCsandWSs.py -i SM_inputs_7TeV/ -a test_alessio_7TeV -b -t templates2D_smd_7TeV_20121023b_GOODvarbin/ -d 3 > test_alessio_7TeV.txt

#############################
## run hypothesis sep
#############################

cp cards_test_alessio_7TeV/HCG/125/* cards_test_alessio_8TeV/HCG/125/.
cd cards_test_alessio_8TeV/HCG/125/
combineCards.py hzz4l_2e2muS_8TeV_ALT.txt hzz4l_4eS_8TeV_ALT.txt hzz4l_4muS_8TeV_ALT.txt > hzz4l_4lS_8TeV_ALT.txt
combineCards.py hzz4l_2e2muS_7TeV_ALT.txt hzz4l_4eS_7TeV_ALT.txt hzz4l_4muS_7TeV_ALT.txt > hzz4l_4lS_7TeV_ALT.txt
combineCards.py hzz4l_2e2muS_8TeV_ALT.txt hzz4l_4eS_8TeV_ALT.txt hzz4l_4muS_8TeV_ALT.txt hzz4l_2e2muS_7TeV_ALT.txt hzz4l_4eS_7TeV_ALT.txt hzz4l_4muS_7TeV_ALT.txt > hzz4l_4lS_ALT.txt

cp ../../../SignalSeparation/execute_SignalSeparationCombine.sh .
cp ../../../SignalSeparation/extractSignificanceStats.C .	

## !!!! note the default in execute_SignalSeparationCombine.sh is mH = 126 !!!!

./execute_SignalSeparationCombine.sh ./ hzz4l_4lS_ALT.txt 2
root -l -n -b extractSignificanceStats.C+
