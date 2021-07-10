#include "parser.hpp"
#include "lexer.hpp"
#include "generated/ast.hpp"

#include "third-party/catch.hpp"

using namespace pierogi;
using namespace pierogi::parser;

class dummy_reporter : public errors::reporter_interface {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
    void report(errors::error_type type, const std::string &message, int line) override {
        // Do nothing
    }
#pragma GCC diagnostic pop
};

static auto error_ignorer = dummy_reporter();

template <typename TExpressionPointer>
void expect_ast_node_type(const std::string& s) {
    auto tokens = lexer::tokenize(s, error_ignorer);
    auto parse_tree = create_ast(tokens, error_ignorer);
    REQUIRE(parse_tree.size() == 1);
    REQUIRE(std::holds_alternative<TExpressionPointer>(parse_tree.front()));
}

template <typename TExpressionPointer, typename TValue>
void expect_ast_node_with_value(const std::string& s, TValue expected) {
    auto tokens = lexer::tokenize(s, error_ignorer);
    auto parse_tree = create_ast(tokens, error_ignorer);
    REQUIRE(parse_tree.size() == 1);
    REQUIRE(std::holds_alternative<TExpressionPointer>(parse_tree.front()));
    TExpressionPointer expression = std::get<TExpressionPointer>(parse_tree.front());
    REQUIRE(expression->value == expected);
}

template <typename TExpressionPointer, typename TExpectedExpressionPointer>
void expect_ast_node_with_inside_type(const std::string& s) {
    auto tokens = lexer::tokenize(s, error_ignorer);
    auto parse_tree = create_ast(tokens, error_ignorer);
    REQUIRE(parse_tree.size() == 1);
    REQUIRE(std::holds_alternative<TExpressionPointer>(parse_tree.front()));
    TExpressionPointer expression = std::get<TExpressionPointer>(parse_tree.front());
    REQUIRE(std::holds_alternative<TExpectedExpressionPointer>(expression->inside));
}

TEST_CASE("Parse valueless literals") {
    expect_ast_node_type<ast::nil_pointer>("nil");
    expect_ast_node_type<ast::true_boolean_pointer>("true");
    expect_ast_node_type<ast::false_boolean_pointer>("false");
}

TEST_CASE("Parse valued literals") {
    expect_ast_node_with_value<ast::number_pointer, types::number>("5", 5);
    expect_ast_node_with_value<ast::string_pointer, types::string>("\"string\"", "string");
}

TEST_CASE("Parse wrapper expressions") {
    expect_ast_node_with_inside_type<ast::group_pointer, ast::number_pointer>("(5)");
    expect_ast_node_with_inside_type<ast::arithmetic_negation_pointer, ast::number_pointer>("-5");
    expect_ast_node_with_inside_type<ast::logical_negation_pointer, ast::true_boolean_pointer>("not true");
}

TEST_CASE("Parse binary expressions") {
    expect_ast_node_type<ast::addition_pointer>("5 + 6");
    expect_ast_node_type<ast::subtraction_pointer>("6 - 5");
    expect_ast_node_type<ast::multiplication_pointer>("5 * 6");
    expect_ast_node_type<ast::division_pointer>("6 / 5");
    expect_ast_node_type<ast::less_than_pointer>("5 < 6");
    expect_ast_node_type<ast::greater_than_pointer>("6 > 5");
    expect_ast_node_type<ast::less_equal_pointer>("5 <= 6");
    expect_ast_node_type<ast::greater_equal_pointer>("6 >= 5");
    expect_ast_node_type<ast::equals_pointer>("5 == 5");
    expect_ast_node_type<ast::not_equals_pointer>("5 /= 6");
}

TEST_CASE("Parse list literals") {
    auto tokens = lexer::tokenize("[1, 2, 3]", error_ignorer);
    auto parse_tree = parser::create_ast(tokens, error_ignorer);
    REQUIRE(parse_tree.size() == 1);
    REQUIRE(std::holds_alternative<ast::list_pointer>(parse_tree.front()));
    auto contents = std::get<ast::list_pointer>(parse_tree.front())->contents;
    REQUIRE(contents.size() == 3);
    REQUIRE(std::all_of(contents.begin(), contents.end(), [](const ast::expression& expression) {
        return std::holds_alternative<ast::number_pointer>(expression);
    }));
}

// should reject [1,2,3,]
// test unclosed parenthesis
// nest existing operators
// nest list expressions
// identifier 123abc should cause error
// reject leading and trailing dot
// test all operator precedences
