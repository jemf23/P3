#!/bin/bash

# Be sure that this file has execution permissions:
# Use the nautilus explorer or chmod +x run_vad.sh
# Put here the program (maybe with path)

#parameter1=($(seq $1 0.1 $2))
#parameter2=($(seq $3 0.1 $4))
#parameter3=($(seq $5 0.1 $6))
parameter1=($(seq 0 0.1 1))
parameter2=($(seq 0 0.1 1))
parameter3=($(seq -0.00015 -0.00005 -0.00025))

parameter1=($(seq 0.8 0.1 1))
parameter2=($(seq 0.3 0.1 0.7))
parameter3=($(seq -0.00015 -0.00005 -0.00020))
rm result_report.txt

######
#!/bin/bash

# Put here the program (maybe with path)
GETF0="get_pitch"

for thr1norm in "${parameter1[@]}"; do
    for thrmaxnorm in "${parameter2[@]}"; do
        for thpower in "${parameter3[@]}"; do
            for fwav in pitch_db/train/*.wav; do
                ff0=${fwav/.wav/.f0}
                param="-1 $thr1norm -2 $thrmaxnorm -3 $thpower"
                echo $param
    #echo "$GETF0 $fwav $ff0 --thr1norm=$parameter1 --thrmaxnorm=$parameter2 --thpower=$parameter3 ----"
    #$GETF0 $fwav $ff0 "--thr1norm $parameter1 --thrmaxnorm $parameter2 --thpower $parameter3" > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
                $GETF0 $fwav $ff0 $param > /dev/null || (echo "Error in $GETF0 $fwav $ff0 $param"; exit 1)
            done
        done
    done
done

pitch_evaluate pitch_db/train/*f0ref | fgrep TOTAL #>> result_report.txt

bresult=$(awk '$4>result{result=$4; param1=$11};END{print param1}' result_report.txt)
echo "Best param value: $bresult"

rm optimal_p.txt
grep $bresult result_report.txt >> optimal_param.txt




exit 0




######
GETF0="get_pitch"


for thr1norm in "${parameter1[@]}"; do
    for thrmaxnorm in "${parameter2[@]}"; do
        for thpower in "${parameter3[@]}"; do
            for fwav in pitch_db/train/*.wav; do
    ff0=${fwav/.wav/.f0}
    #PARAM1= "--thr1norm=$thr1norm --thrmaxnorm=$thrmaxnorm  --thpower=$thpower"
    echo "thr1norm=$thr1norm thrmaxnorm=$thrmaxnorm  thpower=$thpower" >> result_report.txt
    echo "$GETF0 $fwav $ff0 thr1norm=$thr1norm thrmaxnorm=$thrmaxnorm  thpower=$thpower ----"
    $GETF0 $fwav $ff0 "--thr1norm=$thr1norm --thrmaxnorm=$thrmaxnorm  --thpower=$thpower" > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
    pitch_evaluate pitch_db/train/*f0ref | fgrep TOTAL >> result_report.txt
    #$GETF0 $fwav $ff0 $thr1norm $thrmaxnorm  $thpower  > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
            done
        done
    done
done
bresult=$(awk '$4>result{result=$4; param1=$11};END{print param1}' result_report.txt)
echo "Best param value: $bresult"

rm optimal_p.txt
grep $bresult result_report.txt >> optimal_param.txt
exit 0
#####
