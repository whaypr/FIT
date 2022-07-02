#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>

using namespace std;


class Matrix {
public:
	Matrix() {}
	Matrix(int size) {
		data.reserve(size);
		for (int i = 0; i < size; ++i)
			data[i].reserve(size);
	}

	int & operator () (int x, int y) { return data[x][y]; }

private:
	vector<vector<int>> data;
};


class Quadtree {
public:
	Quadtree(int size): size(size), res(Matrix(size)) {}

	void decode(int size, int pos_x = 0, int pos_y = 0) {
		char symbol;
		cin >> symbol;

		// no more recursion --> fill corresponting matrix fields
		if (symbol != 'Q') {
			int value = symbol == 'B' ? 1 : 0;

			for (int i = 0; i < size; ++i)
				for (int j = 0; j < size; ++j)
					res(pos_x + i, pos_y + j) = value;

			return;
		}

		int new_size = size / 2;

		decode(new_size, pos_x,            pos_y           ); // top left
		decode(new_size, pos_x + new_size, pos_y           ); // top right
		decode(new_size, pos_x,            pos_y + new_size); // bottom left
		decode(new_size, pos_x + new_size, pos_y + new_size); // bottom right
	}

	void print() {
		cout << "#define quadtree_width " << size << endl;
		cout << "#define quadtree_height " << size << endl;
		cout << "static char quadtree_bits() = {" << endl;

		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size / 8; ++j) {
				int sum = 0;

				for (int k = 0; k < 8; ++k) {
					if ( res(8*j + k, i) ) sum += pow(2, k);
				}

				cout << "0x" << setfill('0') << setw(2) << hex << sum << ",";
			}
			cout << endl;
		}

		cout << "};" << endl;
	}

private:
	int size; // can be 8, 16, 32, 64, 128, 256, 512
	Matrix res;
};


int main() {
	int size;
	cin >> size;

	Quadtree qt(size);
	qt.decode(size);
	qt.print();

	return 0;
}
