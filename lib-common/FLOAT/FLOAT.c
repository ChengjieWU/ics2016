#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	return (FLOAT)(((long long)a * b) >> 16);
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	/* Dividing two 64-bit integers needs the support of another library
	 * `libgcc', other than newlib. It is a dirty work to port `libgcc'
	 * to NEMU. In fact, it is unnecessary to perform a "64/64" division
	 * here. A "64/32" division is enough.
	 *
	 * To perform a "64/32" division, you can use the x86 instruction
	 * `div' or `idiv' by inline assembly. We provide a template for you
	 * to prevent you from uncessary details.
	 *
	 *     asm volatile ("??? %2" : "=a"(???), "=d"(???) : "r"(???), "a"(???), "d"(???));
	 *
	 * If you want to use the template above, you should fill the "???"
	 * correctly. For more information, please read the i386 manual for
	 * division instructions, and search the Internet about "inline assembly".
	 * It is OK not to use the template above, but you should figure
	 * out another way to perform the division.
	 */
	FLOAT high = a >> 16;
	FLOAT low = a << 16;
	FLOAT quotient, remainder;
	asm volatile ("idiv %2" : "=a"(quotient), "=d"(remainder) : "r"(b), "a"(low), "d"(high));
	return quotient;
}

FLOAT f2F(float a) {
	/* You should figure out how to convert `a' into FLOAT without
	 * introducing x87 floating point instructions. Else you can
	 * not run this code in NEMU before implementing x87 floating
	 * point instructions, which is contrary to our expectation.
	 *
	 * Hint: The bit representation of `a' is already on the
	 * stack. How do you retrieve it to another variable without
	 * performing arithmetic operations on it directly?
	 */
	union {
		float* fp;
		FLOAT* FP;
	} uio;
	uio.fp = &a;
	FLOAT F = *uio.FP;
	int disp = ((F >> 23) & 0xff) - 134;
	unsigned m = F & 0x007fffff;
	m = m | 0x00800000;
	unsigned sym = !(F & 0x80000000);
	if (disp < 0) m = m >> (-disp);
	else m = m << disp;
	if (sym) F = m;
	else F = ~m + 1;
	return F;
}

FLOAT Fabs(FLOAT a) {
	if (a & 0x80000000) a = ~a + 1;
	return a;
}

/* Functions below are already implemented */

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

