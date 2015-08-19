typedef struct
{
	int *halo_count; // number fo vals on other processes
	int **halo;	// vals for points on other processes
	int *value;	// vals for points on local process
}
Field;

Field* init_field(Array* a);
void free_field(Field* f);
//void write_field(char *, Field*, Array*);
