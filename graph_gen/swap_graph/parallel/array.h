typedef struct {
	int x;

	int x_local; 

	int x_offset; // Array offsets

	int **neighbour;
}
Array;

// -------------------------------------
Array* init_array(int num_nodes, int ** graph); 
void free_array(Array *a); 

