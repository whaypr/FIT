#include <iostream>
#include <string>
#include <sstream>
#include <map>

using namespace std;

int main() {
	map<char, int> pos = {
		{'1', 7},
		{'2', 6},
		{'3', 5},
		{'4', 4},
		{'5', 3},
		{'6', 2},
		{'7', 1},
		{'8', 0},
		{'a', 0},
		{'b', 1},
		{'c', 2},
		{'d', 3},
		{'e', 4},
		{'f', 5},
		{'g', 6},
		{'h', 7}
	};

	string board[8][8];

	// reading white and black
	bool white = true;
	for (int i = 0; i < 2; ++i) {
		string line, fig;
		stringstream figures;

		cin.ignore(7); // ignore "White: " and then "Black: "
		getline(cin, line);

		figures.str(line);
		while (getline(figures, fig, ',')) {
			if (fig.length() == 2)
				board[ pos[fig[1]] ][ pos[fig[0]] ] = ( white ? "P" : "p" );
			else
				board[ pos[fig[2]] ][ pos[fig[1]] ] = ( white ? fig[0] : tolower(fig[0]) );
		}

		white = false; // switch to black
	}

	// displaying
	for (int i = 0; i < 8; ++i) {
		cout << "+---+---+---+---+---+---+---+---+" << endl;
		string line = "";

		for (int j = 0; j < 8; ++j) {
			bool cond = (i%2) ^ (j%2);
			string fig = board[i][j];

			line += "|";

			if ( fig == "")
				line += cond ? ":::" : "...";
			else
				line += cond ? (":" + fig + ":") : ("." + fig + ".");
		}
		line += "|";

		cout << line << endl;
	}
	cout << "+---+---+---+---+---+---+---+---+" << endl;

	return 0;
}
