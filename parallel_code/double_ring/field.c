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

	f->halo = malloc(sizeof(int)*2);
	f->value = malloc(sizeof(int)*size);

	for(i=0;i<size;i++){
		d = drand48();
		//pprintf("%f\n", d);
		if(d>0.5) f->value[i] = 1;
		else f->value[i] = -1;
	}
	//for(i=0;i<size;i++)
		//f->value[i]=host.rank;

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
