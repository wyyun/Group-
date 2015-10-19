CC=gcc  # ANSI C compiler
CFLAGS=-O2  # optimizer
OFILES= po.o pan.o 

pan: $(OFILES)
	$(CC) $(CFLAGS) -o  pan $(OFILES) -lm -fopenmp

po.o: po.c 
	$(CC) -DWRAPUP -DBITSTATE -DSTDOUT -DGROUP -w -g3 $(CFLAGS) -c po.c -fopenmp

pan.o: pan.c 
	$(CC) -DSTDOUT -w -g3 $(CFLAGS) -c pan.c 
#group.o: group.c
#	$(CC) -w -g3 $(CFLAGS) -c group.c

clean:
	rm  po.o pan


