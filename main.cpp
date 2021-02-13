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

void dump_string_info(const string& s) {
	ANTLRInputStream is(s);
	pierogiLexer lexer(&is);
	CommonTokenStream tokens(&lexer);
	tokens.fill();
	for (const auto& token : tokens.getTokens()) {
		cout << token->toString() << endl;
	}
	pierogiParser parser(&tokens);
}

int main() {
	string input;
	while ((input = get_input()) != "exit") {
		dump_string_info(input);
	}
	return 0;
}
