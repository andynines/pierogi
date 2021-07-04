#include "parser.hpp"

namespace pierogi::parser {

struct state {
    const std::vector<lexer::token>& tokens;
    errors::reporter_interface& error_reporter;
    std::vector<ast::expression> expressions;
    size_t current_token_index = 0;

    explicit state(const std::vector<lexer::token>& tokens,
                   errors::reporter_interface& error_reporter)
        : tokens(tokens), error_reporter(error_reporter) {}

    [[nodiscard]] bool at_end() const {
        return peek_current().type == lexer::token_type::EOF_;
    }

    [[nodiscard]] const lexer::token& peek_current() const {
        return tokens[current_token_index];
    }

    const lexer::token& consume_current() {
        if (!at_end()) current_token_index++;
        return peek_previous();
    }

    [[nodiscard]] const lexer::token& peek_previous() const {
        return tokens[current_token_index - 1];
    }

    void parse_tokens() {
        while (!at_end()) parse_next_expression();
    }

    void parse_next_expression() {

    }

};

std::vector<ast::expression> create_ast(const std::vector<lexer::token>& tokens,
                                        errors::reporter_interface& error_reporter) {
    state parser(tokens, error_reporter);
    parser.parse_tokens();
    return parser.expressions;
}

} // namespace pierogi::parser
