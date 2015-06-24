#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<getopt.h>
#include<time.h>

void reverse_engineer(int ** in, int ** out, int num_nodes);

int main(int argc, char *argv[]){

	srand(time(NULL));
	int num_nodes, i, opt, nflag, sflag, index1_1, index1_2, index2_1, index2_2, flag, temp, num_swaps;


	nflag=0;
	sflag=0;
	opterr=0;

	// handle command line options
	while ((opt = getopt(argc, argv, "n:s:")) != -1){
		switch (opt){
      
		case 'n':
			num_nodes = atoi(optarg);
			nflag=1;
			break;

		case 's':
			num_swaps = atoi(optarg);
			sflag=1;
			break;
	
		case '?': 
			if(optopt=='n')
				printf("need an argument for -%c\n", optopt);
			else if(optopt=='s')
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

	// set defaults for no vals set
	if(nflag==0) num_nodes=10;
	if(sflag==0) num_swaps=10;


	// assign mem
	int ** a = (int **)malloc(sizeof(int *)*num_nodes);
	int ** b = (int **)malloc(sizeof(int *)*num_nodes);

	for(i=0;i<num_nodes;i++){
		a[i] = (int *)malloc(sizeof(int)*3);
		b[i] = (int *)malloc(sizeof(int)*3);
	}

	// assign neighbours:
	// simply give node the corresponding node in the other set
	// and the next two as neighbours

	for(i=0;i<num_nodes;i++){
		a[i][0]=i;
		a[i][1]=(i+1)%num_nodes;
		a[i][2]=(i+2)%num_nodes;
	}

	// get set b's neighbours
	reverse_engineer(a, b, num_nodes);

	for(i=0;i<num_nodes;i++) printf("%d: %d %d %d\n", i, a[i][0], a[i][1], a[i][2]);
	printf("\n");
	


	// do required no of swaps
	for(i=0;i<num_swaps;i++){

		flag=0;
		while(flag==0){
			// get indices of points to be swapped
			index1_1=(rand())%num_nodes;
			index2_1=rand()%num_nodes;
			index1_2=rand()%3;
			index2_2=rand()%3;
	
			// ensure that you aren't swapping a point with itself
			if(index1_1==index2_1 && index1_2==index2_2){ flag=0;}
			else{ flag=1;}
	
		
			//printf("%d,%d and %d,%d\n", index1_1, index1_2, index2_1, index2_2); 

			// do swap only if two points are to be swapped
			if(flag==1){
				// swap the points
				temp=a[index1_1][index1_2];
				a[index1_1][index1_2]=a[index2_1][index2_2];
				a[index2_1][index2_2]=temp;
	
				// ensure the swap doesn't break the trivalent-bipartite nature
				if(a[index1_1][index1_2] == a[index1_1][(index1_2+1)%3] || a[index1_1][index1_2] == a[index1_1][(index1_2+2)%3]){ flag=0;}
				else if(a[index2_1][index2_2] == a[index2_1][(index2_2+1)%3] || a[index2_1][index2_2] == a[index2_1][(index2_2+2)%3]){ flag=0;}
				else {flag = 1;}

				// if it does then swap back
				if(flag==0){
					temp=a[index1_1][index1_2];
					a[index1_1][index1_2]=a[index2_1][index2_2];
					a[index2_1][index2_2]=temp;
				}
			}
		}
	}

	// print final config
	for(i=0;i<num_nodes;i++) printf("%d: %d %d %d\n", i, a[i][0], a[i][1], a[i][2]);


	// clean up time
	for(i=0;i<num_nodes;i++){
		free(a[i]);
		free(b[i]);
	}

	free(a);
	free(b);

	return 0;
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
/*
	for(i=0;i<num_nodes;i++){
		printf("%d: ", i);
		for(j=0;j<3;j++){
			printf("%d ", out[i][j]);
		}
	printf("\n");
	}
*/
}
