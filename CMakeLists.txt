cmake_minimum_required(VERSION 3.15)
project(spinlock)

set(CMAKE_CXX_STANDARD 14)

SET(SOURCE_LIB spinlock.cpp)
add_library(spinlock STATIC ${SOURCE_LIB})
