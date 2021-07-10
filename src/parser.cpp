#include "parser.hpp"

#include <algorithm>

namespace pierogi::parser {

struct state {
    const std::vector<lexer::token>& tokens;
    errors::reporter_interface& error_reporter;
    std::vector<ast::expression> expressions;
    size_t current_token_index = 0;

    state(const std::vector<lexer::token>& tokens,
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

    [[nodiscard]] bool check(lexer::token_type type) const {
        if (at_end()) return false;
        return peek_current().type == type;
    }

    bool matches_current(lexer::token_type type) {
        if (check(type)) {
            consume_current();
            return true;
        }
        return false;
    }

    lexer::token consume_if_matches(lexer::token_type type, const std::string& message) {
        if (check(type)) return consume_current();
        // TODO: report error here
    }

    void parse_tokens() {
        while (!at_end()) parse_next_expression();
    }

    void parse_next_expression() {
        expressions.push_back(parse_expression());
    }

    ast::expression parse_expression() {
        return parse_equality();
    }

    ast::expression parse_equality() {
        ast::expression expression = parse_comparison();
        while (true) {
            if (matches_current(lexer::token_type::EQUAL_EQUAL)) {
                expression = std::make_shared<ast::equals>(expression, parse_comparison());
                continue;
            }
            if (matches_current(lexer::token_type::NOT_EQUAL)) {
                expression = std::make_shared<ast::not_equals>(expression, parse_comparison());
                continue;
            }
            break;
        }
        return expression;
    }

    // TODO: add construction and concat operators (tentatively) between comparisons and equalities

    ast::expression parse_comparison() {
        ast::expression expression = parse_term();
        while (true) {
            if (matches_current(lexer::token_type::LESS_THAN)) {
                expression = std::make_shared<ast::less_than>(expression, parse_term());
                continue;
            }
            if (matches_current(lexer::token_type::GREATER_THAN)) {
                expression = std::make_shared<ast::greater_than>(expression, parse_term());
                continue;
            }
            if (matches_current(lexer::token_type::LESS_EQUAL)) {
                expression = std::make_shared<ast::less_equal>(expression, parse_term());
                continue;
            }
            if (matches_current(lexer::token_type::GREATER_EQUAL)) {
                expression = std::make_shared<ast::greater_equal>(expression, parse_term());
                continue;
            }
            break;
        }
        return expression;
    }

    ast::expression parse_term() {
        ast::expression expression = parse_factor();
        while (true) {
            if (matches_current(lexer::token_type::PLUS)) {
                expression = std::make_shared<ast::addition>(expression, parse_factor());
                continue;
            }
            if (matches_current(lexer::token_type::MINUS)) {
                expression = std::make_shared<ast::subtraction>(expression, parse_factor());
                continue;
            }
            break;
        }
        return expression;
    }

    ast::expression parse_factor() {
        ast::expression expression = parse_unary();
        while (true) {
            if (matches_current(lexer::token_type::ASTERISK)) {
                expression = std::make_shared<ast::multiplication>(expression, parse_unary());
                continue;
            }
            if (matches_current(lexer::token_type::SLASH)) {
                expression = std::make_shared<ast::division>(expression, parse_unary());
                continue;
            }
            break;
        }
        return expression;
    }

    ast::expression parse_unary() {
        if (matches_current(lexer::token_type::MINUS)) {
            return std::make_shared<ast::arithmetic_negation>(parse_unary());
        }
        if (matches_current(lexer::token_type::NOT)) {
            return std::make_shared<ast::logical_negation>(parse_unary());
        }
        return parse_primary();
    }

    ast::expression parse_primary() {
        if (matches_current(lexer::token_type::NIL)) {
            return std::make_shared<ast::nil>();
        }
        if (matches_current(lexer::token_type::TRUE)) {
            return std::make_shared<ast::true_boolean>();
        }
        if (matches_current(lexer::token_type::FALSE)) {
            return std::make_shared<ast::false_boolean>();
        }
        if (matches_current(lexer::token_type::NUMBER)) {
            return std::make_shared<ast::number>(std::get<types::number>(peek_previous().value));
        }
        if (matches_current(lexer::token_type::STRING)) {
            return std::make_shared<ast::string>(std::get<types::string>(peek_previous().value));
        }
        if (matches_current(lexer::token_type::LEFT_SQUARE_BRACKET)) {
            std::vector<ast::expression> contents;
            if (matches_current(lexer::token_type::RIGHT_SQUARE_BRACKET)) {
                return std::make_shared<ast::list>(contents);
            }
            do {
                contents.push_back(parse_expression());
            } while (matches_current(lexer::token_type::COMMA));
            if (!matches_current(lexer::token_type::RIGHT_SQUARE_BRACKET)) {
                // TODO: throw error for unclosed list
            }
            return std::make_shared<ast::list>(contents);
        }
        if (matches_current(lexer::token_type::LEFT_PARENTHESIS)) {
            ast::expression expression = parse_expression();
            consume_if_matches(lexer::token_type::RIGHT_PARENTHESIS, "Expected closing ')' after expression");
            return std::make_shared<ast::group>(expression);
        }
        // TODO: throw an error here since we haven't matched with anything
    }
};

std::vector<ast::expression> create_ast(const std::vector<lexer::token>& tokens,
                                        errors::reporter_interface& error_reporter) {
    state parser(tokens, error_reporter);
    parser.parse_tokens();
    return parser.expressions;
}

} // namespace pierogi::parser
