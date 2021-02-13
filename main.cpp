#include <iostream>

#include "antlr4-generated/pierogiLexer.h"
#include "antlr4-generated/pierogiParser.h"

using namespace antlr4;
using namespace std;

string get_input() {
	string input;
	cout << "> ";
	getline(cin, input);
	return input;
}

int main() {
	string input;
	while ((input = get_input()) != "exit") {
		ANTLRInputStream is(input);
		pierogiLexer lexer(&is);
		CommonTokenStream tokens(&lexer);
		pierogiParser parser(&tokens);
		cout << parser.getNumberOfSyntaxErrors() << endl;
	}
	return 0;
}
