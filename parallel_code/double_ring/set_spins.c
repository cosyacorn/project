#include<stdlib.h>

void set_spins(int size, int a_spin[], int b_spin[]){

	int i, spin1, spin2;
	double a, b;

	for(i=0;i<size;i++){
		a=drand48();
		b=drand48();
		
		if(a>0.5){
			spin1=1;
		}
		else{
			spin1=-1;
		}

		if(b>0.5){
			spin2=1;
		}
		else{
			spin2=-1;
		}
		

		a_spin[i]=spin1;
		b_spin[i]=spin2;
	}

}
