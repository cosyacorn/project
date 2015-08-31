#include<stdlib.h>
#include<math.h>

int hamiltonian_local(int spin, int spin1, int spin2, int spin3);

int propose_spin_flip(double beta, int spin, int spin1, int spin2, int spin3){
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
