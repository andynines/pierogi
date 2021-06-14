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
	source_reader::source_reader_interface& source;
	std::vector<token> tokens;
	std::stringstream lexeme;
	int line = 1;

	explicit lexer_state(source_reader::source_reader_interface& source) : source(source) {
	}

	void lex_tokens() {
		while (!source.at_end()) lex_next_token();
		tokens.emplace_back(token_type::TOKEN_EOF, "", nullptr, line);
	}

	void lex_next_token() {
		lexeme.str(std::string());
		lexeme.clear();
		lexeme << source.peek_current();
		char c = source.consume_current();
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
				add_token(consume_current_if_matches('=') ? token_type::LESS_EQUAL : token_type::LESS_THAN);
				break;
			case '>':
				add_token(consume_current_if_matches('=') ? token_type::GREATER_EQUAL : token_type::GREATER_THAN);
				break;
			case '!':
				if (consume_current_if_matches('=')) add_token(token_type::NOT_EQUAL); // TODO: raise error for singular !
				break;
			case '.':
				add_token(consume_current_if_matches('.') ? token_type::DOT_DOT : token_type::DOT);
				break;
			case ' ':
			case '\t':
			case '\r':
				break;
			case '\n':
				line++;
				break;
			case '#':
				while (source.peek_current() != '\n' && !source.at_end()) source.consume_current();
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

	void add_token(token_type type) {
		tokens.emplace_back(type, "", nullptr, line);
	}

	bool consume_current_if_matches(char expected) {
		if (source.at_end() || source.peek_current() != expected) return false;
		source.consume_current();
		return true;
	}

	void consume_string() {
		while (source.peek_current() != '"' && !source.at_end()) {
			if (source.peek_current() == '\n') line++;
			source.consume_current();
		}
		source.consume_current(); // Consume closing "
		add_token(token_type::STRING);
	}

	void consume_number() {
		while (is_digit(source.peek_current())) source.consume_current();
		if (source.peek_current() == '.' && is_digit(source.peek_next())) {
			source.consume_current(); // Consume .
			while (is_digit(source.peek_current())) source.consume_current();
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
		while (is_alphanumeric(source.peek_current())) lexeme << source.consume_current();
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

std::vector<token> tokenize(source_reader::source_reader_interface& source) {
	lexer_state lexer(source);
	lexer.lex_tokens();
	return lexer.tokens;
}

} // namespace pierogi::lexer
