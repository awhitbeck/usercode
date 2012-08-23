#!/bin/sh

LIST=""
OUTPUTfile="TotalAsymptoticLimits.root"
for i in $(cat mass.txt); do

    LIST=$LIST"${i}/higgsCombine6channels.Asymptotic.mH${i}.root " 

done

if [ -e $OUTPUTfile ]; then
    rm $OUTPUTfile
fi

hadd $OUTPUTfile $LIST

