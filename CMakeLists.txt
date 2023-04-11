cmake_minimum_required(VERSION 3.0)

project(Themechanger)

include_directories(src/header)
include_directories(lib/json/single_include)

set(SOURCES
    src/src/main.cpp
    src/src/ThemeChanger.cpp
)

set(HEADERS
    src/header/main.hpp
)

add_executable(themechanger ${SOURCES} ${HEADERS})