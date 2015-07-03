#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#include "machine.h" 
#include "array.h" 
#include "field.h" 
/*
Field* init_field(Array* a){

	int size;
	int i,x,y;
	Field *f = malloc(sizeof(Field));

	// Malloc space for active data
	size = a->x_local+2;
	f->active_data = malloc(sizeof(double) * size); 
	for (i=0;i<size;i++)
		f->active_data[i] = 1;


	// Assign pointers to access data
	f->value = malloc(sizeof(double *) * (a->x_local+2) );
	for (i=0;i<a->x_local+2;i++)
		f->value[i] = f->active_data + i * (a->x_local+2) + 1;
	f->value++; // shift to start array at -1

	return f;
}
*/

Field * init_field(Array * a){

	int i, size;
	Field *f = malloc(sizeof(Field));

	size = a->x_local; // store space for all local points

	f->halo = malloc(sizeof(int)*2);
	f->value = malloc(sizeof(int)*size);

	for(i=0;i<size;i++) f->value[i] = 100+host.rank+10*i;

	return f;
}

void free_field(Field* f){

	free(f->halo);
	free(f->value);
	free(f);
}

/*
void write_field(char* filestub, Field *f, Array *a){
	int x,y;
	FILE* o;
	char *filename = malloc(sizeof(char) * (strlen(filestub)+10));

	sprintf(filename,"%s.%.3d",filestub,host.rank);
	o = fopen(filename,"w");

	// Write the active part, with offsets. Don't write process identities
	for (x=0;x<a->x_local;x++)
		for (y=0;y<a->x_local;y++)
			fprintf(o,"%d %d %d\n",x+a->x_offset,y+a->x_offset,f->value[x][y]);

	fclose(o);
	free(filename);
}*/
