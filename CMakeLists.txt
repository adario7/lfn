cmake_minimum_required(VERSION 3.12)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "src/*.cc")

foreach(SRC ${SOURCES})
    get_filename_component(EXE_NAME ${SRC} NAME_WE)
    add_executable(${EXE_NAME} ${SRC})
endforeach()
