#!/bin/bash
if [ $# == 2 ] 
then
    prog=$1
    n=$2
else
    echo "Usage: $0 programm_name matrix_size"
    exit
fi
amount=30
m=60
s=3

mkdir bench/res
for ((i = 1; i <= ${amount}; i++))
do
    ${prog} ${n} ${m} 0 ${s} > log.txt
    for name in gauss swap mult form rev all
    do
        cat log.txt | grep $name >> bench/res/$name.txt
    done
done
rm log.txt

exec 1>> bench.txt
echo "########## n = $n $(/bin/date) ##############"
for name in gauss swap mult form rev all
do
    echo "${name} $(./bench/find_avg ./bench/res/${name}.txt)"
done

rm -r bench/res