#include <iostream>
#include <string>

using namespace std;

class VideoSurveillance {
public:
	bool inspectFloor(int vertices_count) {
		// inspects whether or not it is possible to install video camera to see whole floor

		// reset
		left_max = bottom_max = -2147480000;
		right_min = top_min = 2147480000;

		int x_prev, y_prev, x_start, y_start;
		bool possible = true;

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

		// check if correct (not done immediately in checkWall, because of reading the rest of floor's input)
		if (left_max > right_min || bottom_max > top_min)
			possible = false;

		return possible;
	}

private:
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
};


int main() {
	VideoSurveillance vs;

	// iterate through floors
	for (int floor = 1; ; floor++) {
		// get floor vertices
		int vertices_count;
		cin >> vertices_count;
		if (vertices_count == 0) break; // end of input

		// inspect the floor
		bool possible = vs.inspectFloor(vertices_count);

		// print result
		cout << "Floor #" << floor << endl;
		cout << "Surveillance is " << (possible ? "" : "im") << "possible." << endl;
		cout << endl;
	}

	return 0;
}
