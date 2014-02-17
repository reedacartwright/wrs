#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <cstdint>

#include <iostream>
#include <iomanip>

void r4_exp_setup ( int64_t ke[256], double fe[256], double we[256] );

using namespace std;

int main( int argc, const char* argv[] ) {
	int64_t k[256];
	double f[256];
	double w[256];
	r4_exp_setup(k,f,w);
	int i;
	
	cout << "extern const double ew[256] = {";
	i=0;
	for(auto u : w) {
		cout << ((i % 4 == 0) ? "\n\t" : " ");		
		cout << scientific << setprecision(16) << u;
		cout << ((i % 256 != 255) ? "," : "");
		i++;
	}
	cout << "\n};" << endl << endl;

	cout << "extern const double ef[256] = {";
	i=0;
	for(auto u : f) {
		cout << ((i % 4 == 0) ? "\n\t" : " ");		
		cout << scientific << setprecision(16) << u;
		cout << ((i % 256 != 255) ? "," : "");
		i++;
	}
	cout << "\n};" << endl << endl;
	
	
	cout << "#define N INT64_C\nextern const uint64_t ek[256] = {";
	i=0;
	for(auto u : k) {
		cout << ((i % 4 == 0) ? "\n\t" : " ");		
		cout << "N(0x" << setw(14) << setfill('0') << hex << uppercase << u << ")";
		cout << ((i % 256 != 255) ? "," : "");
		i++;
	}
	cout << "\n};\n#undef N" << endl;

	
}


void r4_exp_setup ( int64_t ke[256], double fe[256], double we[256] )
//****************************************************************************80
//
//  Purpose:
//
//    R4_EXP_SETUP sets data needed by R4_EXP.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 October 2013
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    George Marsaglia, Wai Wan Tsang,
//    The Ziggurat Method for Generating Random Variables,
//    Journal of Statistical Software,
//    Volume 5, Number 8, October 2000, seven pages.
//
//  Parameters:
//
//    Output, uint32_t KE[256], data needed by R4_EXP.
//
//    Output, float FE[256], WE[256], data needed by R4_EXP.
//
{
  double de = 7.69711747013104972;
  int i;
  const double m2 = 72057594037927936.0;
  double q;
  double te = 7.69711747013104972;
  const double ve = 0.0039496598225815571993;

  q = ve / exp ( - de );

  ke[0] = static_cast<int64_t> ( ( de / q ) * m2 );
  ke[1] = 0;

  we[0] = ( q / m2 );
  we[255] = ( de / m2 );

  fe[0] = 1.0;
  fe[255] = ( exp ( - de ) );

  for ( i = 254; 1 <= i; i-- )
  {
    de = - log ( ve / de + exp ( - de ) );
    ke[i+1] = static_cast<int64_t> ( ( de / te ) * m2 );
    te = de;
    fe[i] = ( exp ( - de ) );
    we[i] = ( de / m2 );
  }
  return;
}