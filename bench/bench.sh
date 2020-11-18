#!/bin/bash
if [ $# -eq 2 ] 
then
    prog=$1
    n=$2
else
    echo "Usage: $0 programm_name matrix_size"
    exit
fi
amount=40
m=60
s=3

if [ -d "./bench/res/" ] 
then
    rm -r "./bench/res"
fi
mkdir bench/res

make clean > /dev/null
make bench > /dev/null

for ((i = 1; i <= ${amount}; i++))
do
    ${prog} ${n} ${m} 0 ${s} > ./bench/log.txt 
    echo "############ $i ##############" >> full_bench.txt
    cat ./bench/log.txt >> full_bench.txt
    for name in gauss swap mult form rev all
    do
        cat ./bench/log.txt | grep $name | awk '{print $2}' >> bench/res/$name.txt
    done
done

echo "########### END #############" >> full_bench.txt
rm ./bench/log.txt

gcc -O3 ./bench/find_avg.c -o ./bench/find_avg
sleep 1

exec 1>> bench.txt
echo "########## n = $n $(/bin/date) ##############"
for name in gauss swap mult form rev all
do
    echo "${name} $(./bench/find_avg ./bench/res/${name}.txt)"
done

rm -r bench/res