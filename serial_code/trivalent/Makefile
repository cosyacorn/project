CC=gcc
CFLAGS=-Wall -lm

trivalent: main.c
	$(CC) -o trivalent main.c $(CFLAGS)

clean:
	$(RM) trivalent

test: trivalent test_graph.txt
	./trivalent test_graph.txt
