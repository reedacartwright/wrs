
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

		reservoir res;
		res.reserve(sample_size);
		// setup a table to hold cummulative weights
		std::vector<double> table;

		// Algorithm STD-B
		// First Pass
		double totalw = 0.0;
		for(int64_t i=0;i<stream_size;++i) {
			totalw += stream.get_double52();
			table.push_back(totalw);
		}
		
		// Second Pass, reset stream
		stream.seed(2693652924);
		
		for(int64_t i=0;i<sample_size;++i) {
			double w = totalw*rng.get_double52();
			auto it = lower_bound(table.begin(),table.end(),w);
			res.push_back(it-table.begin());
		}
		cout << res.back() << endl;
	}
	return 0;
}
