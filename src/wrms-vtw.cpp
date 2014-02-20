
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
					
		// Algorithm WRMS-VTW
		double w = stream.get_double52();
		res.assign(sample_size,0);
		
		double t = rand_gamma(rng,sample_size,1.0/w);
		double o = rand_exp(rng,t);
		
		for(int i=1;i<stream_size;++i) {
			w = stream.get_double52();
			double v = 0.0;
			int n = sample_size;
			while(o < w) {
				// Use a partial Fisher-Yates shuffle to identify elements
				// to replace.
				int j = rng.get_uint(n);
				n = n-1;
				res[j] = res[n];
				res[n] = i;
				
				// Calculate the emission time based on the horizontal
				// position of the emission
				v = v+(t-v)*o/w;
				// Simulate the next max-height in the reservoir and
				// check to see if it is above or below the emission time.
				if(n == 0 || (t = t*rand_beta(rng,n,1)) < v ) {
					t = v;
				}
				
				// Update weight and simulate next step
				w = w*(t-v)/t;
				o = rand_exp(rng,t);
				n = n-1;
			}
			o = o-w;		
		}
		cout << res.back() << endl;
	}
	return 0;
}
