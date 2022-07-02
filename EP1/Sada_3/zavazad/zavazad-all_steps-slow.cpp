/**
* Returns correct answers but it's too slow.
*
* Instead of finding the all the steps of solution,
* you can only count number of steps using formula.
*/


#include <iostream>

using namespace std;


class DistributedHanoi {
public:
	DistributedHanoi(int dc, int conts[]): disk_count(dc), containers(conts) {

	}

	int count() {
		sum = id_placed = 0;
		
		int id = 1;

		while ( containers[id] == containers[0] ) {
			id_placed++;
			id++;
		}

		int rest = 3 - containers[0] - containers[id];

		count_inner(id, containers[0], containers[id], rest);

		return sum;
	}	

private:
	int disk_count;
	int * containers;

	int sum, id_placed;

	void count_inner(int id, int from, int to, int help) {
		if (id >= disk_count) return;

		// move tower smaller by one to help platform
		count_inner(id + 1, from, help, to);

		sum++;
		if (id == id_placed + 1) id_placed++;


		// move from help to platform where it belongs, ignore already correct disks
		while ( containers[id+1] == help && id + 1 == id_placed + 1 ) {
			id_placed++;
			id++;
		}

		int rest = 3 - help - containers[id+1];

		count_inner(id + 1, help, containers[id+1], rest);
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

		DistributedHanoi hanoi(containers_count, containers);
		cout << "Je treba " << hanoi.count() << " presunu." << endl;

		delete [] containers;
	}

	return 0;
}
