#!/bin/bash

f=$1

if [ $# == 1 ] 
    then 
    applySmear="false"
    elif [ $# == 2 ] 
    then
    applySmear=$2
    fi

fr="${f%.*}"
ff="${f%.*}_${applySmear}.txt"
echo $f, $ff

awk '/<event>/,/LesHouchesEvents>/' $f | grep -iv "<event>" | grep -iv "</event>" | grep -iv "</LesHouchesEvents>" | grep -iv " 8   1" | grep -iv " -11   -1"  | grep -iv "11   -1" | grep -iv "23    2" | grep -iv "25    2" > $ff

frt='"'"${ff%.txt}"'"'
echo $frt

root -l -q -b "readOutAngles_ILC.C(${frt},false)"


echo "done"


