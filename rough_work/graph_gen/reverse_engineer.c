#include<stdio.h>
#include<stdlib.h>

void reverse_engineer();

int main(){

	int num_nodes;

	num_nodes=4;

	int a[4][3] = { {1,2,0}, {0,2,3}, {3,1,0}, {2,3,1} };
	int b[4][3];

	

	reverse_engineer(a, b, num_nodes);

	return 0;
}

void reverse_engineer(int in[4][3], int out[4][3], int num_nodes){

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

	for(i=0;i<num_nodes;i++){
		for(j=0;j<3;j++){
			printf("%d ", out[i][j]);
		}
	printf("\n");
	}
}
