#include <stdlib.h> 
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

	a->x_offset = host.num_nodes_local * a->x_local;


	pprintf("Offsets: %d\n",a->x_offset);


	pprintf("%ZRunning Ising model on %d processes\n",host.np);
	//pprintf("%ZGrid size = %d x %d\n",g->nx,g->ny);

}


Array * init_array(int num_nodes){

	Array * a = malloc(sizeof(Array));
	a->x = num_nodes;

	check_array(a);
	return a;
}

void free_array(Array *a)
{
}
// =======================================

