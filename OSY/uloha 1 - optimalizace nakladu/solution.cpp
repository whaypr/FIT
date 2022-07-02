#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <array>
#include <iterator>
#include <set>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>
#include <deque>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <pthread.h>
#include <semaphore.h>
#include "progtest_solver.h"
#include "sample_tester.h"
using namespace std;
#endif /* __PROGTEST__ */ 

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class CCargoPlanner {
  public:
    CCargoPlanner () { atEnd = false; shipID = customerID = shipsCompleted = 0; shipsTotal = 9999; }
    static int               SeqSolver                     ( const vector<CCargo> & cargo,
                                                             int               maxWeight,
                                                             int               maxVolume,
                                                             vector<CCargo>  & load );
    void                     Start                         ( int               sales,
                                                             int               workers );
    void                     Stop                          ( void );

    void                     Customer                      ( ACustomer         customer );
    void                     Ship                          ( AShip             ship );

  private:
    bool atEnd;

    vector<ACustomer> customers;
    int customerID;
    mutex mtxCustomer;

    vector<AShip> ships;
    int shipID;
    mutex mtxShip;
    int shipsTotal, shipsCompleted;

    condition_variable shipsEmpty;
    condition_variable cargosReady;

    vector<thread> threads;
    void seller ( int tid );
    void worker ( int tid );

    struct Item {
      vector<CCargo> cargo;
      int sID;
    };
    mutex mtxBuff;
    vector<Item> buff;
    vector<int> quotesRemains;

    vector<int> completeQuotesShips;
};  
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int CCargoPlanner::SeqSolver ( const vector<CCargo> & cargo, int maxWeight, int maxVolume, vector<CCargo>  & load ) {
  return ProgtestSolver( cargo, maxWeight, maxVolume, load );
}

//----------------------------------------------------------------------------------
void CCargoPlanner::Start ( int sales, int workers ) {

  // create sellers
  for ( int i = 0; i < sales; ++i )
    threads.push_back( thread ( &CCargoPlanner::seller, this, i ) );

  // create workers
  for ( int i = 0; i < workers; ++i )
    threads.push_back( thread ( &CCargoPlanner::worker, this, i ) );
}

//----------------------------------------------------------------------------------
void CCargoPlanner::Stop ( ) {
  atEnd = true;
  shipsEmpty.notify_one();
  cargosReady.notify_one();
  shipsTotal = ships.size();
  printf("AT END!\n\n");

  for ( auto & t : threads )
    t.join(); 
  printf("DONE!\n\n");
}

//----------------------------------------------------------------------------------
void CCargoPlanner::Customer ( ACustomer customer ) {
  customers.push_back( customer );
  printf("*** Customer added ***\n");
}

//----------------------------------------------------------------------------------
void CCargoPlanner::Ship ( AShip ship ) {
  unique_lock<mutex> ul( mtxShip );
    ships.push_back( ship );
    quotesRemains.push_back( customers.size() );
    printf("*** Ship added ***\n");
    shipsEmpty.notify_one();
}

//----------------------------------------------------------------------------------
void CCargoPlanner::seller ( int tid ) {
  int cID;
  int sID;
  printf("+SE %d: ready\n", tid);

  while ( true ) {
    unique_lock<mutex> ulC( mtxCustomer );
      if ( atEnd && (size_t)shipID >= ships.size() )
        break;
      // wait for another ship to proccess
      shipsEmpty.wait( ulC, [ this ] () { return ( (size_t)shipID < ships.size() || atEnd ); } );
      if ( atEnd && shipID == shipsTotal )
        break;

      sID = shipID;
      cID = customerID++; // here is always just one seller
      printf("+SE %d: ship %d customer %d PROCCESSING\n", tid, sID, cID);

      // all customers are quoted for the current ship -> next ship
      if ( (size_t)customerID == customers.size() ) {
        unique_lock<mutex> ulS( mtxShip );
          shipID++;
        ulS.unlock();
        customerID = 0;
      }

    ulC.unlock();

    // try to wake up another seller
    shipsEmpty.notify_one();

    // quote customer
    unique_lock<mutex> ulS( mtxShip );
      AShip ship = ships[sID];
    ulS.unlock();

    vector<CCargo> cargo;
    customers[cID]->Quote( ship->Destination(), cargo );
    printf("+SE %d: ship %d customer %d OBTAINED\n", tid, sID, cID);

    // push obtained cargo with number of the ship to buffer
    Item item = { cargo, sID };
    unique_lock<mutex> ulB( mtxBuff );
      buff.push_back( item );
      
      ulS.lock();
        (quotesRemains[sID])--;

        // wake up worker if there is work for him
        if ( quotesRemains[sID] == 0 ) {
          completeQuotesShips.push_back(sID);
          printf("+SE %d: ship %d cargo READY\n", tid, sID);
          cargosReady.notify_one();
        }
      ulS.unlock();
    ulB.unlock();

    if ( atEnd && (size_t)shipID >= ships.size() )
      break;
  }

  shipsEmpty.notify_all();
  printf("SELLER %d DONE!\n", tid);
}

//----------------------------------------------------------------------------------
void CCargoPlanner::worker ( int tid ) {
  int sID;
  printf("-WO %d: ready\n", tid);
  
  while ( true ) {
    vector<CCargo> cargo;

    unique_lock<mutex> ulB( mtxBuff );
      cargosReady.wait( ulB, [ this ] () { return ( ! completeQuotesShips.empty() || shipsCompleted == shipsTotal ); } );
      if ( atEnd && completeQuotesShips.empty() && shipsCompleted == shipsTotal ) 
        break;
      else if ( completeQuotesShips.empty() )
        continue;

      sID = completeQuotesShips.back();
      completeQuotesShips.pop_back();
      printf("-WO %d: ship %d PROCCESSING\n", tid, sID);

      for (vector<Item>::iterator it = buff.begin(); it != buff.end(); ) {
        if ( it->sID == sID ) {
          cargo.insert( cargo.end(), it->cargo.begin(), it->cargo.end() );
          it = buff.erase(it);
        } else
          ++it;
      }
    ulB.unlock();

    // try to wake up another worker
    cargosReady.notify_one();

    // find optimal load
    unique_lock<mutex> ulS( mtxShip );
        AShip ship = ships[sID];
    ulS.unlock();

    printf("-WO %d, ship %d LOADING\n", tid, sID);

    vector<CCargo> load;
    SeqSolver ( cargo, ship->MaxWeight(), ship->MaxVolume(), load );

    // load the ship
    ship->Load( load );

    ulB.lock();
        shipsCompleted++;
    ulB.unlock();

    if ( atEnd && completeQuotesShips.empty() && shipsCompleted == shipsTotal ) {
      shipsEmpty.notify_all();
      break;
    }
  }

  cargosReady.notify_one();
  shipsEmpty.notify_all();
  printf("WORKER %d DONE!\n", tid);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef __PROGTEST__
int main ( ) {
  CCargoPlanner  test;
  vector<AShipTest> ships;
  vector<ACustomerTest> customers { make_shared<CCustomerTest> (), make_shared<CCustomerTest> () };

  ships . push_back ( g_TestExtra[0] . PrepareTest ( "New York", customers ) );
  ships . push_back ( g_TestExtra[1] . PrepareTest ( "Barcelona", customers ) );
  ships . push_back ( g_TestExtra[8] . PrepareTest ( "Perth", customers ) );
  ships . push_back ( g_TestExtra[2] . PrepareTest ( "Kobe", customers ) );
  // add more ships here

  for ( auto x : customers )
    test . Customer ( x );

  test . Start ( 3, 2 );

  for ( auto x : ships ) {
    test . Ship ( x );
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }

  test . Stop  ();

  for ( auto x : ships )
    cout << x->Destination() << ": " << ( x->Validate() ? "ok" : "fail" ) << endl;
  return 0;  
}
#endif /* __PROGTEST__ */ 
