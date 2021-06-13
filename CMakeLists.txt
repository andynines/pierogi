cmake_minimum_required(VERSION 3.17)
project(pierogi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(TEST_SRC_DIR "${CMAKE_SOURCE_DIR}/test")
file(GLOB TEST_SRC CONFIGURE_DEPENDS "${TEST_SRC_DIR}/*")

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
file(GLOB SRC CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")

add_library(pierogi-core SHARED "${SRC}")
target_include_directories(pierogi-core PUBLIC "${INCLUDE_DIR}")
target_link_libraries(pierogi-core stdc++fs)

add_executable(pierogi-tests "${TEST_SRC}")
target_link_libraries(pierogi-tests PUBLIC pierogi-core)
target_include_directories(pierogi-tests
		PUBLIC "${INCLUDE_DIR}"
		PUBLIC "${INCLUDE_DIR}/third-party")

add_executable(pierogi-main main.cpp)
target_link_libraries(pierogi-main PUBLIC pierogi-core)
target_include_directories(pierogi-main
		PUBLIC "${INCLUDE_DIR}"
		PUBLIC "${INCLUDE_DIR}/third-party")
