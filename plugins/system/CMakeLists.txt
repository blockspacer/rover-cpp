cmake_minimum_required(VERSION 3.0)
project(system VERSION 1.0)

if (UNIX AND NOT APPLE)
    set(SRC_PLATFORM src/platform-linux/*.cpp)
elseif (APPLE)
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
endif ()

SET(CMAKE_LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../lib)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../lib)
