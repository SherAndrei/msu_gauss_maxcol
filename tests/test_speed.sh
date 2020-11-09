
(( r = 0, m = 60, s = 1 ));           \
for ((n = 1000; n <= 16000; n *= 2));  \
    do 
        ./a.out $n $m $r $s | grep 'elapsed' ; \
done