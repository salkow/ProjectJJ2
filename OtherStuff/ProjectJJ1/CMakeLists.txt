cmake_minimum_required(VERSION 3.5)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TEST_TARGET "run_tests")

project(${TEST_TARGET})

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,leak,undefined")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,leak,undefined")

add_compile_options(-Wall -Wextra -Wpedantic
        -Wdisabled-optimization -Wformat=2 -Winit-self
        -Wmissing-declarations -Wmissing-include-dirs
        -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow
        -Wsign-conversion -Wsign-promo
        -Wstrict-overflow=2 -Wswitch-default -Wundef -Wno-unused
        -ggdb -O0)

file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)
file(GLOB_RECURSE MAIN_SOURCES src/*.cpp)

find_package(Catch2 REQUIRED PATHS "${PROJECT_SOURCE_DIR}/lib")

add_executable(${TEST_TARGET} ${MAIN_SOURCES} ${TEST_SOURCES})
target_link_libraries(${TEST_TARGET} PRIVATE Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(${TEST_TARGET})
