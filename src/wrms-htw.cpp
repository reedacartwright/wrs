
#include <cstdlib>
#include <iostream>
#include <queue>

#include "xorshift64.h"
#include "rexp.h"

// implement reservoir as a heap of pairs
typedef std::pair<double,int> element;
typedef std::vector<element> reservoir;

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

		reservoir res(sample_size,make_pair(0.0,0));

		// Algorithm WRMS-HTW
		double w = stream.get_double52();
		double v = 0.0;
		for(int64_t i=sample_size;i>0;--i) {
			v += rand_exp(rng,w);
			res[i-1].first = v;
		}
		
		double t = res.front().first;
		double o = rand_exp(rng,t);
		
		for(int64_t i=1;i<stream_size;++i) {
			w = stream.get_double52();
			v = 0.0;
			while(o < w) {
				v = v+(t-v)*o/w;		
				pop_heap(res.begin(),res.end());
				res.back().first = v;
				res.back().second = i;
				push_heap(res.begin(),res.end());
				t = res.front().first;
				w = w*(t-v)/t;
				o = rand_exp(rng,t);				
			}
			o -= w;
		}
				
		cout << res.front().second << endl;
	}
	return 0;
}
