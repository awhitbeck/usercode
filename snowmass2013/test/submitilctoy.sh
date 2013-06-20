#!/bin/bash

# Edit the template file submitPBSJobTemplate.pbs 
# make sure you edit 
#    1. output directory
#    2. the macro (.C) file with the correct argument
#--------tips------------------
# use showq -u username to check the status of the job
# use qpeek jobid to check the output before it finishes
#-------end of tips-------------
 
export NJOBS=$1
export NTOYSPERJOB=$2
export ISPURETOY=$3

if [ ! $# -eq 3 ]; then
        echo "USAGE: ./submitilctoy.sh NJOBS NTOYSPERJOB ISPURETOY
        NJOBS - number of jobs
        NTOYSPERJOB - number of toys per job 
        ISPURETOY - choose true for puretoy and false for embedded "
    exit 1
fi

i=0; 

while [ $i -lt $NJOBS ] ; 
  do qsub submitPBSJobTemplate.pbs -v "pureToys=$ISPURETOY, ntoysperjob=$NTOYSPERJOB, seed_index=$i"
  i=$[i+1];  
done
