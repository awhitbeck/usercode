#!/bin/sh

WORKINGdir=`pwd`

list0btag=""
list1btag=""
list2btag=""
list6channels=""

for i in $(cat mass.txt); do

cd $i

combine -M Asymptotic CMS_hzz2l2q_${i}_6channels.txt -n 6channels -m $i

combine -M Asymptotic CMS_hzz2l2q_${i}_0btag.txt -n 0btag -m $i
combine -M Asymptotic CMS_hzz2l2q_${i}_1btag.txt -n 1btag -m $i
combine -M Asymptotic CMS_hzz2l2q_${i}_2btag.txt -n 2btag -m $i

list0btag=$list0btag" ${i}/higgsCombine0btag.Asymptotic.mH${i}.root"
list1btag=$list1btag" ${i}/higgsCombine1btag.Asymptotic.mH${i}.root"
list2btag=$list2btag" ${i}/higgsCombine2btag.Asymptotic.mH${i}.root"
list6channels=$list6channels" ${i}/higgsCombine6channels.Asymptotic.mH${i}.root"

cd $WORKINGdir

done

hadd -f higgsCombine0btag.Asymptotic.TOTAL.root $list0btag
hadd -f higgsCombine1btag.Asymptotic.TOTAL.root $list1btag
hadd -f higgsCombine2btag.Asymptotic.TOTAL.root $list2btag

hadd -f higgsCombine6channels.Asymptotic.TOTAL.root $list6channels

exit
