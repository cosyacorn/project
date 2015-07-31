CC = mpicc
CFLAGS = -Wall
OBJS = swap_alg.o

swap: $(OBJS)
	$(CC) $(CFLAGS) -o swap $(OBJS)

.PHONY: clean
clean:
	$(RM) swap $(OBJS)
