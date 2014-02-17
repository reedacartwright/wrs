#include <cstdint>

#include "xorshift64.h"
#include "rexp.h"

/*
 *     Ziggurat method as implemented in GSL
 *     George Marsaglia, Wai Wan Tsang
 *     The Ziggurat Method for Generating Random Variables
 *     Journal of Statistical Software, vol. 5 (2000), no. 8
 *     http://www.jstatsoft.org/v05/i08/
 */
 
extern const double ytab[256];
extern const uint32_t ktab[256];
extern const double wtab[256];
 
/* position of right-most step */
// #define PARAM_R 3.44428647676

/*
double rand_normal_org(xorshift64 &rng, double mu, double sigma) {
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
*/

double rand_normal(xorshift64 &rng, double mu, double sigma) {
	const double R = 3.6554204190269415;
	double x, y;
	
	for(;;) {
		uint64_t u = rng.get_uint64();
		// use the top 8 high bits for b
		uint64_t b = u >> 56;
		// use the rest for a
		int64_t a = static_cast<int64_t>(u << 8); 
		
		x = a * wtab[b];

		if (abs(a) < ktab[b])
			break;

		if(b < 255) {
			y = ytab[b + 1] + (ytab[b] - ytab[b + 1]) * rng.get_double52();
		} else {
			double U1 = 2.0*rng.get_double53() - (1.0-(DBL_EPSILON/2.0));
			double U2 = rng.get_double52();
			x = R - log(abs(U1)) / R;
			y = exp(-R * (x - 0.5 * R)) * U2;
			x = copysign(x, U1);
		}

		if (y < exp(-0.5 * x * x))
			break;
    }
	return mu + sigma * x;
}

