cmake_minimum_required(VERSION 3.0)

project(www VERSION 1.0 LANGUAGES CXX)

include_directories(${CMAKE_JS_INC})

message(Test ${CMAKE_JS_INC})

file(GLOB SRCS_G
        src/*.js
        src/views/*.js
        src/routes/*.js
        src/components/*.js
        )

add_library(${PROJECT_NAME} SHARED ${SRCS_G} ${CMAKE_JS_SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})