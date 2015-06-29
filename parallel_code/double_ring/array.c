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

// Check local geometry fits easily
/*  g->nx_local = g->nx / host.p_nx;
  if (g->nx_local * host.p_nx != g->nx)
  {
    pprintf("%ZGrid mismatch in X-dir\n");
    MPI_Abort(MPI_COMM_WORLD,1); 
  }
  if (g->nx_local % 2 == 1)
  {
    pprintf("%ZLocal X grid size must be even\n");
    MPI_Abort(MPI_COMM_WORLD,1); 
  }

  g->ny_local = g->ny / host.p_ny;
  if (g->ny_local * host.p_ny != g->ny)
  {
    pprintf("%ZGrid mismatch in Y-dir\n");
    MPI_Abort(MPI_COMM_WORLD,1); 
  }
  if (g->ny_local % 2 == 1)
  {
    pprintf("%ZLocal Y grid size must be even\n");
    MPI_Abort(MPI_COMM_WORLD,1); 
  }
*/
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

