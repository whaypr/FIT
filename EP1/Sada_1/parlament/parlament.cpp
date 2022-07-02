#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main() {
	// one extra space for sentinels in each dimension
	// indexing from 1 as it is in the specifications
	int parl[1001][1001];

	for (int i = 0; i < 1001; ++i)
		parl[0][i] = 0; // row sentinels
	for (int i = 0; i < 1001; ++i)
		parl[i][0] = 0; // chair sentinels


	// ITERATE THROUGH PARLIAMENTS
	int iterations;
	cin >> iterations;
	for (; iterations > 0; --iterations) {

		// ITERATE THROUGH ROWS
		int rows, chairs; // number of rows and number of chairs in each row
		cin >> rows >> chairs;
		string line;
		getline(cin, line); // ignore newline before reading lines as a whole
							// cin.ignore(1) works only if there is no other whitespace !!!
		for (int i = 1; i <= rows; ++i) {

			// ITERATE THROUGH CHAIRS (AND PRECALCULATE VALUES)
			getline(cin, line);
			stringstream ss(line);
			int rating;
			for (int j = 1; j <= chairs; ++j) {
				ss >> rating;
				parl[i][j] = rating + parl[i-1][j] + parl[i][j-1] - parl[i-1][j-1];
			}
		}


		// ITERATE THROUGH QUERIES (AND PRINT ANSWER)
		int queries, r1, ch1, r2, ch2;
		cin >> queries;
		for (int i = 0; i < queries; ++i) {
			cin >> r1 >> ch1 >> r2 >> ch2;
			int sum = parl[r2][ch2] - parl[r2][ch1-1] - parl[r1-1][ch2] + parl[r1-1][ch1-1];

			cout << "Absolutni hodnota pohodlnosti je " << sum << " bodu." << endl;

		}

		if (iterations != 1)
			cout << endl;
	}

	return 0;
}
