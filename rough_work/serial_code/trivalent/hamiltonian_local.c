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
