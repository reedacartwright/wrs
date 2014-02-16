#pragma once
#ifndef RGAMMA_H
#define RGAMMA_H

#include "xorshift64.h"
#include "rnormal.h"

/* New version based on Marsaglia and Tsang, "A Simple Method for
 * generating gamma variables", ACM Transactions on Mathematical
 * Software, Vol 26, No 3 (2000), p363-372.
 *
 * Implemented by J.D.Lamb@btinternet.com, minor modifications for GSL
 * by Brian Gough
 *
 * Imported from GSL's gamma.c.
 */
inline double rand_gamma(xorshift64 &rng, double a, double b) {
	if(a < 1.0)
		return rand_gamma(rng,1.0+a,b)*pow(rng.get_double52(), 1.0/a);

	double x, v, u;
	double d = a - 1.0/3.0;
	double c = (1.0/3.0)/sqrt(d);

	for(;;) {
		do {
			x = rand_normal(rng,0.0,1.0);
			v = 1.0 + c * x;
		} while (v <= 0);
		v = v * v * v;
		u = rng.get_double52();
		if (u < 1 - 0.0331 * x * x * x * x) 
			break;
		if (log(u) < 0.5 * x * x + d * (1 - v + log(v)))
			break;
	}
	return b*d*v;
}

#endif // RGAMMA_H

