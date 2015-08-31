#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<unistd.h>
#include<ctype.h>

int main(int argc, char * argv[]){

	int num_nodes, i, k, nflag, opt;
	FILE * graph;
	graph=fopen("double_ring.txt", "w");

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


	if(nflag==0) num_nodes=4;
	
	fprintf(graph, "%d\n", num_nodes);

	for(k=0;k<2;k++){
		for(i=0;i<num_nodes;i++){
			fprintf(graph, "%d;%d,%d,%d\n", i, (num_nodes+i-1)%num_nodes, i, (i+1)%num_nodes); 
		}
	}

	fclose(graph);
	return 0;
}
