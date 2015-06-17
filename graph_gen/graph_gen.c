#include<stdio.h>
#include<stdlib.h>
#include<time.h>


int main(int argc, char * argv[]){

	if(argc != 2){
		printf("\n ERROR! correct usage: %s <integer>\n", argv[0]);
		return 1;
	}else{
	
		int num_nodes, i, j;
		srand(time(NULL));

		num_nodes=atoi(argv[1]);
		//printf("%d\n", num_nodes);
		if(num_nodes%2!=0){
			printf("ERROR! Number of nodes must be even. Exiting.\n");
			return 1;
		}

		int ** a = (int **)malloc(sizeof(int *)*num_nodes);
		int ** b = (int **)malloc(sizeof(int *)*num_nodes);
		int * first = (int *)malloc(sizeof(int)*num_nodes);
		int * second = (int *)malloc(sizeof(int)*num_nodes);
		int * third = (int *)malloc(sizeof(int)*num_nodes);

		for(i=0;i<num_nodes;i++){
			a[i] = (int *)malloc(sizeof(int)*3);
			b[i] = (int *)malloc(sizeof(int)*3);

			first[i] = i;
			second[i] = i;
			third[i] = i;
		}


		for(i=0;i<num_nodes;i++){
			a[i][0]=first[rand()%num_nodes];
		}




		for(i=0;i<num_nodes;i++){
			free(a[i]);
			free(b[i]);
		}
		free(a);
		free(b);
		free(first);
		free(second);
		free(third);

	}
	return 0;
}
