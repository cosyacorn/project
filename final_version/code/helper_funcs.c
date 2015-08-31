#include<stdlib.h>
#include<math.h>
#include<mpi.h>
#include "array.h"
#include "field.h"
#include "comms.h"
#include "machine.h"

#include <stdio.h>


//===== HAMILTONIAN =====//
int hamiltonian_local(int spin, int spin1, int spin2, int spin3){
	int H, a, b, c;
  
	if(spin==spin1)
		a=1;
	else
		a=0;
	if(spin==spin2)
		b=1;
	else
		b=0;
	if(spin==spin3)
		c=1;
	else
		c=0;

	H=-(a+b+c);
	return H;
}



//===== UPDATE ONE FIELD =====//
// only visible in this file
void update_one_field(int size, Field *f_a, Field *f_b, double beta, Array *a){

	int i, j, k, prop, ham, new_ham;
	int proc;
	int spin[3];
	double r, n;
	int val;

	// loop over points in local array
	for(i=0;i<size;i++){
		// loop over neighbours
		for(j=0;j<3;j++){
			// determine if neighbour is local or on another proc
			if(a->neighbour[i][j] < host.rank*size || a->neighbour[i][j] >= (host.rank+1)*size){
				// set spin1 to halo
				k=0; // set index to zero		
				proc = a->neighbour[i][j]/size; // first index determines the rank of proc

				while(f_b->halo[proc][k]/2 != a->neighbour[i][j]%a->x_local){
					k++;
				}

				val = f_b->halo[proc][k]%2;

				if( val == 0){
					spin[j] = -1;
				} else {
					spin[j] = 1;
				}

			} else { 
				spin[j] = f_b->value[a->neighbour[i][j]%size];
			}
		}
	

		// determine hamiltonian and do proposal etc..
		ham = hamiltonian_local(f_a->value[i], spin[0], spin[1], spin[2]);
		prop = -1*f_a->value[i];
		new_ham = hamiltonian_local(prop, spin[0], spin[1], spin[2]);

		if(new_ham<ham){
			f_a->value[i] = prop;
		}
		else{
			n=drand48();
			r=exp((-2)*(beta)*((double)new_ham - (double)ham));
			if(r>=n)
				f_a->value[i] = prop;
		}
	}	
}

//===== UPDATE =====//
void update(int size, Field *f_a, Field *f_b, double beta, Array *a, Array *b){

	update_one_field(size, f_a, f_b, beta, a);

	send_boundary_data(f_a, a);
	
	update_one_field(size, f_b, f_a, beta, b);

	send_boundary_data(f_b, b);

}

//===== MAGNETISATION =====//
double magnetisation(Field* phi, Array* a){

	int i; 
	int sum_local=0;
	int sum_global;

	for (i=0;i<a->x_local;i++)
		sum_local += phi->value[i];

	MPI_Allreduce(&sum_local, &sum_global, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  return fabs((double) sum_global) / ((double) a->x);
 
}
