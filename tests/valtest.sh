#!/bin/bash
DATA="./tests/files"
prog="./a.out"

if [[ -x ${prog} ]]; then
    echo "########################### ${prog} ###########################"
    echo "=========================== a.txt   ==========================="
    valgrind --leak-check=full --track-origins=yes -s ${prog} 4 3 4 0 "${DATA}/a.txt" 
    echo "=========================== a20.txt ==========================="
    valgrind --leak-check=full --track-origins=yes -s ${prog} 4 3 4 0 "${DATA}/a20.txt" 
    echo "=========================== b.txt  ==========================="
    valgrind --leak-check=full --track-origins=yes -s ${prog} 4 3 4 0 "${DATA}/b.txt" 
    echo "=========================== c.txt  ==========================="
    valgrind --leak-check=full --track-origins=yes -s ${prog} 6 3 6 0 "${DATA}/c.txt" 
    echo "=========================== d.txt  ==========================="
    valgrind --leak-check=full --track-origins=yes -s  ${prog} 6 3 6 0 "${DATA}/d.txt" 
    echo "=========================== e.txt  ==========================="
    valgrind --leak-check=full --track-origins=yes -s  ${prog} 6 3 6 0 "${DATA}/e.txt" 
    echo "=========================== f.txt  ==========================="
    valgrind --leak-check=full --track-origins=yes -s  ${prog} 6 3 6 0 "${DATA}/f.txt" 
    for ((s = 1; s <= 4; s++));
        do for ((n = 3; n <= 30; n++))
            do for ((m = 3; m <= $n; m += 3))
                do echo "=========================== n = $n, m = $m, s = $s  ===========================" 
                   valgrind --leak-check=full --track-origins=yes -s  ${prog} $n $m 0 $s
                done
            done
        done   
fi