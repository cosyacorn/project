#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

void read_graph(FILE * graph, int ** a, int ** b, int num_points);

int main(int argc, char * argv[]){

	int rank, num_nodes, i;

	int a[4][3] = {{0,1,3},{0,2,3},{0,1,2},{1,2,3}};
	int b[4][3] = {{0,1,2},{0,2,3},{1,2,3},{0,1,3}};


	MPI_Init(&argc,&argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	printf("a[1][1] = %d\n", a[1][1]+rank);

	MPI_Finalize();
	return 0;
}
