#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<getopt.h>
#include<unistd.h>
#include<ctype.h>


//function declarations
void set_spins(int size, int * a_spin, int * b_spin);
void print_spins(int size, int * a_spin, int * b_spin);
int hamiltonian_local(int spin, int spin1, int spin2, int spin3);
int propose_spin_flip(double beta, int spin, int spin1, int spin2, int spin3);

int main(int argc, char * argv[]){


	srand48(time(NULL));

	int  i, j, k, num_points, num_iters, act, prod, fflag, bflag, iflag, opt;
	double beta, mag, time_taken;
	clock_t begin, end;
	FILE * graph;

	fflag=0;
	bflag=0;
	iflag=0;

	while ((opt = getopt(argc, argv, "f:b:i:")) != -1){
		switch (opt){
      
		case 'f':
			graph = fopen(optarg, "r");
			fflag=1;
			printf("%s\n", optarg);
			break;

		case 'b':
			beta = atof(optarg);
			bflag=1;
			break;

		case 'i':
			num_iters = atoi(optarg);
			iflag=1;
			break;

		case '?': 
			if(optopt=='f')
				printf("need a file as argument for -%c\n", optopt);
			else if(optopt=='b')
				printf("need an argument for -%c\n", optopt);
			else if(optopt=='i')
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

	if(fflag==0) graph=fopen("test_graph.txt", "r");
	if(bflag==0) beta=0.001;
	if (iflag==0) num_iters=1000;


	// READ GRAPH FROM FILE
		
	//graph = fopen(argv[1], "r");

	fscanf(graph, "%d\n", &num_points);
	
	printf("Points in each set: %d\n", num_points);

	int ** a = (int **)malloc(sizeof(int *)*num_points);
	int ** b = (int **)malloc(sizeof(int *)*num_points);

	for(i=0;i<num_points;i++){
		a[i] = (int *)malloc(sizeof(int)*3);
		b[i] = (int *)malloc(sizeof(int)*3);
	}

	// read in points from file
	for(i=0;i<num_points;i++){
		fscanf(graph, "%d;", &k);
		fscanf(graph, "%d,%d,%d\n", &a[k][0], &a[k][1], &a[k][2]);
	}

	for(i=0;i<num_points;i++){
		fscanf(graph, "%d;", &k);
		fscanf(graph, "%d,%d,%d\n", &b[k][0], &b[k][1], &b[k][2]);
	}

	fclose(graph);
	// FINISH READING GRAPH

	// PRINT GRAPH
	if(num_points<20){
		for(k=0;k<2;k++){
			printf("Set %d:\n", k);
			for(i=0;i<num_points;i++){
				printf("neighbours of point %d: ", i);
				for(j=0;j<3;j++){
					printf("%d ", a[i][j]);
				}
				printf("\n");
			}
		}
	}

	int * a_spin = (int *)malloc(sizeof(int)*num_points);
	int * b_spin = (int *)malloc(sizeof(int)*num_points);

	
	set_spins(num_points, a_spin, b_spin);
	//print_spins(num_points, a_spin, b_spin);

	double sum;

	// begin timing
	begin = clock();

	mag = 0.0;
	for(k=0;k<num_iters;k++){
		sum=0.0;
		//update a
		for(i=0;i<num_points;i++){
			a_spin[i] = propose_spin_flip(beta, a_spin[i], b_spin[a[i][0]], b_spin[a[i][1]], b_spin[a[i][2]]);
		}

		//update b
		for(i=0;i<num_points;i++){
			b_spin[i] = propose_spin_flip(beta, b_spin[i], a_spin[b[i][0]], a_spin[b[i][1]], a_spin[b[i][2]]);
		}
		//printf("Update %d:\n", k+1);
		//print_spins(num_points, a_spin, b_spin);
		//printf("\n");
			
		//calculate the magentisation
		//mag=0;
		for(i=0;i<num_points;i++){
			sum+=((double)a_spin[i]+(double)b_spin[i]);
		}
		sum=fabs(sum/(double)num_points);
		mag+=sum;		

		//printf("Magnetisation: %f\n", mag);

		/*
		//calculate the action
		act=0;
		for(i=0;i<num_points;i++){
			prod=0;
			for(j=0;j<3;j++){
				prod+=b_spin[a[i][j]];
			}
			act+=1-a_spin[i]*prod;
		}
		printf("Action: %d\n", act);
		*/
	}

	// end timing
	end = clock();

	// calculate time
	time_taken = (double)(end - begin)/CLOCKS_PER_SEC;	

	printf("beta: %lf; avg magnetisation %lf; time taken: %lf\n",beta,mag/(double) num_iters, time_taken);

	for(i=0;i<num_points;i++){
		free(a[i]);
		free(b[i]);
	}

	free(a);
	free(b);
	
	free(a_spin);
	free(b_spin);

	return 0;
}
