#include "lexer.hpp"

#include <catch.hpp>

#define EXPECT_EOF(s) \
	REQUIRE_THAT(tokenize(s), Catch::Equals<token>({ \
		token(token_type::TOKEN_EOF, "", nullptr, 1)}))

#define EXPECT_SINGLE_TOKEN(s, expected_token_type) \
	REQUIRE_THAT(tokenize(s), Catch::Equals<token>({ \
		token((expected_token_type), (s), nullptr, 1), \
		token(token_type::TOKEN_EOF, "", nullptr, 1)}))

using namespace pierogi::lexer;

TEST_CASE("Read EOF from empty source") {
	EXPECT_EOF("");
}

TEST_CASE("Tokenize single characters") {
	EXPECT_SINGLE_TOKEN("\\", token_type::BACKSLASH);
	EXPECT_SINGLE_TOKEN("+", token_type::PLUS);
	EXPECT_SINGLE_TOKEN("-", token_type::MINUS);
	EXPECT_SINGLE_TOKEN("*", token_type::ASTERISK);
	EXPECT_SINGLE_TOKEN("/", token_type::SLASH);
	EXPECT_SINGLE_TOKEN("^", token_type::CARET);
	EXPECT_SINGLE_TOKEN("<", token_type::LESS_THAN);
	EXPECT_SINGLE_TOKEN(">", token_type::GREATER_THAN);
	EXPECT_SINGLE_TOKEN("=", token_type::EQUAL);
	EXPECT_SINGLE_TOKEN("(", token_type::LEFT_PARENTHESIS);
	EXPECT_SINGLE_TOKEN(")", token_type::RIGHT_PARENTHESIS);
	EXPECT_SINGLE_TOKEN("{", token_type::LEFT_BRACE);
	EXPECT_SINGLE_TOKEN("}", token_type::RIGHT_BRACE);
	EXPECT_SINGLE_TOKEN(".", token_type::DOT);
	EXPECT_SINGLE_TOKEN(",", token_type::COMMA);
}

TEST_CASE("Tokenize two-character operators") {
	EXPECT_SINGLE_TOKEN("==", token_type::EQUAL_EQUAL);
	EXPECT_SINGLE_TOKEN("!=", token_type::NOT_EQUAL);
	EXPECT_SINGLE_TOKEN("<=", token_type::LESS_EQUAL);
	EXPECT_SINGLE_TOKEN(">=", token_type::GREATER_EQUAL);
	EXPECT_SINGLE_TOKEN("..", token_type::DOT_DOT);
}

TEST_CASE("Ignore whitespace on one line") {
	REQUIRE_THAT(tokenize(" \t   \r="), Catch::Equals<token>({
		token(token_type::EQUAL, "=", nullptr, 1),
		token(token_type::TOKEN_EOF, "", nullptr, 1)}));
}

TEST_CASE("Ignore comment on its own line") {
	EXPECT_EOF("# Test comment");
}

TEST_CASE("Ignore comment on a line with tokens") {
	REQUIRE_THAT(tokenize("+ # Another test comment"), Catch::Equals<token>({
		token(token_type::PLUS, "+", nullptr, 1),
		token(token_type::TOKEN_EOF, "", nullptr, 1)}));
}

TEST_CASE("Tokens contain correct line number") {
	REQUIRE_THAT(tokenize("\n\n*"), Catch::Equals<token>({
		token(token_type::ASTERISK, "*", nullptr, 3),
		token(token_type::TOKEN_EOF, "", nullptr, 3)}));
}

// TODO: Handle carriage returns
