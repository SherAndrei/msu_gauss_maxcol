(( n = 4000, r = 4, s = 1 ));      \
for ((m = 50; m <= 300; m += 50));    \
	 do ./bin/main $n $m $r $s; \
done
# valgrind --leak-check=full --track-origins=yes 