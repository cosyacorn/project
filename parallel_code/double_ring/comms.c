#include <stdlib.h>

#include "machine.h" 
#include "array.h" 
#include "field.h" 

#include "fomms.h" 

// =======================================
// Only use this structure inside this file 
typedef struct {
	MPI_Request send[2];
	MPI_Status  send_status[2];
	MPI_Request recv[2]; 
	MPI_Status  recv_status[2];

	double *buffer_send_forward;
	double *buffer_recv_forward;

	double *buffer_send_backward;
	double *buffer_recv_backward;

} BoundaryComm; 




	// Don't call these outside this file... 
static void send_forward(Field* f, Array* a, BoundaryComm* c){
	int y;
	// copy to buffer
	//for (y=0;y<a->x_local;y++)
		c->buffer_send_forward[y] = f->value[a->x_local-1][y];

	MPI_Isend(c->buffer_send_forward,a->x_local,MPI_INT,
		host.neighbour[EAST],1000*host.rank,MPI_COMM_WORLD,c->send+EAST);
  
	MPI_Irecv(c->buffer_recv_east,g->ny_local/2,MPI_INT,
		host.neighbour[WEST],1000*host.neighbour[WEST],MPI_COMM_WORLD,
		c->recv+EAST);
}

static void send_backward(Field* f, Array* a, BoundaryComm* c){
	int y;
	// copy to buffer
	for (y=eo;y<g->ny_local;y+=2)
		c->buffer_send_west[y/2] = f->value[0][y];

	MPI_Isend(c->buffer_send_west,g->ny_local/2,MPI_INT,
		host.neighbour[WEST],1000*host.rank+1,MPI_COMM_WORLD,c->send+WEST);
  
	MPI_Irecv(c->buffer_recv_west,g->ny_local/2,MPI_INT,
		host.neighbour[EAST],1000*host.neighbour[EAST]+1,MPI_COMM_WORLD,
		c->recv+WEST);
}

static void unpack(Field* f, Array* a, BoundaryComm* c)
{
	int x,y;

	if (host.neighbour[SOUTH] != -1)
		f->value[x][         -1] = c->buffer_recv_north[x/2]; 

	if (host.neighbour[NORTH] != -1) 
		f->value[x][g->ny_local] = c->buffer_recv_south[x/2]; 

}

static BoundaryComm* init_comm(Array* a)
{
	BoundaryComm *c = malloc(sizeof(BoundaryComm));

	c->buffer_send_forward = malloc(sizeof(double) * 1); 
	c->buffer_recv_backward = malloc(sizeof(double) * 1); 
  
	c->buffer_send_backward = malloc(sizeof(double) * 1); 
	c->buffer_recv_forward = malloc(sizeof(double) * 1); 
  
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

	BoundaryComm* comm = init_comm(g);

	send_forward(f,g,comm);
	send_backward(f,g,comm);

	MPI_Waitall(4,comm->send,comm->send_status);
	MPI_Waitall(4,comm->recv,comm->recv_status);

	unpack(f,g,comm);

	free_comm(comm);
}
