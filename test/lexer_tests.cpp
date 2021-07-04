#include "types.hpp"
#include "errors.hpp"
#include "lexer.hpp"

#include "third-party/catch.hpp"

#include <algorithm>
#include <optional>

using namespace pierogi;
using namespace pierogi::lexer;

class dummy_reporter : public errors::reporter_interface {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
	void report(errors::error_type type, const std::string &message, int line) override {
		// Do nothing
	}
#pragma GCC diagnostic pop
};

static auto error_ignorer = dummy_reporter();

void expect_eof(const std::string& s) {
	auto tokens = tokenize(s, error_ignorer);
	REQUIRE(tokens.size() == 1);
	REQUIRE(tokens[0].type == token_type::EOF_);
}

std::vector<token_type> extract_token_types(const std::string& s) {
	auto tokens = tokenize(s, error_ignorer);
	std::vector<token_type> token_types(tokens.size());
	std::transform(tokens.begin(), tokens.end(), token_types.begin(), [](const token& t) {
		return t.type;
	});
	return token_types;
}

void expect_single_token(const std::string& s, token_type expected) {
	auto token_types = extract_token_types(s);
	REQUIRE_THAT(token_types, Catch::Equals<token_type>({expected, token_type::EOF_}));
}

void expect_token_sequence(const std::string& s, std::vector<token_type>&& expecteds) {
	auto token_types = extract_token_types(s);
	expecteds.push_back(token_type::EOF_);
	REQUIRE_THAT(token_types, Catch::Equals<token_type>(expecteds));
}

void expect_final_line_number(const std::string& s, int expected) {
    auto tokens = tokenize(s, error_ignorer);
    REQUIRE(tokens.back().line == expected);
}

struct test_error_reporter : public errors::reporter_interface {

	std::vector<errors::error_type> error_types;
    std::vector<std::string> near_lexemes;
    std::vector<int> lines;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
	void report(errors::error_type type, const std::string &near_lexeme, int line) override {
		error_types.push_back(type);
        near_lexemes.push_back(near_lexeme);
        lines.push_back(line);
	}
#pragma GCC diagnostic pop
};

void expect_error_type(const std::string& s, errors::error_type expected) {
    auto error_recorder = test_error_reporter();
    tokenize(s, error_recorder);
    REQUIRE(error_recorder.error_types.size() == 1);
    REQUIRE(error_recorder.error_types.back() == expected);
}

void expect_error_near_lexeme(const std::string& s, const std::string& expected) {
    auto error_recorder = test_error_reporter();
    tokenize(s, error_recorder);
    REQUIRE(error_recorder.near_lexemes.size() == 1);
    REQUIRE(error_recorder.near_lexemes.back() == expected);
}

void expect_error_on_line(const std::string& s, int expected) {
    auto error_recorder = test_error_reporter();
    tokenize(s, error_recorder);
    REQUIRE(error_recorder.lines.size() == 1);
    REQUIRE(error_recorder.lines.back() == expected);
}

void expect_string_lexeme_contents(const std::string& s, const types::string& expected) {
    auto tokens = tokenize(s, error_ignorer);
    // The tokens list will always contain at least an EOF, so a front element definitely exists
    const types::value& value = tokens.front().value;
    REQUIRE(std::holds_alternative<types::string>(value));
    REQUIRE(std::get<types::string>(value) == expected);
}

void expect_number_lexeme_contents(const std::string& s, types::number expected) {
    auto tokens = tokenize(s, error_ignorer);
    const types::value& value = tokens.front().value;
    REQUIRE(std::holds_alternative<types::number>(value));
    REQUIRE(std::get<types::number>(value) == Approx(expected));
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

	expect_single_token("==", token_type::EQUAL_EQUAL);
	expect_single_token("/=", token_type::NOT_EQUAL);
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
    expect_single_token("_another1", token_type::IDENTIFIER);
}

TEST_CASE("Ignore whitespace on one line") {
	expect_eof(" \t   \r");
}

TEST_CASE("Ignore comment on its own line") {
	expect_eof("# Test comment");
}

TEST_CASE("Ignore comment on a line with tokens") {
	expect_token_sequence("2 + 2 # Another test comment", {
		token_type::NUMBER, token_type::PLUS, token_type::NUMBER});
}

TEST_CASE("Read longer sequences of tokens") {
    expect_token_sequence("3.0 / 5 + (6 - 7) * 9^4", {
        token_type::NUMBER, token_type::SLASH, token_type::NUMBER,
        token_type::PLUS, token_type::LEFT_PARENTHESIS, token_type::NUMBER,
        token_type::MINUS, token_type::NUMBER, token_type::RIGHT_PARENTHESIS,
        token_type::ASTERISK, token_type::NUMBER, token_type::CARET,
        token_type::NUMBER
    });
    expect_token_sequence("f(x) : [3, true, false]", {
        token_type::IDENTIFIER, token_type::LEFT_PARENTHESIS, token_type::IDENTIFIER,
        token_type::RIGHT_PARENTHESIS, token_type::COLON, token_type::LEFT_SQUARE_BRACKET,
        token_type::NUMBER, token_type::COMMA, token_type::TRUE,
        token_type::COMMA, token_type::FALSE, token_type::RIGHT_SQUARE_BRACKET
    });
    expect_token_sequence("object.method(not enabled, count == 5)", {
        token_type::IDENTIFIER, token_type::DOT, token_type::IDENTIFIER,
        token_type::LEFT_PARENTHESIS, token_type::NOT, token_type::IDENTIFIER,
        token_type::COMMA, token_type::IDENTIFIER, token_type::EQUAL_EQUAL,
        token_type::NUMBER, token_type::RIGHT_PARENTHESIS
    });
    expect_token_sequence("valid = \\x { x <= 100 or x >= 0 and x /= nil }", {
        token_type::IDENTIFIER, token_type::EQUAL, token_type::BACKSLASH,
        token_type::IDENTIFIER, token_type::LEFT_BRACE, token_type::IDENTIFIER,
        token_type::LESS_EQUAL, token_type::NUMBER, token_type::OR,
        token_type::IDENTIFIER, token_type::GREATER_EQUAL, token_type::NUMBER,
        token_type::AND, token_type::IDENTIFIER, token_type::NOT_EQUAL,
        token_type::NIL, token_type::RIGHT_BRACE
    });
    expect_token_sequence("\"Hello \" .. username", {
        token_type::STRING, token_type::DOT_DOT, token_type::IDENTIFIER
    });
    expect_token_sequence(
        "make_adder = \\n { \\x { x + n } }\n" \
        "add_five = make_adder(5)\n" \
        "print(add_five(7)) # 12\n",
        {
            token_type::IDENTIFIER, token_type::EQUAL, token_type::BACKSLASH,
            token_type::IDENTIFIER, token_type::LEFT_BRACE, token_type::BACKSLASH,
            token_type::IDENTIFIER, token_type::LEFT_BRACE, token_type::IDENTIFIER,
            token_type::PLUS, token_type::IDENTIFIER, token_type::RIGHT_BRACE,
            token_type::RIGHT_BRACE, token_type::IDENTIFIER, token_type::EQUAL,
            token_type::IDENTIFIER, token_type::LEFT_PARENTHESIS, token_type::NUMBER,
            token_type::RIGHT_PARENTHESIS, token_type::IDENTIFIER, token_type::LEFT_PARENTHESIS,
            token_type::IDENTIFIER, token_type::LEFT_PARENTHESIS, token_type::NUMBER,
            token_type::RIGHT_PARENTHESIS, token_type::RIGHT_PARENTHESIS
        });
}

TEST_CASE("Use maximal munch to differentiate identifiers from keywords") {
    expect_single_token("andrew", token_type::IDENTIFIER);
    expect_single_token("orchid", token_type::IDENTIFIER);
    expect_single_token("nothing", token_type::IDENTIFIER);
    expect_single_token("nile_river", token_type::IDENTIFIER);
    expect_single_token("trueness", token_type::IDENTIFIER);
    expect_single_token("falsey", token_type::IDENTIFIER);
}

TEST_CASE("Record line numbers") {
    expect_final_line_number("", 1);
    expect_final_line_number("one_line_source = 1", 1);
    expect_final_line_number("multiple = 1\nline = \"s\"\nsource = nil", 3);
    expect_final_line_number("source_with_multiline_string = \"multi\nline\nstring\"\nx = nil", 4);
}

TEST_CASE("Report error for unrecognized characters") {
    // TODO: recognize the % symbol as the modulo operator
    auto unknown_character = GENERATE('@', '$', '~', '!', '?', '`', '%', '|', '&', '\'');
    std::string source("area = pi * r^2");
    source.insert(unknown_character % source.size(), 1, unknown_character);
    expect_error_type(source, errors::error_type::UNRECOGNIZED_CHARACTER);
}

TEST_CASE("Report error for unterminated string") {
    expect_error_type("\"string", errors::error_type::UNTERMINATED_STRING);
}

TEST_CASE("Record lexeme where error occurred") {
    // TODO: Handle non-ASCII characters that cause unrecognized character errors
    expect_error_near_lexeme("| = 5", "|");
    expect_error_near_lexeme("\"unfinished", "\"unfinished");
}

TEST_CASE("Record number of line where error occurred") {
    expect_error_on_line("x = nil\ny = false\n$ = true", 3);
    expect_error_on_line("a = 1\n\"unfinished", 2);
}

TEST_CASE("Extract contents of string tokens") {
    expect_string_lexeme_contents("\"string\"", "string");
    expect_string_lexeme_contents("\"a string with spaces\"", "a string with spaces");
    expect_string_lexeme_contents("\"string with 1 2 34 numbers\"", "string with 1 2 34 numbers");
    expect_string_lexeme_contents("\"characters like Ʃ and £\"", "characters like Ʃ and £");
    expect_string_lexeme_contents("\"a string\nwith multiple\nlines\"", "a string\nwith multiple\nlines");
    // TODO: offer escape sequences in strings, such as \" and \n
}

TEST_CASE("Extract contents of number tokens") {
    expect_number_lexeme_contents("123", 123);
    expect_number_lexeme_contents("456.", 456);
    expect_number_lexeme_contents("000123", 123);
    expect_number_lexeme_contents("123.456", 123.456);
    expect_number_lexeme_contents("0.0125", 0.0125);
    expect_number_lexeme_contents("00001.23", 1.23);
}
