CC=mpicc
CCFLAGS=-O3
INCS=machine.h array.h field.h
LIBS=-lm

OBJS= set_spins.o machine.o array.o field.o

# Run ising with (eg) mpirun -np 8 1000 1000 4 2 
double_ring: main.c $(INCS) $(OBJS)
	$(CC) $(CCFLAGS) -o ring main.c $(OBJS) $(LIBS)

set_spins.o: set_spins.c $(INCS)
	$(CC) $(CCFLAGS) -c set_spins.c 
 
machine.o: machine.c $(INCS)
	$(CC) $(CCFLAGS) -c machine.c

field.o: field.c $(INCS)
	$(CC) $(CCFLAGS) -c field.c
	
clean:
	$(RM)  ring $(OBJS)

