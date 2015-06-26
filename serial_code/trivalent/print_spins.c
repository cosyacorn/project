#include<stdio.h>

void print_spins(int size, int * a_spin, int * b_spin){

	int i;

	printf("Set 0 spins:\n");
	for(i=0;i<size;i++) printf("spin on point %d: %d\n", i, a_spin[i]);
	printf("Set 1 spins:\n");
	for(i=0;i<size;i++) printf("spin on point %d: %d\n", i, b_spin[i]);
}
