import argparse
import pathlib
import re

from typing import Dict, List

IDENTIFIER_PATTERN = re.compile("[a-zA-Z_][a-zA-Z0-9_]*")
TYPE_PATTERN = re.compile("[a-zA-Z_][a-zA-Z0-9_<>:]*")

HEADER_TEMPLATE = """\
#ifndef PIEROGI_AST_HPP
#define PIEROGI_AST_HPP

#include <string>
#include <variant>
#include <vector>

namespace pierogi::ast {{

{forward_declarations}

using expression = std::variant<{all_class_names}>;

{visitor_interface_definition}

{class_definitions}

}} // namespace pierogi::ast

#endif // PIEROGI_AST_HPP
"""

VISITOR_INTERFACE_TEMPLATE = """\
template <typename T>
struct visitor {{
    {visitors}
}};\
"""

DEFINITION_TEMPLATE = """
struct {name} {{
    {constructor}
    {fields}
}};
"""

# TODO: CLEAN UP (EVERYTHING)


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


def generate_definition(name: str, fields: Dict[str, str]) -> str:
    return DEFINITION_TEMPLATE.format(
        name=name,
        fields='\n    '.join(["{0} {1};".format(field_type, field_name) for field_name, field_type in fields.items()]),
        constructor="explicit {name}({arguments}) : {initializers} {{}}".format(
            name=name,
            arguments=", ".join(["{0} {1}".format(field_type, field_name) for field_name, field_type in fields.items()]),
            initializers=", ".join(["{0}({0})".format(field_name) for field_name in fields.keys()])
        )
    )


def generate_visitor_interface(types: List[str]) -> str:
    return VISITOR_INTERFACE_TEMPLATE.format(
        visitors='\n    '.join(["virtual T operator()(const {0}& e) {{ return T{{}}; }}".format(expression_type) for expression_type in types])
    )


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=pathlib.Path, help="Path to input .pstl file")
    parser.add_argument("output", type=pathlib.Path, help="Path to output .hpp file")
    args = parser.parse_args()

    with open(args.input, 'r') as input_file:
        source = input_file.read()
    field_dict = make_field_dict(source)

    with open(args.output, 'w') as output_file:
        output_file.write(HEADER_TEMPLATE.format(
            forward_declarations='\n'.join("struct %s;" % struct_name for struct_name in field_dict.keys()),
            all_class_names=", ".join(field_dict.keys()),
            visitor_interface_definition=generate_visitor_interface(list(field_dict.keys())),
            class_definitions="".join([generate_definition(name, fields) for name, fields in field_dict.items()])
        ))  # TODO: move subtemplates into main template
