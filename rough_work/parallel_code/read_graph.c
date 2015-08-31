#include<stdlib.h>
#include<stdio.h>


void read_graph(FILE *graph, int **a, int **b, int num_points){

	int i, k;

	graph = fopen("test_graph.txt", "r");
	printf("hello\n");
	fscanf(graph, "%d\n", &num_points);

	// read in points from file
	for(i=0;i<num_points;i++){
		fscanf(graph, "%d;", &k);
		fscanf(graph, "%d,%d,%d\n", &a[k][0], &a[k][1], &a[k][2]);
	}

	for(i=0;i<num_points;i++){
		fscanf(graph, "%d;", &k);
		fscanf(graph, "%d,%d,%d\n", &b[k][0], &b[k][1], &b[k][2]);
	}
	printf("hello\n");
	fclose(graph);
	// FINISH READING GRAPH
}
