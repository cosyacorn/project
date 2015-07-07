#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include "machine.h"
#include "array.h"
#include "field.h"
#include "comms.h"
#include "helper_funcs.h"

Machine host;


int main(int argc, char * argv[]){

	int num_nodes, i, **a_graph, **b_graph, num_updates, size;
	double beta, avg, t1, t2;
	Array *a, *b;
	Field *f_a, *f_b;

	num_nodes=3200;
	beta=0.001;
	num_updates=100000;

	MPI_Init(&argc,&argv);

	init_machine(argc,argv, num_nodes);

	srand48(123+host.rank);
	//pprintf("%d\n", host.rank);

	MPI_Barrier(MPI_COMM_WORLD);

	a_graph = make_graph(num_nodes);
	b_graph = make_graph(num_nodes);

	a = init_array(size=num_nodes/host.np, a_graph);
	b = init_array(size, b_graph);


	//pprintf("%ZTotal nodes: %d\n", a->x);

	

	f_a = init_field(a);
	f_b = init_field(b);

	//pprintf("%Z BEFORE:\n");

	//for(i=0; i<num_nodes/host.np; i++)
	//	pprintf("%d: %d %d\n", i, f_a->value[i], f_b->value[i]);
		
	MPI_Barrier(MPI_COMM_WORLD);

	t1 = MPI_Wtime();

	avg=0.0;
	for(i=0;i<num_updates;i++){
		update(size, f_a, f_b, beta, a, b);
		send_boundary_data(f_a, a);
		send_boundary_data(f_b, b);
		avg += magnetisation(f_a,a) + magnetisation(f_b,b);
	}
	
	t2 = MPI_Wtime();

	pprintf("%Zbeta: %lf; avg magnetisation: %lf; time taken %lf\n",beta,avg/(double) num_updates, t2-t1);

	//pprintf("%Z AFTER:\n");

	//for(i=0; i<num_nodes/host.np; i++)
		//pprintf("%Z%d: %d %d\n", i, f_a->value[i], f_b->value[i]);

	//for(i=0; i<num_nodes/host.np; i++)
	//pprintf("halo: %d %d\n", f_a->halo[0], f_a->halo[1]);
	//pprintf("host neighbour %d %d\n", host.neighbour[BACKWARD], host.neighbour[FORWARD]);


	// clean up
	free_array(a);
	free_array(b);

	free_graph(a_graph, num_nodes);
	free_graph(b_graph, num_nodes);

	free_field(f_a);
	free_field(f_b);

	// finish parallel
	MPI_Finalize();

	return 0;
}


