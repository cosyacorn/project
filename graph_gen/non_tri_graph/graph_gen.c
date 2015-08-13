#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<getopt.h>
#include<unistd.h>
#include<ctype.h>

void swap_alg(int num_nodes, int num_swaps, int **a, int **b, int *num_a, int *num_b);

int main(int argc, char * argv[]){

	
	
	int num_nodes, num_swaps, i, j, k, opt, nflag;
	int min, max, range, shift, index;
	double time_taken;


	clock_t begin, end;

	srand(time(NULL));

	nflag=0;
	opterr=0;

	while ((opt = getopt(argc, argv, "n:s:")) != -1){
		switch (opt){
      
		case 'n':
			num_nodes = atoi(optarg);
			nflag=1;
			break;
	
		case '?': 
			if(optopt=='n')
				printf("need an argument for -%c\n", optopt);
			else if(isprint(optopt))
				printf("Unknown option '-%c'\n", optopt);
			else
				printf("what's going on here\n");
				return 1;

	default:
		abort();
		}
	}

	if(nflag==0) num_nodes=10;

	int ** a = (int **)malloc(sizeof(int *)*num_nodes);
	int ** b = (int **)malloc(sizeof(int *)*num_nodes);

	
	int *num_neighbours = (int *)malloc(sizeof(int)*num_nodes);
	int *num_neighbours_b = (int *)malloc(sizeof(int)*num_nodes);

	min = 2; // min edges from vertex
	max = 5; // max edges from vertex

	range = max - min + 1;
	shift = min;

	for(i=0;i<num_nodes;i++){
		num_neighbours[i] = shift + rand()%range;
		a[i] = (int *)malloc(sizeof(int)*num_neighbours[i]);

		num_neighbours_b[i] = 0;
	}


	for(i=0;i<num_nodes;i++){
		for(j=0;j<num_neighbours[i];j++){
			a[i][j] = (i + j)%num_nodes;
			num_neighbours_b[a[i][j]]++;
		}
	}

	for(i=0;i<num_nodes;i++){
		b[i] = (int *)malloc(sizeof(int)*num_neighbours_b[i]);
	}

	for(i=0;i<num_nodes;i++){
		for(j=0;j<num_neighbours_b[i];j++){
			b[i][j]=-1;
		}
	}

	for(i=0;i<num_nodes;i++){
		for(j=0;j<num_neighbours[i];j++){
			index=a[i][j];
			k=0;
			while(b[index][k]!=-1){
				k++;
			}
			b[index][k]=i;
		}
	}

	num_swaps = 20;

	// begin timing
	//begin = clock();
	
	swap_alg(num_nodes, num_swaps, a, b, num_neighbours, num_neighbours_b);

	// end timing
	//end = clock();

	printf("Set A\n");
	for(i=0;i<num_nodes;i++){
		printf("%d: ", i);
		for(j=0;j<num_neighbours[i];j++){
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}

	printf("Set B\n");
	for(i=0;i<num_nodes;i++){
		printf("%d: ", i);
		for(j=0;j<num_neighbours_b[i];j++){
			printf("%d ", b[i][j]);
		}
		printf("\n");
	}

	// calculate time
	//time_taken = (double)(end - begin)/CLOCKS_PER_SEC;	
	
	//print graph


	//printf("Time to execute: %f\n", time_taken);

	//clean up time
	for(i=0;i<num_nodes;i++)
		free(a[i]);
	free(a);
	
	free(num_neighbours);

	
	return 0;
}
