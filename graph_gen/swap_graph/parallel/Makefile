CC = mpicc
CFLAGS = -Wall
INCS = array.h machine.h
OBJS = swap_alg.o array.o machine.o

swap: $(OBJS) $(INCS)
	$(CC) $(CFLAGS) -o swap $(OBJS)

machine.o: machine.c $(INCS)
	$(CC) $(CFLAGS) -c machine.c

array.o: array.c $(INCS)
	$(CC) $(CFLAGS) -c array.c

.PHONY: clean
clean:
	$(RM) swap $(OBJS)
