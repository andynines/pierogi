#include "lexer.hpp"

#include <sstream>
#include <unordered_map>
#include <utility>

namespace pierogi::lexer {

token::token(token_type type, std::string lexeme, types::value value, int line) :
    type(type), lexeme(std::move(lexeme)), value(std::move(value)), line(line) {
}

struct lexer_state {
    std::string source;
    errors::reporter_interface& error_reporter;
    std::vector<token> tokens;
    int line = 1;
    size_t lexeme_start_index = 0, current_char_index = 0;

    lexer_state(std::string source,
                errors::reporter_interface& error_reporter) :
        source(std::move(source)), error_reporter(error_reporter) {
    }
    
    [[nodiscard]] bool at_end() const {
        return current_char_index >= source.size();
    }
    
    char consume_current() {
        if (at_end()) return '\0';
        return source[current_char_index++];
    }
    
    [[nodiscard]] char peek_current() const {
        if (at_end()) return '\0';
        return source[current_char_index];
    }

    [[nodiscard]] char peek_next() const {
        if (current_char_index + 1 >= source.size()) return '\0';
        return source[current_char_index + 1];
    }

    [[nodiscard]] std::string get_current_lexeme() const {
        return source.substr(lexeme_start_index, current_char_index - lexeme_start_index);
    }

    void lex_tokens() {
        while (!at_end()) lex_next_token();
        tokens.emplace_back(token_type::EOF_, "", std::nullopt, line);
    }

    void lex_next_token() {
        lexeme_start_index = current_char_index;
        char c = consume_current();
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
        case '^':
            add_token(token_type::CARET);
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
        case '=':
            add_token(consume_current_if_matches('=') ? token_type::EQUAL_EQUAL : token_type::EQUAL);
            break;
        case '<':
            add_token(consume_current_if_matches('=') ? token_type::LESS_EQUAL : token_type::LESS_THAN);
            break;
        case '>':
            add_token(consume_current_if_matches('=') ? token_type::GREATER_EQUAL : token_type::GREATER_THAN);
            break;
        case '.':
            add_token(consume_current_if_matches('.') ? token_type::DOT_DOT : token_type::DOT);
            break;
        case '/':
            add_token(consume_current_if_matches('=') ? token_type::NOT_EQUAL : token_type::SLASH);
            break;
        case ' ':
        case '\t':
        case '\r':
            break;
        case '\n':
            line++;
            break;
        case '#':
            while (peek_current() != '\n' && !at_end())
                consume_current();
            break;
        case '"':
            // TODO: Recognize single-quoted strings too
            consume_string();
            break;
        default:
            if (is_digit(c)) {
                consume_number();
            } else if (is_alphabetic(c)) {
                consume_word();
            } else {
                error_reporter.report(errors::error_type::UNRECOGNIZED_CHARACTER, get_current_lexeme(), line);
            }
        }

    }

    void add_token(token_type type) {
        add_token(type, "", std::nullopt); // TODO: add a layer of indirection over our internal NIL representation
    }

    void add_token(token_type type, const std::string& lexeme, const types::value& value) {
        tokens.emplace_back(type, lexeme, value, line);
    }

    bool consume_current_if_matches(char expected) {
        if (at_end() || peek_current() != expected) return false;
        consume_current();
        return true;
    }

    void consume_string() {
        while (peek_current() != '"') {
            if (at_end()) {
                error_reporter.report(errors::error_type::UNTERMINATED_STRING, get_current_lexeme(), line);
                return;
            }
            else if (peek_current() == '\n') line++;
            consume_current();
        }
        consume_current(); // Consume closing '"'
        std::string lexeme = get_current_lexeme();
        std::string contents = lexeme.substr(1, lexeme.size() - 2);
        add_token(token_type::STRING, lexeme, contents);
    }

    void consume_number() {
        while (is_digit(peek_current()))
            consume_current();
        if (peek_current() == '.' && is_digit(peek_next())) {
            consume_current(); // Consume '.'
            while (is_digit(peek_current()))
                consume_current();
        }
        const std::string lexeme = get_current_lexeme();
        types::number value = std::stold(lexeme);
        add_token(token_type::NUMBER, lexeme, value);
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
        while (is_alphanumeric(peek_current())) consume_current();
        auto it = keywords.find(get_current_lexeme());
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

std::vector<token> tokenize(const std::string& source,
                            errors::reporter_interface& error_reporter) {
    lexer_state lexer(source, error_reporter);
    lexer.lex_tokens();
    return lexer.tokens;
}

} // namespace pierogi::lexer
