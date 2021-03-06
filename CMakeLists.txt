cmake_minimum_required(VERSION 3.5)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

set(TARGET "inverted_google")

# SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")

project(${TARGET})

set(CMAKE_CXX_STANDARD 17)

add_compile_options(
        "-Wall" "-Wpedantic" "-Wextra"
        "-Wdisabled-optimization" "-Wformat=2" "-Winit-self"
        "-Wmissing-declarations" "-Wmissing-include-dirs"
        "-Wold-style-cast" "-Woverloaded-virtual" "-Wredundant-decls" "-Wshadow"
        "-Wsign-conversion" "-Wsign-promo"
        "-Wstrict-overflow=2" "-Wswitch-default" "-Wundef" "-Wno-unused"
        "$<$<CONFIG:DEBUG>:-O0;-ggdb>"
        "$<$<CONFIG:RELEASE>:-O3>"
)

file(GLOB_RECURSE SOURCES src/*.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_definitions(-DNUM_OF_THREADS=${NUM})
add_executable(${TARGET} ${SOURCES})

#target_compile_definitions(${TARGET} PRIVATE NUM_OF_THREADS=${NUM})


#target_compile_options(${TARGET} PUBLIC -flto -fno-fat-lto-objects)
#target_link_options(${TARGET} PUBLIC -flto -fno-fat-lto-objects)

target_link_libraries(${TARGET} PRIVATE Threads::Threads)

###################################

set(TEST_TARGET "run_tests")

file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)
file(GLOB_RECURSE NO_MAIN_SOURCES src/*.cpp)
list(REMOVE_ITEM NO_MAIN_SOURCES ${CMAKE_SOURCE_DIR}/src/test_through.cpp)
list(REMOVE_ITEM NO_MAIN_SOURCES ${CMAKE_SOURCE_DIR}/src/core.cpp)

find_package(Catch2 REQUIRED PATHS "${PROJECT_SOURCE_DIR}/lib")

add_executable(${TEST_TARGET} ${NO_MAIN_SOURCES} ${TEST_SOURCES})
target_link_libraries(${TEST_TARGET} PRIVATE Catch2::Catch2)
target_link_libraries(${TEST_TARGET} PRIVATE Threads::Threads)

#target_compile_options(${TEST_TARGET} PUBLIC -fno-omit-frame-pointer -fsanitize=address,leak,undefined)
#target_link_options(${TEST_TARGET} PUBLIC -fno-omit-frame-pointer -fsanitize=address,leak,undefined)

include(CTest)
include(Catch)
catch_discover_tests(${TEST_TARGET})
