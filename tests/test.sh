DATA="./tests/files"
prog="./bin/main"

if [[ -x ${prog} ]]; then
    echo "########################### ${prog} ###########################"
    echo "=========================== a.txt   ==========================="
    timeout 1m ${prog} 4 3 4 0 "${DATA}/a.txt" 
    echo "=========================== a20.txt ==========================="
    timeout 1m ${prog} 4 3 4 0 "${DATA}/a20.txt" 
    echo "=========================== b.txt  ==========================="
    timeout 1m ${prog} 4 3 4 0 "${DATA}/b.txt" 
    echo "=========================== c.txt  ==========================="
    timeout 1m ${prog} 6 3 6 0 "${DATA}/c.txt" 
    echo "=========================== d.txt  ==========================="
    timeout 1m ${prog} 6 3 6 0 "${DATA}/d.txt" 
    echo "=========================== e.txt  ==========================="
    timeout 1m ${prog} 6 3 6 0 "${DATA}/e.txt" 
    echo "=========================== f.txt  ==========================="
    timeout 1m ${prog} 6 3 6 0 "${DATA}/f.txt" 
fi