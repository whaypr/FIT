#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;

class DigitalDisplay {
public:
	DigitalDisplay() {
		rows = 7;

		numbers['0'] = { {"+---+"}, {"|   |"}, {"|   |"}, {"+   +"}, {"|   |"}, {"|   |"}, {"+---+"} };
		numbers['1'] = { {"    +"}, {"    |"}, {"    |"}, {"    +"}, {"    |"}, {"    |"}, {"    +"} };
		numbers['2'] = { {"+---+"}, {"    |"}, {"    |"}, {"+---+"}, {"|    "}, {"|    "}, {"+---+"} };
		numbers['3'] = { {"+---+"}, {"    |"}, {"    |"}, {"+---+"}, {"    |"}, {"    |"}, {"+---+"} };
		numbers['4'] = { {"+   +"}, {"|   |"}, {"|   |"}, {"+---+"}, {"    |"}, {"    |"}, {"    +"} };
		numbers['5'] = { {"+---+"}, {"|    "}, {"|    "}, {"+---+"}, {"    |"}, {"    |"}, {"+---+"} };
		numbers['6'] = { {"+---+"}, {"|    "}, {"|    "}, {"+---+"}, {"|   |"}, {"|   |"}, {"+---+"} };
		numbers['7'] = { {"+---+"}, {"    |"}, {"    |"}, {"    +"}, {"    |"}, {"    |"}, {"    +"} };
		numbers['8'] = { {"+---+"}, {"|   |"}, {"|   |"}, {"+---+"}, {"|   |"}, {"|   |"}, {"+---+"} };
		numbers['9'] = { {"+---+"}, {"|   |"}, {"|   |"}, {"+---+"}, {"    |"}, {"    |"}, {"+---+"} };
		numbers[':'] = { {" "}, {" "}, {"o"}, {" "}, {"o"}, {" "}, {" "} };
	}

	void displayTime(string time) {
		string res = "";

		for (int i = 0; i < rows; ++i) {
			for (const char & c : time) {
				res += numbers[c][i] + "  ";
			}
			res.pop_back();
			res.pop_back();
			res += '\n';
		}
		res += '\n';

		cout << res << endl;
	}

private:
	map<char, vector<string>> numbers;
	int cols, rows;
};

int main() {
	DigitalDisplay dd;

	string time;
	getline(cin, time);
	while (time != "end") {
		dd.displayTime(time);
		getline(cin, time);
	}

	cout << "end" << endl;

	return 0;
}
