#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#include "machine.h" 
#include "array.h" 
#include "field.h" 

Field* init_field(Array* a){
	int size;
	int i,x,y;
	Field *f = malloc(sizeof(Field));

	// Malloc space for active data
	size = 2*(a->x_local+2) ;
	f->active_data = malloc(sizeof(double) * size); 
	for (i=0;i<size;i++)
		f->active_data[i] = 1.0;

	// Assign pointers to access data
	f->value = malloc(sizeof(double *) * (a->x_local+2) );
	for (x=0;x<a->x_local+2;x++)
		f->value[x] = f->active_data + x * (a->x_local+2) + 1;
	f->value++; // shift to start array at -1

	return f;
}

void free_field(Field* f){
	int k;
	free(f->active_data);
	// Undo shift of f->value pointer before freeing 
	f->value--;
	free(f->value);
	free(f);
}

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
}
