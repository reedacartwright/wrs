
#include <cstdlib>
#include <iostream>
#include <queue>

#include "xorshift64.h"
#include "rexp.h"


// implement reservoir as a heap of pairs
typedef std::pair<double,int> element;
typedef std::priority_queue<element> reservoir;

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
		// Algorithm RAC-A
		double w = stream.get_double52();
		for(int i=0;i<sample_size;++i) {
			res.emplace(rand_exp(rng,w),0);
		}
		for(int i=1;i<stream_size;++i) {
			w = stream.get_double52();
			double v = rand_exp(rng,w);
			while(v < res.top().first ) {
				res.pop();
				res.emplace(v,i);
				v += rand_exp(rng,w);
			}
		}
		cout << res.top().second << endl;
	}
	return 0;
}
