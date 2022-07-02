#include <iostream>
#include <string>

using namespace std;


int main() {
	for (int number = 1; true; ++number) {
		// load and check data
		string qt1_hex, qt2_hex;

		getline(cin, qt1_hex);
		getline(cin, qt2_hex);
		if (qt1_hex == "0" && qt2_hex == "0")
			break;

		// process data
		string res;


		// print result
		cout << "Image " << number << ":" << endl;
		cout << res << endl << endl;
	}

	return 0;
}
