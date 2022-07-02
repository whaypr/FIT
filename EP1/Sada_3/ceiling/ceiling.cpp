#include <iostream>
#include <string>
#include <set>
#include <vector>

using namespace std;


class BST {
public:
	BST(const int count, const int values[]) {
		for (int i = 0; i < count; ++i)
			Insert(values[i]);
	}

	~BST() { Delete(root); }

	void Insert(const int val) {
		Node * curr = root;
		Node * prev = root;

		while (curr) {
			prev = curr;

			if (val > curr->val)
				curr = curr->right;
			else
				curr = curr->left;
		}

		Node * n = new Node();
		n->val = val;

		if (!prev)
			root = n;
		else if (val > prev->val)
			prev->right = n;
		else
			prev->left = n;
	}

	string getShape() {
		shape = "";
		getShape_inner(root->left, 'L');
		getShape_inner(root->right, 'R');
		return shape;
	}

private:
	struct Node {
		int val;
		Node * left = nullptr;
		Node * right = nullptr;
	};

	Node * root = nullptr;

	string shape;

	void getShape_inner(Node * n, char side) {
		if (n == nullptr) {
			shape += "-";
			return;
		}

		shape += side;

		getShape_inner(n->left, 'L');
		getShape_inner(n->right, 'R');
	}

	void Delete(Node * n) {
		if (n == nullptr) return;

		Delete(n->left);
		Delete(n->right);
		delete n;
	}
};


class TreeShapes {
public:
	TreeShapes(int s) { }

	~TreeShapes() { cout << shapes.size() << endl; }

	void add(const string & shape) { shapes.insert(shape); }

private:
	set<string> shapes;
};


int main() {
	while (true) {
		int prototypes, layers; // p in {1..50}, l in {1..20}
		
		if ( !(cin >> prototypes >> layers) ) break;

		TreeShapes ts(layers);
		int * values = new int[layers];

		for (; prototypes > 0; --prototypes) {
			for (int j = 0; j < layers; ++j)
				cin >> values[j];

			BST bst(layers, values);
			ts.add( bst.getShape() );
		}

		delete [] values;
	}

	return 0;
}
