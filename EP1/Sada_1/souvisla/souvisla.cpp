#include <iostream>

using namespace std;

int main() {
	int iterations, current, last, size, max_size;
	cin >> iterations;
	for (; iterations > 0; --iterations ) {
		size = last = max_size = 0;

		while (cin >> current) {
			if (current == 0) {
				max_size = size > max_size ? size : max_size;
				break;
			}

			if (current >= last)
				size++;
			else {
				max_size = size > max_size ? size : max_size;
				size = 1;
			}

			last = current;
		}

		cout << max_size << endl;
	}

	return 0;
}
