#ifndef PIEROGI_LEXER_HPP
#define PIEROGI_LEXER_HPP

#include <string>
#include <vector>

namespace pierogi::lexer {

enum class token_type {
	// Single-character tokens
	LEFT_PARENTHESIS,
	RIGHT_PARENTHESIS,
	LEFT_BRACE,
	RIGHT_BRACE,
	COMMA,
	DOT,
	MINUS,
	PLUS,
	CARET,
	SLASH,
	BACKSLASH,
	ASTERISK,

	// One or two character tokens
	EQUAL, EQUAL_EQUAL, NOT_EQUAL,
	GREATER_THAN, GREATER_EQUAL,
	LESS_THAN, LESS_EQUAL,
	DOT_DOT,

	// Reserved words
	AND, OR, NOT,

	// Literals
	IDENTIFIER, STRING, NUMBER,

	TOKEN_EOF
};

struct token {
	token_type type;
	std::string lexeme;
	void* literal{};
	int line;

public:
	token(token_type type, std::string lexeme, void* literal, int line);

	explicit operator std::string() const;
	bool operator==(const token& other) const;
	bool operator!=(const token& other) const;
};

std::vector<token> tokenize(std::string source);

} // namespace pierogi::lexer

#endif // PIEROGI_LEXER_HPP
