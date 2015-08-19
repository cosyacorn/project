#include<stdlib.h>
#include<math.h>
#include<mpi.h>
#include "array.h"
#include "field.h"
#include "comms.h"
#include "machine.h"

#include <stdio.h>


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

void print_graph(int ** a, int num_nodes){

	int i;

	for(i=0;i<num_nodes;i++)
		pprintf("%Z%d: %d, %d, %d\n", i, a[i][0], a[i][1], a[i][2]);
}


// free graph
void free_graph(int ** graph, int size){

	int i;

	for(i=0;i<size;i++)
		free(graph[i]);
	free(graph);

}

// calculate hamiltonian
int hamiltonian_local(int spin, int spin1, int spin2, int spin3){
	int H, a, b, c;
  
	if(spin==spin1)
		a=1;
	else
		a=0;
	if(spin==spin2)
		b=1;
	else
		b=0;
	if(spin==spin3)
		c=1;
	else
		c=0;

	H=-(a+b+c);
	return H;
}



// update func (only updates one bipartite set)
// only visible in this file
void update_one_field(int size, Field *f_a, Field *f_b, double beta, Array *a){

	int i, j, k, prop, ham, new_ham;
	int index[3];
	int spin[3];
	double r, n;

	// PROBLEM AREA BELOW //

	// loop over points in local array
	for(i=0;i<size;i++){
		// loop over neighbours
		for(j=0;j<3;j++){
			// determine if neighbour is local or on another proc
			// if on other proc then copy to halo
			if(a->neighbour[i][j] < host.rank*size || a->neighbour[i][j] >= (host.rank+1)*size){
				// set spin1 to halo
				k=0; // set index to zero		
				index[j] = a->neighbour[i][j]/size; // first index determines the rank of proc
				// spin will be set to zero once used so looking for first nonzero value
				while(f_b->halo[index[j]][k] == 0 && k < f_b->halo_count[host.rank]){
					k++;
				}
// DEBUG
printf("rank = %d; k = %d; f_b->halo_count[index[j]] = %d\n", host.rank, k, f_b->halo_count[host.rank]);
				spin[j] = f_b->halo[index[j]][k]; // set spin to halo
				f_b->halo[index[j]][k] = 0; // mark as used
			} else { // if local ...
				spin[j] = f_b->value[a->neighbour[i][j]%size];
			}
		}

		// determine hamiltonian and do proposal etc..
		ham = hamiltonian_local(f_a->value[i], spin[0], spin[1], spin[2]);
		prop = -1*f_a->value[i];
		new_ham = hamiltonian_local(prop, spin[0], spin[1], spin[2]);

		if(new_ham<ham){
			f_a->value[i] = prop;
		}
		else{
			n=drand48();
			r=exp((-2)*(beta)*(new_ham - ham));

			if(r>=n) f_a->value[i] = prop;
		}
	}
}

// update the whole graph
void update(int size, Field *f_a, Field *f_b, double beta, Array *a, Array *b){

	// update field a first
//	pprintf("entering update\n");
	update_one_field(size, f_a, f_b, beta, a);
//	pprintf("finished one update\nentering send\n");
	send_boundary_data(f_a, a);
	MPI_Barrier(MPI_COMM_WORLD);
	//pprintf("exited send\n");
	MPI_Barrier(MPI_COMM_WORLD);
	// then update field b
	update_one_field(size, f_b, f_a, beta, b);
//	pprintf("update 2done\n");
	MPI_Barrier(MPI_COMM_WORLD);
	send_boundary_data(f_b, b);
//	pprintf("exited second update and send\n");
	MPI_Barrier(MPI_COMM_WORLD);
}

double magnetisation(Field* phi, Array* a){

	int x; 
	int sum_local=0,sum_global;

	for (x=0;x<a->x_local;x++)
		sum_local += phi->value[x];

	MPI_Allreduce(&sum_local, &sum_global, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  return fabs((double) sum_global) / ((double) a->x);
 
}
