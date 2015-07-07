CC=mpicc
CFLAGS=-O3 -Wall
INCS=machine.h array.h field.h comms.h helper_funcs.h
LIBS=-lm

OBJS= machine.o array.o field.o comms.o helper_funcs.o

# Run ising with (eg) mpirun -np 8 1000 1000 4 2 
double_ring: main.c $(INCS) $(OBJS)
	$(CC) $(CFLAGS) -o ring main.c $(OBJS) $(LIBS)
 
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

clean:
	$(RM)  ring $(OBJS)

