#! /bin/bash

DIR=$1

cd $DIR

#combine all cards:

combineCards.py CMS_hzz2l2q_ee0b=hzz2l2q_ee0b_7TeV.txt CMS_hzz2l2q_ee1b=hzz2l2q_ee1b_7TeV.txt CMS_hzz2l2q_ee2b=hzz2l2q_ee2b_7TeV.txt CMS_hzz2l2q_mm0b=hzz2l2q_mm0b_7TeV.txt CMS_hzz2l2q_mm1b=hzz2l2q_mm1b_7TeV.txt CMS_hzz2l2q_mm2b=hzz2l2q_mm2b_7TeV.txt > CMS_hzz2l2q_${DIR}_6channels.txt
 
#combine 0btag
 
combineCards.py CMS_hzz2l2q_ee0b=hzz2l2q_ee0b_7TeV.txt CMS_hzz2l2q_mm0b=hzz2l2q_mm0b_7TeV.txt > CMS_hzz2l2q_${DIR}_0btag.txt
 
#combine 1btag
 
combineCards.py CMS_hzz2l2q_ee1b=hzz2l2q_ee1b_7TeV.txt CMS_hzz2l2q_mm1b=hzz2l2q_mm1b_7TeV.txt > CMS_hzz2l2q_${DIR}_1btag.txt

#combine 2btag

combineCards.py CMS_hzz2l2q_ee2b=hzz2l2q_ee2b_7TeV.txt CMS_hzz2l2q_mm2b=hzz2l2q_mm2b_7TeV.txt > CMS_hzz2l2q_${DIR}_2btag.txt

cd -