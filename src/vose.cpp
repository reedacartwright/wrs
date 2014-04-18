/* Algorithm VOSE
 * Features: table alias method to draw values
 *
 * Copyright (C) 2014 Reed A. Cartwright <reed@cartwrig.ht>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include <cstdlib>
#include <iostream>
#include <queue>
#include <vector>

#include "xorshift64.h"
#include "rexp.h"
#include "rbeta.h"
#include "aliastable.h"

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
		res.resize(sample_size);
		// setup a table to hold cummulative weights
		std::vector<double> table;

		// Algorithm STD-B
		// First Pass
		double totalw = 0.0;
		for(int64_t i=0;i<stream_size;++i) {
			totalw += stream.get_double52();
			table.push_back(totalw);
		}
		alias_table at;
		at.create_inplace(table);
		
		// Second Pass, reset stream
		stream.seed(stream_seed);
		
		for(int64_t i=0;i<sample_size;++i) {
			res[i] = at.get(rng.get_uint64());
		}
		cout << res.back() << endl;
	}
	return 0;
}
