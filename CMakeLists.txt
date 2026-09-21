cmake_minimum_required(VERSION 4.2)
project(kvstore CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic -Wconversion)

add_library(kvstore_core
        src/kvstore.cpp
        src/ttl_worker.cpp
)
target_include_directories(kvstore_core PUBLIC include)

add_executable(kvstore_server src/main.cpp)
target_link_libraries(kvstore_server PRIVATE kvstore_core)

enable_testing()
add_subdirectory(test)
