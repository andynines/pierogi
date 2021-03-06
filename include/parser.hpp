#ifndef PIEROGI_PARSER_HPP
#define PIEROGI_PARSER_HPP

#include "generated/ast.hpp"
#include "lexer.hpp"

namespace pierogi::parser {

std::vector<ast::expression> create_ast(const std::vector<lexer::token>& tokens,
                                        errors::reporter_interface& error_reporter);

} // namespace pierogi::parser

#endif // PIEROGI_PARSER_HPP
