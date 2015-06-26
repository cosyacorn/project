#include<stdlib.h>
#include<math.h>

int hamiltonian_local(int spin, int spin1, int spin2, int spin3);

void propose_graph_change.c(int ** a, int ** b, int, int * a_spin, int * b_spin, int num_nodes, int site){

	int n1, n2, H1, H2, temp, index1, index2;

	index1=rand()%3;
	index2=rand()%3;


	
	// swap..... 
	temp=a[site][index1_2];
	a[index1_1][index1_2]=a[index2_1][index2_2];
	a[index2_1][index2_2]=temp;

}
