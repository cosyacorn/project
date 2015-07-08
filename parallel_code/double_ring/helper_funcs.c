#include<stdlib.h>
#include<math.h>
#include<mpi.h>
#include "array.h"
#include "field.h"
#include "comms.h"


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

	H=(a+b+c);
	return H;
}



// update func (only updates one bipartite set)
// only visible in this file
void update_one_field(int size, Field *f_a, Field *f_b, double beta){

	int i, prop, ham, new_ham;
	double r, n;

	// update first point

	ham = hamiltonian_local(f_a->value[0], f_b->halo[0], f_b->value[0], f_b->value[1]);
	prop = -1*f_a->value[0];
	new_ham = hamiltonian_local(prop, f_b->halo[0], f_b->value[0], f_b->value[1]);

	if(new_ham<ham){
		f_a->value[0] = prop;
	}
	else{
		n=drand48();
		r=exp((-2)*(beta)*(new_ham - ham));
		if(r>=n) f_a->value[0] = prop;
	}
	
	// update middle points	

	for(i=1;i<size-2;i++){
		ham = hamiltonian_local(f_a->value[i], f_b->value[i-1], f_b->value[i], f_b->value[i+1]);
		prop = -1*f_a->value[i];			new_ham = hamiltonian_local(prop, f_b->value[i-1], f_b->value[i], f_b->value[i+1]);

		if(new_ham<ham){
			f_a->value[i] = prop;
		}
		else{
			n=drand48();
			r=exp((-2)*(beta)*(new_ham - ham));

			if(r>=n) f_a->value[i] = prop;
		}
	}

	// update last point

	ham = hamiltonian_local(f_a->value[size-1], f_b->halo[1], f_b->value[size-1], f_b->value[size-2]);
	prop = -1*f_a->value[size-1];
	new_ham = hamiltonian_local(prop, f_b->halo[1], f_b->value[size-1], f_b->value[size-2]);

	if(new_ham<ham){
		f_a->value[size-1] = prop;
	}
	else{
		n=drand48();
		r=exp((-2)*(beta)*(new_ham - ham));
		if(r>=n) f_a->value[size-1] = prop;
	}
}

// update the whole graph
void update(int size, Field *f_a, Field *f_b, double beta, Array *a, Array *b){

	// update field a first
	update_one_field(size, f_a, f_b, beta);
	send_boundary_data(f_a, a);

	// then update field b
	update_one_field(size, f_b, f_a, beta);
	send_boundary_data(f_b, b);
}

double magnetisation(Field* phi, Array* a){

	int x; 
	int sum_local=0,sum_global;

	for (x=0;x<a->x_local;x++)
		sum_local += phi->value[x];

	MPI_Allreduce(&sum_local, &sum_global, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  return fabs((double) sum_global) / ((double) a->x);
    
  
}
