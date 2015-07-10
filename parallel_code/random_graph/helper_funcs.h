int ** make_graph(int num_nodes);
void free_graph(int ** graph, int size);
int hamiltonian_local(int spin, int spin1, int spin2, int spin3);
void update(int size, Field *f_a, Field *f_b, double beta, Array *a, Array *b);
double magnetisation(Field* phi, Array* a);
