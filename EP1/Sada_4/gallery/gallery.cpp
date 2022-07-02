/*
*
* Příklad hledání optimální strategie pro hraní her
* s využitím DP
*
*/


#include <iostream>

using namespace std;


int main() {
	int nducks;

	while (cin >> nducks) {
		// init
		int * ducks = new int[nducks];
		int ** best = new int*[nducks+1];

		for (int i = 0; i < nducks; ++i) {
			cin >> ducks[i];

			best[i] = new int[nducks+1];
			for (int j = 0; j <= nducks; ++j)
				best[i][j] = 0; // trivial cases / mantinels

		}

		// finidng the best solution
		for (int len = 2; len <= nducks; ++len) {
			for (int i = 0; i <= nducks - len; ++i) {
				int j = i + len;
				best[i][j] = ( ducks[i] == ducks[j-1] )
						   ? best[i+1][j-1] + 1
						   : max( best[i+1][j], best[i][j-1] );
			}
		}

		// result
		cout << best[0][nducks] << endl;

		// cleanup
		delete [] ducks;
		for(int i = 0; i < nducks; ++i)
		    delete [] best[i];
		delete [] best;
	}

	return 0;
}
