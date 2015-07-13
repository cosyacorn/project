#include <stdlib.h>
#include<stdio.h>
#include "machine.h" 
#include "array.h" 

// -------------------------------------

void check_array(Array* a){

	// Check machine size compatible...
	if (host.num_nodes_tot%host.np!=0){
		pprintf("%ZIncompatible grid parallelisation: %d nodes on %d processes\n",
		host.num_nodes_tot, host.np);
		MPI_Abort(MPI_COMM_WORLD,1); 
	}

	//a->x_offset = host.rank;

	//pprintf("%ZRunning Ising model on %d processes\n",host.np);

	MPI_Barrier(MPI_COMM_WORLD);

	//pprintf("%Z Offset: %d\n",a->x_offset);

	

}


Array * init_array(int num_nodes, int **graph){

	int i, j;

	Array * a = malloc(sizeof(Array));
	a->x_local = num_nodes;
	a->x = a->x_local*host.np;
	a->x_offset = host.rank;

	a->neighbour = (int **) malloc(sizeof(int *)*num_nodes);
	for(i=0;i<num_nodes;i++)
		a->neighbour[i] = (int *) malloc(sizeof(int)*3);

	for(i=0;i<num_nodes;i++){
		for(j=0;j<3;j++){
			a->neighbour[i][j]=graph[host.rank*num_nodes+i][j];
		}
	}
	check_array(a);
	return a;
}

//void print_array(int num_nodes, 

void free_array(Array *a){

	int i;

	for(i=0;i<a->x_local;i++)
		free(a->neighbour[i]);
	free(a->neighbour);
	free(a);
}
// =======================================

