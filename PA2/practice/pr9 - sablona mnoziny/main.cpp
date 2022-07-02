#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include <algorithm>
#include <functional>

#include <stdexcept>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdint>
#include <cctype>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */
//========================================================

#include "progtest.cpp"

//========================================================
#ifndef __PROGTEST__
#include <cassert>

class MyInt {
    int value;
public:
    MyInt () = delete; // do not allow default constructor
    MyInt ( int value ) : value( value ) {} // for creating MyInt from int
    
    // guaranteed functionality:
    MyInt ( const MyInt & src ) { value = src.value; }
    MyInt & operator = ( const MyInt & other ) { value = other.value; return *this; }
    bool operator < ( const MyInt & other ) { return value < other.value; }
};

struct CSetTester
{
    static void test0()
    {
        CSet<string> x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
    }

    static void test1()
    {
        CSet<string> x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        CSet<string> x1 ( x0 );
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }

    static void test2()
    {
        CSet<string> x0;
        CSet<string> x1;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        x1 = x0;
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }

    static void test4()
    {
        CSet<int> x0;
        assert( x0 . Insert( 4 ) );
        assert( x0 . Insert( 8 ) );
        assert( x0 . Insert( 1 ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( 4 ) );
        assert( !x0 . Contains( 5 ) );
        assert( !x0 . Remove( 5 ) );
        assert( x0 . Remove( 4 ) );
    }

    static void test5()
    {
        CSet<MyInt> x0;
        assert( x0 . Insert( 4 ) );
        assert( x0 . Insert( 8 ) );
        assert( x0 . Insert( 1 ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( 4 ) );
        assert( !x0 . Contains( 5 ) );
        assert( !x0 . Remove( 5 ) );
        assert( x0 . Remove( 4 ) );
    }
};

int main ()
{
    CSetTester::test0();
    CSetTester::test1();
    CSetTester::test2();
    CSetTester::test4();
    CSetTester::test5();
    return 0;
}
#endif /* __PROGTEST__ */
