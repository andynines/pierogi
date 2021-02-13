cmake_minimum_required(VERSION 3.17)
project(pierogi)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(INCLUDE_DIR  "${CMAKE_SOURCE_DIR}/include")
set(ANTLR_GENERATED_DIR "${INCLUDE_DIR}/antlr4-generated")
set(GRAMMAR_DIR "${CMAKE_SOURCE_DIR}/grammar")

add_custom_target(
		antlr-headers-generation
		COMMAND antlr4 -Dlanguage=Cpp -no-listener -visitor -o "${ANTLR_GENERATED_DIR}" "${GRAMMAR_DIR}/pierogi.g4")

file(GLOB GENERATED_SRC "${ANTLR_GENERATED_DIR}/*.cpp")

add_executable(pierogi-main main.cpp)
add_dependencies(pierogi-main antlr-headers-generation)
target_sources(pierogi-main PUBLIC "${GENERATED_SRC}")
target_include_directories(pierogi-main
		PUBLIC "${INCLUDE_DIR}"
		PUBLIC "${INCLUDE_DIR}/thirdparty/antlr4-runtime")
target_link_libraries(pierogi-main "${CMAKE_SOURCE_DIR}/lib/libantlr4-runtime.so")
