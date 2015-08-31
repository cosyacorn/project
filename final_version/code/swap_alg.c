#include<stdlib.h>
#include<mpi.h>
#include "machine.h"
#include "array.h"

//===== PARALLEL SWAP ALG =====//
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
			if(host.rank == 0){
				a_first = (rand())%num_nodes; // first point in a picked at random
				ranka_first = a_first/host.num_nodes_local;
			}

			MPI_Bcast(&a_first, 1, MPI_INT, 0, MPI_COMM_WORLD);
			MPI_Bcast(&ranka_first, 1, MPI_INT, 0, MPI_COMM_WORLD);

			if(host.rank == ranka_first){
				index_n1 = rand()%3; // index of first neighbour
				index_n2 = rand()%3; // index of sencond neighbour

				while(index_n1 == index_n2)
					index_n2 = rand()%3; // ensure indices don't match

				// find points in set b connected to point 'index' in a
				b_point1 = a->neighbour[a_first%host.num_nodes_local][index_n1]; // point1 in b
				b_point2 = a->neighbour[a_first%host.num_nodes_local][index_n2]; // point2 in b

				rankb1 = b_point1/host.num_nodes_local;
				rankb2 = b_point2/host.num_nodes_local;
			}

			MPI_Bcast(&b_point1, 1, MPI_INT, ranka_first, MPI_COMM_WORLD);
			MPI_Bcast(&b_point2, 1, MPI_INT, ranka_first, MPI_COMM_WORLD);

			MPI_Bcast(&rankb1, 1, MPI_INT, ranka_first, MPI_COMM_WORLD);
			MPI_Bcast(&rankb2, 1, MPI_INT, ranka_first, MPI_COMM_WORLD);
			
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

					ranka1 = a_point1/host.num_nodes_local;
				}

				if(host.rank == rankb2){
					b_index2 = rand()%3;
					while(b_index2 == index2) // ensure that we don't pick a_first
						b_index2 = rand()%3;

					a_point2  = b->neighbour[b_point2%host.num_nodes_local][b_index2];
					check2 = b->neighbour[b_point2%host.num_nodes_local][3-(index2+b_index2)];

					ranka2 = a_point2/host.num_nodes_local;
				}

				MPI_Bcast(&b_index1, 1, MPI_INT, rankb1, MPI_COMM_WORLD);
				MPI_Bcast(&b_index2, 1, MPI_INT, rankb2, MPI_COMM_WORLD);

				MPI_Bcast(&a_point1, 1, MPI_INT, rankb1, MPI_COMM_WORLD);
				MPI_Bcast(&check1, 1, MPI_INT, rankb1, MPI_COMM_WORLD);

				MPI_Bcast(&a_point2, 1, MPI_INT, rankb2, MPI_COMM_WORLD);
				MPI_Bcast(&check2, 1, MPI_INT, rankb2, MPI_COMM_WORLD);

				MPI_Bcast(&ranka1, 1, MPI_INT, rankb1, MPI_COMM_WORLD);
				MPI_Bcast(&ranka2, 1, MPI_INT, rankb2, MPI_COMM_WORLD);

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
				MPI_Barrier(MPI_COMM_WORLD);
			} while(flag2 == 1);
			MPI_Barrier(MPI_COMM_WORLD);
		} while(flag==1);

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

		if(host.rank == ranka1){
			a->neighbour[a_point1%host.num_nodes_local][index3] = b_point2;
		}

		if(host.rank == ranka2){
			a->neighbour[a_point2%host.num_nodes_local][index4] = b_point1;
		}

		if(host.rank == rankb1){
			b->neighbour[b_point1%host.num_nodes_local][b_index1] = a_point2;
		}

		if(host.rank == rankb2){
			b->neighbour[b_point2%host.num_nodes_local][b_index2] = a_point1;
		}
	MPI_Barrier(MPI_COMM_WORLD);
	}
}
