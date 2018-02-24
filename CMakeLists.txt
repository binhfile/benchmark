cmake_minimum_required(VERSION 3.0.2)
project(app.benchmark)

# source
file (GLOB_RECURSE SRC_LIST ${CMAKE_SOURCE_DIR}/src/*.cpp)
set (CMAKE_CXX_FLAGS "")

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHs")
    set (CMAKE_CXX_FLAGS_RELEASE "/O2 /MD")
else()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set (CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

# mode
if ("${MODE}" STREQUAL "")
    set (MODE "all")
endif()
string (TOUPPER ${MODE} MODE)
message ("MODE: ${MODE}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDO_BENCHMARK_${MODE}")

# application
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} benchmark)
target_link_libraries(${PROJECT_NAME} pthread rt)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    target_link_libraries(${PROJECT_NAME} Shlwapi)
endif()

