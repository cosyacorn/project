#include <stdlib.h>
#include<stdio.h>
#include "machine.h" 
#include "array.h" 

// -------------------------------------

//===== CHECK ARRAY =====//
void check_array(Array* a){

	// Check machine size compatible...
	if (host.num_nodes_tot%host.np!=0){
		pprintf("%ZIncompatible grid parallelisation: %d nodes on %d processes\n", host.num_nodes_tot, host.np);
		MPI_Abort(MPI_COMM_WORLD,1); 
	}
	MPI_Barrier(MPI_COMM_WORLD);
}

//===== INIT ARRAY =====//
Array * init_array(int num_local_nodes){

	int i, j;

	Array * a = malloc(sizeof(Array));

	a->x_local = num_local_nodes;

	a->x = a->x_local*host.np; // total nodes

	a->x_offset = host.rank;

	a->neighbour = (int **) malloc(sizeof(int *)*num_local_nodes);
	for(i=0;i<num_local_nodes;i++)
		a->neighbour[i] = (int *) malloc(sizeof(int)*3);

	for(i=0;i<num_local_nodes;i++){
		for(j=0;j<3;j++){
			a->neighbour[i][j] = (host.rank*num_local_nodes + i-1+j+a->x)%(a->x);
		}
	}
	check_array(a);

	
	return a;
}

//===== PRINT ARRAY =====//
void print_array(Array *a){

	int i, j;

	for(i=0;i<a->x_local;i++)
		for(j=0;j<3;j++)
			pprintf("%d\n", a->neighbour[i][j]);

}

//===== FREE ARRAY =====//
void free_array(Array *a){

	int i;

	for(i=0;i<a->x_local;i++)
		free(a->neighbour[i]);

	free(a->neighbour);
	free(a);
}
