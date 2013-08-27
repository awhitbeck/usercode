#!/bin/bash

export filedir=$1
export acceptance=$2
export sqrts=$3

if [ ! $# -eq 3 ]; then
    echo "USAGE: ./addKD_eezh.sh filedir acceptance sqrtsVal
       filedir  - the directory containing the input ntuples, e.g. samples/ee_ZH
       acceptance - choose from false and true
       sqrts - central masss choose from 250, 350, 500, 1000"
    exit 1
fi

rm -f list_samples.txt;    

# file for the 250 GeV analysis
if [ "$sqrts" -eq "250" ]; then
cat > list_samples.txt <<EOF
unweighted_unpol_g1_1M
unweighted_unpol_g4_1M
unweighted_unpol_f_3_500GeV_5M
ee_ZZ_llbb_250GeV_25M
EOF
fi

if [ "$sqrts" -eq "350" ]; then
cat > list_samples.txt <<EOF
unweighted_unpol_g1_350GeV_2M
unweighted_unpol_g4_350GeV_2M
unweighted_unpol_model8_2M
ee_ZZ_llbb_350GeV_25M
EOF
fi

if [ "$sqrts" -eq "500" ]; then
cat > list_samples.txt <<EOF
unweighted_unpol_g1_500GeV_2M
unweighted_unpol_g4_500GeV_2M
unweighted_unpol_f_3_500GeV_5M
ee_ZZ_llbb_500GeV_25M
EOF
fi

if [ "$sqrts" -eq "1000" ]; then
cat > list_samples.txt <<EOF
unweighted_unpol_g1_1TeV_1M
unweighted_unpol_g4_1TeV_1M
unweighted_unpol_model9_1M
ee_ZZ_llbb_1000GeV_25M
EOF
fi


for i in `cat list_samples.txt` ; do
    sample=${i%%,*}_${acceptance};
    echo "Doing root -l -q -n loadLib.C addKD.C\(\"$filedir\",\"$sample\",$sqrts\)"
    root -l -q -n loadLib.C addKD.C\(\"$filedir\",\"$sample\",$sqrts\)
    mv $filedir/${sample}_withKD.root $filedir/$sample.root
done


