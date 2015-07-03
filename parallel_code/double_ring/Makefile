CC=mpicc
CFLAGS=-O3 -Wall
INCS=machine.h array.h field.h comms.h
LIBS=-lm

OBJS= set_spins.o machine.o array.o field.o comms.o

# Run ising with (eg) mpirun -np 8 1000 1000 4 2 
double_ring: main.c $(INCS) $(OBJS)
	$(CC) $(CFLAGS) -o ring main.c $(OBJS) $(LIBS)

set_spins.o: set_spins.c $(INCS)
	$(CC) $(CFLAGS) -c set_spins.c 
 
machine.o: machine.c $(INCS)
	$(CC) $(CFLAGS) -c machine.c

array.o: array.c $(INCS)
	$(CC) $(CFLAGS) -c array.c

field.o: field.c $(INCS)
	$(CC) $(CFLAGS) -c field.c

comms.o: comms.c $(INCS)
	$(CC) $(CFLAGS) -c comms.c

clean:
	$(RM)  ring $(OBJS)

