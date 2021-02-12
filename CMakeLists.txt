cmake_minimum_required(VERSION 3.17)
project(pierogi)

set(CMAKE_CXX_STANDARD 20)

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(GRAMMAR_DIR "${CMAKE_SOURCE_DIR}/grammar")

add_custom_target(
		antlr-headers-generation
		COMMAND antlr4 -Dlanguage=Cpp -no-listener -visitor -o  "${INCLUDE_DIR}/antlr4-runtime" "${GRAMMAR_DIR}/pierogi.g4"
)

add_executable(pierogi-main main.cpp)
target_include_directories(pierogi-main PUBLIC "${INCLUDE_DIR}" /usr/local/include/antlr4-runtime/)
add_dependencies(pierogi-main antlr-headers-generation)
