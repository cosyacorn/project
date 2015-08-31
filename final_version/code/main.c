#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<mpi.h>
#include "machine.h"
#include "array.h"
#include "field.h"
#include "comms.h"
#include "helper_funcs.h"
#include "swap_alg.h"

Machine host;


int main(int argc, char * argv[]){

	int num_nodes, num_swaps, i, num_updates, size;
	double beta, avg, t1, t2;
	Array *a, *b;
	Field *f_a, *f_b;

	num_nodes=2048;
	beta=0.001;
	num_updates=1000;

	// random graph or double ring graph. comment num swaps out for what's required
	num_swaps=27*2*num_nodes;
//	num_swaps=0;

	MPI_Init(&argc,&argv);

	init_machine(argc,argv, num_nodes); // set up machine gemoetry 

	srand48(time(NULL)+host.rank);
	srand(time(NULL)+host.rank);

	size = num_nodes/host.np; // num nodes per proc

	a = init_array(size); // set up array on each proc
	b = init_array(size); // set up array on each proc

	parallel_swap_alg(num_nodes, num_swaps, a, b); // do swaps in parallel to randomise the graph

	// set fields on the arrays - set spins and determine halo sizes
	f_a = init_field(a); 
	f_b = init_field(b);
	
	set_up_halo(f_a, a);
	set_up_halo(f_b, b);

	//for(i=0;i

	// initialise the halo data	
	send_boundary_data(f_a, a);
	send_boundary_data(f_b, b);

	// begin timint
	t1 = MPI_Wtime();

	pprintf("%Z#beta:\tavg magnetisation:\n");

	for(beta=0.1;beta<=10.00;beta+=0.1){
		avg=0.0;
		for(i=0;i<num_updates;i++){
			update(size, f_a, f_b, beta, a, b); 
			avg += (magnetisation(f_a,a) + magnetisation(f_b,b))/2.0;
		}

		pprintf("%Z%lf\t%lf\n",beta,avg/(double) num_updates);
	}


	t2 = MPI_Wtime();

	pprintf("%Z#total time taken: %lf\n", t2-t1);
	pprintf("%Z#Machine geometry: %d nodes in each set (%d total nodes)  on %d procs. %d nodes per proc\n",host.num_nodes_tot, 2*host.num_nodes_tot, host.np, host.num_nodes_local);
		if(num_swaps != 0)
			pprintf("%Znumber of swaps done to create random graph: %d\n", num_swaps);
		else
			pprintf("%Z#no swaps done. Graph is double ring graph\n");
		pprintf("%Z#number of update steps: %d\n", num_updates);	


	// write results to file
/*	if(host.rank==0){

		FILE* o;
		char *filename = malloc(sizeof(char) * 15);

		sprintf(filename,"results.%.2d.txt",host.np);
		o = fopen(filename,"w");

		fprintf(o, "Machine geometry: %d nodes in each set (%d total nodes)  on %d procs. %d nodes per proc\n",host.num_nodes_tot, 2*host.num_nodes_tot, host.np, host.num_nodes_local);
		fprintf(o, "beta: %lf; avg magnetisation: %lf\n",beta,avg/(double) num_updates);
		fprintf(o, "total time taken: %lf\n", t2-t1);
		if(num_swaps != 0)
			fprintf(o, "number of swaps done to create random graph: %d\n", num_swaps);
		else
			fprintf(o, "no swaps done. Graph is double ring graph\n");
		fprintf(o, "number of update steps: %d\n", num_updates);

		fclose(o);
		free(filename);
	}*/

	// clean up

	free_field(f_a);
	free_field(f_b);

	free_array(a);
	free_array(b);

	// finish parallel
	MPI_Finalize();

	return 0;
}


