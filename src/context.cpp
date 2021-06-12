//#include "context.hpp"
//#include "lexer.hpp"
//
//#include <fstream>
//#include <iostream>
//
//namespace pierogi {
//
//bool context::had_error = false;
//
//context::context(int argc, const char* argv[]) {
//	if (argc > 2) {
//		std::cerr << "Usage: " << argv[0] << " [script]" << std::endl;
//		exit(64);
//	} else if (argc == 2) {
//		run_file(argv[1]);
//	} else {
//		run_prompt();
//	}
//}
//
//void context::run_file(const fs::path& path) {
//	std::ifstream input(path, std::ios::binary);
//	std::string source(
//			(std::istreambuf_iterator<char>(input)),
//			std::istreambuf_iterator<char>());
//	input.close();
//	had_error = false;
//	run(source);
//	if (had_error) exit(65);
//}
//
//void context::run_prompt() {
//	std::string input;
//	for (;;) {
//		std::cout << "pierogi> ";
//		std::getline(std::cin, input);
//		if (input.empty()) break;
//		run(input);
//		had_error = false;
//	}
//}
//
//void context::run(const std::string& source) {
//	pierogi::lexer lexer(source);
//	std::vector<pierogi::token> tokens = lexer.lex_tokens();
//	for (auto token : tokens) {
//		std::cout << static_cast<std::string>(token) << std::endl;
//	}
//}
//
//// TODO: create an error-reporting interface and pass it into the lexer
//// TODO: report on error column in addition to line
//void context::error(int line, const std::string& message) {
//	report(line, "", message);
//}
//
//void context::report(int line, const std::string& where, const std::string& message) {
//	std::cerr << "[line " << line << "] error" << where << ": " << message << std::endl;
//	had_error = true;
//}
//
//} // namespace pierogi
