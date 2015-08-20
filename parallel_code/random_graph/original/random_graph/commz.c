#include<stdlib.h>
#include<stdio.h>
#include "machine.h" 
#include "array.h" 
#include "field.h"
#include "comms.h"

void send_boundary_data(Field* f_b, Array* a){

	// declarations
	int i, j, *k, proc;

	int * send_count;
	int * recv_count;

	int ** buffer_send;
	int ** buffer_recv;

	int upper_lim, lower_lim;

	int *throw, *catch, *send_offset_array, *recv_offset_array;
	int throw_size, catch_size, send_offset, recv_offset;

	// init
	//determine how many to be sent to each process

	upper_lim = (a->x_offset+1)*a->x_local;
	lower_lim = (a->x_offset)*a->x_local;

	send_count = (int *) malloc(sizeof(int) * host.np);

	for(i=0;i<host.np;i++)
		send_count[i]=0;

	for(i=0;i<a->x_local;i++){
		// j=3 for trivalent - need to generalise
		for(j=0;j<3;j++){
			// determine if neighbour is on a different proc
			if(a->neighbour[i][j] < lower_lim || a->neighbour[i][j] >= upper_lim){
				send_count[a->neighbour[i][j]/a->x_local]++; // increase count if it is on diff proc
			}
		}
	}

	// malloc space for send buffer
	buffer_send = (int **) malloc(sizeof(int *) * host.np);

	for(i=0;i<host.np;i++){
		if(send_count[i] != 0){
			buffer_send[i] = (int *) malloc(sizeof(int) * send_count[i]);
		} else {
			buffer_send[i] = NULL;
		}
	}

	for(i=0;i<host.np;i++){
		for(j=0;j<send_count[i];j++){
			buffer_send[i][j]=0;
		}
	}

	recv_count = (int *) malloc(sizeof(int) * host.np);

	for(i=0;i<host.np;i++)
		recv_count[i]=0;

	// match send counts and recv counts

	MPI_Alltoall(send_count, 1, MPI_INT, recv_count, 1, MPI_INT, MPI_COMM_WORLD);

	k = (int *) malloc(sizeof(int) * host.np);

	for(i=0;i<host.np;i++)
		k[i]=0;

	// loop through array points
	for(i=0;i<a->x_local;i++){
		// loop over each points three neighbours
		for(j=0;j<3;j++){
			// determine if neighbour is on a diff proc
			// (using same condition as when determining how much space to malloc
			if(a->neighbour[i][j] < lower_lim || a->neighbour[i][j] >= upper_lim){
				// copy value into buffer
				proc = a->neighbour[i][j]/a->x_local;
				buffer_send[proc][k[proc]] = f_b->value[i];
				// increase relavant counter
				k[proc]++;
			}
		}
	}

	// malloc space for recv buffer
	buffer_recv = (int **) malloc(sizeof(int *) * host.np);

	for(i=0;i<host.np;i++){
		if(recv_count[i] != 0){
			buffer_recv[i] = (int *) malloc(sizeof(int) * recv_count[i]);
		} else {
			buffer_recv[i] = NULL;
		}
	}

	for(i=0;i<host.np;i++){
		for(j=0;j<recv_count[i];j++){
			buffer_recv[i][j]=0;
		}
	}

	//pprintf("everything is awesome\n");


	throw_size = 0;
	catch_size = 0;
	send_offset = 0;
	recv_offset = 0;

	for(i=0;i<host.np;i++){
		throw_size += send_count[i];
		catch_size += recv_count[i];
	}

	throw = (int*) malloc(sizeof(int) * throw_size);
	catch = (int*) malloc(sizeof(int) * catch_size);

	send_offset_array = (int*) malloc(sizeof(int) * host.np);
	recv_offset_array = (int*) malloc(sizeof(int) * host.np);

	for(i=0;i<host.np;i++){
		send_offset_array[i] = send_offset;
		recv_offset_array[i] = recv_offset;
		for(j=0;j<send_count[i];j++){
			throw[j+send_offset] = buffer_send[i][j];
		}
		send_offset += send_count[i];
		recv_offset += recv_count[i];
	}

	//MPI_Barrier(MPI_COMM_WORLD);

	MPI_Alltoallv(throw, send_count, send_offset_array, MPI_INT, catch, recv_count, recv_offset_array, MPI_INT, MPI_COMM_WORLD);

	//MPI_Barrier(MPI_COMM_WORLD);

	//pprintf("catch %d; f_b->halo_count[0] = %d; recv_count[0] = %d\n", catch_size, f_b->halo_count[0], recv_count[0]);
	

//	for(i=0;i<host.np;i++)
//		for(j=0;j<recv_count[i];j++)
	//		pprintf("j+recv_offset_array[i] = %d\n", j+recv_offset_array[i]);


//	pprintf("sizeof f halo = %d\n", sizeof(f_b->halo[0][1]));

	for(i=0;i<host.np;i++){
		for(j=0;j<f_b->halo_count[i];j++){
			pprintf("f_b->halo %d %d = %d\ncatch] = %d\n", i, j, f_b->halo[i][j], catch[j+recv_offset_array[i]]);
			f_b->halo[i][j] = catch[j+recv_offset_array[i]];
		}
	}


	MPI_Barrier(MPI_COMM_WORLD);

	free(k);

	for(i=0;i<host.np;i++){
		free(buffer_send[i]);
		free(buffer_recv[i]);
	}

	free(buffer_send);
	free(buffer_recv);

	free(send_count);
	free(recv_count);

	free(throw);
	free(catch);
	free(send_offset_array);
	free(recv_offset_array);
}
