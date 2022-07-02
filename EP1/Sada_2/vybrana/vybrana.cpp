#include <iostream>
#include <vector>
#include<bits/stdc++.h>

using namespace std;

int main() {
	int iterations;
	cin >> iterations;

	for (int i = 0; i < iterations; ++i) {
		int next_number;
		vector<int> selected; // index: seq len, value: last item of seq of given len

		while (true) {
			cin >> next_number;
			if (next_number == 0) break;

			// there is new longer selected seq -> push new item into vector
			if (selected.empty() || next_number >= selected.back())
				selected.push_back(next_number);
			else {
				auto it = upper_bound(selected.begin(), selected.end(), next_number); // find first num > next_number
				if (it == selected.end())
					selected.back() = next_number; // update last number
				else 
					*it = next_number; // update any other number
			}
		}

		cout << selected.size() << endl;
	}

	return 0;
}
