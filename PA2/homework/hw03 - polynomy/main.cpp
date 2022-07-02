#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#if defined ( __cplusplus ) && __cplusplus > 199711L /* C++ 11 */
#include <memory>
#endif /* C++ 11 */
using namespace std;
#endif /* __PROGTEST__ */
// --------------------------------------------------------------------------------------------- //

#include "progtest.cpp"

// --------------------------------------------------------------------------------------------- //
#ifndef __PROGTEST__
bool smallDiff ( double a, double b ) {
  return abs( a - b ) < 0.0001;
}

bool dumpMatch ( const CPolynomial & x, const vector<double> & ref ) { 
  for ( size_t i = 0; i < ref.size(); ++i )
    if ( ! smallDiff( x[i], ref[i] ) )
      return false;

  return true;
}

int main ( ) {
  CPolynomial a, b, c;
  ostringstream out;

  a[0] = -10;
  a[1] = 3.5;
  a[3] = 1;
  assert ( smallDiff ( a ( 2 ), 5 ) );
  out . str ("");
  out << a;
  assert ( out . str () == "x^3 + 3.5*x^1 - 10" );
  a = a * -2;
  assert ( a . Degree () == 3
           && dumpMatch ( a, vector<double>{ 20.0, -7.0, -0.0, -2.0 } ) );

  out . str ("");
  out << a;

  //b = c = a;
  //b[0] = 6;
  //b[3] = 7;
  //c[2] = 99;
  //a = 2 * b + c + 9;
  //c = c + 9;
  //cout << "A: " << a << endl;
  //cout << "B: " << b << endl;
  //cout << "C: " << c << endl;

  //CPolynomial d(-2);
  //d[5] = 9;
  //cout << d << endl;

  //cout << a << endl;
  //cout << a * (-2) + a << endl;
  //cout << a << endl;

  assert ( out . str () == "- 2*x^3 - 7*x^1 + 20" );
  out . str ("");
  out << b;
  assert ( out . str () == "0" );
  b[5] = -1;
  out . str ("");
  out << b;
  assert ( out . str () == "- x^5" );
  c = a + b;
  assert ( c . Degree () == 5
           && dumpMatch ( c, vector<double>{ 20.0, -7.0, 0.0, -2.0, 0.0, -1.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "- x^5 - 2*x^3 - 7*x^1 + 20" );
  c = a - b;
  assert ( c . Degree () == 5
           && dumpMatch ( c, vector<double>{ 20.0, -7.0, -0.0, -2.0, -0.0, 1.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "x^5 - 2*x^3 - 7*x^1 + 20" );
  c = a * b;
  assert ( c . Degree () == 8
           && dumpMatch ( c, vector<double>{ -0.0, -0.0, 0.0, -0.0, 0.0, -20.0, 7.0, -0.0, 2.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "2*x^8 + 7*x^6 - 20*x^5" );
  assert ( a != b );
  b[5] = 0;
  assert ( !(a == b) );
  a = a * 0;
  assert ( a . Degree () == 0
           && dumpMatch ( a, vector<double>{ 0.0 } ) );

  assert ( a == b );

  //cout << a.Degree() << endl;
  //a[200] = 5;
  //a[50] = 3;
  //cout << a.Degree() << endl;
  //a[200] = 0;
  //cout << a.Degree() << endl;
  return 0;
}
#endif /* __PROGTEST__ */
