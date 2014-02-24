
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
	if(argc < 6) {
		cerr << "Usage: " << argv[0] << 
			" stream-size sample-size number-of-samples stream-seed rng-seed" << endl;
		return 1;
	}
	int64_t stream_size = static_cast<int64_t>(strtod(argv[1],NULL));
	int64_t sample_size = static_cast<int64_t>(strtod(argv[2],NULL));
	int64_t sample_num = static_cast<int64_t>(strtod(argv[3],NULL));
	uint64_t stream_seed = static_cast<uint64_t>(strtoull(argv[4],NULL,0));
	uint64_t rng_seed = static_cast<uint64_t>(strtoull(argv[5],NULL,0));
		
	// construct the main RNG
	xorshift64 rng(rng_seed); //3294533022
		
	for(int64_t g=0;g<sample_num;++g) {
		// construct the stream RNG
		xorshift64 stream(stream_seed); //2693652924

		reservoir res(sample_size,0);
						
		// Algorithm WRMS-VTU
		double w = stream.get_double52();
		
		double t = rand_gamma(rng,sample_size,1.0/w);
		double o = rand_exp(rng,t);
		
		for(int64_t i=1;i<stream_size;++i) {
			w = stream.get_double52();
			while(o < w) {
				int j = rng.get_uint(sample_size);
				res[j] = i;
				//t *= rand_beta(rng,sample_size,1);
				//t *= exp(-rand_exp(rng,sample_size));
				t *= 1.0-rand_exp(rng,sample_size);
				w = w-o;
				o = rand_exp(rng,t);
			}
			o = o-w;		
		}
		cout << res.front() << endl;
	}
	return 0;
}
