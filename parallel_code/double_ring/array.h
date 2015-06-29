typedef struct {
	int x;

	int x_local; 

	int x_offset; // Array offsets
}
Array;

// -------------------------------------
Array* init_array(int num_nodes); 
void free_array(Array *a); 

