#ifndef PIEROGI_LEXER_HPP
#define PIEROGI_LEXER_HPP

#include "source_reader.hpp"
#include "types.hpp"
#include "errors.hpp"

#include <string>
#include <vector>
#include <variant>

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
	EQUAL_EQUAL,
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

    EOF_
};

struct token {
	token_type type;
	std::string lexeme;
	types::value value;
	int line;

	token(token_type type, std::string lexeme, types::value value, int line);
};

std::vector<token> tokenize(source_reader::source_reader_interface& source,
							errors::reporter_interface& error_reporter);

} // namespace pierogi::lexer

#endif // PIEROGI_LEXER_HPP
