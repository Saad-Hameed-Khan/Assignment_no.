#include"tree.h"
#include"vector_tools.h"

int main(int argc, char** argv) {
	iTree myTree;
	// generate random permutation of some sequence of numbers...
	std::vector<int> numbers;
	numbers.resize(10);
	for (int n = 0; n<int(numbers.size()); n++) numbers[n] = n;
	perfect_shuffle(numbers, 0);
	std::cout << numbers << std::endl;
	for (const auto& number : numbers) myTree.insert(number);
	myTree.erase(5);
	std::vector<int> linearized = myTree.traverse(iTree::order::inorder);
	std::cout << linearized << std::endl;
	std::cout << myTree.debuginfo() << std::endl;
	return 0;
}