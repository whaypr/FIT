#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

class Database {
private:
  struct record {
    string name;
    string surname;
    string phone;
  };

  vector<record> records;

  bool validatePhone (const string & phone) {
    if ( phone.length() != 9 || phone[0] == '0' )
      return false;

    for ( auto it = phone.begin(); it != phone.end(); it++ )
      if ( ! isdigit(*it) )
        return false;

    return true;
  }

public:
  bool addRecord (const string & line) {
    string name, surname, phone;
    char tmp;

    // parse line
    stringstream string_stream(line);

    string_stream >> name >> surname;
    if ( string_stream.eof() )
      return false;
    string_stream >> phone;
    while ( string_stream >> tmp )
      if ( tmp != ' ' || tmp != '\t' )
        return false;

    if ( !validatePhone(phone) )
      return false;

    // add new record
    record new_record = {name, surname, phone};
    records.push_back(new_record);

    return true;
  }

  void answerQuery (const string & query, ostream & out) {
    int count = 0;

    for (auto it = records.begin(); it != records.end(); it++) {
      if ( query == it->name || query == it->surname ) {
        out << it->name << ' ' << it->surname << ' ' << it->phone << endl;
        count++;
      }
    }

    out << "-> " << count << endl;
  }
};

bool report ( const string & fileName, ostream & out ) {
  Database database;
  ifstream inputFile(fileName);

  // check if file was opened correctly
  if ( inputFile.fail() )
    return false;

  // read records
  string line;
  while ( getline(inputFile, line) ) {
    if ( line.empty() )
      break;

    if ( !database.addRecord(line) )
      return false;
  }

  // answer queries
  string query;
  while ( getline(inputFile, query) ) { // "inputFile >> query" skipps empty lines which should be interpreted as empty queries
    database.answerQuery(query, out);
  }

  return true;
}

#ifndef __PROGTEST__
int main () {
  report( "tests/test2_in.txt", cout );

  ostringstream oss;
  oss . str ( "" );
  assert ( report( "tests/test0_in.txt", oss ) == true );
  assert ( oss . str () ==
    "John Christescu 258452362\n"
    "John Harmson 861647702\n"
    "-> 2\n"
    "-> 0\n"
    "Josh Dakhov 264112084\n"
    "Dakhov Speechley 865216101\n"
    "-> 2\n"
    "John Harmson 861647702\n"
    "-> 1\n" );
  oss . str ( "" );
  assert ( report( "tests/test1_in.txt", oss ) == false );

  return 0;
}
#endif /* __PROGTEST__ */
