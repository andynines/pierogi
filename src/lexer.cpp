#include "lexer.hpp"

#include <sstream>
#include <unordered_map>
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
	size_t current = 0;
	std::stringstream lexeme;
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
		lexeme.str(std::string());
		lexeme.clear();
		lexeme << get_current();
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
			case '=':
				add_token(token_type::EQUAL);
				break;
			case '(':
				add_token(token_type::LEFT_PARENTHESIS);
				break;
			case ')':
				add_token(token_type::RIGHT_PARENTHESIS);
				break;
			case '[':
				add_token(token_type::LEFT_SQUARE_BRACKET);
				break;
			case ']':
				add_token(token_type::RIGHT_SQUARE_BRACKET);
				break;
			case '{':
				add_token(token_type::LEFT_BRACE);
				break;
			case '}':
				add_token(token_type::RIGHT_BRACE);
				break;
			case ':':
				add_token(token_type::COLON);
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
			case '!':
				if (consume_next_if_matches('=')) add_token(token_type::NOT_EQUAL); // TODO: raise error for singular !
				break;
			case '.':
				add_token(consume_next_if_matches('.') ? token_type::DOT_DOT : token_type::DOT);
				break;
			case ' ':
			case '\t':
			case '\r':
				break;
			case '\n':
				line++;
				break;
			case '#':
				while (lookahead_one() != '\n' && !at_end()) consume_next();
				break;
			case '"':
				consume_string();
				break;
			default:
				if (is_digit(c)) {
					consume_number();
				} else if (is_alphabetic(c)) {
					consume_word();
				}
				break;
		}

	}

	char get_current() {
		return source[current];
	}

	char consume_next() {
		return source[current++];
	}

	void add_token(token_type type) {
		tokens.emplace_back(type, "", nullptr, line);
	}

	bool consume_next_if_matches(char expected) {
		if (at_end() || source[current] != expected) return false;
		current++;
		return true;
	}

	[[nodiscard]] char lookahead_one() const {
		if (at_end()) return '\0';
		else return source[current];
	}

	[[nodiscard]] char lookahead_two() const {
		if (current + 1 >= source.size()) return '\0';
		return source[current + 1];
	}

	void consume_string() {
		while (lookahead_one() != '"' && !at_end()) {
			if (lookahead_one() == '\n') line++;
			consume_next();
		}
		consume_next(); // Consume closing "
		add_token(token_type::STRING);
	}

	void consume_number() {
		while (is_digit(lookahead_one())) consume_next();
		if (lookahead_one() == '.' && is_digit(lookahead_two())) {
			consume_next(); // Consume .
			while (is_digit(lookahead_one())) consume_next();
		}
		add_token(token_type::NUMBER);
	}

	void consume_word() {
		static const std::unordered_map<std::string, token_type> keywords = {
				{"and", token_type::AND},
				{"or", token_type::OR},
				{"not", token_type::NOT},
				{"true", token_type::TRUE},
				{"false", token_type::FALSE},
				{"nil", token_type::NIL}
		};
		while (is_alphanumeric(lookahead_one())) lexeme << consume_next();
		auto it = keywords.find(lexeme.str());
		add_token(it != keywords.end() ? it->second : token_type::IDENTIFIER);
	}

	static bool is_digit(char c) {
		return '0' <= c && c <= '9';
	}

	static bool is_alphabetic(char c) {
		return ('a' <= c && c <= 'z') ||
				('A' <= c && c <= 'Z') ||
				c == '_';
	}

	static bool is_alphanumeric(char c) {
		return is_alphabetic(c) || is_digit(c);
	}
};

std::vector<token> tokenize(std::string source) {
	lexer_state lexer(std::move(source));
	lexer.lex_tokens();
	return lexer.tokens;
}

} // namespace pierogi::lexer
