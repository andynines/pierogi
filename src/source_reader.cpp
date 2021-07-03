#include <source_reader.hpp>

namespace pierogi::source_reader {

repl_reader::repl_reader(const std::string &s) : source(s), current(0) {}

bool repl_reader::at_end() const {
	return current >= source.size();
}

char repl_reader::consume_current() {
	if (at_end()) return '\0';
	return source[current++];
}

char repl_reader::peek_current() const {
	if (at_end()) return '\0';
	return source[current];
}

char repl_reader::peek_next() const {
	if (current + 1 >= source.size()) return '\0';
	return source[current + 1];
}

file_reader::file_reader(const fs::path& path)
	: ifs_current(std::ifstream(path, std::ios::in | std::ios::binary)),
      it_current(iterator(ifs_current)),
      ifs_lookahead(std::ifstream(path, std::ios::in | std::ios::binary)),
      it_lookahead(++iterator(ifs_lookahead)) {}

file_reader::iterator file_reader::end = file_reader::iterator{};

bool file_reader::at_end() const {
	return it_current == end;
}

char file_reader::consume_current() {
	if (at_end()) return '\0';
	it_lookahead++;
	return *(it_current++);
}

char file_reader::peek_current() const {
	if (at_end()) return '\0';
	return *it_current;
}

char file_reader::peek_next() const {
	if (at_end() || it_lookahead == end) return '\0';
	return *it_lookahead;
}

file_reader::~file_reader() {
	ifs_current.close();
	ifs_lookahead.close();
}

} // namespace pierogi::source_reader
