This folder contains my report and the final version of my code.

The code is compiled by the make command (presuming the openmpi module is loaded).
The code can be run for the double ring or a random graph by simply changing the number of swaps in the main.c file. This is doe by just commenting out whichever is wanted.
The code can be tested by the 'make test#' where # is the number of MPI processes to run the code on (can be 1, 2, 4, 8, 16 and 32).
