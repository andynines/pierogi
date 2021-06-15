#ifndef PIEROGI_LEXER_HPP
#define PIEROGI_LEXER_HPP

#include "source_reader.hpp"

#include <string>
#include <vector>

namespace pierogi::lexer {

enum class token_type {
	LEFT_PARENTHESIS,
	RIGHT_PARENTHESIS,
	LEFT_SQUARE_BRACKET,
	RIGHT_SQUARE_BRACKET,
	LEFT_BRACE,
	RIGHT_BRACE,
	COLON,
	COMMA,
	DOT,
	MINUS,
	PLUS,
	CARET,
	SLASH,
	BACKSLASH,
	ASTERISK,

	EQUAL,
	NOT_EQUAL,
	GREATER_THAN,
	GREATER_EQUAL,
	LESS_THAN,
	LESS_EQUAL,
	DOT_DOT,

	AND,
	OR,
	NOT,
	TRUE,
	FALSE,
	NIL,

	IDENTIFIER,
	STRING,
	NUMBER,

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

std::vector<token> tokenize(source_reader::source_reader_interface& source);

} // namespace pierogi::lexer

#endif // PIEROGI_LEXER_HPP
