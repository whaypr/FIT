#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main() {
	int parl[1000][1000];

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

		for (int i = 0; i < rows; ++i) {

			// ITERATE THROUGH CHAIRS
			getline(cin, line);
			stringstream ss(line);
			int rating;
			for (int j = 0; j < chairs; ++j) {
				ss >> rating;
				parl[i][j] = rating;
			}
		}

		// ITERATE THROUGH QUERIES
		int queries, r1, ch1, r2, ch2;
		cin >> queries;
		for (int i = 0; i < queries; ++i) {

			int sum = 0;
			cin >> r1 >> ch1 >> r2 >> ch2;

			for ( int r = r1-1; r <= r2-1; ++r )
				for (int ch = ch1-1; ch <= ch2-1; ++ch)
					sum += parl[r][ch];

			cout << "Absolutni hodnota pohodlnosti je " << sum << " bodu." << endl;

		}

		if (iterations != 1)
			cout << endl;
	}

	return 0;
}
