#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <cstdint>

#include <iostream>
#include <iomanip>

void r4_exp_setup ( int64_t ke[256], double fe[256], double we[256] );

using namespace std;

/* ziggurat.c - construct the tables for "normal.c"
 *
 * For details see the article
 * George Marsaglia, Wai Wan Tsang
 * The Ziggurat Method for Generating Random Variables
 * Journal of Statistical Software, vol. 5 (2000), no. 8
 * http://www.jstatsoft.org/v05/i08/
 *
 * Copyright (C) 2005  Jochen Voss.
 *
 * $Id: ziggurat.c 6509 2005-07-07 18:31:10Z voss $
 */

#define LEVELS 256

/* the target density */
double f (double x) {
	return  exp(-x*x/2);
}

/* the inverse of the target density */
double f_inv (double y) {
	return  sqrt(-2*log(y));
}

double  x[LEVELS];
double  v;

double try_r_value (double r) {
	int  i;

	v = r*f(r) + exp(-0.5*r*r)/r;
	x[LEVELS-1] = r;
	for (i=LEVELS-1; i>1; --i) {
		x[i-1] = f_inv(v/x[i]+f(x[i]));
	}
	return  x[1]*(1-f(x[1]))/v;
}


int main( int argc, const char* argv[] ) {
	double  a, b, aa, bb, r;
	int  i;

	a=0;
	b=10;
	do {
		double  q;
		aa=a, bb=b;
		r=0.5*(a+b);
		q=try_r_value(r);
		if (q>1) {
			b = r;
		} else {
			a = r;
		}
	} while (aa<r && r<bb);
	x[0] = 0;
	
	cout << "const int levels = " << LEVELS << ";\n";
	cout << "const double R = " << scientific << setprecision(16) << r << ";\n\n";
	
	cout << "extern const double ytab[256] = {";
	i=0;
	for(auto u : x) {
		cout << ((i % 4 == 0) ? "\n\t" : " ");		
		cout << scientific << setprecision(16) << f(u);
		cout << ((i % 256 != 255) ? "," : "");
		i++;
	}
	cout << "\n};" << endl << endl;
	
	cout << "#define N INT64_C\nextern const uint64_t ktab[256] = {";
	for(i=0; i < LEVELS; ++i) {
		cout << ((i % 4 == 0) ? "\n\t" : " ");
		if(i < LEVELS-1) {
			cout << "N(0x" << setw(16) << setfill('0') << hex << uppercase <<
				(static_cast<int64_t>(9223372036854775808.0*x[i]/x[i+1])) << ")";
		} else {
			cout << "N(0x" << setw(16) << setfill('0') << hex << uppercase <<
				(static_cast<int64_t>(9223372036854775808.0*r*f(r)/v)) << ")";
		}
			
		cout << ((i % 256 != 255) ? "," : "");
	}
	cout << "\n};\n#undef N" << endl;
	
	cout << "extern const double wtab[256] = {";
	for(i=0; i < LEVELS; ++i) {
		cout << ((i % 4 == 0) ? "\n\t" : " ");
		if(i < LEVELS-1) {
			cout << scientific << setprecision(16) <<
				((x[i+1]/9223372036854775808.0));
		} else {
			cout << scientific << setprecision(16) <<
				((v/(9223372036854775808.0*f(r))));
		}
			
		cout << ((i % 256 != 255) ? "," : "");
	}
	cout << "\n};" << endl;	
}
