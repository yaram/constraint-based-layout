cmake_minimum_required(VERSION 3.8)

project(ConstraintBasedLayout C CXX)

add_library(solver STATIC solver.c)
target_include_directories(solver PUBLIC ../include)

install(TARGETS solver ARCHIVE DESTINATION .)

add_library(constraint_arithmetic STATIC constraint_arithmetic.cpp)
target_link_libraries(constraint_arithmetic PRIVATE solver)
target_include_directories(constraint_arithmetic PUBLIC ../include)

install(TARGETS constraint_arithmetic ARCHIVE DESTINATION .)

add_library(controls STATIC controls.cpp)
target_include_directories(controls PUBLIC ../include)

install(TARGETS controls ARCHIVE DESTINATION .)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_library(system_controls STATIC system_controls_win32.cpp)
    target_include_directories(system_controls PUBLIC ../include)

    install(TARGETS system_controls ARCHIVE DESTINATION .)
endif()