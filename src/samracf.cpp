
#include <cstdlib>
#include <iostream>
#include <queue>
#include <vector>

#include "xorshift64.h"
#include "rexp.h"
#include "rbeta.h"

// implement reservoir as a vector
typedef std::vector<int> reservoir;

using namespace std;

int main( int argc, const char* argv[] ) {
	if(argc < 4) {
		cerr << "Usage: " << argv[0] << 
			" stream-size sample-size number-of-samples" << endl;
		return 1;
	}
	int stream_size = static_cast<int>(strtod(argv[1],NULL));
	int sample_size = static_cast<int>(strtod(argv[2],NULL));
	int sample_num = static_cast<int>(strtod(argv[3],NULL));
	
	// construct the main RNG
	xorshift64 rng(3294533022);
		
	for(int g=0;g<sample_num;++g) {
		// construct the stream RNG
		xorshift64 stream(2693652924);

		reservoir res;				
		// Algorithm RAC-F
		double w = stream.get_double52();
		res.assign(sample_size,0);
		
		double t = rand_gamma(rng,sample_size,1.0/w);
		double o = rand_exp(rng,t);
		
		for(int i=1;i<stream_size;++i) {
			w = stream.get_double52();
			while(o < w) {
				int j = static_cast<int>(sample_size*rng.get_double52());
				res[j] = i;
				t = t*rand_beta(rng,sample_size,1);
				w = w-o;
				o = rand_exp(rng,t);
			}
			o = o-w;		
		}
		cout << res[0] << endl;
	}
	return 0;
}
