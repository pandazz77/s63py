cmake_minimum_required(VERSION 3.15)
project(s63py LANGUAGES CXX)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/s63lib)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 REQUIRED)
find_package(PythonInterp)
find_package(Python)

set(SRC_WRAPPERS
    s63py.cpp
    s63py_wrappers.hpp
)


pybind11_add_module(${PROJECT_NAME} ${SRC_WRAPPERS})
target_link_libraries(${PROJECT_NAME} PRIVATE s63lib pybind11::module pybind11::lto)