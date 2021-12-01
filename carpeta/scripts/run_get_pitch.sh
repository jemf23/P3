#!/bin/bash
if [ "$#" -eq 0 ]
then
GETF0="get_pitch"

for fwav in pitch_db/train/*.wav; do
    ff0=${fwav/.wav/.f0}
    echo "$GETF0 $fwav $ff0 ----"
    $GETF0 $fwav $ff0 > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
done
pitch_evaluate pitch_db/train/*f0ref | fgrep TOTAL
fi

# Put here the program (maybe with path)
if [ "$#" -eq 4 ]
then
    parameter1=$1
    parameter2=$2
    parameter3=$3
    parameter4=$4
    GETF0="get_pitch"

    for fwav in pitch_db/train/*.wav; do
        ff0=${fwav/.wav/.f0}
        param="-1 $parameter1 -2 $parameter2 -3 $parameter3 -4 $parameter4"
        #echo "$GETF0 $fwav $ff0 --thr1norm=$parameter1 --thrmaxnorm=$parameter2 --thpower=$parameter3 ----"
        #$GETF0 $fwav $ff0 "--thr1norm $parameter1 --thrmaxnorm $parameter2 --thpower $parameter3" > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
        $GETF0 $fwav $ff0 $param > /dev/null || (echo "Error in $GETF0 $fwav $ff0 $param"; exit 1)
    done
    echo $param
    pitch_evaluate pitch_db/train/*f0ref | fgrep TOTAL #>> result_report.txt
fi

if [ "$#" -eq 8 ]
then

#parameter1=($(seq $1 0.05 $2))
#parameter2=($(seq $3 0.05 $4))
#parameter3=($(seq $5 -0.00005 $6))
#parameter4=($(seq $7 200 $8))

parameter1=5
parameter2=5
parameter3=-40
parameter4=($(seq 0 1000 8000))

rm result_report.txt

######
#!/bin/bash

# Put here the program (maybe with path)
GETF0="get_pitch"

for thr1norm in "${parameter1[@]}"; do
    for thrmaxnorm in "${parameter2[@]}"; do
        for thpower in "${parameter3[@]}"; do
            for thzcr in "${parameter4[@]}"; do
                for fwav in pitch_db/train/*.wav; do
                    ff0=${fwav/.wav/.f0}
                    param="-1 $thr1norm -2 $thrmaxnorm -3 $thpower -4 $thzcr"
    #echo "$GETF0 $fwav $ff0 --thr1norm=$parameter1 --thrmaxnorm=$parameter2 --thpower=$parameter3 ----"
    #$GETF0 $fwav $ff0 "--thr1norm $parameter1 --thrmaxnorm $parameter2 --thpower $parameter3" > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
                    $GETF0 $fwav $ff0 $param > /dev/null || (echo "Error in $GETF0 $fwav $ff0 $param"; exit 1)
                done
                echo $param
                pitch_evaluate pitch_db/train/*f0ref | fgrep TOTAL #>> result_report.txt
            done
        done
    done
done

#pitch_evaluate pitch_db/train/*f0ref | fgrep TOTAL #>> result_report.txt

#bresult=$(awk '$4>result{result=$4; param1=$11};END{print param1}' result_report.txt)
#echo "Best param value: $bresult"

#rm optimal_p.txt
#grep $bresult result_report.txt >> optimal_param.txt
fi

exit 0
