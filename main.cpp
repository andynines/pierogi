#include <iostream>

#include "antlr4-runtime.h"

using namespace std;

int main() {
	string input;
	antlr4::ANTLRInputStream is; // linker test
	while (input != "exit") {
		cout << "> ";
		getline(cin, input);
		cout << input << endl;
	}
	return 0;
}
