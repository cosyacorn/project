#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

void set_spins(int size, int * a_spin, int * b_spin);
void print_spins(int size, int * a_spin, int * b_spin);
int hamiltonian_local(int spin, int spin1, int spin2, int spin3);
int update_site(double beta, int spin, int spin1, int spin2, int spin3);

int main(int argc, char * argv[]){

	if(argc != 2){
		printf("\n ERROR: correct usage: %s filename\n", argv[0]);
		return 1;
	}else{
		srand48(time(NULL));
		// READ GRAPH FROM FILE
		FILE * graph;
		int ch, i, j, k, num_points, index1, index2, num_iters, act, prod;
		double beta, mag;

		beta=0.5;
		num_iters=100;

		graph = fopen(argv[1], "r");

		num_points=fgetc(graph)-48;
		//ascii code is read in. converted to int by subraacting 48
		printf("%d\n", num_points);

		int ** a = (int **)malloc(sizeof(int *)*num_points);
		int ** b = (int **)malloc(sizeof(int *)*num_points);

		for(i=0;i<num_points;i++){
			a[i] = (int *)malloc(sizeof(int)*3);
			b[i] = (int *)malloc(sizeof(int)*3);
		}
		ch=fgetc(graph); //skip the first '\n' character
		for(k=0;k<2;k++){
			for(i=0;i<num_points;i++){
				j=0;
				index2=0;
				do{
					ch=fgetc(graph);
					if(ch!=';' && ch!=',' && ch!='\n'){
						ch=ch-48;
						if(j==0) index1 = ch;
		
						if(j!=0){
							if(k==0) a[index1][index2] = ch;
							if (k==1) b[index1][index2] = ch;
							//printf("%d\n", ch);
							index2++;
						}
						j++;
					}
				if(ch==EOF) break;
				} while(ch!='\n');
			}
		}
		fclose(graph);
		// FINISH READING GRAPH

		// PRINT GRAPH
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

		int * a_spin = (int *)malloc(sizeof(int)*num_points);
		int * b_spin = (int *)malloc(sizeof(int)*num_points);

	
		set_spins(num_points, a_spin, b_spin);
		print_spins(num_points, a_spin, b_spin);


		for(k=0;k<num_iters;k++){
			//update a
			for(i=0;i<num_points;i++){
				a_spin[i] = update_site(beta, a_spin[i], b_spin[a[i][0]], b_spin[a[i][1]], b_spin[a[i][2]]);
			}

			//update b
			for(i=0;i<num_points;i++){
				b_spin[i] = update_site(beta, b_spin[i], a_spin[b[i][0]], a_spin[b[i][1]], a_spin[b[i][2]]);
			}
			printf("Update %d:\n", k+1);
			print_spins(num_points, a_spin, b_spin);
			//printf("\n");
			
			//calculate the magentisation
			mag=0;
			for(i=0;i<num_points;i++){
				mag+=((double)a_spin[i]+(double)b_spin[i]);
			}

			mag/=(double)num_points;
			printf("Magnetisation: %f\n", mag);

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
		}

		for(i=0;i<num_points;i++){
			free(a[i]);
			free(b[i]);
		}

		free(a);
		free(b);
	
		free(a_spin);
		free(b_spin);
	}


	return 0;
}

void set_spins(int size, int * a_spin, int * b_spin){

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


void print_spins(int size, int * a_spin, int * b_spin){

	int i;

	printf("Set 0 spins:\n");
	for(i=0;i<size;i++) printf("spin on point %d: %d\n", i, a_spin[i]);
	printf("Set 1 spins:\n");
	for(i=0;i<size;i++) printf("spin on point %d: %d\n", i, b_spin[i]);
}


int hamiltonian_local(int spin, int spin1, int spin2, int spin3){
	int H, a, b, c;
  
	if(spin==spin1)
		a=1;
	else
		a=0;
	if(spin==spin2)
		b=1;
	else
		b=0;
	if(spin==spin3)
		c=1;
	else
		c=0;

	H=-(a+b+c);
	return H;
}

int update_site(double beta, int spin, int spin1, int spin2, int spin3){
	int H, H_prop, prop;
	double r, n;

	H=hamiltonian_local(spin, spin1, spin2, spin3);
	prop=-spin;
	H_prop=hamiltonian_local(prop, spin1, spin2, spin3);

	//printf("%d %d\n",H, H_prop);

	if(H_prop<H){
		return prop;
	}
	else{
		n=drand48();
		r=exp((-2)*(beta)*(H_prop-H));
		if(r<n){
			return spin;
		}else{
			return prop;
		}
	}
}

