#ifndef PIEROGI_TYPES_HPP
#define PIEROGI_TYPES_HPP

#include <forward_list>
#include <memory>
#include <optional>
#include <string>
#include <variant>

namespace pierogi::types {

using nil_type = std::nullopt_t;

using number = long double;

using string = std::string;

using boolean = bool;

class list;

using value = std::variant<nil_type, number, string, boolean, list>;

class list : private std::forward_list<value> {

};

} // namespace pierogi::types

#endif // PIEROGI_TYPES_HPP
