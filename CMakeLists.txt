cmake_minimum_required(VERSION 3.0)
project(rover VERSION 1.0)

if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(NO_OUTPUT_DIRS)

    if (APPLE)
        message("Apple build")
        find_library(IOKIT_LIBRARY IOKit)
        find_library(CORE_LIBRARY CoreFoundation)
    else()
        message("Unix build")
    endif()
    set (MAIN_LIBS ${CONAN_LIBS} ${IOKIT_LIBRARY} ${CORE_LIBRARY})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fvisibility=hidden")
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_INSTALL_NAME_DIR "")

enable_testing()

#add_subdirectory(lib/arduino)
#add_subdirectory(lib/json)
#add_subdirectory(lib/serial)
add_subdirectory(mqtt)
add_subdirectory(platform)
add_subdirectory(plugins/system)
add_subdirectory(service)

add_subdirectory(incubator)