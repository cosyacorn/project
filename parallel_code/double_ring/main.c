#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include "machine.h"
#include "array.h"

Machine host;

void read_graph(FILE * graph, int ** a, int ** b, int num_points);
void set_spins(int size, int a_spin[], int b_spin[]);
int ** make_graph(int num_nodes);
void free_graph(int ** graph, int size);




int main(int argc, char * argv[]){

	int rank, num_nodes, i, j, a_spin[4], b_spin[4], **a_graph, **b_graph;

	Array *a, *b;
	num_nodes=16;


	MPI_Init(&argc,&argv);

	init_machine(argc,argv, num_nodes);

	MPI_Barrier(MPI_COMM_WORLD);

	a_graph = make_graph(num_nodes);
	b_graph = make_graph(num_nodes);

	a = init_array(num_nodes/host.np, a_graph);
	b = init_array(num_nodes/host.np, b_graph);

	MPI_Barrier(MPI_COMM_WORLD);


	for(i=0; i<num_nodes/host.np; i++)
		printf("%d %d\n",i, a->neighbours[i][1]);



	free_array(a);
	free_array(b);

	MPI_Finalize();

	free_graph(a_graph, num_nodes);
	free_graph(b_graph, num_nodes);

	return 0;
}



	// double ring graph with n nodes
int ** make_graph(int num_nodes){

	int **graph, i, j;

	graph = (int **) malloc(sizeof(int *)*num_nodes);
	
	for(i=0;i<num_nodes;i++){
		graph[i] = (int *) malloc(sizeof(int)*3);
		
		for(j=0;j<3;j++){
			graph[i][j] = (i-1+j+num_nodes)%num_nodes;
		}
	}

	return graph;
}

void free_graph(int ** graph, int size){

	int i;

	for(i=0;i<size;i++)
		free(graph[i]);
	free(graph);

}
