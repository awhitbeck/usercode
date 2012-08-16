if [ $# != 3 ] 
    then
    echo "You didn't provide enough inputs"
    echo "1: model (./JHUgen_$model ... etc )"
    echo "2: spin"  
    echo "3: number of parallel jobs"
    exit
    fi

model=$1
spin=$2
njobs=$3

i=0

while [ $[$i] -lt $njobs ]
    do 
    qsub submitPBSJob_JHUGen.pbs -v "var1=${model}, var2=${spin}, var3=1000000, var4=${model}_${i}"

    i=$[$i+1]
    done

