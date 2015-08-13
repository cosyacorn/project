#ifndef _COMMS_H_ 
#define _COMMS_H_ 

typedef struct {
	MPI_Request *send;
	MPI_Status  *send_status;
	MPI_Request *recv;
	MPI_Status  *recv_status;

	int **buffer_send;
	int **buffer_recv;

	int *send_count;
	int *recv_count;

} BoundaryComm; 

void send_boundary_data(Field* f, Array* a, BoundaryComm * comm);

BoundaryComm* init_comm(Array* a);

void free_comm(BoundaryComm* c);

#endif /* _COMMS_H_ */
