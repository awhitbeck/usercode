#! /bin/bash

list=""

#merge files for expected limits

for i in $(cat mass.txt);
  do

  if [ -a "higgsCombine${1}.MarkovChainMC.mH${i}.TOTAL.root" ] ; 
      then
      rm higgsCombine$1.MarkovChainMC.mH$i.TOTAL.root
      fi

      hadd higgsCombine$1.MarkovChainMC.mH$i.TOTAL.root `ls ${i}/higgsCombine${1}.MarkovChainMC.mH${i}.*.root`

      list=$list" higgsCombine${1}.MarkovChainMC.mH${i}.TOTAL.root"
 
  done

#merge files for observed limits

if [ -a "higgsCombine${1}.MarkovChainMC.OBS.TOTAL.root" ] ; 
    then
    rm higgsCombine$1.MarkovChainMC.OBS.TOTAL.root
    fi

obsFiles=`ls */higgsCombine${1}.MarkovChainMC.mH*.12345.root`
hadd higgsCombine$1.MarkovChainMC.OBS.TOTAL.root $obsFiles

echo $list

exit