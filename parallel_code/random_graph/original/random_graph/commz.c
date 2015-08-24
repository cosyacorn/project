#include<stdlib.h>
#include<stdio.h>
#include "machine.h" 
#include "array.h" 
#include "field.h"
#include "comms.h"

void send_boundary_data(Field* f_b, Array* a){

	// declarations
	int i, j, *k, proc;

	int * send_recv_count;

	int ** buffer_send;
	int ** buffer_recv;

	int *double_count;

	int upper_lim, lower_lim, val;

	int *throw, *catch, *offset_array;
	int throw_size, catch_size, offset;

	// init
	//determine how many to be sent to each process

	upper_lim = (a->x_offset+1)*a->x_local;
	lower_lim = (a->x_offset)*a->x_local;

	double_count = (int *) malloc(sizeof(int) * a->x);

	for(i=0;i<a->x;i++)
		double_count[i]=0;

	send_recv_count = (int *) malloc(sizeof(int) * host.np);

	for(i=0;i<host.np;i++)
		send_recv_count[i]=0;

	for(i=0;i<a->x_local;i++){
		// j=3 for trivalent - need to generalise
		for(j=0;j<3;j++){
			if(double_count[a->neighbour[i][j]] == 0){
				// determine if neighbour is on a different proc
				if(a->neighbour[i][j] < lower_lim || a->neighbour[i][j] >= upper_lim){
					send_recv_count[a->neighbour[i][j]/a->x_local]++; // increase count if it is on diff proc
					double_count[a->neighbour[i][j]] = 1;
				}
			}
		}
	}

	// malloc space for send buffer
	buffer_send = (int **) malloc(sizeof(int *) * host.np);
	buffer_recv = (int **) malloc(sizeof(int *) * host.np);

	for(i=0;i<host.np;i++){
		if(send_recv_count[i] != 0){
			buffer_send[i] = (int *) malloc(sizeof(int) * send_recv_count[i]);
			buffer_recv[i] = (int *) malloc(sizeof(int) * send_recv_count[i]);
		} else {
			buffer_send[i] = NULL;
			buffer_recv[i] = NULL;
		}
	}

	for(i=0;i<host.np;i++){
		for(j=0;j<send_recv_count[i];j++){
			buffer_send[i][j]=0;
			buffer_recv[i][j]=0;
		}
	}


	k = (int *) malloc(sizeof(int) * host.np);

	for(i=0;i<host.np;i++)
		k[i]=0;

	// loop through array points
	for(i=0;i<a->x_local;i++){
		// loop over each points three neighbours
		for(j=0;j<3;j++){
			// determine if neighbour is on a diff proc
			// (using same condition as when determining how much space to malloc
			if(double_count[a->neighbour[i][j]] == 1 ){
//				printf("yes\n");
				if(a->neighbour[i][j] < lower_lim || a->neighbour[i][j] >= upper_lim){

					// copy value into buffer
					proc = a->neighbour[i][j]/a->x_local;

					if(f_b->value[i] == -1){
						val = 0;
					} else {
						val = 1;
					}

					buffer_send[proc][k[proc]] = 2*a->neighbour[i][j] + val;

					// increase relavant counter
					k[proc]++;
				}
			}
		}
	}



	throw_size = 0;
	offset = 0;

	for(i=0;i<host.np;i++){
		throw_size += send_recv_count[i];
	}

	catch_size = throw_size;

	throw = (int*) malloc(sizeof(int) * throw_size);
	catch = (int*) malloc(sizeof(int) * catch_size);

	offset_array = (int*) malloc(sizeof(int) * host.np);

	for(i=0;i<host.np;i++){
		offset_array[i] = offset;

		for(j=0;j<send_recv_count[i];j++){
			throw[j+offset] = buffer_send[i][j];
		}
		offset += send_recv_count[i];
	}

	//if(host.rank == 1) printf("throw %d %d %d\n", throw[0], throw[1], throw[2]);

	MPI_Alltoallv(throw, send_recv_count, offset_array, MPI_INT, catch, send_recv_count, offset_array, MPI_INT, MPI_COMM_WORLD);



	for(i=0;i<host.np;i++){
		for(j=0;j<send_recv_count[i];j++){
			f_b->halo[i][j] = catch[j+offset_array[i]]; 
		}
	}

	free(k);

	for(i=0;i<host.np;i++){
		free(buffer_send[i]);
		free(buffer_recv[i]);
	}

	free(buffer_send);
	free(buffer_recv);

	free(send_recv_count);

	free(throw);
	free(catch);
	free(offset_array);

}
