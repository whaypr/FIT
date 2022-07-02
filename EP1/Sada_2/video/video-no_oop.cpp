#include <iostream>
#include <string>

#define INT_MIN -2147480000
#define INT_MAX  2147480000

using namespace std;

int left_max, right_min, bottom_max, top_min;
void checkWall(int x_prev, int y_prev, int x, int y) {
	// left wall
	if (x == x_prev && y > y_prev)
		left_max = x > left_max ? x : left_max;
	// right wall
	else if (x == x_prev && y < y_prev)
		right_min = x < right_min ? x : right_min;
	// top wall
	else if (y == y_prev && x > x_prev)
		top_min = y < top_min ? y : top_min;
	// bottom wall
	else if (y == y_prev && x < x_prev)
		bottom_max = y > bottom_max ? y : bottom_max;
}

int main() {
	for (int floor = 1; ; floor++) {
		int vertices_count;
		cin >> vertices_count;
		if (vertices_count == 0) break;

		int x_prev, y_prev, x_start, y_start;
		bool possible = true;

		// reset
		left_max = bottom_max = INT_MIN;
		right_min = top_min = INT_MAX;

		// first point
		cin >> x_start >> y_start;
		x_prev = x_start;
		y_prev = y_start;

		// other points
		while (--vertices_count) {
			int x, y;
			cin >> x >> y;

			checkWall(x_prev, y_prev, x, y);

			x_prev = x;
			y_prev = y;
		}

		// last wall
		checkWall(x_prev, y_prev, x_start, y_start);

		// check if correct
		if (left_max > right_min || bottom_max > top_min)
			possible = false;

		// print result
		cout << "Floor #" << floor << endl;
		cout << "Surveillance is " << (possible ? "" : "im") << "possible." << endl;
		cout << endl;
	}

	return 0;
}
