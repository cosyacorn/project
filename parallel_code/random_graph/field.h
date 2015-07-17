typedef struct
{
	int **halo;	// vals for front and rear points on ahead and behind processes
	int *halo_count;
	int *value;	// vals for points on local process
}
Field;

Field* init_field(Array* a);
void free_field(Field* f);
//void write_field(char *, Field*, Array*);
