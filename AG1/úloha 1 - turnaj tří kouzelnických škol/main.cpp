#include <iostream>
#include <memory>

#define INT_MAX 2000000000
using namespace std;

//--------------------------------------------------
struct Point {
	int x, y;
};

//--------------------------------------------------
struct Field {
	int wall;
	short distance;
};

//--------------------------------------------------
struct Path {
	Point val;
	Path * prev; // TODO: fix memleaks!
};

//--------------------------------------------------
template< typename S >
struct array_deleter {
	// https://stackoverflow.com/a/13062069
	
  	void operator ()( S const * p) { 
    	delete[] p; 
  	}
};

//--------------------------------------------------
template <typename T>
class Matrix {
public:
	Matrix() {}
	Matrix(int w, int h): w(w), h(h) {
		matrix = new T*[w];

		for(int i = 0; i < w; ++i)
		    matrix[i] = new T[h];
	}

	~Matrix() {
		/*for (int i = 0; i < w; ++i)
			delete [] matrix[i];
		delete matrix;*/
	}

	T& operator () (int x, int y) { return matrix[y][x]; }
	T& operator () (Point p) { return matrix[p.y][p.x]; }

private:
	int w, h;
	T ** matrix; // TODO: fix memleaks!
	
};

//--------------------------------------------------
template <typename T>
class Queue {
public:
	Queue(int s) : capacity(s), items(new T[capacity]) {}
	~Queue() { delete [] items; }

	void push(T item) {
		items[(current+count)%capacity] = item;
		count++;
	}

	T pop() {
		T res = items[current];

		current++;
		current %= capacity;
		count--;

		return res;
	}

	bool empty() const { return !count; }

private:
	int capacity, current = 0, count = 0;
	T * items;
};

//--------------------------------------------------
template <typename T>
class Vect {
public:
	Vect() {
		capacity = 10;
		arr = shared_ptr<T[]>(new T[capacity], array_deleter<T>());
		current = 0;
	}

	void push(T data) {
		if (current == capacity) {
			shared_ptr<T[]> temp = shared_ptr<T[]>(new T[capacity*2], array_deleter<T>());

			for (int i = 0; i < capacity; i++)
				temp[i] = arr[i];

			arr = temp;
			capacity *= 2;
		}

		arr[current++] = data;
	}

	T pop() { return arr[--current]; }
	T peek() const { return arr[current-1]; }

	int size() const { return current; }

	T& operator [] (T index) { return arr[index]; }

private:
	shared_ptr<T[]> arr;
	int capacity;
	int current;
};

//--------------------------------------------------
void combination(Vect<short> & c, int k, Vect<Vect<short>> & res) {
	// https://stackoverflow.com/a/43075836

	int combo = (1 << k) - 1; // k bit sets

	while ( combo < (1 << c.size()) ) {
		Vect<short> tmp;

		for (int i = 0; i < c.size(); ++i)
			if ((combo >> i) & 1)
				tmp.push(c[i]);

		res.push(tmp);

		int x = combo & -combo;
		int y = combo + x;
		int z = (combo & ~y);
		combo = z / x;
		combo >>= 1;
		combo |= y;
	}
}

//--------------------------------------------------
class Labyrinth {
public:
	void readInput() {
		// read labyrinth size and number of levers
		cin >> lab_size >> lever_cnt;

		labyrinth = Matrix<Field>{lab_size, lab_size};

		// read levers position and configuration
		levers_conf = Matrix<bool> {lab_size,lever_cnt};
		for (int i = 0; i < lever_cnt; ++i) {
			cin >> levers_pos[i];

			char c;
			for (int j = 0; j < lab_size; ++j) {
				cin >> c;
				levers_conf(i, j) = c - 48;
			}
		}

		// read labyrinth definition
		for (int x = 0; x < lab_size; ++x) {
			for (int y = 0; y < lab_size; ++y) {
				char c; // read input char by char
				cin >> c;
				labyrinth(x, y) = {c - 48, 0}; // make bool from char '0' or '1'
			}
		}

		// read cup position
		cin >> cup.x >> cup.y;
		cup.x--; cup.y--; // my labyrinth is indexed from zero
	}

	//.....................................
	void findPath() {
		Vect<short> ids;
		for (int i = lever_cnt - 1; i >= 0; --i)
			ids.push(i);

		Vect<Point> res;
		Vect<short> res_levers;
		int len_shortest = INT_MAX;

		// generate all combinations of levers
		for ( int i = 0; i <= lever_cnt; ++i ) {
			Vect<Vect<short>> combs;

			// no lever is used
			if (i == 0)
				combs.push(Vect<short>());
			else
				combination(ids, i, combs);

			// apply all combinations of length i
			while ( combs.size() > 0 ) {
				Vect<short> inner = combs.pop();
				Vect<short> lev_tmp = inner;

				int lev_distance = 0;
				bool bottom = false;

				Vect<short> superLever; // all the levers in one
				for (int x = 0; x < lab_size; ++x)
					superLever.push(0);

				// apply one combination and calculate max distance
				while ( inner.size() > 0 ) {
					int l_id = inner.pop();

					if (levers_pos[l_id] == -1)
						bottom = true;
					else
						lev_distance = levers_pos[l_id] > lev_distance ? levers_pos[l_id] : lev_distance;

					// add lever to superLever
					for (int x = 0; x < lab_size; ++x)
						superLever[x] ^= levers_conf(l_id, x);
				}

				lev_distance *= 2;
				if (bottom) lev_distance += 2;

				// try to run through the labyrinth with applied levers
				pair<int, Vect<Point>> res_tmp = bfs(labyrinth, superLever);

				// reset distances
				for (int x = 0; x < lab_size; ++x)
					for (int y = 0; y < lab_size; ++y)
						labyrinth(x, y).distance = 0;

				// new shorter path
				if ( res_tmp.first != -1 && res_tmp.first + lev_distance < len_shortest ) {
					len_shortest = res_tmp.first + lev_distance;
					res_levers = lev_tmp;
					res = res_tmp.second;
				}

				// already shortest path
				if (len_shortest == cup.x + cup.y)
					break;
			}
		}

		// print best result

		if (len_shortest == INT_MAX) {
			cout << -1 << endl;
			return;
		}

			// len
		cout << len_shortest << endl;

			// levers
		for (int i = 0; i < lever_cnt; ++i) {
			if ( res_levers.size() > 0 && res_levers.peek() == i ) {
				res_levers.pop();
				cout << "1" << ( i == lever_cnt - 1 ? "" : " " );
			} else
				cout << "0" << ( i == lever_cnt - 1 ? "" : " " );
		}

		if ( lever_cnt > 0 ) cout << endl;

			// path
		Point p = res.pop();
		cout << "[" << p.x << ";" << p.y << "]";
		while (res.size() > 0) {
			p = res.pop();
			cout << ",[" << p.x << ";" << p.y << "]";
		}
		cout << endl;
	}

private:
	int lab_size, lever_cnt;

	int levers_pos[10];
	Matrix<bool> levers_conf;

	Matrix<Field> labyrinth;
	Point cup;

	int route_len = -1;

	//.....................................
	pair<int,Vect<Point>> bfs(Matrix<Field> & lab, Vect<short> & superLever) {
		if ( (lab(0,0).wall ^ superLever[0]) == 1 || (lab(cup).wall ^ superLever[cup.y]) == 1 ) 
			return {route_len, {}};

		route_len = -1;

		Queue<Path*> q(lab_size * 5);

		Path * p = new Path();
		p->val = {0, 0};
		p->prev = nullptr;

		q.push(p);

		Vect<Path*> paths;
		paths.push(p);

		while (! q.empty()) {
			Path * path = q.pop();
			Point curr = path->val;

			// FOUND
			if (curr.x == cup.x && curr.y == cup.y) {
				route_len += lab(curr).distance + 1; // + 1 for starting at -1

				Vect<Point> route;

				route.push({curr.y + 1, curr.x + 1});
				while (path->prev) {
					path = path->prev;
					route.push({path->val.y + 1, path->val.x + 1});
				}

				while (paths.size() > 0)
					delete paths.pop();

				return {route_len, route};
			}

			Queue<Point> n(4);
			if (curr.x > 0)
				n.push({curr.x-1, curr.y});
			if (curr.x < lab_size - 1)
				n.push({curr.x+1, curr.y});
			if (curr.y > 0)
				n.push({curr.x, curr.y-1});
			if (curr.y < lab_size - 1)
				n.push({curr.x, curr.y+1});

			while ( ! n.empty()) {
				Point p = n.pop();
				Field & p_field = lab(p);

				if ( p_field.distance == 0 && (p_field.wall ^ superLever[p.y]) == 0 ) {
					p_field.distance = lab(curr).distance + 1;

					Path * new_path = new Path();
					paths.push(new_path);
					new_path->val = p;
					new_path->prev = path;
					q.push(new_path);
				}
			}
		}

		while (paths.size() > 0)
			delete paths.pop();

		return {route_len, {}};
	}
};

//--------------------------------------------------
int main() {
	Labyrinth labyrinth;
	labyrinth.readInput();
	labyrinth.findPath();

	return 0;
}
