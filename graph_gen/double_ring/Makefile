CC=gcc
CFLAGS= -Wall

double_ring: ring_gen.c
	$(CC) ring_gen.c -o ring_gen $(CFLAGS)

.PHONY: clean
clean:
	$(RM) ring_gen
