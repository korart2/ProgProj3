cmake_minimum_required(VERSION 3.25)
project(ProgrammingProject3 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ProgrammingProject3
    src/main.cpp
    src/work.cpp
)

find_package(Threads REQUIRED)
target_link_libraries(ProgrammingProject3 PRIVATE Threads::Threads)

# /utf-8: the sources and the output contain Ukrainian text in UTF-8.
if(MSVC)
    target_compile_options(ProgrammingProject3 PRIVATE /W4 /EHsc /permissive- /Zc:__cplusplus /utf-8)
else()
    target_compile_options(ProgrammingProject3 PRIVATE -Wall -Wextra -Wpedantic)
endif()
