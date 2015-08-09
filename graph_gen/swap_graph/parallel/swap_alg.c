#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<getopt.h>
#include<time.h>
#include<ctype.h>
#include<mpi.h>
#include "machine.h"
#include "array.h"


void reverse_engineer(int ** in, int ** out, int num_nodes);
void swap_alg(int num_nodes, int num_swaps, Array *a, Array *b);
void parallel_swap_alg(int num_nodes, int num_swaps, Array *a, Array *b);

Machine host;

int main(int argc, char *argv[]){

	srand(time(NULL));
	int i, j, num_nodes, num_swaps, size_local;
	double time_taken;
	Array *a_loc, *b_loc;
	clock_t begin, end;

	MPI_Init(&argc,&argv);

	num_nodes = 8;
	num_swaps = 1;

	init_machine(argc, argv, num_nodes);

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

	begin = MPI_Wtime(); // begin timing

	for(i=0;i<num_nodes;i++){
		a[i][0]=i;
		a[i][1]=(i+1)%num_nodes;
		a[i][2]=(i+2)%num_nodes;
	}

	// get set b's neighbours
	reverse_engineer(a, b, num_nodes);

	size_local = num_nodes/host.np;

	a_loc = init_array(size_local, a);
	b_loc = init_array(size_local, b);

	// print initial config
	if(num_nodes<10){
		pprintf("%ZInitial config\nSet A:\n");
		for(i=0;i<num_nodes;i++)
			pprintf("%Z%d: %d %d %d\n", i, a[i][0], a[i][1], a[i][2]);

		MPI_Barrier(MPI_COMM_WORLD);
		pprintf("%ZSet B:\n");
		for(i=0;i<num_nodes;i++)
			pprintf("%Z%d: %d %d %d\n", i, b[i][0], b[i][1], b[i][2]);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	parallel_swap_alg(num_nodes, num_swaps, a_loc, b_loc);

	// end timing
	end = MPI_Wtime();

	// calculate time
	time_taken = (double)(end - begin);

	

	// print final config
	if(num_nodes<10){
		for(i=0;i<size_local;i++){
			//for(j=0;j<3;j++){
				a[host.rank*size_local+i][0] = a_loc->neighbour[i][0];
				b[host.rank*size_local+i][0] = b_loc->neighbour[i][0];
				a[host.rank*size_local+i][1] = a_loc->neighbour[i][1];
				b[host.rank*size_local+i][1] = b_loc->neighbour[i][1];
				a[host.rank*size_local+i][2] = a_loc->neighbour[i][2];
				b[host.rank*size_local+i][2] = b_loc->neighbour[i][2];
			//}
		//printf("\n");
		}
		MPI_Barrier(MPI_COMM_WORLD);
		pprintf("%ZFinal config after %d swaps\nSet A:\n", num_swaps);
		for(j=0;j<host.np;j++){
			if(j==host.rank){
				for(i=0;i<size_local;i++){
					printf("%d: %d %d %d\n", i, a_loc->neighbour[i][0], a_loc->neighbour[i][1], a_loc->neighbour[i][2]);
				}
			}
			MPI_Barrier(MPI_COMM_WORLD);
		}
			//pprintf("%Z%d: %d %d %d\n", i, a[i][0], a[i][1], a[i][2]);

		MPI_Barrier(MPI_COMM_WORLD);
		//pprintf("%ZSet B:\n");
		//for(i=0;i<size_local;i++)
			//printf("%d: %d %d %d\n", i, b_loc->neighbour[i][0], b_loc->neighbour[i][1], b_loc->neighbour[i][2]);
			//pprintf("%Z%d: %d %d %d\n", i, b[i][0], b[i][1], b[i][2]);
	}

	pprintf("%ZTime to execute: %f\n", time_taken);

	// clean up time
	for(i=0;i<num_nodes;i++){
		free(a[i]);
		free(b[i]);
	}

	free(a);
	free(b);

	MPI_Finalize();

	return 0;
}

//=====================================//
//====== PARALLEL SWAP ALGORITHM ======//
//=====================================//

void parallel_swap_alg(int num_nodes, int num_swaps, Array * a, Array * b){

	int flag, flag2, count;
	int i;
	int a_first, index_n1, index_n2;
	int index1, index2, index3, index4;
	int b_index1, b_index2;
	int a_point1, a_point2;
	int b_point1, b_point2;

	int ranka_first, rankb1, rankb2, ranka1, ranka2;
	int check1, check2;

// do required no of swaps
	for(i=0;i<num_swaps;i++){
		do{
			flag=0;
			a_first = (rand())%num_nodes; // first point in a picked at random
			ranka_first = a_first/host.num_nodes_local;

			printf("a1 = %d\n", a_first);

			if(host.rank == ranka_first){
				index_n1 = rand()%3; // index of first neighbour
				index_n2 = rand()%3; // index of sencond neighbour

				

				while(index_n1 == index_n2)
					index_n2 = rand()%3; // ensure indices don't match

				// find points in set b connected to point 'index' in a
				b_point1 = a->neighbour[a_first%host.num_nodes_local][index_n1]; // point1 in b
				b_point2 = a->neighbour[a_first%host.num_nodes_local][index_n2]; // point2 in b

				
			}
		
			

			MPI_Bcast(&b_point1, 1, MPI_INT, ranka_first, MPI_COMM_WORLD);
			MPI_Bcast(&b_point2, 1, MPI_INT, ranka_first, MPI_COMM_WORLD);

			printf("b1 %d\t b2 %d\n", b_point1, b_point2);

			rankb1 = b_point1/host.num_nodes_local;
			rankb2 = b_point2/host.num_nodes_local;

			
			// find correct indices
			if(host.rank == rankb1){
				index1=0;
				while(b->neighbour[b_point1%host.num_nodes_local][index1] != a_first)
					index1++;

			}

			if(host.rank == rankb2){
				index2=0;
				while(b->neighbour[b_point2%host.num_nodes_local][index2] != a_first)
					index2++;
			}

			MPI_Bcast(&index1, 1, MPI_INT, rankb1, MPI_COMM_WORLD);
			MPI_Bcast(&index2, 1, MPI_INT, rankb2, MPI_COMM_WORLD);

			count = 0;
			do{
				flag2 = 0;
				count++;

				if(host.rank == rankb1){
					b_index1 = rand()%3;
					while(b_index1 == index1) // ensure that we don't pick a_first
						b_index1 = rand()%3;

					a_point1 = b->neighbour[b_point1%host.num_nodes_local][b_index1];
					check1 = b->neighbour[b_point1%host.num_nodes_local][3-(index1+b_index1)];
				}

				if(host.rank == rankb2){
					b_index2 = rand()%3;
					while(b_index2 == index2) // ensure that we don't pick a_first
						b_index2 = rand()%3;

					a_point2  = b->neighbour[b_point2%host.num_nodes_local][b_index2];
					check2 = b->neighbour[b_point2%host.num_nodes_local][3-(index2+b_index2)];
				}
			

				MPI_Bcast(&b_index1, 1, MPI_INT, rankb1, MPI_COMM_WORLD);
				MPI_Bcast(&b_index2, 1, MPI_INT, rankb2, MPI_COMM_WORLD);

				MPI_Bcast(&a_point1, 1, MPI_INT, rankb1, MPI_COMM_WORLD);
				MPI_Bcast(&check1, 1, MPI_INT, rankb1, MPI_COMM_WORLD);

				MPI_Bcast(&a_point2, 1, MPI_INT, rankb2, MPI_COMM_WORLD);
				MPI_Bcast(&check2, 1, MPI_INT, rankb2, MPI_COMM_WORLD);

				if(a_point1 == a_point2) // both are same point in a
					flag2=1;   

				if(a_point1 == check2) // point1 is same as a neighbour of 2
					flag2=1;

				if(a_point2 == check1)
					flag2=1;

				if(count > 20){
					flag = 1;
					break;
				}
			} while(flag2 == 1);
		} while(flag==1);

		printf("a1 %d\t a2 %d\n", a_point1, a_point2);

		ranka1 = a_point1/host.num_nodes_local;
		ranka2 = a_point2/host.num_nodes_local;

		// set the final points chosen in a
		if(host.rank == ranka1){
			index3=0;
			while(a->neighbour[a_point1%host.num_nodes_local][index3] != b_point1)
				index3++;			
		}

		if(host.rank == ranka2){
			index4=0;
			while(a->neighbour[a_point2%host.num_nodes_local][index4] != b_point2)
				index4++;
		}

		MPI_Bcast(&index3, 1, MPI_INT, ranka1, MPI_COMM_WORLD);
		MPI_Bcast(&index4, 1, MPI_INT, ranka2, MPI_COMM_WORLD);

		//MPI_Barrier(MPI_COMM_WORLD);


		if(host.rank == ranka1){
			a->neighbour[a_point1%host.num_nodes_local][index3] = b_point2;
		}

		if(host.rank == ranka2){
			a->neighbour[a_point2%host.num_nodes_local][index4] = b_point1;
		}

		//MPI_Barrier(MPI_COMM_WORLD);


		if(host.rank == rankb1){
			b->neighbour[b_point1%host.num_nodes_local][b_index1] = a_point2;
		}

		if(host.rank == rankb2){
			b->neighbour[b_point2%host.num_nodes_local][b_index2] = a_point1;
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
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