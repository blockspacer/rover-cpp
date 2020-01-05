cmake_minimum_required(VERSION 3.0)
project(system_testsuite VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fvisibility=hidden")

file(GLOB TEST_SRCS src/*.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/../src)

enable_testing()

add_executable(system_testsuite ${TEST_SRCS})

target_link_libraries(system_testsuite PRIVATE system ${CONAN_LIBS})

add_test(NAME system_testsuite WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND system_testsuite)