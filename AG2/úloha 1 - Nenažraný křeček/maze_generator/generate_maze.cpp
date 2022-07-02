#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

int main() {
    srand (time(NULL));

    int junctions = 1000;
    int corridors = (rand() % ( junctions*(junctions/10) ));

    cout << "{" << (rand() % junctions) << "," << (rand() % junctions) << ",{";
    for (int i = 0; i < junctions-1; ++i)
        cout << (rand() % 10) << ",";
    cout << (rand() % 10) << "},{";
    for (int i = 0; i < corridors-1; ++i)
        cout << "{" << (rand() % junctions) << "," << (rand() % junctions) << "},";
    cout << "{" << (rand() % junctions) << "," << (rand() % junctions) << "}";
    cout << "}};";
}
