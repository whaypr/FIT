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

class CTimeStamp {
  int year, month, day, hour, minute, second;
public:
  CTimeStamp (int year, int month, int day, int hour, int minute, int second)
    : year(year), month(month), day(day), hour(hour), minute(minute), second(second) {
  }

  bool operator <= (CTimeStamp const & other) { 
    if (this->year != other.year)
      return this->year <= other.year;
    else if (this->month != other.month)
      return this->month <= other.month;
    else if (this->day != other.day)
      return this->day <= other.day;
    else if (this->hour != other.hour)
      return this->hour <= other.hour;
    else if (this->minute != other.minute)
      return this->minute <= other.minute;
    else
      return this->second <= other.second;
  } 

  bool operator >= (CTimeStamp const & other) { 
    if (this->year != other.year)
      return this->year >= other.year;
    else if (this->month != other.month)
      return this->month >= other.month;
    else if (this->day != other.day)
      return this->day >= other.day;
    else if (this->hour != other.hour)
      return this->hour >= other.hour;
    else if (this->minute != other.minute)
      return this->minute >= other.minute;
    else
      return this->second >= other.second;
  } 
};

class CContact {
  CTimeStamp timestamp;
  int number1, number2;
public:
  CContact (CTimeStamp timestamp, int number1, int number2)
    : timestamp(timestamp), number1(number1), number2(number2) {}

  CTimeStamp getTimestamp () const {return timestamp;}
  int getFirst () const {return number1;}
  int getSecond () const {return number2;}
};

class CEFaceMask {
  vector<CContact> contacts;
public:
  CEFaceMask & addContact (CContact contact) {
    contacts.push_back(contact);
    return *this;
  }
  
  vector<int> listContacts (int number,
                            CTimeStamp from = CTimeStamp(0,0,0,0,0,0),
                            CTimeStamp to = CTimeStamp(9999,9999,9999,9999,9999,9999)) const {
    vector<int> res;

    for (auto const & it : contacts) {
      if (it.getFirst() == number && it.getSecond() != number &&
          ! count(res.begin(), res.end(), it.getSecond()) &&
          it.getTimestamp() >= from && it.getTimestamp() <= to)
        res.push_back(it.getSecond());
      else if (it.getSecond() == number && it.getFirst() != number &&
               ! count(res.begin(), res.end(), it.getFirst()) &&
               it.getTimestamp() >= from && it.getTimestamp() <= to)
        res.push_back(it.getFirst());
    }

    return res;
  }
};

#ifndef __PROGTEST__
int main () {
  CEFaceMask test;

  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 10, 12, 40, 10 ), 123456789, 999888777 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 123456789, 111222333 ) )
       . addContact ( CContact ( CTimeStamp ( 2021, 2, 5, 15, 30, 28 ), 999888777, 555000222 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 21, 18, 0, 0 ), 123456789, 999888777 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), 123456789, 456456456 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 1, 0, 0, 0 ), 123456789, 123456789 ) );
  assert ( test . listContacts ( 123456789 ) == (vector<int> {999888777, 111222333, 456456456}) );
  assert ( test . listContacts ( 999888777 ) == (vector<int> {123456789, 555000222}) );
  assert ( test . listContacts ( 191919191 ) == (vector<int> {}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), CTimeStamp ( 2021, 2, 21, 18, 0, 0 ) ) == (vector<int> {999888777, 111222333, 456456456}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 1 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {999888777, 111222333}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 10, 12, 41, 9 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {111222333}) );
  return 0;
}
#endif /* __PROGTEST__ */
