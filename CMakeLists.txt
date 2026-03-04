cmake_minimum_required(VERSION 3.10)
project(sobel_operator VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(ENABLE_PROFILING "Enable profiling with gprof" OFF)
option(ENABLE_DEBUG "Enable debug symbols" OFF)

if(ENABLE_PROFILING)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
    message(STATUS "Profiling enabled (-pg flag added)")
endif()

if(ENABLE_DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
    message(STATUS "Debug symbols enabled (-g flag added)")
else()
    # Default to optimized build
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
endif()

file(GLOB_RECURSE ALL_SOURCE_FILES "src/*.c")

add_executable(${PROJECT_NAME}
    ${ALL_SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
