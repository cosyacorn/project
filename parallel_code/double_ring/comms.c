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

	int *buffer_send_forward;
	int *buffer_recv_forward;

	int *buffer_send_backward;
	int *buffer_recv_backward;

} BoundaryComm; 




// Don't call these outside this file... 
static void send_forward(Field* f, Array* a, BoundaryComm* c){

	// copy to buffer
	c->buffer_send_forward[0] = f->value[a->x_local-1];
	//pprintf("%Z value to be sent forward: %d %d\n", f->value[a->x_local-1], c->buffer_send_forward[0]);

	MPI_Isend(c->buffer_send_forward, 1, MPI_INT, host.neighbour[FORWARD], 1,MPI_COMM_WORLD, c->send+FORWARD);
  
	MPI_Irecv(c->buffer_recv_backward, 1, MPI_INT, host.neighbour[BACKWARD], 1,MPI_COMM_WORLD,	c->recv+FORWARD);

}

static void send_backward(Field* f, Array* a, BoundaryComm* c){

	// copy to buffer
	c->buffer_send_backward[0] = f->value[0];

	MPI_Isend(c->buffer_send_backward, 1, MPI_INT, host.neighbour[BACKWARD], 1, MPI_COMM_WORLD,c->send+BACKWARD);
  
	MPI_Irecv(c->buffer_recv_forward, 1, MPI_INT, host.neighbour[FORWARD], 1,MPI_COMM_WORLD, c->recv+BACKWARD);
}

static void unpack(Field* f, Array* a, BoundaryComm* c){

	if (host.neighbour[BACKWARD] != -1)
		f->halo[FORWARD] = c->buffer_recv_forward[0]; 

	if (host.neighbour[FORWARD] != -1) 
		f->halo[BACKWARD] = c->buffer_recv_backward[0]; 

}

static BoundaryComm* init_comm(Array* a){

	BoundaryComm *c = malloc(sizeof(BoundaryComm));

	c->buffer_send_forward = malloc(sizeof(int) * 1); 
	c->buffer_recv_backward = malloc(sizeof(int) * 1); 
  
	c->buffer_send_backward = malloc(sizeof(int) * 1); 
	c->buffer_recv_forward = malloc(sizeof(int) * 1); 
  
	return c; 
}

static void free_comm(BoundaryComm* c){

	free(c->buffer_send_forward);
	free(c->buffer_recv_backward);
  
	free(c->buffer_send_backward);
	free(c->buffer_recv_forward);

	free(c);
}

// ------------------------------------------------------------
//        Externally visible functions:
// ------------------------------------------------------------

void send_boundary_data(Field* f, Array* a){

	BoundaryComm* comm = init_comm(a);

	send_forward(f, a, comm);
	send_backward(f, a, comm);

	MPI_Waitall(2, comm->send, comm->send_status);
	MPI_Waitall(2, comm->recv, comm->recv_status);

	unpack(f, a, comm);

	free_comm(comm);
}
