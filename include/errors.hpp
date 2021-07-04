#ifndef PIEROGI_ERRORS_HPP
#define PIEROGI_ERRORS_HPP

#include <string>
#include <unordered_map>

namespace pierogi::errors {

enum class error_type {
    UNRECOGNIZED_CHARACTER,
    UNTERMINATED_STRING
};

class reporter_interface {
public:
    virtual void report(error_type type, const std::string& near_lexeme, int line) = 0;
};

// TODO: Move this class up to the interpreter's level
class console_reporter : public reporter_interface {
public:
	void report(error_type type, const std::string& near_lexeme, int line) override;
};

} // namespace pierogi::errors

#endif // PIEROGI_ERRORS_HPP
