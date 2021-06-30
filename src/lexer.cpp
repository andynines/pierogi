#include "lexer.hpp"

#include <sstream>
#include <unordered_map>
#include <utility>

namespace pierogi::lexer {

token::token(token_type type, std::string lexeme, types::value value, int line) :
    type(type), lexeme(std::move(lexeme)), value(std::move(value)), line(line) {
}

struct lexer_state {
    source_reader::source_reader_interface& read_head;
    errors::reporter_interface& error_reporter;
    std::vector<token> tokens;
    std::stringstream lexeme;
    int line = 1;

    explicit lexer_state(source_reader::source_reader_interface& source,
                         errors::reporter_interface& error_reporter) :
        read_head(source), error_reporter(error_reporter) {
    }

    void lex_tokens() {
        while (!read_head.at_end()) lex_next_token();
        tokens.emplace_back(token_type::EOF_, "", std::nullopt, line);
    }

    void lex_next_token() {
        lexeme.str(std::string());
        lexeme.clear(); // TODO: determine if this line is necessary
        char c = read_head.consume_current();
        lexeme << c;
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
            if (consume_current_if_matches('=')) {
                add_token(token_type::NOT_EQUAL);
            } else if (consume_current_if_matches('/')) {
                add_token(token_type::SLASH_SLASH);
            } else {
                add_token(token_type::SLASH);
            }
            break;
        case ' ':
        case '\t':
        case '\r':
            break;
        case '\n':
            line++;
            break;
        case '#':
            while (read_head.peek_current() != '\n' && !read_head.at_end())
                read_head.consume_current();
            break;
        case '"':
            consume_string();
            break;
        default:
            if (is_digit(c)) {
                consume_number();
            } else if (is_alphabetic(c)) {
                consume_word();
            } else {
                error_reporter.report(errors::error_type::UNRECOGNIZED_CHARACTER, lexeme.str());
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
        if (read_head.at_end() || read_head.peek_current() != expected) return false;
        read_head.consume_current();
        return true;
    }

    void consume_string() {
        while (read_head.peek_current() != '"') {
            if (read_head.at_end()) {
                error_reporter.report(errors::error_type::UNTERMINATED_STRING, lexeme.str());
                return;
            }
            else if (read_head.peek_current() == '\n') line++;
            lexeme << read_head.consume_current();
        }
        lexeme << read_head.consume_current(); // Consume closing '"'
        types::string contents = lexeme.str();
        contents = contents.substr(1, contents.size() - 2);
        add_token(token_type::STRING, lexeme.str(), contents);
    }

    void consume_number() {
        bool dot_seen = false;
        while (is_digit(read_head.peek_current()))
            read_head.consume_current();
        if (read_head.peek_current() == '.' && is_digit(read_head.peek_next())) {
            dot_seen = true;
            read_head.consume_current(); // Consume '.'
            while (is_digit(read_head.peek_current()))
                read_head.consume_current();
        }
        types::number content;
        //if (dot_seen) content = std::stold(lexeme.str());
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
        while (is_alphanumeric(read_head.peek_current())) lexeme << read_head.consume_current();
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

std::vector<token> tokenize(source_reader::source_reader_interface& source,
                            errors::reporter_interface& error_reporter) {
    lexer_state lexer(source, error_reporter);
    lexer.lex_tokens();
    return lexer.tokens;
}

} // namespace pierogi::lexer
