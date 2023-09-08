#include "BST.h"
#include "BSTNode.h"
#include <iostream>

using namespace std;

int main() {

	BST<int, string> tree;

	cout << "CONSTRUCTING TREE" << endl;
	tree.insert(77, "seventy-seven");
	tree.insert(70, "seventy");
	tree.insert(75, "seventy-five");
	tree.insert(66, "sixty-six");
	tree.insert(79, "seventy-nine");
	tree.insert(68, "sixty-eight");
	tree.insert(67, "sixty-seven");
	tree.insert(69, "sixty-nine");
	tree.insert(90, "ninety");
	tree.insert(85, "eighty-five");
	tree.insert(83, "eighty-three");
	tree.insert(87, "eighty-seven");
	tree.insert(65, "sixty-five");

	cout << "\n\nprint() CALL (CALLS printhelp()):" << endl << endl;
	tree.print();

	cout << "\n\nprintinorder() CALL:" << endl << endl;
	tree.printinorder();
	
	cout << "\n\nprintReverse() CALL:" << endl << endl;
	tree.printReverse();

	return 0;
}// End of Main