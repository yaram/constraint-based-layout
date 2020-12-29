cmake_minimum_required(VERSION 3.8)

project(ConstraintBasedLayout C CXX)

add_library(solver STATIC solver.c)
target_include_directories(solver PUBLIC ../include)

add_library(constraint_arithmetic STATIC constraint_arithmetic.cpp)
target_link_libraries(constraint_arithmetic PRIVATE solver)
target_include_directories(constraint_arithmetic PUBLIC ../include)