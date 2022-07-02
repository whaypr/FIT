#include <iostream>
#include <string>
#include <algorithm>

using namespace std;


class Polish2Reverse {
public:
	Polish2Reverse(string l): line(l) {}

	void reverse() {
		char symbol = line[id];

		if ( isdigit(symbol) ) {
			cout << (first ? "" : " ") << symbol;
			first = false;
			return;
		}

		id++;
		reverse(); // reverse first operand

		id++;
		reverse(); // reverse second operand

		cout << ' ' << symbol;
	}

private:
	string line;
	size_t id = 0;
	bool first = true;
};


int main() {
	string line;
	for (getline(cin, line); line != "0"; getline(cin, line)) {
		line.erase( remove(line.begin(), line.end(), ' '), line.end() );

		Polish2Reverse p2r(line);
		p2r.reverse();

		cout << endl;
	}

	return 0;
}
