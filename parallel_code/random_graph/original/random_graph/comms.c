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

	int **double_count;

} BoundaryComm; 


//===== INIT COMM =====//
static BoundaryComm* init_comm(Array* a){

	int i, j;
	int proc;

	BoundaryComm *c = malloc(sizeof(BoundaryComm));

	c->double_count = (int**)malloc(sizeof(int*)*a->x_local);

	for(i=0;i<a->x_local;i++){
		c->double_count[i] = (int *)malloc(sizeof(int)*host.np);
		for(j=0;j<host.np;j++){
			c->double_count[i][j] = 0;
		}
	}

	//determine how many to be sent/recv to each process

	c->send_count = (int *) malloc(sizeof(int) * host.np);
	c->recv_count = (int *) malloc(sizeof(int) * host.np);

	for(i=0;i<host.np;i++){
		c->send_count[i]=0;
		c->recv_count[i]=0;
	}

	for(i=0;i<a->x_local;i++){
		// j=3 for trivalent - need to generalise
		for(j=0;j<3;j++){
			proc = a->neighbour[i][j]/a->x_local;
			if(c->double_count[i][proc] == 0){
				// determine if neighbour is on a different proc
				if(a->neighbour[i][j] < host.rank*a->x_local || a->neighbour[i][j] >= (host.rank+1)*a->x_local){
					c->send_count[proc]++; // increase count if it is on diff proc
					c->double_count[i][proc] = 1;
				}
			}
		}
	}

	MPI_Alltoall(c->send_count, 1, MPI_INT, c->recv_count, 1, MPI_INT, MPI_COMM_WORLD);


	// malloc space for send buffer
	c->buffer_send = (int **) malloc(sizeof(int *) * host.np);
	c->buffer_recv = (int **) malloc(sizeof(int *) * host.np);
	MPI_Barrier(MPI_COMM_WORLD);

	for(i=0;i<host.np;i++){
		if(c->send_count[i] > 0){
			c->buffer_send[i] = (int *) malloc(sizeof(int) * c->send_count[i]);
		} else { 
			c->buffer_send[i] = NULL;
		}
	}

	for(i=0;i<host.np;i++){
		if(c->recv_count[i] > 0){
			c->buffer_recv[i] = (int *) malloc(sizeof(int) * c->recv_count[i]);
		} else {
			c->buffer_recv[i] = NULL;
		}
	}
	

	for(i=0;i<host.np;i++){
		for(j=0;j<c->send_count[i];j++){
			c->buffer_send[i][j]=0;
		}
	}
	for(i=0;i<host.np;i++){
		for(j=0;j<c->recv_count[i];j++){
			c->buffer_recv[i][j]=0;
		}
	}

	return c; 
}



//===== SEND =====//
static void send(Field* f_a, Array* a, BoundaryComm* c){

	int i, j, proc, *k;
	int val;

	MPI_Request *send = (MPI_Request *) malloc(sizeof(MPI_Request) * (host.np-1));	
	MPI_Request *recv = (MPI_Request *) malloc(sizeof(MPI_Request) * (host.np-1));

	MPI_Status  *send_status = (MPI_Status *) malloc(sizeof(MPI_Status) * (host.np-1));
	MPI_Status  *recv_status = (MPI_Status *) malloc(sizeof(MPI_Status) * (host.np-1));

	MPI_Request * send_ptr = send;
	MPI_Request * recv_ptr = recv;

	k = (int *) malloc(sizeof(int) * host.np); //index for each send buffer

	for(i=0;i<host.np;i++)
		k[i]=0;

	// loop through array points
	for(i=0;i<a->x_local;i++){
		for(j=0;j<3;j++){
			proc = a->neighbour[i][j]/a->x_local;
			if(c->double_count[i][proc] == 1 ){
				
					// copy value into buffer

					if(f_a->value[i] == -1){
						val = 0;
					} else {
						val = 1;
					}

					c->buffer_send[proc][k[proc]] = 2*i + val;

					k[proc]++; // increase relavant counter
					c->double_count[i][proc] = 0; // mark as loaded to buffer
				}
			}
		}
	
	// send send buffer and recv it in the recv buffer
	for(i=0;i<host.np;i++){
		if(host.rank !=i){
			MPI_Isend((c->buffer_send[i]), c->send_count[i], MPI_INT, i, 100*host.rank, MPI_COMM_WORLD, send_ptr);
			send_ptr++;
			MPI_Irecv((c->buffer_recv[i]), c->recv_count[i], MPI_INT, i, 100*i, MPI_COMM_WORLD, recv_ptr);
			recv_ptr++;
		}
	}
	
	MPI_Waitall(host.np-1, send, send_status);
	MPI_Waitall(host.np-1, recv, recv_status);

	free(k);

	free(send);
	free(recv);

	free(send_status);
	free(recv_status);	
}

//===== UNPACK =====//
static void unpack(Field* f_a, Array* a, BoundaryComm* c){

	int i, j;	

	for(i=0;i<host.np;i++){
		for(j=0;j<c->recv_count[i];j++){
			if(c->recv_count[i] != 0){
				f_a->halo[i][j] = c->buffer_recv[i][j];
			}
		}
	}
}


//===== FREE =====//
static void free_comm(BoundaryComm* c, Array *a){

	int i;

	free(c->send_count);
	free(c->recv_count);

	for(i=0;i<host.np;i++)
		free(c->buffer_send[i]);

	for(i=0;i<host.np;i++)
		free(c->buffer_recv[i]);
	
	free(c->buffer_send);
	free(c->buffer_recv);

	for(i=0;i<a->x_local;i++)
		free(c->double_count[i]);

	free(c->double_count);

	free(c);
}

// Can be called from outside file

//===== COMPLETE FUNC =====//
void send_boundary_data(Field* f_a, Array* a){

	BoundaryComm * comm = init_comm(a);

	send(f_a, a, comm);

	unpack(f_a, a, comm);

	free_comm(comm, a);

}
