#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <map>

using namespace std;

string nextSymbol(stringstream & ss) {
	// returns empty string if next symbol is just arbitrary symbol or "end" is stream is empty
	string res = "";

	char c;
	if (!(ss >> c))
		return "end";

	switch (c) {
		case '(':
			res = c;
			if (ss.peek() == '*')
				res += (char)ss.get();
		break;

		case '[': case '{': case '<':
		case ']': case '}': case '>': case ')':
			res = c;
		break;

		case '*':
			if (ss.peek() == ')')
				res = string(1,c) + (char)ss.get(); // at least one of the chars must be a string
		break;
	}

	return res;
}

int main() {
	map<string, string> par = {
		{"(", ")"},
		{"[", "]"},
		{"{", "}"},
		{"<", ">"},
		{"(*", "*)"},
	};

	// iterating through lines
	int iterations;
	cin >> iterations;
	cin.ignore(1); // ignore newline at the end of first line!
	for (; iterations > 0; --iterations) {
		stack<string> s;

		// reseting state
		while (!s.empty())
			s.pop();
		bool correct = true;

		// line parsing
		string line;
		getline(cin, line);
		stringstream ss(line);
		for (string sym = nextSymbol(ss); sym != "end"; sym = nextSymbol(ss)) {
			if (sym == "(" || sym == "[" || sym == "{" || sym == "<" || sym == "(*")
				s.push(sym);
			else if (sym == ")" || sym == "]" || sym == "}" || sym == ">" || sym == "*)") {
				if ( s.empty() || sym != par[s.top()] ) {
					correct = false;
					break;
				}
				s.pop();
			}
		}

		if (!s.empty())
			correct = false;

		cout << ( correct ? "Plan je v poradku." : "V planu je chyba." ) << endl;
	}

	return 0;
}
