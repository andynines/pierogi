#include "lexer.hpp"

#include <sstream>
#include <utility>

namespace pierogi::lexer {

token::token(token_type type, std::string lexeme, void *literal, int line) :
		type(type), lexeme(std::move(lexeme)), literal(literal), line(line) {
}

token::operator std::string() const {
	std::stringstream ss;
	ss << static_cast<int>(type) << " " << lexeme; // not printing literal
	return ss.str();
}

bool token::operator==(const token& other) const {
	return (type == other.type) && (lexeme == other.lexeme) &&
		   (literal == other.literal) && (line == other.line);
}

bool token::operator!=(const token& other) const {
	return !operator==(other);
}

struct lexer_state {
	std::string source;
	std::vector<token> tokens;
	size_t start = 0;
	size_t current = 0;
	int line = 1;

	explicit lexer_state(std::string source) : source(std::move(source)) {
	}

	void lex_tokens() {
		while (!at_end()) lex_next_token();
		tokens.emplace_back(token_type::TOKEN_EOF, "", nullptr, line);
	}

	[[nodiscard]] bool at_end() const {
		return current >= source.length();
	}

	void lex_next_token() {
		start = current;
		char c = consume_next();
		switch (c) {
			case '\\':
				add_token(token_type::BACKSLASH);
				break;
			case '+':
				add_token(token_type::PLUS);
				break;
			case '-':
				add_token(token_type::MINUS);
				break;
			case '*':
				add_token(token_type::ASTERISK);
				break;
			case '/':
				add_token(token_type::SLASH);
				break;
			case '^':
				add_token(token_type::CARET);
				break;
			case '(':
				add_token(token_type::LEFT_PARENTHESIS);
				break;
			case ')':
				add_token(token_type::RIGHT_PARENTHESIS);
				break;
			case '{':
				add_token(token_type::LEFT_BRACE);
				break;
			case '}':
				add_token(token_type::RIGHT_BRACE);
				break;
			case ',':
				add_token(token_type::COMMA);
				break;
			case '<':
				add_token(consume_next_if_matches('=') ? token_type::LESS_EQUAL : token_type::LESS_THAN);
				break;
			case '>':
				add_token(consume_next_if_matches('=') ? token_type::GREATER_EQUAL : token_type::GREATER_THAN);
				break;
			case '=':
				add_token(consume_next_if_matches('=') ? token_type::EQUAL_EQUAL : token_type::EQUAL);
				break;
			case '!':
				if (consume_next_if_matches('=')) add_token(token_type::NOT_EQUAL); // TODO: raise error for singular !
				break;
			case '.':
				add_token(consume_next_if_matches('.') ? token_type::DOT_DOT : token_type::DOT);
				break;
			case ' ':
			case '\t':
				break;
			case '\n':
				line++;
				break;
			case '#':
				while (lookahead_one() != '\n' && !at_end()) consume_next();
				break;
			default:
				break;
		}

	}

	char consume_next() {
		return source[current++];
	}

	void add_token(token_type type) {
		const std::string lexeme = source.substr(start, current);
		tokens.emplace_back(type, lexeme, nullptr, line);
	}

	bool consume_next_if_matches(char expected) {
		if (at_end() || source[current] != expected) return false;
		current++;
		return true;
	}

	char lookahead_one() {
		if (at_end()) return '\0';
		else return source[current];
	}
};

std::vector<token> tokenize(std::string source) {
	lexer_state lexer(std::move(source));
	lexer.lex_tokens();
	return lexer.tokens;
}

} // namespace pierogi::lexer
