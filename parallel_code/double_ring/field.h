#define BOUNDARY_PASSIVE 0
#define BOUNDARY_ACTIVE 1

typedef struct
{
  int *active_data;  	// Storage for the actively managed data-points
  int **value;		// Access pointers
}
Field;

Field* init_field(Array* a);
void free_field(Field* f);
void write_field(char *, Field*, Array*);
