#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#include "machine.h" 
#include "array.h" 
#include "field.h" 


Field * init_field(Array * a){

	int i, size;
	double d;
	Field *f = malloc(sizeof(Field));

	size = a->x_local; // store space for all local points

	f->value = malloc(sizeof(int)*size);

	for(i=0;i<size;i++){
		d = drand48();

		if(d>0.5){
			f->value[i] = 1;
		} else {
			f->value[i] = -1;
		}
	}

	f->halo_count = (int *) malloc(sizeof(int) * host.np);
	
	for(i=0;i<host.np;i++)
		f->halo_count[i]=0;

	return f;
}



void set_up_halo(Field *f_a, Array *a){

	int i, j;
	int *double_count, *send_count, *recv_count;


	double_count = (int*)malloc(sizeof(int)*a->x_local);

	for(i=0;i<a->x_local;i++)
		double_count[i]=0;

	//determine how many to be sent/recv to each process

	send_count = (int *) malloc(sizeof(int) * host.np);
	recv_count = (int *) malloc(sizeof(int) * host.np);

	for(i=0;i<host.np;i++){
		send_count[i]=0;
		recv_count[i]=0;
	}


	for(i=0;i<a->x_local;i++){
		// j=3 for trivalent - need to generalise
		for(j=0;j<3;j++){
			if(double_count[i] == 0){
				// determine if neighbour is on a different proc
				if(a->neighbour[i][j] < host.rank*a->x_local || a->neighbour[i][j] >= (host.rank+1)*a->x_local){
					send_count[a->neighbour[i][j]/a->x_local]++; // increase count if it is on diff proc
					double_count[i] = 1;
				}
			}
		}
	}


	MPI_Alltoall(send_count, 1, MPI_INT, recv_count, 1, MPI_INT, MPI_COMM_WORLD);

	for(i=0;i<host.np;i++)
		f_a->halo_count[i] = recv_count[i];

	f_a->halo = (int**)malloc(sizeof(int *)*host.np);

	for(i=0;i<host.np;i++){
		if(f_a->halo_count[i] != 0){
			f_a->halo[i] = (int*)malloc(sizeof(int)*f_a->halo_count[i]);
		} else {
			f_a->halo[i] = NULL;
		}
	}

}

void free_field(Field* f){

	int i;

	for(i=0;i<host.np;i++)
		free(f->halo[i]);

	free(f->halo);

	free(f->value);

	free(f->halo_count);

	free(f);
}

