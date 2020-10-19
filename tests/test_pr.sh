(( n = 5, r = 5, s = 1 ));     \
for ((m = 1; m <= $n; m++));  \
	 do  valgrind --leak-check=full --track-origin=yes ./bin/main $n $m $r $s; \
done