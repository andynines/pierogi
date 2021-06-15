#include "source_reader.hpp"

#include <catch.hpp>

#include <sstream>

using namespace pierogi::source_reader;

class file_reader_test_wrapper : public file_reader {
public:
	explicit file_reader_test_wrapper(const std::string& s)
		: file_reader(prepare_temp_file(s)) {}

	static const fs::path& prepare_temp_file(const std::string& s) {
		/* WARNING: We use the same temporary file for every test. If we ever wanted to parallelize
		 * these tests, we would need to find a way to create a unique filename for each instance of
		 * this class to use. */
		const fs::path& temp_file_name = get_temp_file_name();
		std::ofstream temp_file(temp_file_name);
		temp_file << s;
		temp_file.close();
		return temp_file_name;
	}

	~file_reader_test_wrapper() override {
		fs::remove(get_temp_file_name());
	}

private:
	static const fs::path& get_temp_file_name() {
		static const fs::path temp_file_name = fs::temp_directory_path() / "temp.prgi";
		return temp_file_name;
	}
};

TEMPLATE_TEST_CASE("Ingest empty string", "", repl_reader, file_reader_test_wrapper) {
	using reader_type = TestType;
	const std::string source;
	auto reader = reader_type(source);

	REQUIRE(reader.at_end());
	REQUIRE(reader.peek_current() == '\0');
	REQUIRE(reader.peek_next() == '\0');

	REQUIRE(reader.consume_current() == '\0');
	REQUIRE(reader.at_end());
	REQUIRE(reader.peek_current() == '\0');
	REQUIRE(reader.peek_next() == '\0');
}

TEMPLATE_TEST_CASE("Ingest single character", "", repl_reader, file_reader_test_wrapper) {
	using reader_type = TestType;
	const std::string source("x");
	auto reader = reader_type(source);

	REQUIRE(!reader.at_end());
	REQUIRE(reader.peek_current() == 'x');
	REQUIRE(reader.peek_next() == '\0');

	REQUIRE(reader.consume_current() == 'x');
	REQUIRE(reader.at_end());
	REQUIRE(reader.peek_current() == '\0');
	REQUIRE(reader.peek_next() == '\0');

	REQUIRE(reader.consume_current() == '\0');
}

TEMPLATE_TEST_CASE("Peeking ahead doesn't affect current position", "", repl_reader, file_reader_test_wrapper) {
	using reader_type = TestType;
	const std::string source("source");
	auto reader = reader_type(source);

	REQUIRE(reader.peek_next() == 'o');
	REQUIRE(reader.peek_current() == 's');

	REQUIRE(reader.consume_current() == 's');
	REQUIRE(reader.consume_current() == 'o');
	REQUIRE(reader.consume_current() == 'u');

	REQUIRE(reader.peek_next() == 'c');
	REQUIRE(reader.peek_current() == 'r');
	REQUIRE(!reader.at_end());
}

TEMPLATE_TEST_CASE("Ingest long, multiline string", "", repl_reader, file_reader_test_wrapper) {
	using reader_type = TestType;
	const std::string source(
		"If you are interested in stories with happy endings, \n" \
		"you would be better off reading some other book. \n" \
		"In this book, not only is there no happy ending, \n" \
		"there is no happy beginning and very few happy things in the middle.");
	auto reader = reader_type(source);

	std::stringstream ss;
	while (!reader.at_end()) ss << reader.consume_current();
	REQUIRE(ss.str() == source);
}
