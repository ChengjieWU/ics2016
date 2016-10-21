#include "trap.h"
#include "FLOAT.h"

FLOAT f(FLOAT x) { 
	/* f(x) = 1/(1+25x^2) */
	return F_div_F(int2F(1), int2F(1) + F_mul_int(F_mul_F(x, x), 25));
}

FLOAT computeT(int n, FLOAT a, FLOAT b, FLOAT (*fun)(FLOAT)) {
	int k;
	FLOAT s,h;
	h = F_div_int((b - a), n); //0.2
	//FLOAT ans = f2F(0.2);
	//nemu_assert(Fabs(h - ans) < f2F(1e-4));
	
	/*FLOAT ans = fun(a);
	FLOAT ans_correct = f2F(0.038462);
	nemu_assert(Fabs(ans - ans_correct) < f2F(1e-4));

	ans = fun(b);
	ans_correct = f2F(0.038462);
	nemu_assert(Fabs(ans - ans_correct) < f2F(1e-4));

	ans = fun(a) + fun(b);
	ans_correct = f2F(0.076923);
	nemu_assert(Fabs(ans - ans_correct) < f2F(1e-4));
	
	ans = F_div_int(fun(a) + fun(b), 2);
	ans_correct = f2F(0.038462);
	nemu_assert(Fabs(ans - ans_correct) < f2F(1e-4));

	ans = F_div_F(fun(a) + fun(b), int2F(2));
	ans_correct = f2F(0.038462);
	nemu_assert(Fabs(ans - ans_correct) < f2F(1e-4));
	*/

	s = F_div_int(fun(a) + fun(b), 2 );
	for(k = 1; k < n; k ++) {
		s += fun(a + F_mul_int(h, k));
		
		switch(k){
				case 1: {
								/*FLOAT x = a + F_mul_int(h, k);
								nemu_assert(Fabs(F_mul_F(x, x) - f2F(0.64)) < f2F(1e-4));
								x = F_mul_int(F_mul_F(x, x), 25);
								nemu_assert(Fabs(x - f2F(16)) < f2F(1e-4));
								x = x + int2F(1);
								nemu_assert(Fabs(x - f2F(17)) < f2F(1e-4));
								x = F_div_F(int2F(1), x);
								nemu_assert(Fabs(x - f2F(0.058824)) < f2F(1e-4));
								*/
								//F_div_F(int2F(1), int2F(1) + F_mul_int(F_mul_F(x, x), 25));
								//nemu_assert(Fabs(s - f2F(0.097286)) < f2F(1e-4));	
						}
				case 2:{
								nemu_assert(F_mul_int(h, k) == f2F(0.4));	
								//nemu_assert(Fabs(F_mul_int(h, k) - f2F(0.4)) < f2F(1e-4));	
								//FLOAT x = a + F_mul_int(h, k);
								//nemu_assert(Fabs(x - f2F(-0.6)) < f2F(1e-4));	
								//x = F_mul_int(F_mul_F(x, x), 25);
								//nemu_assert(Fabs(x - f2F(9)) < f2F(1e-4));
								//nemu_assert(Fabs(s - f2F(0.197286)) < f2F(1e-4));	
					   }
		}
		
	}
	s = F_mul_F(s, h);
	return s;
}

int main() { 
	//FLOAT a = computeT(10, f2F(-1.0), f2F(1.0), f);
	//FLOAT ans = f2F(0.551222);

	computeT(10, f2F(-1.0), f2F(1.0), f);
	//nemu_assert(Fabs(a - ans) < f2F(1e-4));

	return 0;
}
