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

	int num_nodes, num_swaps, i, j, **a_graph, **b_graph, num_updates, size;
	double beta, avg, t1, t2;
	Array *a, *b;
	Field *f_a, *f_b;

	num_nodes=80;
	beta=0.001;
	num_updates=10;
	num_swaps=20000;

	MPI_Init(&argc,&argv);

	init_machine(argc,argv, num_nodes);

	srand48(time(NULL)+host.rank);
	srand(12345);
	
	a_graph = make_graph(num_nodes);
	b_graph = make_graph(num_nodes);	

	if(num_nodes<=10){
		print_graph(a_graph, num_nodes);
	//	print_graph(b_graph, num_nodes);
	}

	size=num_nodes/host.np;

	a = init_array(size, a_graph);
	b = init_array(size, b_graph);

	//pprintf("about to swap\n");
	parallel_swap_alg(num_nodes, num_swaps, a, b);
	MPI_Barrier(MPI_COMM_WORLD);
	//pprintf("after swap\n");

	//if(host.rank == 0)
	//	for(i=0;i<size;i++)
	//		printf("%d: %d %d %d\n", i, a->neighbour[i][0], a->neighbour[i][1], a->neighbour[i][2]);

	MPI_Barrier(MPI_COMM_WORLD);

	//if(host.rank == 1)
	//	for(i=0;i<size;i++)
	//		printf("%d: %d %d %d\n", i+a->x_offset*size, a->neighbour[i][0], a->neighbour[i][1], a->neighbour[i][2]);

	f_a = init_field(a);
	f_b = init_field(b);

	pprintf("halo0 %d halo1 %d\n", f_a->halo_count[0], f_a->halo_count[1]);	

	

	pprintf("entering first send\n");
	send_boundary_data(f_a, a);
	//send_boundary_data(f_b, b);
	MPI_Barrier(MPI_COMM_WORLD);
	pprintf("exited first send\n");

	for(i=0;i<2;i++)
		for(j=0;j<f_a->halo_count[i];j++)
			pprintf("f halo %d %d = %d\n", i, j, f_a->halo[i][j]);

	t1 = MPI_Wtime();

	//for(beta=0.01;beta<1.00;beta+=0.01){
		avg=0.0;
		for(i=0;i<num_updates;i++){
			//printf("burp\n");
	//		update(size, f_a, f_b, beta, a, b);
			//printf("hello %d\n", i); 
	//		avg += magnetisation(f_a,a) + magnetisation(f_b,b);
		}

	//	pprintf("%Zbeta: %lf; avg magnetisation: %lf\n",beta,avg/(double) num_updates);
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

	//pprintf("%Ztotal time taken: %lf\n", t2-t1);
	//pprintf("%d %d %d %d\n", f_a->halo_count[0], f_a->halo_count[1],f_a->halo_count[2],f_a->halo_count[3]);
	
	// write results to file
	if(host.rank==0){

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
	}

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


