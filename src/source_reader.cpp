#include <source_reader.hpp>

namespace pierogi::source_reader { // TODO: read from source_reader interface in lexer

repl_reader::repl_reader(const std::string &s) : source(s), current(0) {}

bool repl_reader::at_end() const {
	return current >= source.size();
}

char repl_reader::get_current() const {
	return source[current];
}

char repl_reader::consume_next() {
	return source[current++];
}

char repl_reader::lookahead_one() const {
	if (at_end()) return '\0';
	else return source[current];
}

char repl_reader::lookahead_two() const {
	if (current + 1 >= source.size()) return '\0';
	return source[current + 1];
}

} // namespace pierogi::source_reader
