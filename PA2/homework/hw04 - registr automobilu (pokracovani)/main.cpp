#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
using namespace std;
#endif /* __PROGTEST__ */
// ********************************************************************************************** //

#include "progtest.cpp"

// ********************************************************************************************** //
#ifndef __PROGTEST__
static bool        matchList ( CCarList          && l,
                               const char         * rz1 = nullptr,
                               const char         * rz2 = nullptr )
{
  for ( ; ! l . AtEnd (); l . Next () ) {
    if ( rz1 && ! strcmp ( l . RZ (), rz1 ) )
      rz1 = nullptr;
    else if ( rz2 && ! strcmp ( l . RZ (), rz2 ) )
      rz2 = nullptr;
    else
      return false;
  }

  return rz1 == nullptr && rz2 == nullptr;
}

int main ( void )
{
  char name[50], surname[50];

  CRegister  b0;
  assert ( b0 . AddCar ( "ABC-12-34", "John", "Smith" ) == true );

  strncpy ( name, "John", sizeof ( name ) ); strncpy ( surname, "Hacker", sizeof ( surname ) );
  assert ( b0 . AddCar ( "ABC-32-22", name, surname ) == true );
  
  strncpy ( name, "Peter", sizeof ( name ) ); strncpy ( surname, "Smith", sizeof ( surname ) );
  assert ( b0 . AddCar ( "XYZ-11-22", name, surname ) == true );
  
  assert ( b0 . CountCars ( "John", "Hacker" ) == 1 );
  assert ( matchList ( b0 . ListCars ( "John", "Hacker" ), "ABC-32-22" ) );
  assert ( b0 . CountOwners ( "ABC-12-34" ) == 1 );
  COwnerList ol0 = b0 . ListOwners ( "ABC-12-34" );

  assert ( ! ol0 . AtEnd () && ! strcmp ( ol0 . Name (), "John" ) && ! strcmp ( ol0 . Surname (), "Smith" ) );
  ol0 . Next ();
  assert ( ol0 . AtEnd () );

  CRegister b1 ( b0 );
  assert ( b0 . Transfer ( "XYZ-11-22", "John", "Hacker" ) == true );
  assert ( b0 . Transfer ( "XYZ-11-22", "Will", "Smith" ) == true );
  assert ( b1 . Transfer ( "XYZ-11-22", "John", "Smith" ) == true );
  assert ( b0 . CountOwners ( "XYZ-11-22" ) == 3 );

  COwnerList ol1 = b0 . ListOwners ( "XYZ-11-22" );
  assert ( ! ol1 . AtEnd () && ! strcmp ( ol1 . Name (), "Will" ) && ! strcmp ( ol1 . Surname (), "Smith" ) );
  ol1 . Next ();
  assert ( ! ol1 . AtEnd () && ! strcmp ( ol1 . Name (), "John" ) && ! strcmp ( ol1 . Surname (), "Hacker" ) );
  ol1 . Next ();
  assert ( ! ol1 . AtEnd () && ! strcmp ( ol1 . Name (), "Peter" ) && ! strcmp ( ol1 . Surname (), "Smith" ) );
  ol1 . Next ();
  assert ( ol1 . AtEnd () );

  assert ( b1 . CountOwners ( "XYZ-11-22" ) == 2 );

  COwnerList ol2 = b1 . ListOwners ( "XYZ-11-22" );
  assert ( ! ol2 . AtEnd () && ! strcmp ( ol2 . Name (), "John" ) && ! strcmp ( ol2 . Surname (), "Smith" ) );
  ol2 . Next ();
  assert ( ! ol2 . AtEnd () && ! strcmp ( ol2 . Name (), "Peter" ) && ! strcmp ( ol2 . Surname (), "Smith" ) );
  ol2 . Next ();
  assert ( ol2 . AtEnd () );

  b1 = b0;
  assert ( b0 . DelCar ( "XYZ-11-22" ) == true );
  assert ( b1 . DelCar ( "ABC-12-34" ) == true );
  assert ( b0 . CountCars ( "John", "Smith" ) == 1 );
  assert ( matchList ( b0 . ListCars ( "John", "Smith" ), "ABC-12-34" ) );
  assert ( b1 . CountCars ( "John", "Smith" ) == 0 );
  assert ( matchList ( b1 . ListCars ( "John", "Smith" ) ) );

  CRegister  b2;
  assert ( b2 . AddCar ( "ABC-12-34", "John", "Smith" ) == true );
  assert ( b2 . AddCar ( "ABC-32-22", "John", "Hacker" ) == true );
  assert ( b2 . AddCar ( "XYZ-11-22", "Peter", "Smith" ) == true );
  assert ( b2 . AddCar ( "XYZ-11-22", "Jane", "Black" ) == false );
  assert ( b2 . DelCar ( "AAA-11-11" ) == false );
  assert ( b2 . DelCar ( "XYZ-11-22" ) == true );
  assert ( b2 . Transfer ( "BBB-99-99", "John", "Smith" ) == false );
  assert ( b2 . Transfer ( "ABC-12-34", "John", "Smith" ) == false );
  assert ( b2 . Transfer ( "XYZ-11-22", "John", "Smith" ) == false );
  assert ( b2 . CountCars ( "George", "White" ) == 0 );
  assert ( matchList ( b2 . ListCars ( "George", "White" ) ) );
  assert ( b2 . CountOwners ( "AAA-AA-AA" ) == 0 );
  
  COwnerList ol3 = b2 . ListOwners ( "AAA-AA-AA" );
  assert ( ol3 . AtEnd () );

  return 0;
}
#endif /* __PROGTEST__ */
