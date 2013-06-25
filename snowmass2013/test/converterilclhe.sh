#!/bin/bash

# without selections, run 
# ./converterilclhe.sh event.lhe
# with selections, run
# ./converterilclhe.sh event.lhe true 
inputLHEfile=$1

if [ $# == 1 ] 
    then 
    applyAcc="false"
    elif [ $# == 2 ] 
    then
    applyAcc=$2
    fi

outputTXT="${inputLHEfile%.*}_${applyAcc}"
echo "Input LHE file: "$inputLHEfile
echo "Processed text file: "$outputTXT".txt"

awk '/<event>/,/LesHouchesEvents>/' $inputLHEfile | grep -iv "<event>" | grep -iv "</event>" | grep -iv "</LesHouchesEvents>" | grep -iv " -11   -1"  | grep -iv "11   -1" | grep -iv "23    2" | grep -iv "25    2" | grep -iv "23    0" | grep -iv "25    0" > $outputTXT".txt"
export DEBUG=false
root -l -n -q "readOutAngles_ILC.C+(\"${outputTXT}\",$applyAcc,$DEBUG)"


echo "done"


