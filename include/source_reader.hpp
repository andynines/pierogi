#ifndef PIEROGI_SOURCE_READER_HPP
#define PIEROGI_SOURCE_READER_HPP

#include <experimental/filesystem>
#include <fstream>
#include <iterator>
#include <string>

namespace pierogi::source_reader {

namespace fs = std::experimental::filesystem;

class source_reader_interface {
public:
	virtual ~source_reader_interface() = default;

	[[nodiscard]] virtual bool at_end() const = 0;
	virtual char consume_current() = 0;
	[[nodiscard]] virtual char peek_current() const = 0;
	[[nodiscard]] virtual char peek_next() const = 0;

protected:
	source_reader_interface() = default;
};

class repl_reader : public source_reader_interface {
public:
	explicit repl_reader(const std::string& s);
	[[nodiscard]] bool at_end() const override;
	char consume_current() override;
	[[nodiscard]] char peek_current() const override;
	[[nodiscard]] char peek_next() const override;

private:
	std::string source;
	size_t current;
};

class file_reader : public source_reader_interface {
public:
	using iterator = std::istreambuf_iterator<char>;

	explicit file_reader(const fs::path& path);
	[[nodiscard]] bool at_end() const override;
	char consume_current() override;
	[[nodiscard]] char peek_current() const override;
	[[nodiscard]] char peek_next() const override;

	~file_reader() override;

private:
	static iterator end;

	std::ifstream ifs_current; iterator it_current;
	std::ifstream ifs_lookahead; iterator it_lookahead;
};

} // namespace pierogi::source_reader

#endif // PIEROGI_SOURCE_READER_HPP
