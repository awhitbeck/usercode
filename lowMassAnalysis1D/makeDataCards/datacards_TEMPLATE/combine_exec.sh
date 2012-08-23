#! /bin/bash

#============ Get Variables ==========

cmsswDir=$1
datacardDir=$2
datacard=$3
doObs=$4
algo=$5
nToys=$6
label=$7
mass=$8
seed=$9

echo "Hi I will be in charge of your job today . . . "
echo "-----------------------------------------------"
echo "cmsswDir:    "$cmsswDir
echo "datacardDir: "$datacardDir
echo "datacard:    "$datacard
echo "doObs:       "$doObs 
echo "algo:        "$algo 
echo "nToys:       "$nToys
echo "label:       "$label
echo "mass:        "$mass
echo "seed:        "$seed 
echo "================================================"

#============= Set CMSSW =============

echo "Setting up your CMSSW environment . . . "

batchDir=`pwd`

export SCRAM_ARCH=slc5_amd64_gcc434
cd $cmsswDir
eval `scramv1 runtime -sh`

echo "Double checking CMSSW_BASE:"
echo $CMSSW_BASE

#============= Set options ============

OPTIONS="-M ${algo} -m ${mass} -n ${label} -d ${datacard} -H ProfileLikelihood "

if [ $doObs = "true" ]; 
    then
    OPTIONS="${OPTIONS} -s 12345"
    else
    OPTIONS="${OPTIONS} -s ${seed} -t ${nToys}"
    fi

#============== RUN !!! ================

echo "About to run:" 
echo "combine ${OPTIONS}"
echo "Here we go . . . "

cd $datacardDir

combine $OPTIONS

echo "Done!"


exit
