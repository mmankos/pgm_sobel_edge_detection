cmake_minimum_required(VERSION 3.10)
project(sobel_operator VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ALL_SOURCE_FILES "src/*.c")

add_executable(${PROJECT_NAME}
    ${ALL_SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
