cmake_minimum_required(VERSION 3.0)
project(system VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

if (UNIX AND NOT APPLE)
    message("Build Unix")
    set(SRC_PLATFORM src/platform-linux/*.cpp)
elseif (APPLE)
    message("Build Apple")
    set(SRC_PLATFORM src/platform-mac-os/*.cpp)
endif ()


file(GLOB SRCS_G
        src/*.cpp
        src/rpc/*.cpp
        ${SRC_PLATFORM}
)

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/../../platform/src
)

add_library(system SHARED ${SRCS_G})

add_subdirectory(testsuite)

if (UNIX AND NOT APPLE)
    target_link_libraries(system platform ${CONAN_LIBS})
elseif (APPLE)
    #find_library(IOKIT_LIBRARY IOKit)
    target_link_libraries(system platform ${CONAN_LIBS} ${IOKIT_LIBRARY})
endif()

SET(CMAKE_LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../lib)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../lib)
