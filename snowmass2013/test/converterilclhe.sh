#!/bin/bash

inputLHEfile=$1

if [ $# == 1 ] 
    then 
    applySmear="false"
    elif [ $# == 2 ] 
    then
    applySmear=$2
    fi

outputTXT="${inputLHEfile%.*}_${applySmear}"
echo "Input LHE file: "$inputLHEfile
echo "Processed text file: "$outputTXT".txt"

awk '/<event>/,/LesHouchesEvents>/' $inputLHEfile | grep -iv "<event>" | grep -iv "</event>" | grep -iv "</LesHouchesEvents>" | grep -iv " -11   -1"  | grep -iv "11   -1" | grep -iv "23    2" | grep -iv "25    2" | grep -iv "23    0" | grep -iv "25    0" > $outputTXT".txt"

root -l -n -q "readOutAngles_ILC.C+(\"${outputTXT}\",false)"


echo "done"


