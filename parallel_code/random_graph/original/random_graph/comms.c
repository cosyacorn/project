#include<stdlib.h>
#include<stdio.h>
#include "machine.h" 
#include "array.h" 
#include "field.h"
#include "comms.h" 

// =======================================
// Only use this structure inside this file 

//===== COMM STRUCT =====//
typedef struct {

	int **buffer_send;
	int **buffer_recv;

	int *send_count;
	int *recv_count;

} BoundaryComm; 


//===== INIT COMM =====//
static BoundaryComm* init_comm(Array* a){

	int i, j;
	BoundaryComm *c = malloc(sizeof(BoundaryComm));

	MPI_Request * send = (MPI_Request *) malloc(sizeof(MPI_Request) * (host.np-1));
	MPI_Request * recv = (MPI_Request *) malloc(sizeof(MPI_Request) * (host.np-1));

	MPI_Status * send_status = (MPI_Status *) malloc(sizeof(MPI_Status) * (host.np-1));
	MPI_Status * recv_status = (MPI_Status *) malloc(sizeof(MPI_Status) * (host.np-1));

	MPI_Request * send_ptr = send;
	MPI_Request * recv_ptr = recv; 

	//determine how many to be sent to each process

	c->send_count = (int *) malloc(sizeof(int) * host.np);

	for(i=0;i<host.np;i++)
		c->send_count[i]=0;

	for(i=0;i<a->x_local;i++){
		// j=3 for trivalent - need to generalise
		for(j=0;j<3;j++){
			// determine if neighbour is on a different proc
			if(a->neighbour[i][j] < host.rank*a->x_local || a->neighbour[i][j] >= (host.rank+1)*a->x_local){
				c->send_count[a->neighbour[i][j]/a->x_local]++; // increase count if it is on diff proc
			}
		}
	}

	// malloc space for send buffer
	c->buffer_send = (int **) malloc(sizeof(int *) * host.np);

	for(i=0;i<host.np;i++){
		if(c->send_count[i] != 0){
			c->buffer_send[i] = (int *) malloc(sizeof(int) * c->send_count[i]);
		} else {
			c->buffer_send[i] = NULL;
		}
	}

	for(i=0;i<host.np;i++){
		for(j=0;j<c->send_count[i];j++){
			c->buffer_send[i][j]=0;
		}
	}

	c->recv_count = (int *) malloc(sizeof(int) * host.np);

	for(i=0;i<host.np;i++)
		c->recv_count[i]=0;

	// match send counts and recv counts
	for(i=0;i<host.np;i++){
		if(host.rank != i){
			MPI_Isend(&c->send_count[i], 1, MPI_INT, i, 100*host.rank, MPI_COMM_WORLD, send_ptr);
			send_ptr++;
			MPI_Irecv(&c->recv_count[i], 1, MPI_INT, i, 100*i, MPI_COMM_WORLD, recv_ptr);
			recv_ptr++;
		}
	}

	MPI_Waitall(host.np-1, send, send_status);
	MPI_Waitall(host.np-1, recv, recv_status);



	MPI_Barrier(MPI_COMM_WORLD);

	free(send);
	free(send_status);
	free(recv);
	free(recv_status);

	return c; 
}



//===== SEND =====//
static void send(Field* f_b, Array* a, BoundaryComm* c){

	int i, j, proc, *k;
	int upper_lim, lower_lim;

	MPI_Request *send = (MPI_Request *) malloc(sizeof(MPI_Request) * (host.np-1));	
	MPI_Request *recv = (MPI_Request *) malloc(sizeof(MPI_Request) * (host.np-1));

	MPI_Status  *send_status = (MPI_Status *) malloc(sizeof(MPI_Status) * (host.np-1));
	MPI_Status  *recv_status = (MPI_Status *) malloc(sizeof(MPI_Status) * (host.np-1));

	MPI_Request * send_ptr = send;
	MPI_Request * recv_ptr = recv;

	upper_lim = (a->x_offset+1)*a->x_local;
	lower_lim = (a->x_offset)*a->x_local;

	// initiate counter
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
				c->buffer_send[proc][k[proc]] = f_b->value[i];
				// increase relavant counter
				k[proc]++;
			}
		}
	}

	// malloc space for recv buffer
	c->buffer_recv = (int **) malloc(sizeof(int *) * host.np);

	for(i=0;i<host.np;i++){
		if(c->recv_count[i] != 0){
			c->buffer_recv[i] = (int *) malloc(sizeof(int) * c->recv_count[i]);
		} else {
			c->buffer_recv[i] = NULL;
		}
	}

	for(i=0;i<host.np;i++){
		for(j=0;j<c->recv_count[i];j++){
			c->buffer_recv[i][j]=0;
		}
	}

	// send send buffer and recv it in the recv buffer
	for(i=0;i<host.np;i++){
		if(host.rank !=i){
			MPI_Isend(&c->buffer_send[i][0], c->send_count[i], MPI_INT, i, 100*host.rank, MPI_COMM_WORLD, send_ptr);
			send_ptr++;
			MPI_Irecv(&c->buffer_recv[i][0], c->recv_count[i], MPI_INT, i, 100*i, MPI_COMM_WORLD, recv_ptr);
			recv_ptr++;
		}
	}

	
	MPI_Waitall(host.np-1, send, send_status);
	MPI_Waitall(host.np-1, recv, recv_status);

	//if(host.rank == 0) printf("send %d\n", c->send_count[0]);

//	if(host.rank == 1) printf("recv %d\n", c->recv_count[1]);

	free(k);

	free(send);
	free(recv);

	free(send_status);
	free(recv_status);
}

// func to load recv buff into halo
static void unpack(Field* f_b, Array* a, BoundaryComm* c){

	int i, j;	

	for(i=0;i<host.np;i++)
		pprintf("send count to; recv count; halo count: %d %d %d\n", c->send_count[i], c->recv_count[i], f_b->halo_count[i]);


	for(i=0;i<host.np;i++){
		for(j=0;j<c->recv_count[i];j++){
			if(c->send_count[i] != 0){

				f_b->halo[i][j] = c->buffer_recv[i][j];
			}
		}
	}

	for(i=0;i<host.np;i++){
		
		free(c->buffer_send[i]);
		free(c->buffer_recv[i]);
	}

	free(c->buffer_send);
	free(c->buffer_recv);

	free(c->send_count);
	free(c->recv_count);

	free(c);

}


//===== FREE =====//
static void free_comm(BoundaryComm* c){

	int i;

	for(i=0;i<host.np;i++){
		
		free(c->buffer_send[i]);
		free(c->buffer_recv[i]);
	}

	free(c->buffer_send);
	free(c->buffer_recv);

	free(c->send_count);
	free(c->recv_count);

	free(c);
}

// Can be called from outside file

//===== COMPLETE FUNC =====//
void send_boundary_data(Field* f_b, Array* a){

	BoundaryComm * comm = init_comm(a);

	send(f_b, a, comm);
	//pprintf("%Zsent\n");
	MPI_Barrier(MPI_COMM_WORLD);
	unpack(f_b, a, comm);
	//pprintf("%Zreceived\n");
	MPI_Barrier(MPI_COMM_WORLD);
	//free_comm(comm);
	//pprintf("%Zfreed\n");
	MPI_Barrier(MPI_COMM_WORLD);
}
