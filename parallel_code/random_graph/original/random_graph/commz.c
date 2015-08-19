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

	MPI_Request *send = (MPI_Request *) malloc(sizeof(MPI_Request) * (host.np-1));	
	MPI_Request *recv = (MPI_Request *) malloc(sizeof(MPI_Request) * (host.np-1));

	MPI_Status  *send_status = (MPI_Status *) malloc(sizeof(MPI_Status) * (host.np-1));
	MPI_Status  *recv_status = (MPI_Status *) malloc(sizeof(MPI_Status) * (host.np-1));

	MPI_Request * send_ptr = send;
	MPI_Request * recv_ptr = recv;
	
	MPI_Request * send2 = (MPI_Request *) malloc(sizeof(MPI_Request) * (host.np-1));
	MPI_Request * recv2 = (MPI_Request *) malloc(sizeof(MPI_Request) * (host.np-1));

	MPI_Status * send_status2 = (MPI_Status *) malloc(sizeof(MPI_Status) * (host.np-1));
	MPI_Status * recv_status2 = (MPI_Status *) malloc(sizeof(MPI_Status) * (host.np-1));

	MPI_Request * send_ptr2 = send2;
	MPI_Request * recv_ptr2 = recv2; 


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
	for(i=0;i<host.np;i++){
		if(host.rank != i){
			MPI_Isend(&send_count[i], 1, MPI_INT, i, 100*host.rank, MPI_COMM_WORLD, send_ptr);
			send_ptr++;
			MPI_Irecv(&recv_count[i], 1, MPI_INT, i, 100*i, MPI_COMM_WORLD, recv_ptr);
			recv_ptr++;
		}
	}

	MPI_Waitall(host.np-1, send, send_status);
	MPI_Waitall(host.np-1, recv, recv_status);


	// send

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

	// send send buffer and recv it in the recv buffer
	for(i=0;i<host.np;i++){
		if(host.rank !=i){
			MPI_Isend(&buffer_send[i][0], send_count[i], MPI_INT, i, 100*host.rank, MPI_COMM_WORLD, send_ptr2);
			send_ptr2++;
			MPI_Irecv(&buffer_recv[i][0], recv_count[i], MPI_INT, i, 100*i, MPI_COMM_WORLD, recv_ptr2);
			recv_ptr2++;
		}
	}

	MPI_Waitall(host.np-1, send2, send_status2);
	MPI_Waitall(host.np-1, recv2, recv_status2);

	//if(host.rank == 0) printf("send %d\n", c->send_count[0]);

//	if(host.rank == 1) printf("recv %d\n", c->recv_count[1]);

	// unpack

	for(i=0;i<host.np;i++){
		for(j=0;j<recv_count[i];j++){
			f_b->halo[i][j] = buffer_recv[i][j];
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	free(k);

	free(send);
	free(send_status);
	free(recv);
	free(recv_status);

	free(send2);
	free(send_status2);
	free(recv2);
	free(recv_status2);

	for(i=0;i<host.np;i++){
		free(buffer_send[i]);
		free(buffer_recv[i]);
	}

	free(buffer_send);
	free(buffer_recv);

	free(send_count);
	free(recv_count);
}
