
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
		res.reserve(sample_size);

		// Algorithm WRMS-HTW
		double w = stream.get_double52();
		double v = 0.0;
		for(int i=0;i<sample_size;++i) {
			v += rand_exp(rng,w);
			res.emplace_back(v,0);
		}
		make_heap(res.begin(),res.end());
		
		double t = res.front().first;
		double o = rand_exp(rng,t);
		
		for(int i=1;i<stream_size;++i) {
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
