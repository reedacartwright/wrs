#include <cstdint>

#include "xorshift64.h"

/*
 *     Ziggurat method as implemented in GSL
 *     George Marsaglia, Wai Wan Tsang
 *     The Ziggurat Method for Generating Random Variables
 *     Journal of Statistical Software, vol. 5 (2000), no. 8
 *     http://www.jstatsoft.org/v05/i08/
 */
 
extern const double ytab[128];
extern const uint32_t ktab[128];
extern const double wtab[128];
 
/* position of right-most step */
#define PARAM_R 3.44428647676

double rand_normal(xorshift64 &rng, double mu, double sigma) {
	uint32_t i, j;
	int sign;
	double x, y;

	for(;;) {
		uint32_t k = rng.get_uint32();
		i = (k & 0xFF);
		j = (k >> 8) & 0xFFFFFF;

		sign = (i & 0x80) ? +1 : -1;
		i &= 0x7f;

		x = j * wtab[i];

		if (j < ktab[i])
			break;

		if(i < 127) {
			double y0 = ytab[i], y1 = ytab[i + 1];
			double U1 = rng.get_double52();
			y = y1 + (y0 - y1) * U1;
		} else {
			double U1 = rng.get_double52();
			double U2 = rng.get_double52();
			x = PARAM_R - log(U1) / PARAM_R;
			y = exp(-PARAM_R * (x - 0.5 * PARAM_R)) * U2;
		}

		if (y < exp(-0.5 * x * x))
			break;
    }
	return mu + sign * sigma * x;	
}

