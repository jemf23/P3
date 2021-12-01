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
rm result_report.txt

GETF0="get_pitch"

for fwav in pitch_db/train/*.wav; do
    for thr1norm in "${parameter[@]}"; do
        for thrmaxnorm in "${parameter2[@]}"; do
            for thpower in "${parameter3[@]}"; do
    ff0=${fwav/.wav/.f0}
    PARAM1= "--thr1norm=$thr1norm --thrmaxnorm=$thrmaxnorm  --thpower=$thpower"
    echo -n "thr1norm=$thr1norm thrmaxnorm=$thrmaxnorm  thpower=$thpower" >> result_report.txt
    echo "$GETF0 $fwav $ff0 $PARAM1 ----"
    $GETF0 $fwav $ff0 $PARAM1 | fgrep TOTAL
    #$GETF0 $fwav $ff0 $thr1norm $thrmaxnorm  $thpower  > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
            done
        done
    done
done

pitch_evaluate pitch_db/train/*f0ref

bresult=$(awk '$11>result{result=$11; param1=$11};END{print param1}' result_report.txt)
echo "Best param value: $bresult"

rm optimal_p.txt
grep $bresult result_report.txt >> optimal_param.txt
exit 0
#####
