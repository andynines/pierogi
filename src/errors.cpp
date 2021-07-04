#include "errors.hpp"

#include <iostream>

namespace pierogi::errors {

void console_reporter::report(error_type type, const std::string& near_lexeme, int line) {
    const std::unordered_map<error_type, std::string> error_type_names({
        {error_type::UNRECOGNIZED_CHARACTER, "Unrecognized token near " + near_lexeme}
    });
}

} // namespace pierogi::errors
