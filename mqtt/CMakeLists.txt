cmake_minimum_required(VERSION 3.0)
project(mqtt VERSION 1.0)

file(GLOB SRCS_G
        src/*.cpp
        src/protocol/*.cpp
)

include_directories(
        /usr/local/include
        ${PROJECT_SOURCE_DIR}/src
)

link_directories(
        /usr/local/lib
)

add_executable(mqtt ${SRCS_G})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)

target_link_libraries(mqtt ${MAIN_LIBS})



