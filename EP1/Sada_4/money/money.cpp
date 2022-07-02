/**
*
* Variace na zjednodušený problém batohu (závažíčka)
*
*/

#include <iostream>
#include <vector>

using namespace std;

#define INT_MAX 2147480000
using ll = long long;


ll getTenders(vector<ll>& tenders) {
	ll sum = 0;

	while (true) {
		double tmp_value;
		cin >> tmp_value;
		if (tmp_value == -1) return sum;
		
		ll value = tmp_value * 100 + 0.5;

		int number;
		cin >> number;

		while(number--) {
			tenders.push_back(value);
			sum += value;
		}

		char dummy;
		cin >> dummy;
	}
}


void findAmounts(int amounts[], const vector<ll>& tenders, const ll size) {
	for (const auto & t : tenders) {
		for (long i = size; i >= 0; --i) {
			if (i - t < 0 || amounts[i-t] == INT_MAX) continue;
			amounts[i] = min( amounts[i], amounts[i-t] + 1 );
		}
	}
}


int main() {
	while (true) {
		double tmp_amount;
		cin >> tmp_amount;
		if (tmp_amount == -1) break;

		ll amount = tmp_amount * 100 + 0.5;

		// get tenders
		vector<ll> tendersCustomer, tendersShopkeeper;

		ll sumCustomer = getTenders(tendersCustomer);
		ll sumShopkeeper = getTenders(tendersShopkeeper);

		// find amounts
		int * amountsCustomer = new int[sumCustomer+1];
		int * amountsShopkeeper = new int[sumShopkeeper+1];

		amountsCustomer[0] = amountsShopkeeper[0] = 0;
		for (ll i = 1; i <= sumCustomer; ++i)
			amountsCustomer[i] = INT_MAX;
		for (ll i = 1; i <= sumShopkeeper; ++i)
			amountsShopkeeper[i] = INT_MAX;

		findAmounts(amountsCustomer, tendersCustomer, sumCustomer);
		findAmounts(amountsShopkeeper, tendersShopkeeper, sumShopkeeper);

		// find difference
		int minim = INT_MAX;

		for (ll i = amount; i <= min(sumCustomer, sumShopkeeper + amount); ++i)
			if (amountsCustomer[i] == INT_MAX || amountsShopkeeper[i-amount] == INT_MAX)
				continue;
			else
				minim = min( minim, amountsCustomer[i] + amountsShopkeeper[i-amount] );

		// print result
		if (minim < INT_MAX)
			cout << minim << " tenders must be exchanged." << endl;
		else
			cout << "The payment is impossible." << endl;

		delete [] amountsCustomer;
		delete [] amountsShopkeeper;
	}

	return 0;
}
