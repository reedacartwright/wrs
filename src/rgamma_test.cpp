extern "C" {
#include <unif01.h>
#include <bbattery.h>
}

#include <gsl/gsl_cdf.h>

#include <cmath>
#include <iostream>

#include "xorshift64.h"
#include "rexp.h"
#include "rgamma.h"

xorshift64 rng(3294533021);

double rgamma_inv(void) {
	double x = rand_gamma(rng,100,1.0);
	return gsl_cdf_gamma_P(x,100,1.0);
}

int main( int argc, const char* argv[] ) {
	unif01_Gen *gen;
	gen = unif01_CreateExternGen01("RANDGAMMA", rgamma_inv);

	if(argc < 2 || argv[1][0] == 's')
		bbattery_SmallCrush(gen);
	else if(argv[1][0] == 'm')
		bbattery_Crush(gen);
	else if(argv[1][0] == 'b')
		bbattery_BigCrush(gen);
	else {
		std::cerr << "Unknown Test" << std::endl;
		unif01_DeleteExternGen01(gen);
		return 1;
	}

	unif01_DeleteExternGen01(gen);
	
	return 0;
}
