#include<stdio.h>
#include<stdlib.h>

int main(int argc, char * argv[]){

	int num_nodes, i, k;
	FILE * graph;
	graph=fopen("double_ring.txt", "w");


	if(argc==1) num_nodes=4;
	if(argc==2) num_nodes=atoi(argv[1]);
	if(argc>2) return 1;
	fprintf(graph, "%d\n", num_nodes);

	for(k=0;k<2;k++){
		for(i=0;i<num_nodes;i++){
			fprintf(graph, "%d;%d,%d,%d\n", i, (num_nodes+i-1)%num_nodes, i, (i+1)%num_nodes); 
		}
	}

	fclose(graph);
	return 0;
}
