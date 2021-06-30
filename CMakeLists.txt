cmake_minimum_required(VERSION 3.17)
project(pierogi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(TEST_SRC_DIR "${CMAKE_SOURCE_DIR}/test")
file(GLOB TEST_SRC CONFIGURE_DEPENDS "${TEST_SRC_DIR}/*")

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
file(GLOB SRC CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")

set(PSTL_DIR "${CMAKE_SOURCE_DIR}/pstl")

add_custom_target(pierogi-ast
		COMMAND python3 "${PSTL_DIR}/pstl.py" "${PSTL_DIR}/main.pstl" "${INCLUDE_DIR}/generated/ast.hpp"
		DEPENDS "${PSTL_DIR}/pstl.py" "${PSTL_DIR}/main.pstl")

add_library(pierogi-core SHARED "${SRC}")
add_dependencies(pierogi-core pierogi-ast)
target_include_directories(pierogi-core PUBLIC "${INCLUDE_DIR}")
target_link_libraries(pierogi-core stdc++fs)

add_executable(pierogi-tests "${TEST_SRC}")
target_link_libraries(pierogi-tests PUBLIC pierogi-core)
target_compile_definitions(pierogi-tests PRIVATE TEST_ASSET_DIR="${CMAKE_SOURCE_DIR}/test/prgi")

add_executable(pierogi-main main.cpp)
target_link_libraries(pierogi-main PUBLIC pierogi-core)
