import argparse
import pathlib
import re

from typing import Dict, List

IDENTIFIER_PATTERN = re.compile("[a-zA-Z_][a-zA-Z0-9_]*")
TYPE_PATTERN = re.compile("[a-zA-Z_][a-zA-Z0-9_<>:]*")

HEADER_TEMPLATE = """\
#ifndef PIEROGI_AST_HPP
#define PIEROGI_AST_HPP

#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace pierogi::ast {{

{forward_declarations}

{pointer_type_aliases}

using expression = std::variant<{all_node_pointer_types}>;

template <typename T>
struct visitor {{
    {visitor_methods}
}};

{class_definitions}

}} // namespace pierogi::ast

#endif // PIEROGI_AST_HPP
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


def make_source_dict(source: str) -> Dict[str, Dict[str, str]]:
    lines = filter(lambda line: line.strip() != '', source.split('\n'))
    source_dict = {extract_name(line): extract_fields(line) for line in lines}
    return source_dict


def get_node_types(source_dict: Dict[str, Dict[str, str]]) -> List[str]:
    return list(source_dict.keys())


def get_field_names(node_type: str, source_dict: Dict[str, Dict[str, str]]) -> List[str]:
    return list(source_dict[node_type].keys())


def generate_node_pointer_types(node_types: List[str]) -> List[str]:
    return ["{0}_pointer".format(node_type) for node_type in node_types]


def generate_forward_declarations(node_types: List[str]) -> str:
    declarations = ["struct {0};".format(node_type) for node_type in node_types]
    return "\n".join(declarations)


def generate_pointer_type_aliases(node_types: List[str], node_pointer_types: List[str]) -> str:
    alias_template = "using {0} = std::unique_ptr<{1}>;"
    aliases = [alias_template.format(node_pointer_type, node_type)
               for node_pointer_type, node_type in zip(node_pointer_types, node_types)]
    return "\n".join(aliases)


def generate_node_pointer_type_list(node_types: List[str]) -> str:
    return ", ".join(node_types)


def generate_visitor_methods(node_types: List[str]) -> str:
    method_template = "virtual T operator()(const {0}& e) {{ return T{{}}; }}"
    methods = [method_template.format(node_type) for node_type in node_types]
    return "\n    ".join(methods)


def generate_constructor_arguments(node_type: str, source_dict: Dict[str, Dict[str, str]]) -> str:
    field_names = get_field_names(node_type, source_dict)
    arguments = ["expression {0}".format(field_name) for field_name in field_names]
    return ", ".join(arguments)


def generate_field_initializers(node_type: str, source_dict: Dict[str, Dict[str, str]]) -> str:
    field_names = get_field_names(node_type, source_dict)
    initializers = ["{0}({0})".format(field_name) for field_name in field_names]
    return ", ".join(initializers)


def generate_fields(node_type: str, source_dict: Dict[str, Dict[str, str]]) -> str:
    field_type_dict = source_dict[node_type]
    fields = ["{1} {0};".format(field_name, field_type) for field_name, field_type in field_type_dict.items()]
    return "\n    ".join(fields)


def generate_class_definitions(node_types: List[str], source_dict: Dict[str, Dict[str, str]]) -> str:
    class_definition_template = """struct {node_type} final {{
    explicit {node_type}({constructor_arguments}) : {field_initializers} {{}}
    {fields}
}};"""
    definitions = [class_definition_template.format(
        node_type=node_type,
        constructor_arguments=generate_constructor_arguments(node_type, source_dict),
        field_initializers=generate_field_initializers(node_type, source_dict),
        fields=generate_fields(node_type, source_dict)
    ) for node_type in node_types]
    return "\n\n".join(definitions)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=pathlib.Path, help="Path to input .pstl file")
    parser.add_argument("output", type=pathlib.Path, help="Path to output .hpp file")
    args = parser.parse_args()

    with open(args.input, "r") as input_file:
        source = input_file.read()

    source_dict = make_source_dict(source)
    node_types = get_node_types(source_dict)
    node_pointer_types = generate_node_pointer_types(node_types)

    with open(args.output, "w") as output_file:
        output_file.write(HEADER_TEMPLATE.format(
            forward_declarations=generate_forward_declarations(node_types),
            pointer_type_aliases=generate_pointer_type_aliases(node_types, node_pointer_types),
            all_node_pointer_types=generate_node_pointer_type_list(node_pointer_types),
            visitor_methods=generate_visitor_methods(node_types),
            class_definitions=generate_class_definitions(node_types, source_dict)
        ))
