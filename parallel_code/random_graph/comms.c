#include<stdlib.h>
#include<stdio.h>
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

	int *send_count;
	int *recv_count;

} BoundaryComm; 




// Don't call these outside this file... 
static void send(Field* f, Array* a, BoundaryComm* c){

	int i, j, *k;

	k = (int *) malloc(sizeof(int) * host.np);

	for(i=0;i<host.np;i++) k[i]=0;

	// copy to buffer
	for(i=0;i<a->x_local;i++){
		for(j=0;j<3;j++){
			if(a->neighbour[i][j] < host.rank*a->x_local || a->neighbour[i][j] >= (host.rank+1)*a->x_local){
				c->buffer_send[a->neighbour[i][j]/a->x_local][k[a->neighbour[i][j]/a->x_local]] = f->value[i];
				k[a->neighbour[i][j]/a->x_local]++;
			}
		}
	}

	for(i=0;i<host.np;i++){
		for(j=0;j<host.np;j++){
			if(host.rank==i && i!=j){
				pprintf("sending %d elements\n", c->send_count[j]);
				MPI_Isend(c->buffer_send[j], c->send_count[j], MPI_INT, j, 1,MPI_COMM_WORLD, c->send);
				//pprintf("send count[%d] = %d\n",j, c->send_count[j]);
			}
			if(host.rank==j && i!=j){
				MPI_Irecv(c->buffer_recv[i], c->recv_count[i], MPI_INT, i, 1,MPI_COMM_WORLD, c->recv);
				pprintf("recveiving %d elements.. %d\n", c->recv_count[i], c->buffer_recv[i][0]);
			}
		}
	}

	/*if(host.rank==0){
		for(i=0;i<host.np;i++){
			printf("%d\n", c->send_count[i]);
			for(j=0;j<c->send_count[i];j++){
				//printf("%d ", c->buffer_send[i][j]);
			}
			//printf("\n");
		}
	}

	printf("done\n");*/
}

static void unpack(Field* f, Array* a, BoundaryComm* c){

	int i, j;

	for(i=0;i<host.np;i++){
		for(j=0;j<c->recv_count[i];j++){
			//if(i != j)
				//f->halo[i][j] = c->buffer_recv[i][j];
		}
	}

	//pprintf("halo %d buff %d\n", f->halo[0][0], c->buffer_recv[0][0]);

}



static BoundaryComm* init_comm(Array* a){

	int i, j;
	BoundaryComm *c = malloc(sizeof(BoundaryComm));

	c->send_count = (int *) malloc(sizeof(int) * host.np);
	c->recv_count = (int *) malloc(sizeof(int) * host.np);

	for(i=0;i<host.np;i++) c->send_count[i]=0;

	//determine how many to be sent to each process

	for(i=0;i<a->x_local;i++){
		// j=3 for trivalent - need to generalise
		for(j=0;j<3;j++){
			if(a->neighbour[i][j] < host.rank*a->x_local || a->neighbour[i][j] >= (host.rank+1)*a->x_local){
				c->send_count[a->neighbour[i][j]/a->x_local]++;
			}
		}
	}

	c->buffer_send = (int **) malloc(sizeof(int *) * host.np);
	for(i=0;i<host.np;i++){
		c->buffer_send[i] = (int *) malloc(sizeof(int) * c->send_count[i]);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	for(i=0;i<host.np;i++){
		for(j=0;j<host.np;j++){
			MPI_Isend(&c->send_count[i], 1, MPI_INT, j, 1,MPI_COMM_WORLD, c->send);
			MPI_Irecv(&c->recv_count[j], 1, MPI_INT, i, 1,MPI_COMM_WORLD, c->recv);
		}
	}

	//pprintf("recv_count[0] = %d recv_count[1] = %d\n", c->recv_count[0], c->recv_count[1]);

	c->buffer_recv = (int **) malloc(sizeof(int *) * host.np);
	for(i=0;i<host.np;i++){
		c->buffer_recv[i] = (int *) malloc(sizeof(int) * c->recv_count[i]);
	}


	return c; 
}


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

// ------------------------------------------------------------
//        Externally visible functions:
// ------------------------------------------------------------

void send_boundary_data(Field* f, Array* a){

	BoundaryComm* comm = init_comm(a);

	send(f, a, comm);


//	MPI_Waitall(2, comm->send, comm->send_status);
	MPI_Waitall(comm->recv_count[host.rank], comm->recv, comm->recv_status);

//	unpack(f, a, comm);

	free_comm(comm);
}
