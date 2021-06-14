#include "source_reader.hpp"
#include "lexer.hpp"

#include <catch.hpp>

#include <algorithm>

using namespace pierogi::source_reader;
using namespace pierogi::lexer;

const fs::path test_assets_dir = fs::path(TEST_ASSET_DIR);

void expect_eof(const std::string& s) {
	auto reader = repl_reader(s);
	auto tokens = tokenize(reader);
	REQUIRE(tokens.size() == 1);
	REQUIRE(tokens[0].type == token_type::TOKEN_EOF);
}

std::vector<token_type> extract_token_types(const std::string& s) {
	auto reader = repl_reader(s);
	auto tokens = tokenize(reader);
	std::vector<token_type> token_types(tokens.size());
	std::transform(tokens.begin(), tokens.end(), token_types.begin(), [](const token& t) {
		return t.type;
	});
	return token_types;
}

void expect_single_token(const std::string& s, token_type expected) {
	auto token_types = extract_token_types(s);
	REQUIRE_THAT(token_types, Catch::Equals<token_type>({expected, token_type::TOKEN_EOF}));
}

void expect_token_sequence(const std::string& s, std::vector<token_type>&& expecteds) {
	auto token_types = extract_token_types(s);
	expecteds.push_back(token_type::TOKEN_EOF);
	REQUIRE_THAT(token_types, Catch::Equals<token_type>(expecteds));
}

TEST_CASE("Read EOF from empty source") {
	expect_eof("");
}

TEST_CASE("Recognize each token type individually") {
	expect_single_token("\\", token_type::BACKSLASH);
	expect_single_token("+", token_type::PLUS);
	expect_single_token("-", token_type::MINUS);
	expect_single_token("*", token_type::ASTERISK);
	expect_single_token("/", token_type::SLASH);
	expect_single_token("^", token_type::CARET);
	expect_single_token("<", token_type::LESS_THAN);
	expect_single_token(">", token_type::GREATER_THAN);
	expect_single_token("=", token_type::EQUAL);
	expect_single_token("(", token_type::LEFT_PARENTHESIS);
	expect_single_token(")", token_type::RIGHT_PARENTHESIS);
	expect_single_token("[", token_type::LEFT_SQUARE_BRACKET);
	expect_single_token("]", token_type::RIGHT_SQUARE_BRACKET);
	expect_single_token("{", token_type::LEFT_BRACE);
	expect_single_token("}", token_type::RIGHT_BRACE);
	expect_single_token(":", token_type::COLON);
	expect_single_token(".", token_type::DOT);
	expect_single_token(",", token_type::COMMA);

	expect_single_token("!=", token_type::NOT_EQUAL);
	expect_single_token("<=", token_type::LESS_EQUAL);
	expect_single_token(">=", token_type::GREATER_EQUAL);
	expect_single_token("..", token_type::DOT_DOT);

	expect_single_token("and", token_type::AND);
	expect_single_token("or", token_type::OR);
	expect_single_token("not", token_type::NOT);
	expect_single_token("true", token_type::TRUE);
	expect_single_token("false", token_type::FALSE);
	expect_single_token("nil", token_type::NIL);

	expect_single_token("\"string\"", token_type::STRING);
	expect_single_token("123", token_type::NUMBER);
	expect_single_token("123.45", token_type::NUMBER);
	expect_single_token("identifier", token_type::IDENTIFIER);
}

TEST_CASE("Ignore whitespace on one line") {
	expect_eof(" \t   \r");
}

TEST_CASE("Ignore comment on its own line") {
	expect_eof("# Test comment");
}

TEST_CASE("Ignore comment on a line with tokens") {
	expect_single_token("+ # Another test comment", token_type::PLUS);
}

TEST_CASE("Recognize all tokens in sequence") {

}

//TEST_CASE("Tokens contain correct line number") {
//	REQUIRE_THAT(tokenize("\n\n*"), Catch::Equals<token>({
//		token(token_type::ASTERISK, "*", nullptr, 3),
//		token(token_type::TOKEN_EOF, "", nullptr, 3)}));
//}

// Extract string contents (all kinds of characters as contents)
// Increment line number when string has newlines
// Multiline strings
// number contents
// reject leading and trailing dot
// all sorts of error reporting
// or vs orchid, etc.
// identifier 123abc should cause error
