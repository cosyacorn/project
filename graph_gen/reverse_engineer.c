#include<stdio.h>
#include<stdlib.h>


void reverse_engineer();



int main(){

	int num_nodes, i, index;

	num_nodes=4;

	int ** a = (int **)malloc(sizeof(int *)*num_nodes);
	int ** b = (int **)malloc(sizeof(int *)*num_nodes);

	for(i=0;i<num_nodes;i++){
		a[i] = (int *)malloc(sizeof(int)*3);
		b[i] = (int *)malloc(sizeof(int)*3);
	}

	return 0;
}

void reverse_engineer(){



}
