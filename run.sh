gcc -g3 -w -gstabs+ -c pan.c -fopenmp
gcc -DWRAPUP -DGROUP -DBITSTATE -gstabs+ -c -g3 -w   po.c -fopenmp
gcc -o pan pan.o po.o -fopenmp
#for i in $(seq 10);do
    ./pan
#done
