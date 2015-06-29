#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "machine.h" 

// ====================================================================
// ==================== A parallel printf function ====================
// ====================================================================

void pprintf(char *format, ...){
	va_list l;
	char* new_format; 

	va_start(l,format);

	// RANK-ZERO ONLY
	if (format[0]=='%' && format[1]=='Z'){
		if (host.rank==0){
			format+=2;
			vprintf(format,l);
		}
	} else {
		// GLOBAL

		// problem with buffering -- make format and send to vprintf
		new_format = malloc(sizeof(char)*strlen(format)+6);

		sprintf(new_format,"P%d:%s",host.rank,format);
		vprintf(new_format,l);

		free(new_format); 
	}
	va_end(l);
}

void init_machine(int argc, char *argv[], int tot_nodes){

	if (argc != 1) MPI_Abort(MPI_COMM_WORLD,1);

	host.num_nodes_tot = tot_nodes;

	MPI_Comm_size(MPI_COMM_WORLD,&host.np);
	MPI_Comm_rank(MPI_COMM_WORLD,&host.rank);

	if(tot_nodes%host.np!=0) MPI_Abort(MPI_COMM_WORLD, 1);

	host.num_nodes_local = tot_nodes/host.np;

	pprintf("%ZMachine geometry: %d nodes on %d procs. %d nodes per proc\n",host.num_nodes_tot , host.np, host.num_nodes_local);

	// Work out neighbouring process ranks
	host.neighbour[BACKWARD] = (host.rank - 1 + host.np)%host.np;
	host.neighbour[FORWARD] = (host.rank + 1)%host.np;

}
