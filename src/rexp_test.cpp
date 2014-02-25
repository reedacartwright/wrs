
extern "C" {
#include <unif01.h>
#include <bbattery.h>
}

#include <vector>

#include "xorshift64.h"
#include "rexp.h"

using namespace std;

xorshift64 rng(3294533022);

double rexp_inv(void) {
	double e = rand_exp(rng);
	//double e = rand_exp_inv(rng);
	return exp(-e);
}

int main( int argc, const char* argv[] ) {
	unif01_Gen *gen;
	gen = unif01_CreateExternGen01("RANDEXP", rexp_inv);
	bbattery_SmallCrush(gen);
	//bbattery_Crush(gen);
	unif01_DeleteExternGen01(gen);
	
	return 0;
}
