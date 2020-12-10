#!/bin/bash
prog="./a.out"

function assert {
    if [ $# -ne 2 ]
    then
        echo "Usage: [RESULT] [EXPECTING]"
        echo "Your usage: "$@""
    else 
        if [ "$1" != "$2" ]
        then
            echo "Wrong! "
            echo "  Result    : $1"
            echo "  Expecting : $2"
        else
            echo "Correct!"
        fi
fi
}

for ((m = 1; m <= 30; m++)) do
    for ((n = $m; n <= 30; n++)) do
        for ((p = 1; p <= 24; p++)) do
        echo "############ n=$n p=$p m=$m 0 1: ############"
        # assert "$($prog $n $p $m 0 1 | grep Norm)" "Norm: 0.000000e+00"
        echo "$(valgrind $prog $n $p $m 0 1 2>&1 | grep ERROR)"
        done
    done
done