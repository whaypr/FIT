/**
*
* Variace na zjednodušený problém batohu (závažíčka),
* jen jedeme zleva doprava a peníze používáme neomezeně opakovaně
*
*/

#include <iostream>
#include <map>

using namespace std;

#define INT_MAX 2147480000


int main() {
	int iterations;
	cin >> iterations;

	while (iterations--) {
		int wEmpty, wFilled, nCoinTypes;
		cin >> wEmpty >> wFilled >> nCoinTypes;

		wFilled = wFilled - wEmpty;

		int * mins = new int[wFilled+1];
		mins[0] = 0;
		for (int i = 1; i <= wFilled; ++i)
			mins[i] = INT_MAX;

		map<int, int> coins;

		while (nCoinTypes--) {
			int cval, cweig;
			cin >> cval >> cweig;

			if ( coins.find(cweig) == coins.end() )
			  coins[cweig] = cval;
			else
			  coins[cweig] = min( coins[cweig], cval );
		}

		for (int i = 0; i < wFilled; ++i) {
			if (mins[i] == INT_MAX) continue;

			for (const auto & c : coins) {
				if (i + c.first > wFilled) continue;

				mins[i + c.first] = min( mins[i + c.first], mins[i] + c.second );
			}
		}

		if (mins[wFilled] < INT_MAX)
			cout << "The minimum amount of money in the piggy-bank is " << mins[wFilled];
		else
			cout << "This is impossible";
		cout << "." << endl;

		delete [] mins;
	}

	return 0;
}
