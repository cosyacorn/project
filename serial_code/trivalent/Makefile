CC=gcc
CFLAGS=-Wall -lm

trivalent: main.c
	$(CC) $(CFLAGS) -o trivalent main.c

clean:
	$(RM) trivalent

test: trivalent test_graph.txt
	./trivalent test_graph.txt
