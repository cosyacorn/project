CC=mpicc
CFLAGS=-O0 -Wall -g
INCS=machine.h array.h field.h comms.h helper_funcs.h swap_alg.h
LIBS=-lm

OBJS= machine.o array.o field.o comms.o helper_funcs.o swap_alg.o

# Run ising with (eg) mpirun -np 8 1000 1000 4 2 
double_ring: main.c $(INCS) $(OBJS)
	$(CC) $(CFLAGS) -o random_graph main.c $(OBJS) $(LIBS)
 
machine.o: machine.c $(INCS)
	$(CC) $(CFLAGS) -c machine.c

array.o: array.c $(INCS)
	$(CC) $(CFLAGS) -c array.c

field.o: field.c $(INCS)
	$(CC) $(CFLAGS) -c field.c

comms.o: comms.c $(INCS)
	$(CC) $(CFLAGS) -c comms.c

helper_funcs.o: helper_funcs.c $(INCS)
	$(CC) $(CFLAGS) -c helper_funcs.c

swap_alg.o: swap_alg.c $(INCS)
	$(CC) $(CFLAGS) -c swap_alg.c

clean:
	$(RM)  random_graph $(OBJS) 

