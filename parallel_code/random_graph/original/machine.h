#ifndef _MACHINE_H_ 
#define _MACHINE_H_ 
// #include <stdarg.h>

#include "mpi.h"

#define BACKWARD 0
#define FORWARD 1


typedef struct 
{
	int np;
	int rank;
	int num_nodes_tot;
	int num_nodes_local;

	int neighbour[2];
}
Machine; 

extern Machine host; 

void init_machine(int argc, char *argv[], int tot_nodes); 
void pprintf(char *format, ...);

#endif /* _MACHINE_H_ */
