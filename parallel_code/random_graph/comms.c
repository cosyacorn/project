#include <stdlib.h>
#include "machine.h" 
#include "array.h" 
#include "field.h" 
#include "comms.h" 

// =======================================
// Only use this structure inside this file 
typedef struct {
	MPI_Request send[2];
	MPI_Status  send_status[2];
	MPI_Request recv[2]; 
	MPI_Status  recv_status[2];

	int **buffer_send;
	int **buffer_recv;

	//int *buffer_send_backward;
	//int *buffer_recv_backward;

} BoundaryComm; 




// Don't call these outside this file... 
/*static void send_forward(Field* f, Array* a, BoundaryComm* c){

	// copy to buffer
	c->buffer_sends[0] = f->value[a->x_local-1];
	//pprintf("%Z value to be sent forward: %d %d\n", f->value[a->x_local-1], c->buffer_send_forward[0]);

	MPI_Isend(c->buffer_send_forward, 1, MPI_INT, host.neighbour[FORWARD], 1,MPI_COMM_WORLD, c->send+FORWARD);
  
	MPI_Irecv(c->buffer_recv_backward, 1, MPI_INT, host.neighbour[BACKWARD], 1,MPI_COMM_WORLD,	c->recv+FORWARD);

}*/

/*static void send_backward(Field* f, Array* a, BoundaryComm* c){

	// copy to buffer
	c->buffer_send_backward[0] = f->value[0];

	MPI_Isend(c->buffer_send_backward, 1, MPI_INT, host.neighbour[BACKWARD], 1, MPI_COMM_WORLD,c->send+BACKWARD);
  
	MPI_Irecv(c->buffer_recv_forward, 1, MPI_INT, host.neighbour[FORWARD], 1,MPI_COMM_WORLD, c->recv+BACKWARD);
}*/

/*static void unpack(Field* f, Array* a, BoundaryComm* c){

	if (host.neighbour[BACKWARD] != -1) // data from in front being sent to halo[1] of proccess behind
		f->halo[FORWARD] = c->buffer_recv_forward[0]; 

	if (host.neighbour[FORWARD] != -1) 
		f->halo[BACKWARD] = c->buffer_recv_backward[0]; 

}*/

static BoundaryComm* init_comm(Array* a){

	int i, j, k, *send_count, *recv_count;
	BoundaryComm *c = malloc(sizeof(BoundaryComm));

	send_count = malloc(sizeof(int) * host.np);
	recv_count = malloc(sizeof(int) * host.np);

	for(i=0;i<host.np;i++) send_count[i]=0;

	//determine how many to be sent to each process

	for(i=0;i<a->x_local;i++){
		// j=3 for trivalent - need to generalise
		for(j=0;j<3;j++){
			if(a->neighbour[i][j] < host.rank*a->x_local || a->neighbour[i][j] >= (host.rank+1)*a->x_local){
				send_count[a->neighbour[i][j]/a->x_local]++;
			}
		}
	}

	c->buffer_send = (int **) malloc(sizeof(int *) * host.np);
	for(i=0;i<host.np;i++){
		c->buffer_send[i] = (int *) malloc(sizeof(int) * send_count[i]);
	}


	for(i=0;i<a->x_local;i++){
		k=0;
		for(j=0;j<3;j++){
			if(a->neighbour[i][j] < host.rank*a->x_local || a->neighbour[i][j] >= (host.rank+1)*a->x_local){
				c->buffer_send[a->neighbour[i][j]/a->x_local][k] = a->neighbour[i][j];
				k++;
			}
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	for(i=0;i<host.np;i++){
		for(j=0;j<host.np;j++){
			MPI_Isend(&send_count[i], 1, MPI_INT, j, 1,MPI_COMM_WORLD, c->send);
			MPI_Irecv(&recv_count[j], 1, MPI_INT, i, 1,MPI_COMM_WORLD, c->recv);
		}
	}

	c->buffer_recv = (int **) malloc(sizeof(int *) * host.np);
	for(i=0;i<host.np;i++){
		c->buffer_recv[i] = (int *) malloc(sizeof(int)*recv_count[i]);
	}

	//free(send_count);
	//free(recv_count);

	return c; 
}


static void free_comm(BoundaryComm* c){

	free(c->buffer_send);
	free(c->buffer_recv);

	free(c);
}

// ------------------------------------------------------------
//        Externally visible functions:
// ------------------------------------------------------------

void send_boundary_data(Field* f, Array* a){

	BoundaryComm* comm = init_comm(a);

//	send_forward(f, a, comm);
//	send_backward(f, a, comm);

//	MPI_Waitall(2, comm->send, comm->send_status);
//	MPI_Waitall(2, comm->recv, comm->recv_status);

//	unpack(f, a, comm);

//	free_comm(comm);
}
