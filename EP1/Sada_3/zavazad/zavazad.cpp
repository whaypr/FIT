#include <iostream>

using namespace std;


class ContainerDistributor {
public:
	ContainerDistributor(int n, int conts[]): count(n), containers(conts) { }

	int countMoves() { return count_inner(0, containers[0]); }

private:
	int count; // number of containers
	int * containers; // each container is characterized by its destination number

	// remember only current platform
	// destination platform is always one of the others and it doesn't matter which one it is (we are only counting)
	int count_inner(int id, int current) {
		if (id == count)
			return 0;

		if (containers[id] == current)
			return count_inner(id + 1, current);

		int smaller = ( 1 << (count - id - 1) ); // simulating recursively moving hanoi tower smaller by one
		return smaller + count_inner(id + 1, 3 - containers[id] - current);
	}
	
};


int main() {
	int tasks;
	cin >> tasks;

	for (; tasks > 0; --tasks) {
		int containers_count;
		cin >> containers_count;

		int * containers = new int[containers_count];

		for (int i = 0; i < containers_count; ++i) {
			cin >> containers[i];
		}

		ContainerDistributor hanoi(containers_count, containers);
		cout << "Je treba " << hanoi.countMoves() << " presunu." << endl;

		delete [] containers;
	}

	return 0;
}
