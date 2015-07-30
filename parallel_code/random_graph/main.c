#include<stdio.h>
#include<stdlib.h>
#include<time.h>
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

	num_nodes=100;
	beta=0.001;
	num_updates=100000;

	MPI_Init(&argc,&argv);

	init_machine(argc,argv, num_nodes);

	srand48(time(NULL)+host.rank);

	a_graph = make_graph(num_nodes);
	b_graph = make_graph(num_nodes);

	size=num_nodes/host.np;

	a = init_array(size, a_graph);
	b = init_array(size, b_graph);	

	f_a = init_field(a);
	f_b = init_field(b);
	
	send_boundary_data(f_a, a);
	send_boundary_data(f_b, b);

	t1 = MPI_Wtime();


	//for(beta=0.01;beta<1.00;beta+=0.01){
		avg=0.0;
		for(i=0;i<num_updates;i++){
			update(size, f_a, f_b, beta, a, b);
			avg += magnetisation(f_a,a) + magnetisation(f_b,b);
		}

		pprintf("%Zbeta: %lf; avg magnetisation: %lf\n",beta,avg/(double) num_updates);
	//}
/*
	for(beta=1.00;beta>0.00;beta-=0.01){
		avg=0.0;
		for(i=0;i<num_updates;i++){
			update(size, f_a, f_b, beta, a, b);
			send_boundary_data(f_a, a);
			send_boundary_data(f_b, b);
			avg += magnetisation(f_a,a) + magnetisation(f_b,b);
		}
	
		t2 = MPI_Wtime();

		//pprintf("%Zbeta: %lf; avg magnetisation: %lf\n",beta,avg/(double) num_updates);
	}
*/

	t2 = MPI_Wtime();

	pprintf("%Ztotal time taken: %lf\n", t2-t1);

	// write results to file
/*	if(host.rank==0){

		FILE* o;
		char *filename = malloc(sizeof(char) * 15);

		sprintf(filename,"results.%.2d.txt",host.np);
		o = fopen(filename,"w");

		fprintf(o, "Machine geometry: %d nodes on %d procs. %d nodes per proc\n",host.num_nodes_tot , host.np, host.num_nodes_local);
		fprintf(o, "beta: %lf; avg magnetisation: %lf\n",beta,avg/(double) num_updates);
		fprintf(o, "total time taken: %lf\n", t2-t1);

		fclose(o);
		free(filename);
	}
*/
	// clean up

	free_field(f_a);
	free_field(f_b);

	free_array(a);
	free_array(b);

	free_graph(a_graph, num_nodes);
	free_graph(b_graph, num_nodes);

	// finish parallel
	MPI_Finalize();

	return 0;
}


