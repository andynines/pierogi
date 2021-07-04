import pstl

import unittest


class ParserTest(unittest.TestCase):

    def test_name_extraction(self):
        self.assertEqual(pstl.extract_name("negation | expression operand"), "negation")
        self.assertEqual(pstl.extract_name("negation     |expression operand"), "negation")

    def test_field_extraction(self):
        self.assertEqual(pstl.extract_fields("negation | expression operand"), {"operand": "expression"})
        self.assertEqual(pstl.extract_fields("binary | expression lhs, token op, expression rhs"),
                         {
                             "lhs": "expression",
                             "op": "token",
                             "rhs": "expression"
                         })
        self.assertEqual(pstl.extract_fields("body | std::vector<expression> expressions"),
                         {"expressions": "std::vector<expression>"})
        self.assertEqual(pstl.extract_fields("memberless"), {})

    def test_complete_parsing(self):
        self.assertEqual(pstl.make_source_dict("""
            negation | expression operand
            binary | expression lhs, token op, expression rhs
            call | std::string name, list args
            empty
            """),
                         {
                             "negation": {"operand": "expression"},
                             "binary": {
                                 "lhs": "expression",
                                 "op": "token",
                                 "rhs": "expression"
                             },
                             "call": {
                                 "name": "std::string",
                                 "args": "list"
                             },
                             "empty": {}
                         })


if __name__ == "__main__":
    unittest.main()
