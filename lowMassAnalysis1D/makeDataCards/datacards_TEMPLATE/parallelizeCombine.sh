#! /bin/bash

#=============================
# input arguments are:
# 1 - label (0btag, 1btag, 2btag, 6channels)
# 2 - number of jobs to submit
# 3 - number of toys per job
# 4 - mass
# 5 - batch queue (optional)
#=============================

doObs=false
algo="MarkovChainMC"
label=$1

if [ $doObs = "true" ]; 
    then
    NMAXJOBS=1
    NTOYS=1
    else
    NMAXJOBS=$2
    NTOYS=$3
    fi
mass=$4
queue="dummyqueue"
OUTDIR="./"

if [ $# -gt 4 ]
then
    queue=$5
else
    queue=8nh
fi
ijob=1
chmod 744 combine_exec.sh

while [ $ijob -le $NMAXJOBS ]
do

  myrand=$RANDOM #random number generator (short integer: [0-32767])
  datacardDir=`pwd`"/${mass}/"
  JOBNAME="combine_${myrand}"
  LOGFILE="log_batch_combine_${myrand}.out"
  bsub -q $queue -J $JOBNAME -oo ${OUTDIR}/${mass}/$LOGFILE combine_exec.sh $CMSSW_BASE $datacardDir "CMS_hzz2l2q_${mass}_${label}.txt" $doObs $algo $NTOYS $label $mass $myrand
  let ijob=ijob+1
done


