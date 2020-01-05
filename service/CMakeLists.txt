cmake_minimum_required(VERSION 3.0)
project(service VERSION 1.0)

file(GLOB SRCS_G
        src/*.cpp
        src/exception/*.cpp
        src/http/*.cpp
        src/rpc/*.cpp
        src/properties/*.cpp
        src/subsystem/*.cpp
        src/serial/*.cpp
        )

file(GLOB LIB_SRCS_G
        src/exception/*.cpp
        src/properties/*.cpp
        src/subsystem/*.cpp
        src/http/*.cpp
        src/rpc/*.cpp
        src/serial/*.cpp
        )

include_directories(
        /usr/local/include
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/../platform/src
        ${PROJECT_SOURCE_DIR}/lib
)

link_directories(
        /usr/local/lib
)

add_executable(service ${SRCS_G})
add_library(service_lib ${LIB_SRCS_G})
add_subdirectory(testsuite)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
set(PROPERTIES_DIR ${EXECUTABLE_OUTPUT_PATH}/../etc)
set(PLUGINS_DIR ${EXECUTABLE_OUTPUT_PATH}/../lib)

file(MAKE_DIRECTORY ${PROPERTIES_DIR})
file(MAKE_DIRECTORY ${PLUGINS_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/settings.json DESTINATION ${PROPERTIES_DIR})

message("Copy FE")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/www/build/ DESTINATION ${EXECUTABLE_OUTPUT_PATH}/../var/www)

target_link_libraries(service platform dl serial)



