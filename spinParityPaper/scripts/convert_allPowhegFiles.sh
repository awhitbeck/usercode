#!/bin/sh


for i in $(ls PowhegFiles/4GeVcutPowheg/pwgevents*.lhe)
    do

    qsub submitPBS_converter_Powheg.pbs -v "var1=${i}, var2=true"
    #qsub submitPBS_converter_Powheg.pbs -v "var1=${i}, var2=false"

    done
