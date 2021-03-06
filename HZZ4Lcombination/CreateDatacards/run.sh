#!/bin/sh

if [ $# -lt 2 ] 
    then 
  
    echo """this script expects 2 arguments
            model label
            model index: 0=0+/0-, 1=0+/2m+, 
                         2=0+/0h+, 3=0+/1+, 
                         4=0+/1- (default=-1, dummy, no effect)
         """
    exit
fi

python makeDCsandWSsSMD.py -i SM_inputs_7TeV_MorAppr -a $1_7TeV -b -t templates2D_$1_7TeV --spinHyp=$2 > $1_7TeV.txt

python makeDCsandWSsSMD.py -i SM_inputs_8TeV_MorAppr -a $1_8TeV -b -t templates2D_$1_8TeV --spinHyp=$2 > $1_8TeV.txt

cd cards_$1_8TeV/HCG/126/
cp ../../../cards_$1_7TeV/HCG/126/* .
cp ../../../../SignalSeparation/ex* .


combineCards.py hzz4l_2e2muS_7TeV_ALT.txt hzz4l_2e2muS_8TeV_ALT.txt hzz4l_4eS_7TeV_ALT.txt hzz4l_4eS_8TeV_ALT.txt hzz4l_4muS_7TeV_ALT.txt hzz4l_4muS_8TeV_ALT.txt > hzz4l_ALT.txt

combineCards.py hzz4l_2e2muS_7TeV.txt hzz4l_2e2muS_8TeV.txt hzz4l_4eS_7TeV.txt hzz4l_4eS_8TeV.txt hzz4l_4muS_7TeV.txt hzz4l_4muS_8TeV.txt > hzz4l.txt

./execute_SignalSeparationCombine.sh ./ hzz4l_ALT.txt 2
root -l -n -b -q "extractSignificanceStats.C+ (true)"

echo "============================="
echo "EXPECTED SIGNAL 0+ VS ${1}"
echo "============================="

combine -M ProfileLikelihood hzz4l.txt --signif -m 126 -t -1 --expectSignal=1

echo "============================="
echo "OBSERVED SIGNAL 0+ VS ${1}"
echo "============================="

combine -M ProfileLikelihood hzz4l.txt --signif -m 126