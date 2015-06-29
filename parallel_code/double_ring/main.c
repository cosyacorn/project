#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include "machine.h"

Machine host;

void read_graph(FILE * graph, int ** a, int ** b, int num_points);
void set_spins(int size, int a_spin[], int b_spin[]);


int main(int argc, char * argv[]){

	int rank, num_nodes, i, a_spin[4], b_spin[4];

	// double ring with 4 points
	int a[4][3] = {{3,0,1},{0,1,2},{1,2,3},{2,3,0}};
	int b[4][3] = {{3,0,1},{0,1,2},{1,2,3},{2,3,0}};


	//set_spins(4, a_spin, b_spin);

	MPI_Init(&argc,&argv);

	init_machine(argc,argv, 4);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//printf("a[1][1] = %d\n", a[1][1]+rank);
	//pprintf("%Zhello\n");


	MPI_Finalize();
	return 0;
}
