#!/bin/sh


for i in $(ls PowhegFiles/pwgevents*wResolution.root | awk -F ".root" '{print $1}')
  do

  #root -l -n -b -q "run_addDtoTree.C (\"${i}\")"
  qsub submitPBS_addDtoTree.pbs -v "var1=${i}"
  
done
