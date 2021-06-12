//#ifndef PIEROGI_CONTEXT_HPP
//#define PIEROGI_CONTEXT_HPP
//
//#include <experimental/filesystem>
//#include <string>
//
//namespace fs = std::experimental::filesystem;
//
//namespace pierogi {
//
//class context {
//	static bool had_error;
//
//public:
//	context(int argc, const char* argv[]);
//	static void error(int line, const std::string& message);
//
//private:
//	static void run_file(const fs::path& path);
//	static void run_prompt();
//	static void run(const std::string& source);
//	static void report(int line, const std::string& where, const std::string& message);
//};
//
//} // namespace pierogi
//
//#endif // PIEROGI_CONTEXT_HPP
