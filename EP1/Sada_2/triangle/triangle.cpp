#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;


class Matrix {
public:
	Matrix() {}
	Matrix(int w, int h): data( vector<vector<int>>(w, vector<int>(h)) ) {}

	int & operator () (int x, int y) { return data[x][y]; }

private:
	vector<vector<int>> data;
};


class Triangle {
public:
	Triangle(int h): height(h), height_mantinels(h+1) {}

	void load() {
		triangle = Matrix(2 * height_mantinels - 1, height_mantinels);
		possible_triangles = Matrix(2 * height_mantinels - 1, height_mantinels);


		// read actual triangle
		for (int y = 1; y <= height; ++y) {
			for (int x = y; x <= 2 * height - y; ++x) {
				char unit;
				cin >> unit;
				triangle(x, y) = unit == '#' ? 0 : 1;
			}
		}
	}

	int getSubTriangleMaxArea() {
		int max_triangle = 0;

		// triangles poiting down (as orig tr)
		for (int y = 1; y <= height; ++y) {
			for (int x = y; x <= height*2 - y; x += 2) { 
		 		if ( !triangle(x, y) ) 
		  			possible_triangles(x, y) = 0; 
		 		else if ( !triangle(x, y-1) ) {
		  			possible_triangles(x, y) = 1; 
		  			if (max_triangle == 0) max_triangle++;
		 		} else {
		 			int val = min( possible_triangles(x-1, y-1), possible_triangles(x+1, y-1) ) + 1;
		 			possible_triangles(x, y) = val; 
		 			max_triangle = val > max_triangle ? val : max_triangle;
		 		}
			}
		}

		// triangles poiting up
		for (int y = height - 1; y >= 1; --y) {
			for (int x = y+1; x <= height*2 - y; x += 2) { 
		 		if ( !triangle(x, y) ) 
		  			possible_triangles(x, y) = 0; 
		 		else if ( !triangle(x, y+1) ) {
		  			possible_triangles(x, y) = 1;
		  			if (max_triangle == 0) max_triangle++;
		 		} else {
		 			int val = min( possible_triangles(x-1, y+1), possible_triangles(x+1, y+1) ) + 1;
		 			possible_triangles(x, y) = val; 
		 			max_triangle = val > max_triangle ? val : max_triangle;
		 		}
			}
		}

		// calculate actual area
		int area = 0;
		for (int i = 2 * max_triangle - 1; i >= 1; i -= 2)
			area += i;

		return area;
	}

private:
	int height;
	int height_mantinels;
	Matrix triangle; // whole triangle
	Matrix possible_triangles; // in each unit triangle remember the biggest triangle it can end in it
};


int main() {
	// iterate through triangles
	for (int triangle = 1; ; ++triangle) {
		// get triangle height
		int height;
		cin >> height;
		if (height == 0) break; // end of input

		// find the biggest triangle
		Triangle tr(height);
		tr.load();
		int area = tr.getSubTriangleMaxArea();

		// print result
		cout << "Triangle #" << triangle << endl;
		cout << "The largest triangle area is " << area << "." << endl;
		cout << endl;
	}

	return 0;
}
