#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<getopt.h>
#include<unistd.h>
#include<ctype.h>


int main(int argc, char * argv[]){

	
	
	int num_nodes, i, index, r, flag, count, temp1, num[3], c, d, opt, nflag;
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
	int * first = (int *)malloc(sizeof(int)*num_nodes);
	int * second = (int *)malloc(sizeof(int)*num_nodes);
	int * third = (int *)malloc(sizeof(int)*num_nodes);

	// begin timing
	begin = clock();

	for(i=0;i<num_nodes;i++){
		a[i] = (int *)malloc(sizeof(int)*3);

		first[i] = i;
		second[i] = i;
		third[i] = i;
	}
		
	//set first neighbour
	for(i=0;i<num_nodes;i++){
		flag=0;
		count=0;
		while(flag==0){
			r=first[index=rand()%num_nodes];
			if(r!=-1){
				a[i][0]=r;
				first[index]=-1;
				flag=1;
			}
		}
	}

	//set second neighbour
	for(i=0;i<num_nodes;i++){
		flag=0;
		count=0;
		while(flag==0){
			r=second[index=rand()%num_nodes];
			if(r!=-1 && r!=a[i][0]){
				a[i][1]=r;
				second[index]=-1;
				flag=1;
				if(i==num_nodes-2) temp1=r;
			}
			//if r equals the last first neighbour entry
			if(i==(num_nodes-1) && r==a[i][0]){
				a[i-1][1]=a[i][0];
				a[i][1]=temp1;
				flag=1;	
			}
		}
	}


	//set third neighbour
	for(i=0;i<num_nodes-3;i++){
		flag=0;
		while(flag==0){
			r=third[index=rand()%num_nodes];
			if(r!=-1 && r!=a[i][0] && r!=a[i][1]){
				a[i][2]=r;
				third[index]=-1;
				flag=1;
			}		
		}
	}


	count=0;
	for(i=0;i<num_nodes;i++){
		if(third[i]!=-1 && count==0){
			num[0]=third[i];
			count++;
			third[i]=-1;
		}
		if(third[i]!=-1 && count==1){
			num[1]=third[i];
			count++;
			third[i]=-1;
		}
		if(third[i]!=-1 && count==2){
			num[2]=third[i];
			count++;
			third[i]=-1;
		}
	}

	flag=0;
	while(flag==0){

		a[num_nodes-3][2] = num[rand()%3];
		//printf("%d \n", a[num_nodes-3][2]);

		c=num[rand()%3];
		a[num_nodes-2][2] = c;
		while(c == a[num_nodes-3][2]){
			c=num[rand()%3];
			a[num_nodes-2][2] = c;
		}

		d=num[rand()%3];
		a[num_nodes-1][2] = d;
		while(d == a[num_nodes-3][2] || d == a[num_nodes-2][2]){
			d=num[rand()%3];
			a[num_nodes-1][2] = d;

		}
		if(a[num_nodes-3][2] != a[num_nodes-3][1] && a[num_nodes-3][2] != a[num_nodes-3][0] &&
			a[num_nodes-2][2] != a[num_nodes-2][1] && a[num_nodes-2][2] != a[num_nodes-2][0] &&
			a[num_nodes-1][2] != a[num_nodes-1][1] && a[num_nodes-1][2] != a[num_nodes-1][0]){
			flag=1;
			} else flag=0;
	}
	
	// end timing
	end = clock();

	// calculate time
	time_taken = (double)(end - begin)/CLOCKS_PER_SEC;	
	
	//print graph
	if(num_nodes<10){
		for(i=0;i<num_nodes;i++){
			printf("%d;%d,%d,%d\n", i, a[i][0], a[i][1], a[i][2]);
		}
	}

	printf("Time to execute: %f\n", time_taken);

	//clean up time
	for(i=0;i<num_nodes;i++)
		free(a[i]);
	free(a);
	free(first);
	free(second);
	free(third);

	
	return 0;
}
