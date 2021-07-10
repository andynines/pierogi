#ifndef PIEROGI_TYPES_HPP
#define PIEROGI_TYPES_HPP

#include <forward_list>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <variant>

namespace pierogi::types {

using nil_type = std::nullopt_t; // TODO: needed?

using number = long double;

using string = std::string;

using boolean = bool; // TODO: needed?

class list;

using value = std::variant<nil_type, number, string, boolean, list>;

class list {
    std::forward_list<value> contents;
public:
    explicit list(const std::vector<value>& contents) {
        this->contents.assign(contents.begin(), contents.end());
    }
    list(std::initializer_list<value>&& contents) : contents(contents) {}
};

} // namespace pierogi::types

#endif // PIEROGI_TYPES_HPP
