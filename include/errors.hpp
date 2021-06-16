#ifndef PIEROGI_ERRORS_HPP
#define PIEROGI_ERRORS_HPP

#include <string>

namespace pierogi::errors {

enum class error_type {
	SYNTAX
};

class reporter_interface {
	virtual void report(error_type type, const std::string& message) = 0;
};

}

#endif // PIEROGI_ERRORS_HPP
