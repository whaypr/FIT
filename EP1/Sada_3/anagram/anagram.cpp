#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <set>

using namespace std;


class Permutaions {
public:
	Permutaions(const int m, const string & w): max(m), anagram( new char[max] ) {
		symbols = set<char>( w.begin(), w.end() );

		for (const char & s : symbols)
			counts[s] = used[s] = 0;

		for (const char & c : w)
			counts[c]++;
	}

	~Permutaions() {
		cout << perms << endl;
	}

	void generate(int alen) { // alen = anagram length
		if (alen == max) {
			perms++;
			cout << anagram << endl;
			return;
		}

		for (const auto & symbol : symbols) {
			if (used[symbol] == counts[symbol])			
				continue;

			used[symbol]++;

			anagram[alen] = symbol;
			generate(alen + 1);
			
			used[symbol]--;
		}
	}

private:
	const int max; // max permutation length
	char * anagram;
	int perms = 0; // number of already generated permutations

	set<char> symbols;
	map<char, int> counts; // number of each symbol
	map<char, int> used; // number of current usage of each symbol
};


int main() {
	while (true) {
		// read input
		int max;
		cin >> max;

		if (max == 0) break;

		string word;
		getline(cin, word); // skip newline after number
		getline(cin, word);

		// preprocess
		sort(word.begin(), word.end());

		// generate
		Permutaions perm(max, word);
		perm.generate(0);
	}

	return 0;
}
