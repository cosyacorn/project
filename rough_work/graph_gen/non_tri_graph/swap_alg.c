#include<stdio.h>
#include<stdlib.h>

void swap_alg_once(int num_nodes, int **a, int **b, int *num_a, int *num_b);

void swap_alg(int num_nodes, int num_swaps, int **a, int **b, int *num_a, int *num_b){

	int i;

	printf("hello\n");

	for(i=0;i<num_swaps/2;i++){
		swap_alg_once(num_nodes, a, b, num_a, num_b);
		swap_alg_once(num_nodes, b, a, num_b, num_a);
	}
}

void swap_alg_once(int num_nodes, int **a, int **b, int *num_a, int *num_b){

	int flag, flag2;
	int i, count;
	int a_first, index_n1, index_n2;
	int index1, index2, index3, index4;
	int b_index1, b_index2;
	int a_point1, a_point2;
	int b_point1, b_point2;

	do {
		a_first = (rand())%num_nodes; // first point in a picked at random

		printf("a first = %d\n", a_first);

		count=0;
		flag2=0;
		index_n1 = rand()%num_a[a_first]; // index of first neighbour
		index_n2 = rand()%num_a[a_first]; // index of sencond neighbour
		while(index_n1 == index_n2)
			index_n2 = rand()%num_a[a_first]; // ensure indices don't match

		// find points in set b connected to point 'index' in a
		b_point1 = a[a_first][index_n1]; // point1 in b
		b_point2 = a[a_first][index_n2]; // point2 in b

		index1=0;
		index2=0;

		// find correct indices
		while(b[b_point1][index1] != a_first)
			index1++;

		while(b[b_point2][index2] != a_first)
			index2++;

		do{
			flag=0;
			count++;
			b_index1 = rand()%num_b[b_point1]; // neighbour index for first point in b to get point in a
			b_index2 = rand()%num_b[b_point2]; // same for second point

			// ensure that we don't pick a_first
			while(b_index1 == index1)
				b_index1 = rand()%num_b[b_point1];

			while(b_index2 == index2)
				b_index2 = rand()%num_b[b_point2];


			if(b[b_point1][b_index1] == b[b_point2][b_index2]) // both are same point in a
				flag=1;    

			for(i=0;i<num_b[b_point2];i++){
				if(i != index2 && i != b_index2){
					if(b[b_point1][b_index1] == b[b_point2][i]) // point1 is same as a neighbour of 2
						flag=1;
				}
			}

			for(i=0;i<num_b[b_point1];i++){
				if(i != index1 && i != b_index1){
					if(b[b_point2][b_index2] == b[b_point1][i]) // point1 is same as a neighbour of 2
						flag=1;
				}
			}

			if( num_b[b_point1] == 2 || num_b[b_point2] == 2)
				flag2 = 1;

			//if(b[b_point1][b_index1] == b[b_point2][3-(index2+b_index2)]) // point1 is same as a neighbour of 2
				//flag=1;

			//if(b[b_point2][b_index2] == b[b_point1][3-(index1+b_index1)])
				//flag=1;

			// ensure that it stops and starts again if no valid neighbour is found
			if(count>20){
				flag2=1;
				break;
			}

		} while(flag==1);
	} while (flag2==1);

	// set the final points chosen in a
	a_point1 = b[b_point1][b_index1];
	a_point2 = b[b_point2][b_index2];


	index3=0;
	index4=0;

	// find correct indices
	while(a[a_point1][index3] != b_point1)
		index3++;

	while(a[a_point2][index4] != b_point2)
		index4++;

		// swap the points
	a[a_point1][index3] = b_point2;
	a[a_point2][index4] = b_point1;

	b[b_point2][b_index2] = a_point1;
	b[b_point1][b_index1] = a_point2;
}

void reverse_engineer(int ** in, int ** out, int num_nodes){

	int i, j, index, k;

	for(i=0;i<num_nodes;i++){
		for(j=0;j<3;j++){
			out[i][j]=-1;
		}
	}

	for(i=0;i<num_nodes;i++){
		for(j=0;j<3;j++){
			index=in[i][j];
			k=0;
			while(out[index][k]!=-1){
				k++;
			}
			out[index][k]=i;
		}
	}
}
