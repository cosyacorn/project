#include<stdio.h>
#include<stdlib.h>
#include<time.h>


int main(int argc, char * argv[]){

	if(argc != 2){
		printf("\n ERROR! correct usage: %s <integer>\n", argv[0]);
		return 1;
	}else{
	
		int num_nodes, i, index;
		srand(time(NULL));

		num_nodes=atoi(argv[1]);

		int ** a = (int **)malloc(sizeof(int *)*num_nodes);
		int ** b = (int **)malloc(sizeof(int *)*num_nodes);
		int * first = (int *)malloc(sizeof(int)*num_nodes);
		int * second = (int *)malloc(sizeof(int)*num_nodes);
		int * third = (int *)malloc(sizeof(int)*num_nodes);

		for(i=0;i<num_nodes;i++){
			a[i] = (int *)malloc(sizeof(int)*3);
			b[i] = (int *)malloc(sizeof(int)*3);

			first[i] = i;
			second[i] = i;
			third[i] = i;
		}

		int r, flag, count, temp1, temp2;
		
		//set first neighbour
		for(i=0;i<num_nodes;i++){
			flag=0;
			count=0;
			while(flag==0){
				r=first[index=rand()%num_nodes];
				if(r!=-1){
					a[i][0]=r;
					//printf("%d ", r);
					first[index]=-1;
					flag=1;
					//count++;
					//if(count==2) break;
				}
			}
		}
		for(i=0;i<num_nodes;i++) printf("%d", a[i][0]);
		printf("\n");

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

		for(i=0;i<num_nodes;i++) printf("%d", a[i][1]);
		printf("\n");

		int case1, case2;
		//set third neighbour
		for(i=0;i<num_nodes-3;i++){
			//flag=0;
			//count=0;
			while(flag==0){
				r=third[index=rand()%num_nodes];
				//count++;
				//printf("%d ", r);
				if(r!=-1 && r!=a[i][0] && r!=a[i][1]){
					a[i][2]=r;
					//printf("%d ", r);
					third[index]=-1;
					flag=1;
					//if(i==num_nodes-3) temp1=r;
					//if(i==num_nodes-2) temp2=r;
				}
				/*if(i==num_nodes-2 && count > 20 && r!=-1){
					printf("swap0\n");
					a[i-1][2]=r;
					a[i][2]=temp1;
					temp1=r;
					flag=1;
				}
				if(i==num_nodes-1 && count>20 && r!=-1){
					if(r!=a[i-2][0] && r!=a[i-2][1]){
						printf("swap1\n");
						a[i-2][2]=r;
						a[i][2]=temp1;
						flag=1;
					} else if(r!=a[i-1][0] && r!=a[i-1][1]){
						printf("swap2\n");
						a[i-1][2]=r;
						a[i][2]=temp2;
						flag=1;
					}
				}*/
			}
		}
		int num1, num2, num3;
		count=0;
		for(i=0;i<num_nodes;i++){
			if(third[i]!=-1 && count==0){
				num1=third[i];
				count++;
				third[i]=-1;
			}
			if(third[i]!=-1 && count==1){
				num2=third[i];
				count++;
				third[i]=-1;
			}
			if(third[i]!=-1 && count==2){
				num3=third[i];
				count++;
				third[i]=-1;
			}
		}
		int yes1[3], yes2[3], yes3[3];
		for(i=num_nodes-3;i<num_nodes;i++){
			if(num1!=a[i][0] && num1!=a[i][1]){
				yes1[i-num_nodes+3]=1;
			} else yes1[i-num_nodes+3]=0;
			if(num2!=a[i][0] && num2!=a[i][1]){
				yes2[i-num_nodes+3]=1;
			} else yes2[i-num_nodes+3]=0;
			if(num3!=a[i][0] && num3!=a[i][1]){
				yes3[i-num_nodes+3]=1;
			} else yes3[i-num_nodes+3]=0;
		}

		//printf("\n");
		//print graph
		for(i=0;i<num_nodes;i++){
			printf("%d: %d, %d, %d\n", i, a[i][0], a[i][1], a[i][2]);
		}


		//clean up time
		for(i=0;i<num_nodes;i++){
			free(a[i]);
			free(b[i]);
		}
		free(a);
		free(b);
		free(first);
		free(second);
		free(third);

	}
	return 0;
}
