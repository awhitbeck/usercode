#! /bin/bash

### for M in $( cat masses_short.txt ); do ./mergeCombinationFiles.sh $M ; done

LISTFILES=""
mass=$1
DIR="./${mass}/combine_2l2q_MarkovChainMC_CMS_hzz2l2q_${mass}_6channels"
STEM="higgsCombine2l2q.MarkovChainMC.mH${mass}."

for file in $( ls ${DIR}/${STEM}[0-9]*  )
do
LISTFILES=${LISTFILES}" $file "
done

echo "Merging: $LISTFILES"
hadd -f $DIR/$STEM"TOTAL.root" $LISTFILES