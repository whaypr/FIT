#include <iostream>
#include <vector>
#include <string>
#include <functional>

using namespace std;


class Heap {
public:
    Heap(vector<int> & data) : data(data), root(0) {}

    void build() {
        for (int i = (data.size() + root) / 2; i > root; i--)
            bubbleDown(i);
    }

    void insert(int value) {
        data.push_back(value);
        bubbleUp(data.size());
    }

    int findMin() {
        // actually remove all already removed items
        while (true) {
            auto p = to_be_removed.find(data[0]);
            if (p == to_be_removed.end()) break;
            to_be_removed.erase(p);
            extractMin();
        }

        return data[0];
    }

    void extractMin() {
        int tmp = data[0];

        data[0] = data.back();
        data[data.size() - 1] = tmp;

        data.pop_back();
        bubbleDown(1);
    }

    void remove(int item) {
        to_be_removed.insert(item);
    }

    void sort() {
        for (auto it = data.begin(); it < data.end() - 2 ; it++) {
            build();
            root++;
        }
        root = 0; //reset root
    }

    void print(string title = "") {
        if ( !title.empty() ) cout << title << ":" << endl << "   ";

        for (const auto & item : data)
            cout << item << ' ';
        cout << endl;
    }

private:
    vector<int> data;
    set<int> to_be_removed;
    int root;

    void bubbleDown(int father_index) {
        int father = data[father_index - 1];
        size_t left_son_index = 2 * (father_index - root) + root;
        size_t right_son_index = left_son_index + 1;

        while (left_son_index <= data.size()) {

            // sons
            int left_son = data[left_son_index - 1];
            int right_son;
            if (right_son_index <= data.size())
                right_son = data[right_son_index - 1];
            else
                right_son = left_son + 1; // always higher

            // smaller of sons
            int smallest;
            int smallest_index;
            if (left_son <= right_son) {
                smallest = left_son;
                smallest_index = left_son_index;
            } else {
                smallest = right_son;
                smallest_index = right_son_index;
            }

            if (father <= smallest)
                return;

            // swap father with son
            data[father_index - 1] = smallest;
            data[smallest_index - 1] = father;

            father_index = smallest_index;
            left_son_index = 2 * father_index;
            right_son_index = left_son_index + 1;
        }
    }

    void bubbleUp(int son_index) {
        while (son_index - 1) {
            int father_index = son_index / 2;

            int son = data[son_index - 1];
            int father = data[father_index - 1];

            if (father <= son)
                return;

            data[son_index - 1] = father;
            data[father_index - 1] = son;

            son_index = father_index;
        }
    }
};


class SampleTester {
public:
    void readSamples() {
        int samples_count;
        cin >> samples_count;

        for (int i = 0; i < samples_count; ++i) {
            int time, value;
            cin >> time >> value;


        }
    }

    void answerQueries() {
        int conditions_count;
        cin >> conditions_count;

        for (int i = 0; i < conditions_count; ++i) {
            // read query
            int interval;
            string r, f;
            cin >> r >> f >> interval;

            // prepare relation
            function<bool(int,int)> rel;

            if (r == "lt")
                rel = less<int>();
            else // r == "gt"
                rel = greater<int>();

            // prepare function
            function<int(void)> func;

            if (f == "min")
                func = [=]() -> int { return this->minimum(); };
            else if (f == "max")
                func = [=]() -> int { return this->maximum(); };
            else // f == "avg"
                func = [=]() -> int { return this->average(); };

            // answer query
            int count = 0;




            // print result
            cout << count << endl;
        }
    }

private:
    int minimum() { return 0; }
    int maximum() { return 0; }
    int average() { return 0; }

};


int main() {
    SampleTester tester;
    tester.readSamples();
    tester.answerQueries(); 

    return 0;
}
