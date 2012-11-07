## instructions for checking out and install code:
##################################################

scram p CMSSW CMSSW_5_2_5
cd CMSSW_5_2_5/src
cmsenv

## Higgs combination
cvs co HiggsAnalysis/CombinedLimit
scram b -j4

cvs co -A -d HZZ4Lcombination/CreateDatacards UserCode/Whitbeck/HZZ4Lcombination/CreateDatacards
cd HZZ4Lcombination/CreateDatacards
cvs co -d SM_inputs_7TeV -r bonato_supermela_20121107 UserCode/HZZ4L_Combination/CombinationPy/CreateDatacards/SM_inputs_7TeV
cvs co -d SM_inputs_8TeV -r bonato_supermela_20121107 UserCode/HZZ4L_Combination/CombinationPy/CreateDatacards/SM_inputs_8TeV
cvs co -d include -r superMELAresultsHCP UserCode/HZZ4L_Combination/CombinationPy/CreateDatacards/include
cvs co -d templates2D -r superMELAresultsHCP UserCode/HZZ4L_Combination/CombinationPy/CreateDatacards/templates2D
cvs co -d CMSdata -r superMELAresultsHCP UserCode/HZZ4L_Combination/CombinationPy/CreateDatacards/CMSdata

cvs co -d SignalSeparation UserCode/HZZ4L_Combination/CombinationPy/SignalSeparation

./buildPackage.sh

#####################################################################
## check signal significance as in standard analysis (mela vs m4l) ##
#####################################################################

## reproduce the HCP pvalues for 125

python makeDCsandWSs.py -i SM_inputs_8TeV/ -a test_pvals_8TeV -b > test_pvals_8TeV.txt
python makeDCsandWSs.py -i SM_inputs_7TeV/ -a test_pvals_7TeV -b > test_pvals_7TeV.txt

cp cards_test_pvals_7TeV/HCG/125/* cards_test_pvals_8TeV/HCG/125/.

cd cards_test_pvals_8TeV/HCG/125/

combineCards.py hzz4l_2e2muS_8TeV.txt hzz4l_4muS_8TeV.txt hzz4l_4eS_8TeV.txt hzz4l_2e2muS_7TeV.txt hzz4l_4muS_7TeV.txt hzz4l_4eS_7TeV.txt > hzz4l.txt

#Expected
combine -M ProfileLikelihood hzz4l_4lS.txt -m 125 --signif -t -1 --expectSignal=1
#Observed
combine -M ProfileLikelihood hzz4l_4lS.txt -m 125 --signif


#########################################################################################
## hypothesis separation and signal singificance with supermela vs pseudo/gravi mela   ##
#########################################################################################

## From HZZ4Lcombination/CreateDatacards/

scp -r hep.pha.jhu.edu:/scratch0/hep/whitbeck/4lHelicity/HCPanalysis/superMELA_withInterference/CMSSW_5_2_5/src/HiggsAnalysis/HZZ4L_CombinationPy/CreateDatacards/templates2D_8TeV_pseudoMELA .
scp -r hep.pha.jhu.edu:/scratch0/hep/whitbeck/4lHelicity/HCPanalysis/superMELA_withInterference/CMSSW_5_2_5/src/HiggsAnalysis/HZZ4L_CombinationPy/CreateDatacards/templates2D_7TeV_pseudoMELA .
scp -r lxplus.cern.ch:/afs/cern.ch/user/b/bonato/public/cards_HypTest_Combine_HCPlumi_SMD_20121029/CMSdata .

## modify input files so that 

channels all qqZZ ggZZ zjets

doHypTest True

## this can be done automatically inside SM_inputs* by:
cp -r SM_inputs_7TeV SM_inputs_7TeV_hyptest
cd SM_inputs_7TeV_hyptest
sed -i 's|systematic CMS_zz4l_mean True|systematic CMS_zz4l_mean False|' inputs_*.txt    
sed -i 's|systematic CMS_zz4l_sigma True|systematic CMS_zz4l_sigma False|' inputs_*.txt    
sed -i 's|systematic CMS_zz4l_n True|systematic CMS_zz4l_n False|' inputs_*.txt    
sed -i 's|systematic CMS_zz4l_gamma True|systematic CMS_zz4l_gamma False|' inputs_*.txt    

sed -i 's|ggH qqH WH ZH ttH|all|' inputs_*.txt
sed -i 's|doHypTest False|doHypTest True|' inputs_*.txt
## To test spin2 hyp:
sed -i 's|#altHypLabel _ALT|altHypLabel gravi|' inputs_*.txt
## or to test 0- hyp:
sed -i 's|#altHypLabel _ALT|altHypLabel pseudo|' inputs_*.txt

cd ..
cp -r SM_inputs_8TeV SM_inputs_8TeV_hyptest
cd SM_inputs_8TeV_hyptest
## and redo all the sed commands as above

cd ..

##########################################################
## generate cards and run signal/background significance 
##########################################################

python makeDCsandWSs.py -i SM_inputs_8TeV_hyptest/ -a test_HypSep_8TeV -b -t templates2D_8TeV_pseudoMELA/ -d 3 > test_HypSep_8TeV.txt

python makeDCsandWSs.py -i SM_inputs_7TeV_hyptest/ -a test_HypSep_7TeV -b -t templates2D_7TeV_pseudoMELA/ -d 3 > test_HypSep_7TeV.txt

cp cards_test_HypSep_7TeV/HCG/125/* cards_test_HypSep_8TeV/HCG/125/.
cd cards_test_HypSep_8TeV/HCG/125/
combineCards.py hzz4l_2e2muS_8TeV.txt hzz4l_4eS_8TeV.txt hzz4l_4muS_8TeV.txt hzz4l_2e2muS_7TeV.txt hzz4l_4eS_7TeV.txt hzz4l_4muS_7TeV.txt > hzz4l_4lS.txt

#Expected
combine -M ProfileLikelihood hzz4l_4lS.txt -m 125 --signif -t -1 --expectSignal=1
#Observed
combine -M ProfileLikelihood hzz4l_4lS.txt -m 125 --signif

#############################
## run hypothesis sep
#############################

cp cards_test_HypSep_7TeV/HCG/125/* cards_test_HypSep_8TeV/HCG/125/.
cd cards_test_HypSep_8TeV/HCG/125/
combineCards.py hzz4l_2e2muS_8TeV_ALT.txt hzz4l_4eS_8TeV_ALT.txt hzz4l_4muS_8TeV_ALT.txt hzz4l_2e2muS_7TeV_ALT.txt hzz4l_4eS_7TeV_ALT.txt hzz4l_4muS_7TeV_ALT.txt > hzz4l_4lS_ALT.txt

cp ../../../SignalSeparation/execute_SignalSeparationCombine.sh .
cp ../../../SignalSeparation/extractSignificanceStats.C .	

## !!!! note the default in execute_SignalSeparationCombine.sh is mH = 126 !!!!

./execute_SignalSeparationCombine.sh ./ hzz4l_4lS_ALT.txt 2
root -l -n -b extractSignificanceStats.C+
