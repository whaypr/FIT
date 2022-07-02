#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;


struct Point {
	int x, y;

	friend istream& operator >> (istream& is, Point& p) {
        return is >> p.x >> p.y;
     }
};


int main() {
	const double EPS = 1E-5;

	Point tr;
	double r;

	while ( (cin >> tr >> r) && r >= 0 ) {
		int npoints;
		cin >> npoints;

		vector<double> angles;

		for (int i = 0; i < npoints; ++i) {
			Point p;
			cin >> p;

			// push only angles of points in radius
			if ( hypot(tr.x - p.x, tr.y - p.y) < r + EPS )
				angles.push_back( atan2(tr.y - p.y, tr.x - p.x) );
		}

		// sort angles to be able to add them one by one
		sort(angles.begin(), angles.end());

		vector<double> shifted;
		for (const auto & a : angles)
			shifted.push_back(a + 2 * M_PIl);

		// append points shifted by one period to corretly compare points after reaching full circle and starting the new one
		angles.insert( angles.end(), shifted.begin(), shifted.end() );

		int maxPoints = 0, points = 0;
		size_t idFirst = 0;

		for (size_t i = 0; i < angles.size(); ++i) {
			double angle = angles[i] - angles[idFirst];
			points++;
			
			while (angle > M_PIl + EPS) {
				points--;
				idFirst++;
				angle = angles[i] - angles[idFirst];
			}

			maxPoints = max(maxPoints, points);
		}

		cout << maxPoints << endl;
	}

	return 0;
}
