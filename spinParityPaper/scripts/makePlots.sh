#!/bin/sh

if [ $1 == "roc" ] ; then 
    
    root -l -n -b -q " melaROCcurve.C(\"pseudomelaLD\",\"psScalar\",\"pseudo-MELA\") "
    root -l -n -b -q " melaROCcurve.C(\"0hPlusmelaLD\",\"0hPlus\",\"0hPlus-MELA\") "
    root -l -n -b -q " melaROCcurve.C(\"vectormelaLD\",\"vector\",\"vector-MELA\") "
    root -l -n -b -q " melaROCcurve.C(\"psVectormelaLD\",\"psVector\",\"psVector-MELA\") "
    root -l -n -b -q " melaROCcurve.C(\"gravimelaLD\",\"minGrav\",\"gravi-MELA\") "
    root -l -n -b -q " melaROCcurve.C(\"2hPlusmelaLD\",\"2hPlus\",\"2hPlus-MELA\") "
    root -l -n -b -q " melaROCcurve.C(\"2hMinusmelaLD\",\"2hMinus\",\"2hMinus-MELA\") " 

fi


if [ $1 == "kin" ]
    then

    root -l -n -b -q "melaDistributions.C (0)"
    root -l -n -b -q "melaDistributions.C (1)"
    root -l -n -b -q "melaDistributions.C (2)"
    root -l -n -b -q "melaDistributions.C (3)"
    root -l -n -b -q "melaDistributions.C (4)"
    root -l -n -b -q "melaDistributions.C (5)"
    root -l -n -b -q "melaDistributions.C (6)"
    root -l -n -b -q "melaDistributions.C (7)"

    root -l -n -b -q "melaDistributions_2D.C (0)"
    root -l -n -b -q "melaDistributions_2D.C (1)"
    root -l -n -b -q "melaDistributions_2D.C (2)"
    root -l -n -b -q "melaDistributions_2D.C (3)"
    root -l -n -b -q "melaDistributions_2D.C (4)"
    root -l -n -b -q "melaDistributions_2D.C (5)"
    root -l -n -b -q "melaDistributions_2D.C (6)"

    fi

if [ $1 == "hypsep" ]
    then

    root -l -n -b -q "drawhypsep.C (\"pseudomelaLD\",22,100,30)"
    root -l -n -b -q "drawhypsep.C (\"gravimelaLD\",22,100,10)"
    root -l -n -b -q "drawhypsep.C (\"vectormelaLD\",22,100,30)"
    root -l -n -b -q "drawhypsep.C (\"psVectormelaLD\",22,100,30)"
    root -l -n -b -q "drawhypsep.C (\"2hPlusmelaLD\",22,100,100)"
    root -l -n -b -q "drawhypsep.C (\"2hMinusmelaLD\",22,100,100)"
    root -l -n -b -q "drawhypsep.C (\"0hPlusmelaLD\",22,100,30)" 

    fi

if [ $1 == "toys" ]
    then

    n=0 
    if [ $# -gt 1 ] 
	then 
	n=$2
    fi
	while [ $[$n] -ge 0 ] ; do 
	  
	  qsub submitPBS_runToys.pbs -v "var1=0, var2=10."
	  qsub submitPBS_runToys.pbs -v "var1=1, var2=10."
	  #qsub submitPBS_runToys.pbs -v "var1=2, var2=22."
	  #qsub submitPBS_runToys.pbs -v "var1=3, var2=22."
	  #qsub submitPBS_runToys.pbs -v "var1=4, var2=10."
	  #qsub submitPBS_runToys.pbs -v "var1=5, var2=10."
	  #qsub submitPBS_runToys.pbs -v "var1=6, var2=22."


	  n=$[$n-1]

	done

    fi


if [ $1 == "-1" ] 
    then

    root -l -n -b -q "angularDistributions_bkg.C (0,50)"
    root -l -n -b -q "angularDistributions_bkg.C (1,65)"
    root -l -n -b -q "angularDistributions_bkg.C (2,20)"
    root -l -n -b -q "angularDistributions_bkg.C (3,20)"
    root -l -n -b -q "angularDistributions_bkg.C (4,20)"
    root -l -n -b -q "angularDistributions_bkg.C (5,20)"
    root -l -n -b -q "angularDistributions_bkg.C (6,20)"
    
    fi

if [ $1 == "0" ] 
    then

    root -l -n -b -q "angularDistributions_spin0.C (0,50)"
    root -l -n -b -q "angularDistributions_spin0.C (1,50)"
    root -l -n -b -q "angularDistributions_spin0.C (2,20)"
    root -l -n -b -q "angularDistributions_spin0.C (3,20)"
    root -l -n -b -q "angularDistributions_spin0.C (4,20)"
    root -l -n -b -q "angularDistributions_spin0.C (5,20)"
    root -l -n -b -q "angularDistributions_spin0.C (6,20)"
    
    fi

if [ $1 == "1" ] 
    then

    root -l -n -b -q "angularDistributions_spin1.C (0,50)"
    root -l -n -b -q "angularDistributions_spin1.C (1,50)"
    root -l -n -b -q "angularDistributions_spin1.C (2,20)"
    root -l -n -b -q "angularDistributions_spin1.C (3,20)"
    root -l -n -b -q "angularDistributions_spin1.C (4,20)"
    root -l -n -b -q "angularDistributions_spin1.C (5,20)"
    root -l -n -b -q "angularDistributions_spin1.C (6,20)"
    
    fi


if [ $1 == "2" ] 
    then

    root -l -n -b -q "angularDistributions_spin2.C (0,50)"
    root -l -n -b -q "angularDistributions_spin2.C (1,50)"
    root -l -n -b -q "angularDistributions_spin2.C (2,20)"
    root -l -n -b -q "angularDistributions_spin2.C (3,20)"
    root -l -n -b -q "angularDistributions_spin2.C (4,20)"
    root -l -n -b -q "angularDistributions_spin2.C (5,20)"
    root -l -n -b -q "angularDistributions_spin2.C (6,20)"

    fi


