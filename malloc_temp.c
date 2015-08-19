#include<stdio.h>
#include<stdlib.h>

int main(){

	int ** a = (int **) malloc(sizeof(int *)*2);
	int b[2];
	int i;


	b[0]=0;
	b[1]=1;

	for(i=0;i<2;i++){
		if(b[i]==0){
			a[i] = NULL;
		} else{
			a[i] = (int *) malloc(sizeof(int)*b[i]);
		}
	}

	a[1][0] = 123;

	printf("%d\n", a[1][0]);

	for(i=0;i<2;i++)
		free(a[i]);

	free(a);

	return 0;
}
