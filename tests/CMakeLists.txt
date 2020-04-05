cmake_minimum_required(VERSION 3.15)
project(tests)
set(CMAKE_CXX_STANDARD 14)

set(SOURCE_LIB ../spinlock.cpp)
add_library(spinlock STATIC ${SOURCE_LIB})

add_executable(tests tests.cpp)
target_link_libraries(tests spinlock)