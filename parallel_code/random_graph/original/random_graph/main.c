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

	int num_nodes, num_swaps, i, j, num_updates, size;
	int **a_graph, **b_graph;
	double beta, avg, t1, t2;
	Array *a, *b;
	Field *f_a, *f_b;

	num_nodes=32;
	beta=0.001;
	num_updates=10;
	num_swaps=2000;

	MPI_Init(&argc,&argv);

	init_machine(argc,argv, num_nodes); // set up machine gemoetry 

	srand48(time(NULL)+host.rank);
	srand(12345);
	
	// create graph
	a_graph = make_graph(num_nodes); // assign points in set A
	b_graph = make_graph(num_nodes);	// assign points in set B

	size = num_nodes/host.np; // num nodes per proc

	a = init_array(size, a_graph); // set up array on each proc
	b = init_array(size, b_graph); // set up array on each proc

	//pprintf("about to swap\n");
	parallel_swap_alg(num_nodes, num_swaps, a, b); // do swaps in parallel to randomise the graph

	// set fields on the arrays - set spins and determine halo sizes
	f_a = init_field(a); 
	f_b = init_field(b);

	for(i=0;i<b->x_local;i++){
		if(host.rank==0){
			printf("f val main = %d\n", f_b->value[i]);
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);

	
	pprintf("%Zentering first send\n");
	MPI_Barrier(MPI_COMM_WORLD);
	send_boundary_data(f_a, a); // send halo data on A
	MPI_Barrier(MPI_COMM_WORLD);
	pprintf("%Zsecond send incoming\n");
	MPI_Barrier(MPI_COMM_WORLD);
	send_boundary_data(f_b, b); // send halo data on B
	MPI_Barrier(MPI_COMM_WORLD);
	pprintf("%Zexited first send\n");
	MPI_Barrier(MPI_COMM_WORLD);

	t1 = MPI_Wtime();


	pprintf("Entering update\n");

//	for(i=0;i<num_updates;i++)
		update(size, f_a, f_b, beta, a, b); // do one update step

	pprintf("exiting updates\n");

	t2 = MPI_Wtime();

	pprintf("%Ztotal time taken: %lf\n", t2-t1);
	
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

//	pprintf("free fields\n");
//	MPI_Barrier(MPI_COMM_WORLD);
	free_field(f_a);
	free_field(f_b);

//	pprintf("free arrays\n");
//	MPI_Barrier(MPI_COMM_WORLD);

	free_array(a);
	free_array(b);

//	pprintf("free graphs\n");
//	MPI_Barrier(MPI_COMM_WORLD);

	free_graph(a_graph, num_nodes);
	free_graph(b_graph, num_nodes);

//	pprintf("all frees done, mpi finalise\n");
//	MPI_Barrier(MPI_COMM_WORLD);

	// finish parallel
	MPI_Finalize();

	return 0;
}


