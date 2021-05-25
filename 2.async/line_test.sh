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

for ((m = 1; m <= 10; m++)) do
    for ((p = 1; p <= 5; p++)) do
        for ((n = $m; n <= 10; n++)) do
            echo "### n = $n p = $p m = $m ###"
            $prog $n $p $m 0 1 | grep "n_workable_lines"
        done
        echo "##########################"
    done
    echo "++++++++++++++++++++++++++++++++"
    echo "++++++++++++++++++++++++++++++++"
    echo "++++++++++++++++++++++++++++++++"
done