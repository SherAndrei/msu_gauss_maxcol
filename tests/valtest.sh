#!/bin/bash
DATA="./tests/files"
prog="./a.out"

valgr="valgrind --leak-check=full --track-origins=yes --errors-for-leak-kinds=all -s"

rm val.txt
exec 1>> val.txt
if [[ -x ${prog} ]]; then
    echo "########################### ${prog} ###########################"
    echo "=========================== a.txt   ==========================="
    $valgr ${prog} 4 3 4 0 "${DATA}/a.txt" 2>&1 | grep ERROR
    echo "=========================== a20.txt ==========================="
    $valgr ${prog} 4 3 4 0 "${DATA}/a20.txt" 2>&1 | grep ERROR 
    echo "=========================== b.txt  ==========================="
    $valgr ${prog} 4 3 4 0 "${DATA}/b.txt" 2>&1 | grep ERROR
    echo "=========================== c.txt  ==========================="
    $valgr ${prog} 6 3 6 0 "${DATA}/c.txt" 2>&1 | grep ERROR
    echo "=========================== d.txt  ==========================="
    $valgr ${prog} 6 3 6 0 "${DATA}/d.txt" 2>&1 | grep ERROR
    echo "=========================== e.txt  ==========================="
    $valgr ${prog} 6 3 6 0 "${DATA}/e.txt" 2>&1 | grep ERROR
    echo "=========================== f.txt  ==========================="
    $valgr ${prog} 6 3 6 0 "${DATA}/f.txt" 2>&1 | grep ERROR
    for ((s = 1; s <= 4; s++));
        do for ((n = 3; n <= 30; n++))
            do for ((m = 3; m <= $n; m += 3))
                do echo "=========================== n = $n, m = $m, s = $s  ===========================" 
                   $valgr  ${prog} $n $m 0 $s 2>&1 | grep ERROR
                done
            done
        done   
fi