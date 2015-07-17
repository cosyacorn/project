#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<getopt.h>
#include<time.h>
#include<ctype.h>

void reverse_engineer(int ** in, int ** out, int num_nodes);
void swap_alg(int num_nodes, int num_swaps, int **a, int **b);

int main(int argc, char *argv[]){

	srand(time(NULL));
	int num_nodes, i, opt, nflag, sflag, index1_1, index1_2, index2_1, index2_2, flag, temp, temp2, num_swaps;
	double time_taken;
	clock_t begin, end;

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

	begin = clock(); // begin timing

	for(i=0;i<num_nodes;i++){
		a[i][0]=i;
		a[i][1]=(i+1)%num_nodes;
		a[i][2]=(i+2)%num_nodes;
	}

	// get set b's neighbours
	reverse_engineer(a, b, num_nodes);

	// print initial config
	printf("Initial config\nSet A:\n");
	for(i=0;i<num_nodes;i++) printf("%d: %d %d %d\n", i, a[i][0], a[i][1], a[i][2]);

	printf("Set B:\n");
	for(i=0;i<num_nodes;i++) printf("%d: %d %d %d\n", i, b[i][0], b[i][1], b[i][2]);


	swap_alg(num_nodes, num_swaps, a, b);
	
	// end timing
	end = clock();

	// calculate time
	time_taken = (double)(end - begin)/CLOCKS_PER_SEC;

	// print final config
	printf("Final config after %d swaps\nSet A:\n", num_swaps);
	for(i=0;i<num_nodes;i++) printf("%d: %d %d %d\n", i, a[i][0], a[i][1], a[i][2]);

	printf("Set B:\n");
	for(i=0;i<num_nodes;i++) printf("%d: %d %d %d\n", i, b[i][0], b[i][1], b[i][2]);

	printf("Time to execute: %f\n", time_taken);

	// clean up time
	for(i=0;i<num_nodes;i++){
		free(a[i]);
		free(b[i]);
	}

	free(a);
	free(b);

	return 0;
}


void swap_alg(int num_nodes, int num_swaps, int **a, int **b){

	int flag;
	int i;
	int a_first, index_n1, index_n2;
	int b_index1, b_index2;
	int a_point1, a_point2;
	int b_point1, b_point2;
	int b_temp;

// do required no of swaps
	for(i=0;i<num_swaps;i++){

		a_first = (rand())%num_nodes; // first point in a picked at random

		index_n1 = rand()%3; // index of first neighbour
		index_n2 = rand()%3; // index of sencond neighbour
		while(index_n1 == index_n2)
			index_n2 = rand()%3; // ensure indices don't match

		// find points in set b connected to point 'index' in a
		b_point1 = a[a_first][index_n1]; // point1 in b
		b_point2 = a[a_first][index_n2]; // point2 in b

		printf("point in a picked:%d\npoints in b neighbouring: %d %d\n", a_first, b_point1, b_point2);

		// find points in set a connected to points point1/2 

		do{
			b_index1 = rand()%3; // neighbour index for first point in b to get point in a
			b_index2 = rand()%3; // same for second point

			while(b[b_point1][b_index1] == b[b_point2][b_index2])
				b_index1 = rand()%3;
	
			//while(b[b_point2][b_index2] == a[a_first][index_n2])
			//	b_index2 = rand()%3;

			a_point1 = b[b_point1][b_index1];
			a_point2 = b[b_point2][b_index2];
		} while(a_point1 == a_first || a_point2 == a_first);

			// ensure that the same point isn't picked twice in set a
//			while(a_point1 == index || a_point2 == index || a_point2 == b[b_point1][(b_index1+1)%3] || a_point2 == b[b_point1][(b_index1+2)%3] || a_point1 == b[b_point2][(b_index2+1)%3] || a_point1 == b[b_point2][(b_index2+2)%3] ){
	//			b_index1 = rand()%3;
	//			b_index2 = rand()%3;

	//			while(b[b_point1][b_index1] == a[index][index2])
		//			b_index1 = rand()%3;

			//	while(b[b_point2][b_index2] == a[index][index3])
				//	b_index2 = rand()%3;
	

			//	a_point1 = b[b_point1][b_index1];
			//	a_point2 = b[b_point2][b_index2];

				//for(i=0;i<3i++){
					//if(a[

				//}

			//}
		//}	

		int index2, index3;

		index2=0;
		index3=0;
		while(a[a_point1][index2] != b[b_point1][b_index1]) index2++;
		while(a[a_point2][index3] != b[b_point2][b_index2]) index3++;

		

		b_temp = b[b_point2][b_index2];
		a[a_point1][index2] = b[b_point2][b_index2];
		a[a_point2][index3] = b[b_point1][b_index1];

		b[b_point2][b_index2] = a_point1;
		b[b_point1][b_index1] = a_point2;

		printf("points in a: %d %d\n", a_point1, a_point2);

	}
/*
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
	
				for(i=0;i<3;i++){
					printf("b %d  a %d\n", b[a[index1_1][index1_2]][i], index1_1);
					if(b[a[index1_1][index1_2]][i]==index1_1) temp2=i;
					
				}
				printf("%d\n", temp2);

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
*/

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
