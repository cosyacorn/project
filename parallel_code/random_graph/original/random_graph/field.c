#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#include "machine.h" 
#include "array.h" 
#include "field.h" 


Field * init_field(Array * a){

	int i, size;
	double d;
	Field *f = malloc(sizeof(Field));

	size = a->x_local; // store space for all local points

	f->value = malloc(sizeof(int)*size);

	for(i=0;i<size;i++){
		d = drand48();

		if(d>0.5){
			f->value[i] = 1;
		} else {
			f->value[i] = -1;
		}
	}

	f->halo_count = (int *) malloc(sizeof(int) * host.np);
	
	for(i=0;i<host.np;i++)
		f->halo_count[i]=0;

	return f;
}



void set_up_halo(Field *f_a, Field *f_b, Array *a){

	int i, j;

	for(i=0;i<a->x_local;i++){
		for(j=0;j<3;j++){
		//	pprintf("a->neighbour[i][j] = %d; host.rank*a->x_local = %d; (host.rank+1)*a->x_local = %d\n", a->neighbour[i][j], host.rank*a->x_local, (host.rank+1)*a->x_local);
			if(a->neighbour[i][j] < host.rank*a->x_local || a->neighbour[i][j] >= (host.rank+1)*a->x_local){
				f_a->halo_count[a->neighbour[i][j]/a->x_local]++;
//				printf("YES\n");
			}
		}
	}

	f_b->halo = (int **) malloc(sizeof(int *) * host.np);

	for(i=0;i<host.np;i++){
		if(f_a->halo_count[i] != 0){
			f_b->halo[i] = (int *) malloc(sizeof(int) * f_a->halo_count[i]);
		} else {
			f_b->halo[i] = NULL;
		}
	}

	for(i=0;i<host.np;i++){
		for(j=0;j<f_a->halo_count[i];j++){
			f_b->halo[i][j]=0;
			if(host.rank==0 && i==2 && j==3) printf("this guy %d\n", f_b->halo[i][j]);
		}
	}	

//	pprintf("FIELD val %d\n", f_a->value[0]++);
//	printf("f halo val field = %d\n", f_b->halo_count[0]);
}

void free_field(Field* f){

//	pprintf("in free field\n");

	int i;

	for(i=0;i<host.np;i++)
		free(f->halo[i]);

	free(f->halo);

	free(f->value);

	free(f->halo_count);

	free(f);
}

