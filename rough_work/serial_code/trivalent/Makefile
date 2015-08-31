CC=gcc
CFLAGS=-Wall -lm

trivalent: main.c set_spins.c print_spins.c hamiltonian_local.c propose_spin_flip.c
	$(CC) -o trivalent main.c set_spins.c print_spins.c hamiltonian_local.c propose_spin_flip.c $(CFLAGS)

clean:
	$(RM) trivalent

test: trivalent test_graph.txt
	./trivalent test_graph.txt
