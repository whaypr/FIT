#include <iostream>
#include <map>
#include <set>

using namespace std;

int main() {
	// ITERATE THROUGH SHAPES
	int iterations;
	cin >> iterations;
	for (; iterations > 0; --iterations) {
		// key: verical level (y)
		// value: ordered list of points on particular level (x)
		map<int, set<int>> m;

		int x,y;
		cin >> x >> y;

		// ITERATE THROUGH LINE & LOAD DATA
		char direction;
		while ( cin >> direction ) {
			if (direction == '.')
				break;

			switch (direction) {
				case 'N':
					y++;

					if ( m.find(y) == m.end() ) 
						m[y] = set<int>{x};
					else
						m[y].insert(x);
				break;

				case 'S':
					if ( m.find(y) == m.end() ) 
						m[y] = set<int>{x};
					else
						m[y].insert(x);

					y--;
				break;

				case 'E':
					x++;
				break;

				case 'W':
					x--;
				break;
			}
		}

    	// COUNT SQUARES (AREA)
    	int area = 0;
    	for (const auto & y : m) {
    		int i = 0, left;
			for ( const auto & x : y.second ) {
				if (i % 2 == 0)
					left = x;
				else
					area += x - left;

				i++;
			}
    	}

    	cout << "The area is " << area << " squares." << endl;
	}

	return 0;
}
