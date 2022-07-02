#include <iostream>
#include <algorithm>

using namespace std;


struct Point {
	int x, y;

	friend istream& operator >> (istream& is, Point& p) {
        return is >> p.x >> p.y;
     }
};


struct Line {
	Point s,e;
};


bool inRect(Point r1, Point r2, Point p) {
	Point topleft, bottomright ;
	if (r1.x < r2.x) {
		topleft = r1;
		bottomright = r2;
	} else {
		topleft = r2;
		bottomright = r1;
	}

	if (topleft.y < bottomright.y) {
		int tmp = topleft.y;
		topleft.y = bottomright.y;
		bottomright.y = tmp;
	}

	return (p.x >= topleft.x && p.x <= bottomright.x && p.y >= bottomright.y && p.y <= topleft.y);
}


int main() {
	const double EPS = 1E-5;

	int ncases;
	cin >> ncases;

	while(ncases--) {
		Point lstart, lend, rtopleft, rbottomright;
		cin >> lstart >> lend >> rtopleft >> rbottomright;

		// line start or line end is inside the rectangle
		if ( inRect(rtopleft, rbottomright, lstart) || inRect(rtopleft, rbottomright, lend) ) {
			cout << 'T' << endl;
			continue;
		}

		// line intersects with at least one side of the rectangle
		Point rbottomleft = { rtopleft.x, rbottomright.y };
		Point rtopright   = { rbottomright.x, rtopleft.y };

		bool intersects = false;

		for (const Line& l : { Line{rtopleft, rtopright}, Line{rtopright, rbottomright}, Line{rbottomright, rbottomleft}, Line{rbottomleft, rtopleft} }) {
			double D0 = (l.e.y - l.s.y)*(lend.x - lstart.x) - (l.e.x - l.s.x)*(lend.y - lstart.y);
			double D1 = (l.e.x - l.s.x)*(lstart.y - l.s.y) - (l.e.y - l.s.y)*(lstart.x - l.s.x);
			double D2 = (lend.x - lstart.x)*(lstart.y - l.s.y) - (lend.y - lstart.y)*(lstart.x - l.s.x);

			// parallel
			if ( abs(D0) < EPS) {
				// not colinear
			 	if ( abs(D1) > 0 && abs(D2) > 0 )
			 		continue;

				int x = lstart.x;

			 	// use y
			 	if ( abs(x - lend.x) < EPS && abs(x - l.s.x) < EPS && abs(x - l.e.x) < EPS ) {
			 		int ls = min( lstart.y, lend.y );
			 		int le = max( lstart.y, lend.y );

			 		int rs = min( l.s.y, l.e.y );
			 		int re = max( l.s.y, l.e.y );

			 		if ( (ls < rs + EPS && le + EPS > rs) || (rs < ls + EPS && re + EPS > ls) ) {
			 			intersects = true;
			 			break;
			 		}
			 		continue;
			 	// use x
			 	} else {
			 		int ls = min( lstart.x, lend.x );
			 		int le = max( lstart.x, lend.x );

			 		int rs = min( l.s.x, l.e.x );
			 		int re = max( l.s.x, l.e.x );

			 		if ( (ls < rs + EPS && le + EPS > rs) || (rs < ls + EPS && re + EPS > ls) ) {
			 			intersects = true;
			 			break;
			 		}
			 		continue;
			 	}
			 }

			 double l_param = D1 / D0; 
			 double r_param = D2 / D0;

			 // intersection is inside lines
			 if ( l_param + EPS > 0 && l_param < 1 + EPS && r_param + EPS > 0 && r_param < 1 + EPS ) {
			 	intersects = true;
			 	break;
			 }
		}

		cout << ( intersects ? 'T' : 'F' ) << endl;
	}

	return 0;
}
