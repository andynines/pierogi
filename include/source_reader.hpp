#ifndef PIEROGI_SOURCE_READER_HPP
#define PIEROGI_SOURCE_READER_HPP

#include <string>

#include <cstddef>

namespace pierogi::source_reader {

class source_reader_interface {
public:
	virtual ~source_reader_interface() = default;

	[[nodiscard]] virtual bool at_end() const = 0;
	[[nodiscard]] virtual char get_current() const = 0;
	virtual char consume_next() = 0;
	[[nodiscard]] virtual char lookahead_one() const = 0;
	[[nodiscard]] virtual char lookahead_two() const = 0;

protected:
	source_reader_interface() = default;
};

class repl_reader : public source_reader_interface {
public:
	explicit repl_reader(const std::string& s);
	[[nodiscard]] bool at_end() const override;
	[[nodiscard]] char get_current() const override;
	char consume_next() override;
	[[nodiscard]] char lookahead_one() const override;
	[[nodiscard]] char lookahead_two() const override;

private:
	std::string source;
	size_t current;
};

} // namespace pierogi::source_reader

#endif // PIEROGI_SOURCE_READER_HPP
