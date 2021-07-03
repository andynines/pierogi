#ifndef PIEROGI_TYPES_HPP
#define PIEROGI_TYPES_HPP

#include <forward_list>
#include <memory>
#include <optional>
#include <string>
#include <variant>

namespace pierogi::types {

using nil_type = std::nullopt_t;

class number {
    static constexpr double epsilon = 1e-4;

public:
    using integer = long long int;
    using decimal = long double;

    number() { value.emplace<integer>(0); }
    explicit number(integer i) { value.emplace<integer>(i); }
    explicit number(decimal d) { value.emplace<decimal>(d); }

    bool operator==(const number& rhs) const {
        if (std::holds_alternative<integer>(value)) {
            return std::holds_alternative<integer>(rhs.value) &&
                (std::get<integer>(value) == std::get<integer>(rhs.value));
        } else {
            return std::holds_alternative<decimal>(rhs.value) &&
                std::abs(std::get<decimal>(value) - std::get<decimal>(rhs.value)) <= epsilon;
        }
    }

    bool operator!=(const number& rhs) const {
        return !operator==(rhs);
    }

private:
    std::variant<integer, decimal> value;
};

using string = std::string;

using boolean = bool;

class list;

using value = std::variant<nil_type, number, string, boolean, list>;

class list : private std::forward_list<value> {

};

} // namespace pierogi::types

#endif // PIEROGI_TYPES_HPP
