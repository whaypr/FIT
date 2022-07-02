#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <functional>
#include <iomanip>
#include <cmath>

using namespace std;

#define INT_MAX  2147480000
#define INT_MIN -2147480000

class SampleTester {
public:
	void printSamples() {
		for (const auto & s : samples) {
			string min_extreme, max_extreme;
			if (s.second.nearest_min.extreme_seen_before) min_extreme = "!";
			if (s.second.nearest_max.extreme_seen_before) max_extreme = "!";

			cout << "[ "       << setw(11) << s.first;
			cout << " : "      << setw(11) << s.second.value;
			cout << " ] MIN: " << setw(11) << min_extreme << s.second.nearest_min.distance;
			cout << " MAX: "   << setw(11) << max_extreme << s.second.nearest_max.distance;
			cout << endl;
		}
	}

	void readSamples(string line) {
		// READ SAMPLES AND PREPROCESS THEM

		int samples_count;
		stringstream ss(line);
		ss >> samples_count;

		list<int> mins;
		int global_min = INT_MAX;

		list<int> maxs;
		int global_max = INT_MIN;

			// first sample (has no nearest min and max)
		if (samples_count > 0) {
			int time;
			Sample s;

			getline(cin, line);
			ss.clear();
			ss.str(line);
			ss >> time >> s.value;

			global_min = s.value;
			global_max = s.value;

			s.nearest_min.distance = 0;
			s.nearest_max.distance = 0;
			s.prev_sample = -1;

			samples[time] = s;
		}

			// other samples
		for (int i = 1; i < samples_count; ++i) {
			int time;
			Sample s;

			getline(cin, line);
			ss.clear();
			ss.str(line);
			ss >> time >> s.value;

			bool min_seen_before = false;
			bool max_seen_before = false;

			auto previous = prev(samples.end(), 1);

				// nearest min
			if (previous->second.value < s.value) {
				s.nearest_min.distance = time - previous->first;
				mins.push_front(previous->first);
			} else if (s.value < global_min) {
				s.nearest_min.distance = 0;
				global_min = s.value;
			} else if (s.value == global_min) {
				s.nearest_min.distance = previous->second.nearest_min.distance + (time - previous->first);
				min_seen_before = true;
			} else {
				for (const auto & m : mins) {
					if (samples[m].value < s.value) {
						s.nearest_min.distance = time - m;
						break;
					}
				}
			}

				// nearest max
			if (previous->second.value > s.value) {
				s.nearest_max.distance = time - previous->first;
				maxs.push_front(previous->first);
			} else if (s.value > global_max) {
				s.nearest_max.distance = 0;
				global_max = s.value;
			} else if (s.value == global_max) {
				s.nearest_max.distance = previous->second.nearest_max.distance + (time - previous->first);
				max_seen_before = true;
			} else {
				for (const auto & m : maxs) {
					if (samples[m].value > s.value) {
						s.nearest_max.distance = time - m;
						break;
					}
				}
			}

			s.prev_sample = time - previous->first;
			s.nearest_min.extreme_seen_before = min_seen_before;
			s.nearest_max.extreme_seen_before = max_seen_before;

			samples[time] = s;
		}
	}

	void answerQueries() {
		// READ QUERIES AND ANSWER THEM

		int conditions_count;

		string line;
		getline(cin, line);
		stringstream ss(line);
		ss >> conditions_count;

		for (int i = 0; i < conditions_count; ++i) {
			string r, f;
			int interval;

			getline(cin, line);
			ss.clear();
			ss.str(line);
			ss >> r >> f >> interval;

			function<bool(int,int)> rel;
			int count = 0;

			// answer
			if (f == "min") {

				for (const auto & s : samples) {
					if (interval < s.second.prev_sample || s.second.prev_sample == -1) continue;

					if (r == "lt") {
						if (s.second.nearest_min.distance == 0) count++;
						else if (s.second.nearest_min.distance > interval ) count++;
					} else {
						if (s.second.nearest_min.distance == 0) continue;
						else if ( !s.second.nearest_min.extreme_seen_before && s.second.nearest_min.distance <= interval ) count++;
					}
				}

			} else if (f == "max") {

				for (const auto & s : samples) {
					if (interval < s.second.prev_sample || s.second.prev_sample == -1) continue;

					if (r == "lt") {
						if (s.second.nearest_max.distance == 0) continue;
						else if ( !s.second.nearest_max.extreme_seen_before && abs(s.second.nearest_max.distance) <= interval ) count++;
					} else {
						if (s.second.nearest_max.distance == 0) count++;
						else if ( abs(s.second.nearest_max.distance) > interval ) count++;
					}
				}

			} else { // f == "avg"
				if (r == "lt") rel = less<int>(); else rel = greater<int>();

				int sum, items, time = 0;
				map<int, Sample>::iterator prev;
				bool first = true;

				for (auto curr = samples.begin(); curr != samples.end(); curr++) {
					// first sample
					if (first) {
						sum = curr->second.value;
						prev = curr;
						items = 1;
						first = false;
						continue;
					}

					curr--;
					int tmp = curr->first;
					curr++;
					time += curr->first - tmp;

					while (time > interval) {
						sum -= prev->second.value;
						items--;

						int tmp = prev->first;
						prev++;
						time -= prev->first - tmp;
					}


					if ( sum != 0 && rel(curr->second.value, sum / items) ) count++;

					sum += curr->second.value;
					items++;
				}
			}

			cout << count << endl;
		}
	}

private:
	struct Nearest {
		bool extreme_seen_before;
		int distance; // either to nearest min/max or to same extrem as this is
	};

	struct Sample {
		int value;
		Nearest nearest_min; // time diff from this to the min
		Nearest nearest_max; // time diff from this to the max

		int prev_sample; // time diff from previous sample
	};

	map<int, Sample> samples;

	int findNearest(const map<int, Sample> & s, const int val) {
		map<int, Sample>::const_reverse_iterator last_element_not_greater_than(s.upper_bound(val));

	  	if (s.rend() == last_element_not_greater_than)
	  		return -1;

	  	return last_element_not_greater_than->first;
	}
};


int main() {
	string count_line;
	getline(cin, count_line);

	while ( !count_line.empty() ) {
		SampleTester tester;
		tester.readSamples(count_line);
		tester.printSamples();
		tester.answerQueries();

		getline(cin, count_line);
	}

	return 0;
}
