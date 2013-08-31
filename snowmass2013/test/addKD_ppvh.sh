#!/bin/bash

export filedir=$1
export acceptance=$2

if [ ! $# -eq 2 ]; then
    echo "USAGE: ./addKD_ppzh.sh filedir acceptance
       filedir  - the directory containing the input ntuples, e.g. samples/pp_ZH
       acceptance - choose from false and true" 
    exit 1
fi

rm -f list_samples.txt;
cat > list_samples.txt <<EOF
pp_ZH_llbb_g1_1M
pp_ZH_llbb_g4_1M
pp_ZH_llbb_g1_p_g4_1M
pp_ZH_llbb_g1_p_ig4_1M
pp_ZH_llbb_0p_toydata
pp_ZH_llbb_0m_toydata
pp_ZH_llbb_0mix_toydata
pp_ZH_llbb_0mix_f3p5_toydata
pp_ZH_llbb_0mix_f3p5_phi3piover2_toydata
pp_ZZ_llbb_25M
EOF


for i in `cat list_samples.txt` ; do
    sample=${i%%,*}_${acceptance};
    root -l -q -n loadLib.C addKDPPVH.C\(\"$filedir\",\"$sample\",$acceptance\)
    mv $filedir/${sample}_withKD.root $filedir/$sample.root
done
