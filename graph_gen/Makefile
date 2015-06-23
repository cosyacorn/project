CC=gcc
CFLAGS= -Wall

all: graph_gen rev

graph_gen: graph_gen.c
	$(CC) graph_gen.c -o graph_gen $(CFLAGS)

rev: reverse_engineer.c
	$(CC) reverse_engineer.c -o rev $(CFLAGS)

.PHONY: clean
clean:
	$(RM) graph_gen rev
