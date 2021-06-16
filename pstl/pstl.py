import argparse
import pathlib
import re

from typing import Dict

IDENTIFIER_PATTERN = re.compile("[a-zA-Z_][a-zA-Z0-9_]*")
TYPE_PATTERN = re.compile("[a-zA-Z_][a-zA-Z0-9_<>:]*")

HEADER_TEMPLATE = """
// Generated at {generation_time}
#ifndef PIEROGI_AST_HPP
#define PIEROGI_AST_HPP

#include <variant>

namespace pierogi::ast {

using expression = std::variant<{all_class_names}>;

{class_definitions}

} // namespace pierogi::ast

#endif // PIEROGI_AST_HPP
"""


def extract_name(line: str) -> str:
    name = line.split('=')[0].strip()
    assert re.fullmatch(IDENTIFIER_PATTERN, name)
    return name


def extract_fields(line: str) -> Dict[str, str]:
    fields = line.split('=')[1]
    pairs = [s.split() for s in fields.split(',')]
    field_types = {pair[1]: pair[0] for pair in pairs}
    for name, type in field_types.items():
        assert re.fullmatch(IDENTIFIER_PATTERN, name) and re.fullmatch(TYPE_PATTERN, type)
    return field_types


def make_field_dict(source: str) -> Dict[str, Dict[str, str]]:
    lines = filter(lambda line: line.strip() != '', source.split('\n'))
    field_dict = {extract_name(line): extract_fields(line) for line in lines}
    return field_dict


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=pathlib.Path, help="Path to .pstl file")
    args = parser.parse_args()

    with open(args.input, 'r') as input_file:
        source = input_file.read()
    field_dict = make_field_dict(source)
    print(field_dict)
