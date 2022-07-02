#include <iostream>
#include <iomanip>
#include <cmath>
#include <bits/stdc++.h>

using namespace std;

int main() {
	ios::sync_with_stdio(false);

	const double EPS = 1E-8;

	int iterations;
	cin >> iterations;
	for (; iterations != 0; cin >> iterations) {
		int count = 0;
		double len_sq = 0, x1, y1, x2, y2;

		for (; iterations > 0; --iterations) {
			cin >> x1 >> y1 >> x2 >> y2;
			double curr_len_sq = pow(x1 - x2, 2) + pow(y1 - y2, 2);

			if (abs(curr_len_sq - len_sq) < EPS)
				count++;
			else if (curr_len_sq > len_sq) { // epsilon already checked
				count = 1;
				len_sq = curr_len_sq;
			}
		}

		cout << fixed << setprecision(2) << count << " usecek ma delku " << sqrt(len_sq) << "." << endl;
	}

	return 0;
}
